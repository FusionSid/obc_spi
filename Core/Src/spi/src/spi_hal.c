#include "spi_hal.h"

static spi_handle_t s_handle;
static bool s_init = false;

static spi_status_t convert_hal_status(HAL_StatusTypeDef status);

spi_status_t spi_init(SPI_HandleTypeDef *hspi, uint32_t tx_timeout_ms, uint32_t rx_timeout_ms) {
    if (hspi == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    s_handle.hspi = hspi;
    s_handle.send_timeout_ms = tx_timeout_ms;
    s_handle.receive_timeout_ms = rx_timeout_ms;

    s_init = true;

    return SPI_WORKED;
}

void spi_device_select(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, 0); }
void spi_device_deselect(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, 1); }

spi_status_t spi_send(const uint8_t *data, uint16_t length) {
    if (!s_init) {
        return SPI_ERR_NOT_INITALISED;
    }

    HAL_StatusTypeDef status = HAL_SPI_Transmit(s_handle.hspi, (uint8_t *)data, length, s_handle.send_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_recieve(uint8_t *data, uint16_t length) {
    if (!s_init) {
        return SPI_ERR_NOT_INITALISED;
    }

    if (data == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    HAL_StatusTypeDef status = HAL_SPI_Receive(s_handle.hspi, data, length, s_handle.receive_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_send_recieve(const uint8_t *tx_data, uint8_t *rx_data, uint16_t length) {
    if (!s_init) {
        return SPI_ERR_NOT_INITALISED;
    }

    if (tx_data == NULL || rx_data == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    HAL_StatusTypeDef status =
        HAL_SPI_TransmitReceive(s_handle.hspi, (uint8_t *)tx_data, rx_data, length, s_handle.send_timeout_ms);
    return convert_hal_status(status);
}

spi_status_t spi_recieve_byte(uint8_t *byte) { return spi_recieve(byte, 1); }
spi_status_t spi_send_byte(uint8_t byte) { return spi_send(&byte, 1); }

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