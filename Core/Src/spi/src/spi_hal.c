#include "spi_hal.h"
#include "spi_packet.h"

#include <string.h>

#define SPI_BUS_HAL_PLACEHOLDER_BYTE (0)
// only adding this placeholder stuff cause I saw in apss 2 code that the rx and tx buffers are shared
// so this is like the equivalent of the transmit no garbage part
static const uint8_t s_dummy_tx[SPI_PACKET_MAX_DATA_SIZE] = {SPI_BUS_HAL_PLACEHOLDER_BYTE};

static SPI_HandleTypeDef s_hspi;

static spi_status_t mx_spi_init(void);
static void (*s_tx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_rxtx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_error_cb)(SPI_HandleTypeDef *hspi);

SPI_HandleTypeDef *spi_hal_get_handle(void) { return &s_hspi; }

void spi_device_select(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_RESET); }
void spi_device_deselect(const spi_cs_config_t *cs) { HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_SET); }

static spi_status_t register_callbacks() {
    if (HAL_SPI_RegisterCallback(&s_hspi, HAL_SPI_TX_COMPLETE_CB_ID, s_tx_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    if (HAL_SPI_RegisterCallback(&s_hspi, HAL_SPI_TX_RX_COMPLETE_CB_ID, s_rxtx_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    if (HAL_SPI_RegisterCallback(&s_hspi, HAL_SPI_ERROR_CB_ID, s_error_cb) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    return SPI_WORKED;
}

spi_status_t spi_hal_init(void (*tx_cb)(SPI_HandleTypeDef *hspi), void (*rxtx_cb)(SPI_HandleTypeDef *hspi),
                          void (*err_cb)(SPI_HandleTypeDef *hspi)) {
    if (tx_cb == NULL || rxtx_cb == NULL || err_cb == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    spi_status_t mx_status = mx_spi_init();
    if (mx_status != SPI_WORKED) {
        return mx_status;
    }

    s_tx_cb = tx_cb;
    s_rxtx_cb = rxtx_cb;
    s_error_cb = err_cb;

    return register_callbacks();
}

void spi_hal_cs_init(const spi_cs_config_t *cs) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(cs->cs_port, cs->cs_pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = cs->cs_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(cs->cs_port, &GPIO_InitStruct);
}

spi_status_t spi_hal_reinit(SPI_HandleTypeDef *hspi) {
    if (hspi == NULL || hspi != &s_hspi) {
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

static spi_status_t mx_spi_init(void) {

    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */
    /* Configure PLL3P = 32 MHz (HSI 64 / PLLM 4 * PLLN 10 / PLLP 5)
     * so that SPI1 kernel clock = PLL3P / 256 = 125 kHz */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI123;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
    PeriphClkInitStruct.PLL3.PLL3M = 4;
    PeriphClkInitStruct.PLL3.PLL3N = 10;
    PeriphClkInitStruct.PLL3.PLL3P = 5;
    PeriphClkInitStruct.PLL3.PLL3Q = 1;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    s_hspi.Instance = SPI1;
    s_hspi.Init.Mode = SPI_MODE_MASTER;
    s_hspi.Init.Direction = SPI_DIRECTION_2LINES;
    s_hspi.Init.DataSize = SPI_DATASIZE_8BIT;
    s_hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    s_hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
    s_hspi.Init.NSS = SPI_NSS_SOFT;
    s_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    s_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    s_hspi.Init.TIMode = SPI_TIMODE_DISABLE;
    s_hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    s_hspi.Init.CRCPolynomial = 0x1021;
    s_hspi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    s_hspi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    s_hspi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    s_hspi.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    s_hspi.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    s_hspi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    s_hspi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    s_hspi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    s_hspi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    s_hspi.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&s_hspi) != HAL_OK) {
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
}

// SPI1_IRQHandler should call this guy:
void spi_irq_handler(void) { HAL_SPI_IRQHandler(&s_hspi); }