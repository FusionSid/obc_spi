#include "spi_fsm.h"
#include "log.h"
#include <string.h>

#ifdef NEW_PACKET_FORMAT
#define SPI_FSM_OFFSET_LEN_L 3
#define SPI_FSM_OFFSET_LEN_H 4
#else
#define SPI_FSM_OFFSET_LEN_L 2
#define SPI_FSM_OFFSET_LEN_H 3
#endif

typedef struct {
    spi_fsm_state_t state;
    SPI_HandleTypeDef *hspi;
    const spi_cs_config_t *cs;

    uint8_t tx_buf[SPI_PACKET_MAX_PACKET_SIZE];
    uint16_t tx_len;

    uint8_t rx_buf[SPI_PACKET_MAX_PACKET_SIZE];
    uint16_t rx_data_length;

    uint32_t rx_wait_start_entry_tick;
    uint32_t rx_wait_start_timeout_ms;
    bool last_valid;
    spi_fsm_result_t last_result;
    spi_packet_t last_packet;

    void (*notify_cb)(void *ctx);

    bool expects_response;
} spi_fsm_t;

static spi_fsm_t s_fsm = {0};

static void fsm_enter_state_wait_start(void);
static void fsm_enter_state_header(void);
static void fsm_enter_state_payload(void);
static void fsm_enter_state_crc(void);
static void fsm_finish(spi_fsm_result_t result);

static void handle_state_wait_start(void);
static void handle_state_header(void);
static void handle_state_payload(void);
static void handle_state_crc(void);

