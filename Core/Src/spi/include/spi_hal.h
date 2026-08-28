#pragma once

#include "spi_status.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

#define SPI_SPI_NUMBER (SPI5)
#define SPI_GPIO_PORT (GPIOF)
#define SPI_SCK_PIN (GPIO_PIN_7)
#define SPI_MISO_PIN (GPIO_PIN_8)
#define SPI_MOSI_PIN (GPIO_PIN_9)

typedef struct {
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} spi_cs_config_t;

void spi_device_select(const spi_cs_config_t *cs);
void spi_device_deselect(const spi_cs_config_t *cs);

spi_status_t spi_hal_init(void (*tx_cb)(SPI_HandleTypeDef *hspi), void (*rxtx_cb)(SPI_HandleTypeDef *hspi),
                          void (*err_cb)(SPI_HandleTypeDef *hspi));
void spi_hal_msp_init(SPI_HandleTypeDef *hspi);
void spi_hal_msp_deinit(SPI_HandleTypeDef *hspi);
void spi_hal_cs_init(const spi_cs_config_t *cs);
spi_status_t spi_hal_reinit(SPI_HandleTypeDef *hspi);

void spi_irq_handler(void);

SPI_HandleTypeDef *spi_hal_get_handle();

HAL_StatusTypeDef spi_hal_abort_it(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef spi_hal_transmit_it(SPI_HandleTypeDef *hspi, const uint8_t *tx_data, uint16_t length);
HAL_StatusTypeDef spi_hal_receive_it(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint16_t length);