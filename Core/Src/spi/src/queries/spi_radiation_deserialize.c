#include "queries/spi_radiation_generated.h"
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

#warning "implement HEALTH_CHECK (query 0x01)"
bool deserialize_radiation_health_check(radiation_health_check_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_health_check */
    // TODO: implement — HEALTH_CHECK (query 0x01)
    // this must take buffer and unpack into radiation_health_check_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_health_check */
}
// notes: echo query
#warning "implement ECHO (query 0x02)"
bool deserialize_radiation_echo(radiation_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_echo */
    // TODO: implement — ECHO (query 0x02)
    // this must take buffer and unpack into radiation_echo_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_echo */
}
// notes: dequeues up to 10 timestamps
#warning "implement TIMESTAMP_RAD_DATA (query 0x03)"
bool deserialize_radiation_timestamp_rad_data(radiation_timestamp_rad_data_outputs_t *outputs, const uint8_t *buffer,
                                              uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_timestamp_rad_data */
    // TODO: implement — TIMESTAMP_RAD_DATA (query 0x03)
    // this must take buffer and unpack into radiation_timestamp_rad_data_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_timestamp_rad_data */
}
#warning "implement RAW_ADC_DATA (query 0x04)"
bool deserialize_radiation_raw_adc_data(radiation_raw_adc_data_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_raw_adc_data */
    // TODO: implement — RAW_ADC_DATA (query 0x04)
    // this must take buffer and unpack into radiation_raw_adc_data_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_raw_adc_data */
}
#warning "implement READ_TEMP (query 0x05)"
bool deserialize_radiation_read_temp(radiation_read_temp_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_read_temp */
    // TODO: implement — READ_TEMP (query 0x05)
    // this must take buffer and unpack into radiation_read_temp_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_read_temp */
}
#warning "implement READ_EVENT_STATS (query 0x06)"
bool deserialize_radiation_read_event_stats(radiation_read_event_stats_outputs_t *outputs, const uint8_t *buffer,
                                            uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_read_event_stats */
    // TODO: implement — READ_EVENT_STATS (query 0x06)
    // this must take buffer and unpack into radiation_read_event_stats_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_read_event_stats */
}
#warning "implement SET_SCHMITT_CALIBRATION (query 0x07)"
bool deserialize_radiation_set_schmitt_calibration(radiation_set_schmitt_calibration_outputs_t *outputs,
                                                   const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_set_schmitt_calibration */
    // TODO: implement — SET_SCHMITT_CALIBRATION (query 0x07)
    // this must take buffer and unpack into radiation_set_schmitt_calibration_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_set_schmitt_calibration */
}
#warning "implement LOG_EVENT (query 0x08)"
bool deserialize_radiation_log_event(radiation_log_event_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_log_event */
    // TODO: implement — LOG_EVENT (query 0x08)
    // this must take buffer and unpack into radiation_log_event_outputs_t.
    (void)outputs;
    (void)buffer;
    (void)buffer_len;
    return false;

    /* USER CODE END deserialize_radiation_log_event */
}