#include "queries/spi_thermal_generated.h"
#include <string.h>

bool deserialize_thermal_acknowledge(thermal_acknowledge_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    if (buffer_len != 1) {
        return false;
    }
    outputs->ack = buffer[0];
    return true;
}

bool deserialize_thermal_echo(thermal_echo_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    if (buffer_len > sizeof(outputs->data)) {
        return false;
    }
    memcpy(outputs->data, buffer, buffer_len);
    outputs->dataLen = buffer_len;
    return true;
}

bool deserialize_thermal_rtd_data(thermal_rtd_data_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    if (buffer_len != 8) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        outputs->value[i] = ((uint16_t)buffer[2 * i] << 8) | buffer[2 * i + 1];
    }
    return true;
}