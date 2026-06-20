#include "spi_hal.h"

static spi_status_t convert_hal_status(HAL_StatusTypeDef status);

spi_status_t spi_init(spi_handle_t *handle, SPI_HandleTypeDef *hspi, uint32_t tx_timeout_ms, uint32_t rx_timeout_ms) {
    if (handle == NULL || hspi == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    handle->hspi = hspi;
    handle->send_timeout_ms = tx_timeout_ms;
    handle->receive_timeout_ms = rx_timeout_ms;

    return SPI_WORKED;
}

void spi_device_select(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, 0); }
void spi_device_deselect(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, 1); }

spi_status_t spi_send(spi_handle_t *handle, const uint8_t *data, uint16_t length) {
    if (handle == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    HAL_StatusTypeDef status = HAL_SPI_Transmit(handle->hspi, (uint8_t *)data, length, handle->send_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_recieve(spi_handle_t *handle, uint8_t *data, uint16_t length) {
    if (handle == NULL || data == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    HAL_StatusTypeDef status = HAL_SPI_Receive(handle->hspi, data, length, handle->receive_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_send_recieve(spi_handle_t *handle, const uint8_t *tx_data, uint8_t *rx_data, uint16_t length) {
    if (handle == NULL || tx_data == NULL || rx_data == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(handle->hspi, (uint8_t *)tx_data, rx_data, length, handle->send_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_recieve_byte(spi_handle_t *handle, uint8_t *byte) { return spi_recieve(handle, byte, 1); }
spi_status_t spi_send_byte(spi_handle_t *handle, uint8_t byte) { return spi_send(handle, &byte, 1); }

static spi_status_t convert_hal_status(HAL_StatusTypeDef status) {
    switch (status) {
    case HAL_OK:
        return SPI_WORKED;
    case HAL_TIMEOUT:
        return SPI_ERR_TIMEDOUT;
    default:
        return SPI_ERR_HAL;
    }
}