#include "spi_hal.h"
#include "spi_packet.h"

#include <string.h>

#define SPI_BUS_HAL_PLACEHOLDER_BYTE (0)
// only adding this placeholder stuff cause I saw in apss 2 code that the rx and tx buffers are shared
// so this is like the equivalent of the transmit no garbage part
static const uint8_t s_dummy_tx[SPI_PACKET_MAX_DATA_SIZE] = {SPI_BUS_HAL_PLACEHOLDER_BYTE};

static SPI_HandleTypeDef *s_hspi;
static void (*s_tx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_rxtx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_error_cb)(SPI_HandleTypeDef *hspi);

void spi_device_select(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_RESET); }

void spi_device_deselect(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_SET); }

static spi_status_t register_callbacks() {
    if (HAL_SPI_RegisterCallback(s_hspi, HAL_SPI_TX_COMPLETE_CB_ID, s_tx_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    if (HAL_SPI_RegisterCallback(s_hspi, HAL_SPI_TX_RX_COMPLETE_CB_ID, s_rxtx_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    if (HAL_SPI_RegisterCallback(s_hspi, HAL_SPI_ERROR_CB_ID, s_error_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    return SPI_WORKED;
}

spi_status_t spi_hal_init(SPI_HandleTypeDef *hspi, void (*tx_cb)(SPI_HandleTypeDef *hspi),
                          void (*rxtx_cb)(SPI_HandleTypeDef *hspi), void (*err_cb)(SPI_HandleTypeDef *hspi)) {
    if (hspi == NULL || tx_cb == NULL || rxtx_cb == NULL || err_cb == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    s_hspi = hspi;
    s_tx_cb = tx_cb;
    s_rxtx_cb = rxtx_cb;
    s_error_cb = err_cb;

    return register_callbacks();
}

spi_status_t spi_hal_reinit(SPI_HandleTypeDef *hspi) {
    if (hspi == NULL || hspi != s_hspi) {
        return SPI_ERR_NOT_INITALISED;
    }

    spi_hal_abort_it(hspi);

    if (HAL_SPI_DeInit(hspi) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    if (HAL_SPI_Init(hspi) != HAL_OK) {
        return SPI_ERR_HAL;
    }

    return register_callbacks();
}

HAL_StatusTypeDef spi_hal_abort_it(SPI_HandleTypeDef *hspi) { return HAL_SPI_Abort_IT(hspi); }

HAL_StatusTypeDef spi_hal_transmit_it(SPI_HandleTypeDef *hspi, const uint8_t *tx_data, uint16_t length) {
    return HAL_SPI_Transmit_IT(hspi, (uint8_t *)tx_data, length);
}

HAL_StatusTypeDef spi_hal_receive_it(SPI_HandleTypeDef *hspi, uint8_t *rx_data, uint16_t length) {
    if (length > SPI_PACKET_MAX_DATA_SIZE) {
        return HAL_ERROR;
    }
    return HAL_SPI_TransmitReceive_IT(hspi, (uint8_t *)s_dummy_tx, rx_data, length);
}