spi_status_t spi_fsm_init(SPI_HandleTypeDef *hspi) {
    if (hspi == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    memset(&s_fsm, 0, sizeof(s_fsm));
    s_fsm.hspi = hspi;
    s_fsm.state = SPI_FSM_STATE_IDLE;
    return SPI_WORKED;
}
#ifdef NEW_PACKET_FORMAT
spi_status_t spi_fsm_send(const spi_cs_config_t *cs, uint8_t payload, uint8_t query, const uint8_t *data,
                          uint16_t data_len, uint32_t start_byte_timeout_ms, void (*notify_cb)(void *ctx),
                          bool expects_response)
#else
spi_status_t spi_fsm_send(const spi_cs_config_t *cs, uint8_t query, const uint8_t *data, uint16_t data_len,
                          uint32_t start_byte_timeout_ms, void (*notify_cb)(void *ctx), bool expects_response)
#endif
{
    if (s_fsm.hspi == NULL) {
        return SPI_ERR_NOT_INITALISED;
    }
    if (s_fsm.state != SPI_FSM_STATE_IDLE || cs == NULL || notify_cb == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    s_fsm.notify_cb = notify_cb;

#ifdef NEW_PACKET_FORMAT
    log_printf("using new format\r\n");
    spi_status_t build_status = spi_packet_build(s_fsm.tx_buf, payload, query, data, data_len);
#else
    log_printf("using old format\r\n");
    spi_status_t build_status = spi_packet_build(s_fsm.tx_buf, query, data, data_len);
#endif
    if (build_status != SPI_WORKED) {
        return build_status;
    }

    s_fsm.expects_response = expects_response;
    s_fsm.tx_len = (uint16_t)(SPI_PACKET_HEADER_SIZE + data_len + SPI_PACKET_FOOTER_SIZE);
    s_fsm.cs = cs;
    s_fsm.rx_wait_start_timeout_ms =
        (start_byte_timeout_ms != 0) ? start_byte_timeout_ms : SPI_FSM_DEFAULT_START_TIMEOUT_MS;
    s_fsm.last_valid = false;
    s_fsm.state = SPI_FSM_STATE_TX;

    log_as_bytes(s_fsm.tx_buf, s_fsm.tx_len);

    spi_device_select(cs);

    if (spi_hal_transmit_it(s_fsm.hspi, s_fsm.tx_buf, s_fsm.tx_len) != HAL_OK) {
        spi_device_deselect(cs);
        s_fsm.cs = NULL;
        s_fsm.state = SPI_FSM_STATE_FATAL_ERROR;
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
}

spi_status_t spi_fsm_take_last(spi_fsm_result_t *result_out, spi_packet_t *packet_out) {
    if (!s_fsm.last_valid) {
        return SPI_ERR_INVALID_ARGS;
    }

    if (result_out != NULL) {
        *result_out = s_fsm.last_result;
    }
    if (packet_out != NULL) {
        *packet_out = s_fsm.last_packet;
    }

    s_fsm.last_valid = false;
    return SPI_WORKED;
}

spi_fsm_state_t spi_fsm_get_state(void) { return s_fsm.state; }

void spi_fsm_reset(void) {
    if (s_fsm.cs != NULL) {
        spi_device_deselect(s_fsm.cs);
    }
    s_fsm.cs = NULL;
    s_fsm.last_valid = false;
    s_fsm.state = SPI_FSM_STATE_IDLE;
}

static void fsm_enter_state_wait_start(void) {
    s_fsm.state = SPI_FSM_STATE_RX_WAIT_START;
    s_fsm.rx_wait_start_entry_tick = HAL_GetTick();
    if (spi_hal_receive_it(s_fsm.hspi, &s_fsm.rx_buf[0], 1) != HAL_OK) {
        fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
    }
}

static void fsm_enter_state_header(void) {
    s_fsm.state = SPI_FSM_STATE_RX_HEADER;
    if (spi_hal_receive_it(s_fsm.hspi, &s_fsm.rx_buf[1], SPI_PACKET_HEADER_SIZE - 1) != HAL_OK) {
        fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
    }
}

static void fsm_enter_state_payload(void) {
    s_fsm.state = SPI_FSM_STATE_RX_PAYLOAD;
    if (spi_hal_receive_it(s_fsm.hspi, &s_fsm.rx_buf[SPI_PACKET_HEADER_SIZE], s_fsm.rx_data_length) != HAL_OK) {
        fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
    }
}

static void fsm_enter_state_crc(void) {
    s_fsm.state = SPI_FSM_STATE_RX_CRC;
    uint16_t crc_offset = (uint16_t)(SPI_PACKET_HEADER_SIZE + s_fsm.rx_data_length);
    if (spi_hal_receive_it(s_fsm.hspi, &s_fsm.rx_buf[crc_offset], SPI_PACKET_FOOTER_SIZE) != HAL_OK) {
        fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
    }
}

static void fsm_finish(spi_fsm_result_t result) {
    if (s_fsm.cs != NULL) {
        spi_device_deselect(s_fsm.cs);
    }
    s_fsm.cs = NULL;

    s_fsm.last_result = result;
    s_fsm.last_valid = true;
    s_fsm.state = (result == SPI_FSM_RESULT_BUS_ERROR) ? SPI_FSM_STATE_FATAL_ERROR : SPI_FSM_STATE_IDLE;

    if (s_fsm.notify_cb != NULL) {
        s_fsm.notify_cb(&s_fsm);
    }
}

void spi_fsm_on_tx_complete_it(SPI_HandleTypeDef *hspi) {
    if (hspi != s_fsm.hspi || s_fsm.state != SPI_FSM_STATE_TX) {
        return;
    }

    if (s_fsm.expects_response) {
        fsm_enter_state_wait_start();
    } else {
        fsm_finish(SPI_FSM_RESULT_OK);
    }
}

void spi_fsm_on_rx_complete_it(SPI_HandleTypeDef *hspi) {
    if (hspi != s_fsm.hspi) {
        return;
    }

    switch (s_fsm.state) {
    case SPI_FSM_STATE_RX_WAIT_START:
        handle_state_wait_start();
        break;
    case SPI_FSM_STATE_RX_HEADER:
        handle_state_header();
        break;
    case SPI_FSM_STATE_RX_PAYLOAD:
        handle_state_payload();
        break;
    case SPI_FSM_STATE_RX_CRC:
        handle_state_crc();
        break;
    default:
        break;
    }
}

void spi_fsm_on_error_it(SPI_HandleTypeDef *hspi) {
    if (hspi != s_fsm.hspi || s_fsm.state == SPI_FSM_STATE_UNINIT || s_fsm.state == SPI_FSM_STATE_IDLE) {
        return;
    }
    fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
}

static void handle_state_wait_start(void) {
    if (s_fsm.rx_buf[0] != SPI_PACKET_START_BYTE) {
        uint32_t elapsed = HAL_GetTick() - s_fsm.rx_wait_start_entry_tick;
        if (elapsed >= s_fsm.rx_wait_start_timeout_ms) {
            fsm_finish(SPI_FSM_RESULT_START_TIMEOUT);
        } else if (spi_hal_receive_it(s_fsm.hspi, &s_fsm.rx_buf[0], 1) != HAL_OK) {
            fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
        }
        return;
    }
    fsm_enter_state_header();
}

static void handle_state_header(void) {
    uint16_t data_length =
        (uint16_t)s_fsm.rx_buf[SPI_FSM_OFFSET_LEN_L] | (uint16_t)((uint16_t)s_fsm.rx_buf[SPI_FSM_OFFSET_LEN_H] << 8);

    if (data_length > SPI_PACKET_MAX_DATA_SIZE) {
        fsm_finish(SPI_FSM_RESULT_BAD_LENGTH);
        return;
    }

    s_fsm.rx_data_length = data_length;
    if (data_length > 0) {
        fsm_enter_state_payload();
    } else {
        fsm_enter_state_crc();
    }
}

static void handle_state_payload(void) { fsm_enter_state_crc(); }

static void handle_state_crc(void) {
    uint16_t total_len = (uint16_t)(SPI_PACKET_HEADER_SIZE + s_fsm.rx_data_length + SPI_PACKET_FOOTER_SIZE);
    spi_packet_t parsed;
    spi_status_t parse_status = spi_packet_parse(s_fsm.rx_buf, total_len, &parsed);

    if (parse_status == SPI_WORKED) {
#ifdef NEW_PACKET_FORMAT
        s_fsm.last_packet.payload = parsed.payload;
#endif
        s_fsm.last_packet.query = parsed.query;
        s_fsm.last_packet.length = parsed.length;
        s_fsm.last_packet.data = parsed.data;
        fsm_finish(SPI_FSM_RESULT_OK);
    } else if (parse_status == SPI_ERR_CRCBAD) {
        fsm_finish(SPI_FSM_RESULT_BAD_CRC);
    } else {
        fsm_finish(SPI_FSM_RESULT_BUS_ERROR);
    }
}