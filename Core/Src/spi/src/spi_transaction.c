#include "spi_transaction.h"
#include "cmsis_os2.h"
#include <stdbool.h>
#include <string.h>

#define SPI_REQ_QUEUE_LEN 8
#define SPI_TASK_STACK_SZ 3072
#define SPI_TASK_PRIORITY osPriorityNormal

#define WORKER_EVENT_RX_DONE (1UL << 0)

typedef void (*spi_callback_t)(spi_response_code_t result, spi_packet_t *response, void *user_data);

typedef struct {
    spi_payload_t device;
    uint8_t query_code;
    const uint8_t *data;
    uint16_t data_len;
    bool expects_response;
    spi_packet_t response_data;
    spi_callback_t callback;
    void *user_data;
} internal_spi_request_t;

struct blocking_context {
    spi_response_code_t *result;
    spi_packet_t *response_out;
    osEventFlagsId_t event;
};

static osMessageQueueId_t s_spi_req_queue;
static osThreadId_t s_spi_worker_thread;

static SPI_HandleTypeDef *s_hspi;
static const spi_device_config_t *s_device_table;
static uint8_t s_device_count;

static void fsm_notify(void *ctx) { osThreadFlagsSet(s_spi_worker_thread, WORKER_EVENT_RX_DONE); }

static spi_response_code_t spi_execute(internal_spi_request_t *req);

static void blocking_completion_callback(spi_response_code_t result, spi_packet_t *response, void *user_data) {
    if (user_data) {
        struct blocking_context *ctx = (struct blocking_context *)user_data;
        if (ctx->result) {
            *ctx->result = result;
        }
        if (ctx->response_out && response) {
            *ctx->response_out = *response;
        }
        osEventFlagsSet(ctx->event, 1);
    }
}

static osStatus_t spi_transact_async(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                     bool expects_response, spi_callback_t callback, void *user_data) {
    internal_spi_request_t req = {
        .device = device,
        .query_code = query_code,
        .data = data,
        .data_len = data_len,
        .expects_response = expects_response,
        .callback = callback,
        .user_data = user_data,
    };
    return osMessageQueuePut(s_spi_req_queue, &req, 0, 0);
}

spi_response_code_t spi_transact(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                 spi_packet_t *response, bool expects_response) {
    spi_response_code_t result = SPI_RESP_BUS_ERROR;

    struct blocking_context context = {.result = &result, .response_out = response, .event = osEventFlagsNew(NULL)};

    if (context.event == NULL) {
        return SPI_RESP_BUS_ERROR;
    }

    osStatus_t status = spi_transact_async(device, query_code, data, data_len, expects_response,
                                           blocking_completion_callback, &context);

    if (status != osOK) {
        osEventFlagsDelete(context.event);
        return SPI_RESP_BUS_ERROR;
    }

    uint32_t flags = osEventFlagsWait(context.event, 1, osFlagsWaitAny, 5000);

    osEventFlagsDelete(context.event);

    if (flags == (uint32_t)osFlagsErrorTimeout) {
        return SPI_RESP_TIMEOUT;
    }

    return result;
}

void spi_task(void *arg) {
    s_spi_worker_thread = osThreadGetId();
    internal_spi_request_t req;

    for (;;) {
        osStatus_t q_stat = osMessageQueueGet(s_spi_req_queue, &req, NULL, osWaitForever);
        if (q_stat != osOK) continue;

        spi_response_code_t res = spi_execute(&req);

        if (req.callback) {
            req.callback(res, &req.response_data, req.user_data);
        }
    }
}

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

static spi_response_code_t spi_execute(internal_spi_request_t *req) {
    const spi_device_config_t *dev = &s_device_table[req->device];
    uint32_t timeout_ms = dev->start_byte_timeout_ms;
    spi_response_code_t response_code = SPI_RESP_BUS_ERROR;

    if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
        recover_from_fatal_error();
    }

    for (uint8_t attempt = 0; attempt < dev->max_send_retries; attempt++) {

        osThreadFlagsClear(WORKER_EVENT_RX_DONE);

#ifdef NEW_PACKET_FORMAT
        spi_status_t send_status = spi_fsm_send(&dev->cs, req->device, req->query_code, req->data, req->data_len,
                                                timeout_ms, fsm_notify, req->expects_response);
#else
        spi_status_t send_status = spi_fsm_send(&dev->cs, req->query_code, req->data, req->data_len, timeout_ms,
                                                fsm_notify, req->expects_response);
#endif

        if (send_status != SPI_WORKED) {
            if (spi_fsm_get_state() == SPI_FSM_STATE_FATAL_ERROR) {
                recover_from_fatal_error();
                continue;
            } else if (spi_fsm_get_state() != SPI_FSM_STATE_IDLE) {
                return SPI_RESP_BUSY;
            }
            continue;
        }

        uint32_t flags = osThreadFlagsWait(WORKER_EVENT_RX_DONE, osFlagsWaitAny, timeout_ms + 1000);
        if (flags == (uint32_t)osFlagsErrorTimeout) {
            recover_from_fatal_error();
            response_code = SPI_RESP_TIMEOUT;
            continue;
        }

        spi_fsm_result_t fsm_result;
        if (spi_fsm_take_last(&fsm_result, &req->response_data) != SPI_WORKED) {
            fsm_result = SPI_FSM_RESULT_BUS_ERROR;
        }
        response_code = map_fsm_result(fsm_result);

        if (response_code == SPI_RESP_OK) {
            return SPI_RESP_OK;
        }
        if (response_code == SPI_RESP_BUS_ERROR) {
            recover_from_fatal_error();
        }
    }
    return response_code;
}

spi_status_t spi_service_init(const spi_device_config_t *device_table, uint8_t device_count) {
    if (device_table == NULL || device_count == 0) {
        return SPI_ERR_INVALID_ARGS;
    }

    if (spi_hal_init(spi_fsm_on_tx_complete_it, spi_fsm_on_rx_complete_it, spi_fsm_on_error_it) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    SPI_HandleTypeDef *hspi = spi_hal_get_handle();

    s_hspi = hspi;
    s_device_table = device_table;
    s_device_count = device_count;

    if (spi_fsm_init(hspi) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    s_spi_req_queue = osMessageQueueNew(SPI_REQ_QUEUE_LEN, sizeof(internal_spi_request_t), NULL);
    if (s_spi_req_queue == NULL) {
        return SPI_ERR_HAL;
    }

    const osThreadAttr_t task_attr = {
        .name = "spi_task",
        .stack_size = SPI_TASK_STACK_SZ,
        .priority = SPI_TASK_PRIORITY,
    };

    s_spi_worker_thread = osThreadNew(spi_task, NULL, &task_attr);
    if (s_spi_worker_thread == NULL) {
        osMessageQueueDelete(s_spi_req_queue);
        s_spi_req_queue = NULL;
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
}