// generated for payload RADIATION at time:
#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t ack;
} radiation_acknowledge_outputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4090];
} radiation_echo_inputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4090];
} radiation_echo_outputs_t;

typedef struct {
    uint16_t dataLen;
    uint32_t timestamps[10];
} radiation_event_timestamps_outputs_t;

typedef struct {
    uint16_t value;
} radiation_adc_value_outputs_t;

// ACKNOWLEDGE (query 0x01)
bool deserialize_radiation_acknowledge(radiation_acknowledge_outputs_t *outputs, const uint8_t *buffer,
                                       uint16_t buffer_len);

spi_response_code_t radiation_query_acknowledge(radiation_acknowledge_outputs_t *resp);

// ECHO (query 0x02) — echo query
void serialize_radiation_echo(const radiation_echo_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len);
bool deserialize_radiation_echo(radiation_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t radiation_query_echo(const radiation_echo_inputs_t *req, radiation_echo_outputs_t *resp);

// EVENT_TIMESTAMPS (query 0x03) — dequeues up to 10 timestamps
bool deserialize_radiation_event_timestamps(radiation_event_timestamps_outputs_t *outputs, const uint8_t *buffer,
                                            uint16_t buffer_len);

spi_response_code_t radiation_query_event_timestamps(radiation_event_timestamps_outputs_t *resp);

// ADC_VALUE (query 0x04)
bool deserialize_radiation_adc_value(radiation_adc_value_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len);

spi_response_code_t radiation_query_adc_value(radiation_adc_value_outputs_t *resp);
