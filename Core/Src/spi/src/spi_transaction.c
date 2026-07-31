#include "spi_transaction.h"
#include "cmsis_os2.h"
#include "log.h"
#include <stdbool.h>
#include <string.h>

static SPI_HandleTypeDef *s_hspi;
static const spi_device_config_t *s_device_table;
static uint8_t s_device_count;
static volatile bool s_transaction_done;

static void fsm_notify(void *ctx) { s_transaction_done = true; }

static void recover_from_fatal_error(void) {
    spi_hal_reinit(s_hspi);
    spi_fsm_reset();
}

static spi_response_code_t map_fsm_result(spi_fsm_result_t fsm_result) {
    switch (fsm_result) {
    case SPI_FSM_RESULT_OK:
        return SPI_RESP_OK;
    case SPI_FSM_RESULT_BAD_CRC:
        return SPI_RESP_BAD_CRC;
    case SPI_FSM_RESULT_START_TIMEOUT:
        return SPI_RESP_TIMEOUT;
    case SPI_FSM_RESULT_BAD_LENGTH:
    case SPI_FSM_RESULT_BUS_ERROR:
    default:
        return SPI_RESP_BUS_ERROR;
    }
}

spi_status_t spi_service_init(SPI_HandleTypeDef *hspi, const spi_device_config_t *device_table, uint8_t device_count) {
    if (hspi == NULL || device_table == NULL || device_count == 0) {
        return SPI_ERR_INVALID_ARGS;
    }

    s_hspi = hspi;
    s_device_table = device_table;
    s_device_count = device_count;
    s_transaction_done = false;

    spi_status_t status1 = spi_fsm_init(hspi);
    if (status1 != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    spi_status_t status2 =
        spi_hal_init(hspi, spi_fsm_on_tx_complete_it, spi_fsm_on_rx_complete_it, spi_fsm_on_error_it);
    if (status2 != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
}

spi_response_code_t spi_transact(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                 spi_packet_t *response, bool expects_response) {
    if (device >= s_device_count || data_len > SPI_PACKET_MAX_DATA_SIZE || (data_len != 0 && data == NULL)) {
        return SPI_RESP_INVALID_ARGS;
    }

    const spi_device_config_t *dev = &s_device_table[device];
    uint32_t timeout_ms = dev->start_byte_timeout_ms;

    if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
        recover_from_fatal_error();
    }

    spi_response_code_t response_code = SPI_RESP_BUS_ERROR;

    const uint16_t hard_attempt_cap = (uint16_t)dev->max_send_retries * 4u + 4u;
    uint16_t total_attempts = 0;

    for (uint8_t attempt = 0; attempt < dev->max_send_retries; attempt++) {
        total_attempts++;
        log_printf("spi_transact: attempt %u/%u (total_attempts=%u) state_before=%d\r\n", attempt + 1,
                   dev->max_send_retries, total_attempts, (int)spi_fsm_get_state());

        if (total_attempts > hard_attempt_cap) {
            log_printf("spi_transact: HARD CAP HIT (%u), aborting transaction\r\n", hard_attempt_cap);
            recover_from_fatal_error();
            return SPI_RESP_BUS_ERROR;
        }

        s_transaction_done = false;

#ifdef NEW_PACKET_FORMAT
        spi_status_t send_status = spi_fsm_send(&dev->cs, (uint8_t)device, query_code, data, data_len, timeout_ms,
                                                fsm_notify, expects_response);
#else
        spi_status_t send_status =
            spi_fsm_send(&dev->cs, query_code, data, data_len, timeout_ms, fsm_notify, expects_response);
#endif
        if (send_status != SPI_WORKED) {
            log_printf("spi_transact: spi_fsm_send failed, status=%d, fsm_state=%d\r\n", (int)send_status,
                       (int)spi_fsm_get_state());
            if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
                recover_from_fatal_error();
                continue;
            } else if (spi_fsm_get_state() != SPI_FSM_STATE_IDLE) {
                return SPI_RESP_BUSY;
            }
            response_code = SPI_RESP_BUS_ERROR;
            continue;
        }

        uint32_t wait_start = osKernelGetTickCount();
        while (!s_transaction_done) {
            osDelay(1);
            if ((osKernelGetTickCount() - wait_start) > (timeout_ms + 1000)) {
                log_printf("spi_transact: WATCHDOG - s_transaction_done never set, forcing recovery\r\n");
                recover_from_fatal_error();
                s_transaction_done = true;
                response_code = SPI_RESP_TIMEOUT;
                break;
            }
        }
        if (response_code == SPI_RESP_TIMEOUT) {
            continue;
        }

        spi_fsm_result_t fsm_result;
        spi_packet_t packet = {0};
        if (spi_fsm_take_last(&fsm_result, &packet) != SPI_WORKED) {
            fsm_result = SPI_FSM_RESULT_BUS_ERROR;
        }

        response_code = map_fsm_result(fsm_result);
        log_printf("spi_transact: attempt %u result=%d (fsm_result=%d)\r\n", attempt + 1, (int)response_code,
                   (int)fsm_result);

        if (response_code == SPI_RESP_OK) {
            if (response != NULL) {
                *response = packet;
            }

            return SPI_RESP_OK;
        }

        if (response_code == SPI_RESP_BUS_ERROR) {
            recover_from_fatal_error();
        }
    }

    return response_code;
}

/*
general pseudocode plan

spi transact:
    clear current notifcaiton
    enqueue req into queue
    wait for notification from spi task
    return the data to caller

spi_task:
    while true:
    dequeue req from queue
    call spi_execute(req)
    notify task

spi_execute
    try upto 3 times:
        spi_fsm_send(...req, notify=new_notify)
        wait for notification from notify_cb
        set req.data = take_last()
        return

new_notify:
    send notification from isr (replaces transaction done)
*/