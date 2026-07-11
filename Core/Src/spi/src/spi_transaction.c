#include "spi_transaction.h"
#include "log.h"
#include <stdbool.h>
#include <string.h>

static SPI_HandleTypeDef *s_hspi;
static const spi_device_config_t *s_device_table;
static uint8_t s_device_count;
static volatile bool s_transaction_done;

static void fsm_notify(void *ctx) { s_transaction_done = true; }

static void recover_from_fatal_error(void) {
    spi_bus_hal_reinit(s_hspi);
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

spi_status_t spi_bus_service_init(SPI_HandleTypeDef *hspi, const spi_device_config_t *device_table,
                                  uint8_t device_count) {
    if (hspi == NULL || device_table == NULL || device_count == 0) {
        return SPI_ERR_INVALID_ARGS;
    }

    s_hspi = hspi;
    s_device_table = device_table;
    s_device_count = device_count;
    s_transaction_done = false;

    spi_status_t status1 = spi_fsm_init(hspi, fsm_notify, NULL);
    if (status1 != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    spi_status_t status2 =
        spi_bus_hal_init(hspi, spi_fsm_on_tx_complete_it, spi_fsm_on_rx_complete_it, spi_fsm_on_error_it);
    if (status2 != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
}

spi_response_code_t spi_bus_transact(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                     uint8_t out_data[SPI_PACKET_MAX_DATA_SIZE], uint16_t *out_len) {
    if (device >= s_device_count || data_len > SPI_PACKET_MAX_DATA_SIZE || (data_len != 0 && data == NULL)) {
        return SPI_RESP_INVALID_ARGS;
    }

    const spi_device_config_t *dev = &s_device_table[device];
    uint32_t timeout_ms = dev->start_byte_timeout_ms;

    if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
        recover_from_fatal_error();
    }

    spi_response_code_t response_code = SPI_RESP_BUS_ERROR;

    for (uint8_t attempt = 0; attempt < dev->max_send_retries; attempt++) {
        s_transaction_done = false;

        log_printf("SEnding to device %i\r\n", device);
#ifdef NEW_PACKET_FORMAT
        spi_status_t send_status = spi_fsm_send(&dev->cs, (uint8_t)device, query_code, data, data_len, timeout_ms);
#else
        spi_status_t send_status = spi_fsm_send(&dev->cs, query_code, data, data_len, timeout_ms);
#endif
        if (send_status != SPI_WORKED) {
            if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
                recover_from_fatal_error();
                continue;
            } else if (spi_fsm_get_state() != SPI_FSM_STATE_IDLE) {
                return SPI_RESP_BUSY;
            }
            response_code = SPI_RESP_BUS_ERROR;
            continue;
        }

        while (!s_transaction_done) {
            HAL_Delay(1);
        }

        spi_fsm_result_t fsm_result;
        spi_packet_t packet = {0};
        if (spi_fsm_take_last(&fsm_result, &packet) != SPI_WORKED) {
            fsm_result = SPI_FSM_RESULT_BUS_ERROR;
        }

        response_code = map_fsm_result(fsm_result);
        if (response_code == SPI_RESP_OK) {
            if (out_data != NULL && packet.length > 0) {
                memcpy(out_data, packet.data, packet.length);
            }
            if (out_len != NULL) {
                *out_len = packet.length;
            }
            return SPI_RESP_OK;
        }

        if (response_code == SPI_RESP_BUS_ERROR) {
            recover_from_fatal_error();
        }
    }

    return response_code;
}
