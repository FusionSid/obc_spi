#pragma once

#include "spi_transaction.h"

typedef struct {
    uint8_t ack;
} thermal_ack_response_t;

typedef struct {
    uint8_t *data;
    uint16_t length;
} thermal_echo_request_t;

typedef struct {
    uint8_t data[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t length;
} thermal_echo_response_t;

typedef struct {
    uint16_t value[4];
} thermal_rtd_response_t;

spi_response_code_t thermal_query_ack(thermal_ack_response_t *resp);
spi_response_code_t thermal_query_echo(const thermal_echo_request_t *req, thermal_echo_response_t *resp);
spi_response_code_t thermal_query_get_rtd(thermal_rtd_response_t *resp);

spi_response_code_t thermal_query_ack2();
spi_response_code_t thermal_query_echo2();
spi_response_code_t thermal_query_get_rtd2();