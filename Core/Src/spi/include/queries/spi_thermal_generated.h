// generated for payload THERMAL at time:
#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t ack;
} thermal_acknowledge_outputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[676];
} thermal_echo_inputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[676];
} thermal_echo_outputs_t;

typedef struct {
    uint16_t value[4];
} thermal_rtd_data_outputs_t;

// ACKNOWLEDGE (query 0x01)
bool deserialize_thermal_acknowledge(thermal_acknowledge_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len);

spi_response_code_t thermal_query_acknowledge(thermal_acknowledge_outputs_t *resp);

// ECHO (query 0x02) — echo query
void serialize_thermal_echo(const thermal_echo_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len);
bool deserialize_thermal_echo(thermal_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t thermal_query_echo(const thermal_echo_inputs_t *req, thermal_echo_outputs_t *resp);

// RTD_DATA (query 0x03)
bool deserialize_thermal_rtd_data(thermal_rtd_data_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t thermal_query_rtd_data(thermal_rtd_data_outputs_t *resp);
