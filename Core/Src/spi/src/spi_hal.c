#include "spi_hal.h"
#include "spi_packet.h"

#include <string.h>

#define SPI_BUS_HAL_PLACEHOLDER_BYTE (0)
// only adding this placeholder stuff cause I saw in apss 2 code that the rx and tx buffers are shared
// so this is like the equivalent of the transmit no garbage part
static const uint8_t s_dummy_tx[SPI_PACKET_MAX_DATA_SIZE] = {SPI_BUS_HAL_PLACEHOLDER_BYTE};

static SPI_HandleTypeDef s_hspi1;
static SPI_HandleTypeDef *s_hspi;

static spi_status_t mx_spi1_init(void);
static void (*s_tx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_rxtx_cb)(SPI_HandleTypeDef *hspi);
static void (*s_error_cb)(SPI_HandleTypeDef *hspi);

SPI_HandleTypeDef *spi_hal_get_handle(void) { return &s_hspi1; }

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

spi_status_t spi_hal_init(void (*tx_cb)(SPI_HandleTypeDef *hspi), void (*rxtx_cb)(SPI_HandleTypeDef *hspi),
                          void (*err_cb)(SPI_HandleTypeDef *hspi)) {
    if (tx_cb == NULL || rxtx_cb == NULL || err_cb == NULL) {
        return SPI_ERR_INVALID_ARGS;
    }

    spi_status_t mx_status = mx_spi1_init();
    if (mx_status != SPI_WORKED) {
        return mx_status;
    }

    s_hspi = &s_hspi1;
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

// sort of a copy of the error handler from main.h
static void spi_hal_error(void) {
    __disable_irq();
    while (1) {
    }
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static spi_status_t mx_spi1_init(void) {

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
        spi_hal_error();
        return SPI_ERR_HAL;
    }
    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    s_hspi1.Instance = SPI1;
    s_hspi1.Init.Mode = SPI_MODE_MASTER;
    s_hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    s_hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    s_hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    s_hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    s_hspi1.Init.NSS = SPI_NSS_SOFT;
    s_hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    s_hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    s_hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    s_hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    s_hspi1.Init.CRCPolynomial = 0x1021;
    s_hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    s_hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    s_hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    s_hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    s_hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    s_hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    s_hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    s_hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    s_hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    s_hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&s_hspi1) != HAL_OK) {
        spi_hal_error();
        return SPI_ERR_HAL;
    }

    return SPI_WORKED;
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}