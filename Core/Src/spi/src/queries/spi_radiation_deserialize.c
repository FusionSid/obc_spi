#include "queries/spi_radiation_generated.h"
/* USER CODE BEGIN Includes */
#include <string.h>

static inline uint16_t rd_u16(const uint8_t *b) { return (uint16_t)b[0] | ((uint16_t)b[1] << 8); }

static inline uint32_t rd_u32(const uint8_t *b) {
    return (uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

/* USER CODE END Includes */

bool deserialize_radiation_health_check(radiation_health_check_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_health_check */
    if (buffer_len != 1) {
        return false;
    }
    outputs->health = (buffer[0] != 0);
    return true;
    /* USER CODE END deserialize_radiation_health_check */
}
// notes: echo query
bool deserialize_radiation_echo(radiation_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_echo */
    if (buffer_len > sizeof(outputs->data)) {
        return false;
    }
    memcpy(outputs->data, buffer, buffer_len);
    outputs->dataLen = buffer_len;
    return true;
    /* USER CODE END deserialize_radiation_echo */
}
// notes: dequeues up to 10 timestamps
bool deserialize_radiation_timestamp_rad_data(radiation_timestamp_rad_data_outputs_t *outputs, const uint8_t *buffer,
                                              uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_timestamp_rad_data */
    if (buffer_len != 7) {
        return false;
    }
    outputs->year = rd_u16(&buffer[0]);
    outputs->month = buffer[2];
    outputs->day = buffer[3];
    outputs->hour = buffer[4];
    outputs->minute = buffer[5];
    outputs->second = buffer[6];
    return true;
    /* USER CODE END deserialize_radiation_timestamp_rad_data */
}
bool deserialize_radiation_raw_adc_data(radiation_raw_adc_data_outputs_t *outputs, const uint8_t *buffer,
                                        uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_raw_adc_data */
    if (buffer_len == 2) {
        outputs->adc_raw = rd_u16(buffer);
    } else if (buffer_len == 4) {
        outputs->adc_raw = rd_u32(buffer);
    } else {
        return false;
    }
    return true;
    /* USER CODE END deserialize_radiation_raw_adc_data */
}
bool deserialize_radiation_read_temp(radiation_read_temp_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_read_temp */
    if (buffer_len == 1) {
        outputs->temperature = 0;
        outputs->i2c_status = false;
        return true;
    }
    if (buffer_len != 5) {
        return false;
    }

    outputs->temperature = (uint32_t)(int32_t)(int16_t)rd_u16(&buffer[2]);
    outputs->i2c_status = (buffer[4] == 0);
    return true;
    /* USER CODE END deserialize_radiation_read_temp */
}
bool deserialize_radiation_read_event_stats(radiation_read_event_stats_outputs_t *outputs, const uint8_t *buffer,
                                            uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_read_event_stats */
    if (buffer_len != 16) {
        return false;
    }
    outputs->event_count = rd_u32(&buffer[0]);
    outputs->gamma_event_count = rd_u32(&buffer[4]);
    outputs->alpha_event_count = rd_u32(&buffer[8]);
    outputs->beta_event_count = rd_u32(&buffer[12]);
    return true;
    /* USER CODE END deserialize_radiation_read_event_stats */
}
bool deserialize_radiation_set_schmitt_calibration(radiation_set_schmitt_calibration_outputs_t *outputs,
                                                   const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_set_schmitt_calibration */
    if (buffer_len != 16) {
        return false;
    }
    outputs->VTH_Rise = rd_u32(&buffer[0]);
    outputs->VTL_Rise = rd_u32(&buffer[4]);
    outputs->VTH_Fall = rd_u32(&buffer[8]);
    outputs->VTL_Fall = rd_u32(&buffer[12]);
    return true;
    /* USER CODE END deserialize_radiation_set_schmitt_calibration */
}
bool deserialize_radiation_log_event(radiation_log_event_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_radiation_log_event */
    if (buffer_len != 1) {
        return false;
    }
    outputs->succesful_logging = (buffer[0] != 0);
    return true;
    /* USER CODE END deserialize_radiation_log_event */
}