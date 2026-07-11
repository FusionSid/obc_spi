#pragma once

#include "spi_hal.h"
#include "spi_packet.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SPI_FSM_STATE_UNINIT = 0,
    SPI_FSM_STATE_IDLE,
    SPI_FSM_STATE_TX,
    SPI_FSM_STATE_RX_WAIT_START,
    SPI_FSM_STATE_RX_HEADER,
    SPI_FSM_STATE_RX_PAYLOAD,
    SPI_FSM_STATE_RX_CRC,
    SPI_FSM_STATE_FATAL_ERROR,
} spi_fsm_state_t;

#define SPI_FSM_DEFAULT_START_TIMEOUT_MS (50)

spi_status_t spi_fsm_init(SPI_HandleTypeDef *hspi, void (*notify_cb)(void *ctx), void *notify_ctx);

#ifdef NEW_PACKET_FORMAT
spi_status_t spi_fsm_send(const spi_cs_config_t *cs, uint8_t payload, uint8_t query, const uint8_t *data,
                          uint16_t data_len, uint32_t start_byte_timeout_ms);
#else
spi_status_t spi_fsm_send(const spi_cs_config_t *cs, uint8_t query, const uint8_t *data, uint16_t data_len,
                          uint32_t start_byte_timeout_ms);
#endif
spi_status_t spi_fsm_take_last(spi_fsm_result_t *result_out, spi_packet_t *packet_out);
spi_fsm_state_t spi_fsm_get_state(void);

void spi_fsm_reset(void);

void spi_fsm_on_tx_complete_it(SPI_HandleTypeDef *hspi);
void spi_fsm_on_rx_complete_it(SPI_HandleTypeDef *hspi);
void spi_fsm_on_error_it(SPI_HandleTypeDef *hspi);
