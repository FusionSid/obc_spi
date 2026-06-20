#pragma once

#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"

typedef struct {
    SPI_HandleTypeDef *hspi;
    uint32_t send_timeout_ms;
    uint32_t receive_timeout_ms;
} spi_handle_t;

typedef struct {
    uint16_t cs_pin;
    GPIO_TypeDef *cs_port;
} spi_cs_config_t;

spi_status_t spi_init(SPI_HandleTypeDef *hspi, uint32_t tx_timeout_ms, uint32_t rx_timeout_ms);

void spi_device_select(const spi_cs_config_t *cs);
void spi_device_deselect(const spi_cs_config_t *cs);

spi_status_t spi_send(const uint8_t *data, uint16_t length);
spi_status_t spi_send_byte(uint8_t byte);

spi_status_t spi_recieve(uint8_t *data, uint16_t length);
spi_status_t spi_recieve_byte(uint8_t *byte);

spi_status_t spi_send_recieve(const uint8_t *tx_data, uint8_t *rx_data, uint16_t length);
