#pragma once

#include "spi_status.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} spi_cs_config_t;

void spi_device_select(const spi_cs_config_t *cs);
void spi_device_deselect(const spi_cs_config_t *cs);

spi_status_t spi_bus_hal_init(SPI_HandleTypeDef *hspi, void (*tx_cb)(SPI_HandleTypeDef *hspi),
                              void (*rxtx_cb)(SPI_HandleTypeDef *hspi), void (*err_cb)(SPI_HandleTypeDef *hspi));
spi_status_t spi_bus_hal_reinit(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef spi_bus_hal_abort_it(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef spi_bus_hal_transmit_it(SPI_HandleTypeDef *hspi, const uint8_t *tx_data, uint16_t length);
HAL_StatusTypeDef spi_bus_hal_receive_it(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint16_t length);
