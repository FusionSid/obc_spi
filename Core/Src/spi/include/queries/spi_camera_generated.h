// generated for payload CAMERA at time:
#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t cam_chosen;
    uint16_t width;
    uint16_t height;
    uint8_t quality;
} camera_img_params_outputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4000];
} camera_send_image_packet_inputs_t;

// STARTUP (query 0x00) — empty handshake

spi_response_code_t camera_query_startup(void);

// IMG_PARAMS (query 0x01)
bool deserialize_camera_img_params(camera_img_params_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t camera_query_img_params(camera_img_params_outputs_t *resp);

// GET_SIZE (query 0x03)

spi_response_code_t camera_query_get_size(void);

// SEND_IMAGE_PACKET (query 0x04) — up to 4000 bytes of image data
void serialize_camera_send_image_packet(const camera_send_image_packet_inputs_t *inputs, uint8_t *buffer,
                                        uint16_t *buffer_len);

spi_response_code_t camera_query_send_image_packet(const camera_send_image_packet_inputs_t *req);
