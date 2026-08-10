// generated for payload CAMERA at time:
#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t status;
} camera_health_check_outputs_t;

typedef struct {
    uint8_t cam_chosen;
    uint16_t width;
    uint16_t height;
    uint8_t quality;
} camera_img_params_inputs_t;

typedef struct {
    uint16_t image_size;
} camera_get_size_outputs_t;

typedef struct {
    uint8_t packet_operation;
} camera_get_packet_inputs_t;

typedef struct {
    uint16_t dataLen;
    uint8_t data[4000];
} camera_get_packet_outputs_t;

// HANDSHAKE (query 0x00) — To get the pi going

spi_response_code_t camera_query_handshake(void);

// HEALTH_CHECK (query 0x01) — See receive state machine
bool deserialize_camera_health_check(camera_health_check_outputs_t *outputs, const uint8_t *buffer,
                                     uint16_t buffer_len);

spi_response_code_t camera_query_health_check(camera_health_check_outputs_t *resp);

// IMG_PARAMS (query 0x02) — Cam chosen is 0 or 1, width and height in pixels, quality in percentage.
void serialize_camera_img_params(const camera_img_params_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len);

spi_response_code_t camera_query_img_params(const camera_img_params_inputs_t *req);

// GET_SIZE (query 0x03)
bool deserialize_camera_get_size(camera_get_size_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t camera_query_get_size(camera_get_size_outputs_t *resp);

// GET_PACKET (query 0x04) — 0 = get next packet, 1 = resend packet
void serialize_camera_get_packet(const camera_get_packet_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len);
bool deserialize_camera_get_packet(camera_get_packet_outputs_t *outputs, const uint8_t *buffer, uint16_t buffer_len);

spi_response_code_t camera_query_get_packet(const camera_get_packet_inputs_t *req, camera_get_packet_outputs_t *resp);

// SHUTDOWN (query 0x05) — Pi will shut itself down before OBC shuts down whole payload

spi_response_code_t camera_query_shutdown(void);
