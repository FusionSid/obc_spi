#pragma once

#include "queries/spi_camera_generated.h"
#include <stdbool.h>
#include <stdint.h>

#define TAKE_IMG_RETRY_MS 5000u
#define TAKE_IMG_TIMEOUT_MS 30000u

typedef enum {
    IMG_IDLE = 0,
    IMG_HANDSHAKE,
    IMG_HEALTH_CHECK,
    IMG_PARAMS,
    IMG_GET_SIZE,
    IMG_GET_PACKET,
    IMG_SHUTDOWN,
    IMG_DONE,
    IMG_ERROR,
} take_image_state_t;

typedef struct {
    take_image_state_t state;
    uint32_t state_start;
    uint32_t last_try;

    camera_img_params_inputs_t params;
    uint16_t image_size;
    uint16_t bytes_received;
    uint8_t packet_op;

    uint8_t *buf;
    uint32_t buf_cap;
} take_image_ctx_t;

bool take_image(uint8_t *img_buf, uint16_t img_buffer_size, uint16_t *bytes_recieved,
                camera_img_params_inputs_t *params);