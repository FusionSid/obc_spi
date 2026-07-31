#pragma once

#include "spi_fsm.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdint.h>

typedef struct {
    spi_cs_config_t cs;
    uint32_t start_byte_timeout_ms;
    uint8_t max_send_retries;
} spi_device_config_t;

spi_status_t spi_service_init(SPI_HandleTypeDef *hspi, const spi_device_config_t *device_table, uint8_t device_count);
spi_response_code_t spi_transact(spi_payload_t device, uint8_t query_code, const uint8_t *data, uint16_t data_len,
                                 spi_packet_t *response, bool expects_response);