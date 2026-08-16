#include "queries/take_image.h"
#include "main.h"
#include <string.h>

static void take_image_start(take_image_ctx_t *ctx, const camera_img_params_inputs_t *params, uint8_t *buf, uint32_t buf_cap) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->params = *params;
    ctx->buf = buf;
    ctx->buf_cap = buf_cap;
    ctx->state = IMG_HANDSHAKE;
    ctx->state_start = HAL_GetTick();
}

static bool havent_timeout_yet(take_image_ctx_t *ctx, uint32_t now) {
    if (now - ctx->state_start > TAKE_IMG_TIMEOUT_MS) {
        ctx->state = IMG_ERROR;
        return false;
    }

    if (now - ctx->last_try < TAKE_IMG_RETRY_MS) {
        return false;
    }

    ctx->last_try = now;
    return true;
}

static void next_state(take_image_ctx_t *ctx, take_image_state_t s) {
    ctx->state = s;
    ctx->state_start = HAL_GetTick();
    ctx->last_try = 0;
}

static bool take_image_tick(take_image_ctx_t *ctx) {
    uint32_t now = HAL_GetTick();

    switch (ctx->state) {

    case IMG_HANDSHAKE:
        if (havent_timeout_yet(ctx, now) && camera_query_handshake() == SPI_RESP_OK) {
            next_state(ctx, IMG_HEALTH_CHECK);
        }
        break;

    case IMG_HEALTH_CHECK:
        if (havent_timeout_yet(ctx, now)) {
            camera_health_check_outputs_t hc;
            if (camera_query_health_check(&hc) == SPI_RESP_OK && hc.status == 0) {
                next_state(ctx, IMG_PARAMS);
            }
        }
        break;

    case IMG_PARAMS:
        if (havent_timeout_yet(ctx, now) && camera_query_img_params(&ctx->params) == SPI_RESP_OK) {
            next_state(ctx, IMG_GET_SIZE);
        }
        break;

    case IMG_GET_SIZE:
        if (havent_timeout_yet(ctx, now)) {
            camera_get_size_outputs_t sz;
            if (camera_query_get_size(&sz) == SPI_RESP_OK && sz.image_size > 0 && sz.image_size <= ctx->buf_cap) {
                ctx->image_size = sz.image_size;
                ctx->bytes_received = 0;
                ctx->packet_op = 0;
                next_state(ctx, IMG_GET_PACKET);
            }
        }
        break;

    case IMG_GET_PACKET:
        if (havent_timeout_yet(ctx, now)) {
            camera_get_packet_inputs_t req = {.packet_operation = ctx->packet_op};
            camera_get_packet_outputs_t pkt;

            if (camera_query_get_packet(&req, &pkt) == SPI_RESP_OK && pkt.dataLen > 0 &&
                ctx->bytes_received + pkt.dataLen <= ctx->buf_cap) {
                memcpy(ctx->buf + ctx->bytes_received, pkt.data, pkt.dataLen);
                ctx->bytes_received += pkt.dataLen;
                ctx->packet_op = 0;

                if (ctx->bytes_received >= ctx->image_size) {
                    next_state(ctx, IMG_SHUTDOWN);
                } else {
                    ctx->state_start = now;
                    ctx->last_try = now;
                }
            } else {
                ctx->packet_op = 1;
            }
        }
        break;

    case IMG_SHUTDOWN:
        if (havent_timeout_yet(ctx, now) && camera_query_shutdown() == SPI_RESP_OK) {
            next_state(ctx, IMG_DONE);
        }
        break;

    case IMG_DONE:
    case IMG_ERROR:
    default:
        break;
    }

    return (ctx->state == IMG_DONE || ctx->state == IMG_ERROR);
}

bool take_image(uint8_t *img_buf, uint16_t img_buffer_size, uint16_t *bytes_recieved,
                camera_img_params_inputs_t *params) {
    take_image_ctx_t img_ctx;

    take_image_start(&img_ctx, params, img_buf, img_buffer_size);
    while (1) {
        if (take_image_tick(&img_ctx)) {
            if (img_ctx.state == IMG_DONE) {
                *bytes_recieved = img_ctx.bytes_received;
                return true;
            }
            return false;
        }
    }
}