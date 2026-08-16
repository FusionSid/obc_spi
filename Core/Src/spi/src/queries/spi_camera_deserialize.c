#include "queries/spi_camera_generated.h"
#include <string.h>

bool deserialize_camera_health_check(camera_health_check_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len) {
    if (buffer_len != 1) {
        return false;
    }

    outputs->status = buffer[0];
    return true;
}
bool deserialize_camera_get_size(camera_get_size_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    if (buffer_len != 2) {
        return false;
    }

    outputs->image_size = ((uint16_t)buffer[1] << 8) | buffer[0];
    return true;
}

// notes: 0 = get next packet, 1 = resend packet
bool deserialize_camera_get_packet(camera_get_packet_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len) {
    if (buffer_len > sizeof(outputs->data)) {
        return false;
    }
    memcpy(outputs->data, buffer, buffer_len);
    outputs->dataLen = buffer_len;
    return true;
}