#include "queries/spi_thermal_generated.h"
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

bool deserialize_thermal_acknowledge(thermal_acknowledge_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_thermal_acknowledge */
    if (buffer_len != 1) {
        return false;
    }
    outputs->ack = buffer[0];
    return true;

    /* USER CODE END deserialize_thermal_acknowledge */
}
// notes: echo query
bool deserialize_thermal_echo(thermal_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_thermal_echo */
    if (buffer_len > sizeof(outputs->data)) {
        return false;
    }
    memcpy(outputs->data, buffer, buffer_len);
    outputs->dataLen = buffer_len;
    return true;

    /* USER CODE END deserialize_thermal_echo */
}
bool deserialize_thermal_rtd_data(thermal_rtd_data_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    /* USER CODE BEGIN deserialize_thermal_rtd_data */
    if (buffer_len != 8) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        outputs->value[i] = ((uint16_t)buffer[2 * i] << 8) | buffer[2 * i + 1];
    }
    return true;

    /* USER CODE END deserialize_thermal_rtd_data */
}