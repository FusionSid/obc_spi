// generated for payload RADIATION at time:
#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool health;
} radiation_health_check_outputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4090];
} radiation_echo_inputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4090];
} radiation_echo_outputs_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} radiation_timestamp_rad_data_outputs_t;

typedef struct {
    uint32_t adc_raw;
} radiation_raw_adc_data_outputs_t;

typedef struct {
    uint32_t temperature;
    bool i2c_status;
} radiation_read_temp_outputs_t;

typedef struct {
    uint32_t event_count;
    uint32_t gamma_event_count;
    uint32_t alpha_event_count;
    uint32_t beta_event_count;
} radiation_read_event_stats_outputs_t;

typedef struct {
    uint32_t VTH_Rise;
    uint32_t VTL_Rise;
    uint32_t VTH_Fall;
    uint32_t VTL_Fall;
} radiation_set_schmitt_calibration_inputs_t;

typedef struct {
    uint32_t VTH_Rise;
    uint32_t VTL_Rise;
    uint32_t VTH_Fall;
    uint32_t VTL_Fall;
} radiation_set_schmitt_calibration_outputs_t;

typedef struct {
    bool succesful_logging;
} radiation_log_event_outputs_t;

// HEALTH_CHECK (query 0x01)
bool deserialize_radiation_health_check(radiation_health_check_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len);

spi_response_code_t radiation_query_health_check(radiation_health_check_outputs_t *resp);

// ECHO (query 0x02) — echo query
void serialize_radiation_echo(const radiation_echo_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len);
bool deserialize_radiation_echo(radiation_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t radiation_query_echo(const radiation_echo_inputs_t *req, radiation_echo_outputs_t *resp);

// TIMESTAMP_RAD_DATA (query 0x03) — dequeues up to 10 timestamps
bool deserialize_radiation_timestamp_rad_data(radiation_timestamp_rad_data_outputs_t *outputs, const uint8_t *buffer,
                                              uint16_t buffer_len);

spi_response_code_t radiation_query_timestamp_rad_data(radiation_timestamp_rad_data_outputs_t *resp);

// RAW_ADC_DATA (query 0x04)
bool deserialize_radiation_raw_adc_data(radiation_raw_adc_data_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len);

spi_response_code_t radiation_query_raw_adc_data(radiation_raw_adc_data_outputs_t *resp);

// READ_TEMP (query 0x05)
bool deserialize_radiation_read_temp(radiation_read_temp_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len);

spi_response_code_t radiation_query_read_temp(radiation_read_temp_outputs_t *resp);

// READ_EVENT_STATS (query 0x06)
bool deserialize_radiation_read_event_stats(radiation_read_event_stats_outputs_t *outputs, const uint8_t *buffer,
                                            uint16_t buffer_len);

spi_response_code_t radiation_query_read_event_stats(radiation_read_event_stats_outputs_t *resp);

// SET_SCHMITT_CALIBRATION (query 0x07)
void serialize_radiation_set_schmitt_calibration(const radiation_set_schmitt_calibration_inputs_t *inputs,
                                                 uint8_t *buffer, uint16_t *buffer_len);
bool deserialize_radiation_set_schmitt_calibration(radiation_set_schmitt_calibration_outputs_t *outputs,
                                                   const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t radiation_query_set_schmitt_calibration(const radiation_set_schmitt_calibration_inputs_t *req,
                                                            radiation_set_schmitt_calibration_outputs_t *resp);

// LOG_EVENT (query 0x08)
bool deserialize_radiation_log_event(radiation_log_event_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len);

spi_response_code_t radiation_query_log_event(radiation_log_event_outputs_t *resp);
