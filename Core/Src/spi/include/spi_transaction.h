#pragma once

#include "spi_hal.h"
#include "spi_status.h"

spi_status_t spi_transaction(const spi_cs_config_t *cs, uint8_t query,
                                       const uint8_t *tx_data, uint16_t tx_length, uint8_t *rx_data,
                                       uint16_t rx_buffer_size, uint16_t *rx_length_out, uint32_t timeout_ms);