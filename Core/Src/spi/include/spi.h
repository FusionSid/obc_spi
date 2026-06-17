#pragma once

#include <stdint.h>

#include "stm32h7xx_hal.h"

#define CS_Pin GPIO_PIN_14
#define CS_GPIO_Port GPIOD
#define SPI_TIMEOUT_MS 100
#define SPI_BYTE_TIMEOUT_MS 50

#define QUERY_LIST                                                                                                     \
    X(QUERY_NOT_USED)                                                                                                  \
    X(QUERY_ACKNOWLEDGE)                                                                                               \
    X(QUERY_ECHO)                                                                                                      \
    X(QUERY_RTD_DATA)                                                                                                  \
    X(QUERY_THERMISTOR_DATA)                                                                                           \
    X(QUERY_TIME)                                                                                                      \
    X(QUERY_STORE_ERRORS)                                                                                              \
    X(QUERY_SEND_ERRORS)

#define SPI_STATUS_LIST                                                                                                \
    X(SPI_WORKED)                                                                                                      \
    X(SPI_ERR_HAL)                                                                                                     \
    X(SPI_ERR_TIMEDOUT)                                                                                                \
    X(SPI_ERR_CRCBAD)                                                                                                  \
    X(SPI_ERR_OVERFLOW)                                                                                                \
    X(SPI_ERR_BUILD_FAIL)

#define X(name) name,
typedef enum { QUERY_LIST } spi_query_code_t;
typedef enum { SPI_STATUS_LIST } spi_status_t;
#undef X

static inline void spi_cs_low() { HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); };

static inline void spi_cs_high() { HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); };

spi_status_t spi_receive_packet(SPI_HandleTypeDef *hspi, uint8_t *data_out, uint16_t *data_length_out,
                                uint16_t data_buf_size, uint32_t timeout_ms);

spi_status_t spi_transaction(SPI_HandleTypeDef *hspi, spi_query_code_t query_code, const uint8_t *sending_data,
                             uint16_t sending_data_length, uint8_t *recieved_data, uint16_t *recieved_data_length,
                             uint16_t recieving_buffer_size, uint32_t timeout_ms);

static inline const char *spi_query_to_string(spi_query_code_t query) {
    switch (query) {
#define X(name)                                                                                                        \
    case name:                                                                                                         \
        return #name;
        QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}

static inline const char *spi_status_to_string(spi_status_t status) {
    switch (status) {
#define X(name)                                                                                                        \
    case name:                                                                                                         \
        return #name;
        SPI_STATUS_LIST
#undef X
    default:
        return "invalid status type";
    }
}
