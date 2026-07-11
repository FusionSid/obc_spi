#pragma once

#include "spi_fsm.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdint.h>

typedef enum {
    SPI_RESP_OK = 0,
    SPI_RESP_DEVICE_ERROR,
    SPI_RESP_TIMEOUT,
    SPI_RESP_BAD_CRC,
    SPI_RESP_BUS_ERROR,
    SPI_RESP_BUSY,
    SPI_RESP_INVALID_ARGS,
} spi_response_code_t;

typedef struct {
    spi_cs_config_t cs;
    uint32_t start_byte_timeout_ms;
    uint8_t max_send_retries;
} spi_device_config_t;

spi_status_t spi_service_init(SPI_HandleTypeDef *hspi, const spi_device_config_t *device_table, uint8_t device_count);
spi_response_code_t spi_transact(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                 uint8_t out_data[SPI_PACKET_MAX_DATA_SIZE], uint16_t *out_len);