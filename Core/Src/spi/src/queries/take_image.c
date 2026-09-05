#include "queries/take_image.h"
#include "log.h"
#include "main.h"
#include <string.h>

static void take_image_start(take_image_ctx_t *ctx, const camera_img_params_inputs_t *params, uint8_t *buf,
                             uint32_t buf_cap) {
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
        log_printf("Timed out the TOTAL request 30s\r\n");
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
            log_printf("switching to health check state\r\n");
            next_state(ctx, IMG_HEALTH_CHECK); // handshake worked, move to health check
        }
        break;

    case IMG_HEALTH_CHECK:
        if (havent_timeout_yet(ctx, now)) {
            camera_health_check_outputs_t hc;
            if (camera_query_health_check(&hc) == SPI_RESP_OK && hc.status == 0) {
                // health check worked, and status is 0 meaning successfull capture
                log_printf("switching to img params state\r\n");
                next_state(ctx, IMG_PARAMS); // move to img params
            }
        }
        break;

    case IMG_PARAMS:
        if (havent_timeout_yet(ctx, now) && camera_query_img_params(&ctx->params) == SPI_RESP_OK) {
            log_printf("switching to get size state\r\n");
            next_state(ctx, IMG_GET_SIZE); // image parms query worked, moved to get size
        }
        break;

    case IMG_GET_SIZE:
        if (havent_timeout_yet(ctx, now)) {
            camera_get_size_outputs_t sz;
            // get the size of image, and make sure its non zero, and can fit in provided buffer
            if (camera_query_get_size(&sz) == SPI_RESP_OK && sz.image_size > 0 && sz.image_size <= ctx->buf_cap) {
                ctx->image_size = sz.image_size;
                ctx->bytes_received = 0;
                ctx->packet_op = 0; // meaning we want next packet
                log_printf("Switching to get packet state\r\n");
                next_state(ctx, IMG_GET_PACKET); // move to get packet state
            }
        }
        break;

    case IMG_GET_PACKET:
        if (havent_timeout_yet(ctx, now)) {
            camera_get_packet_inputs_t req = {.packet_operation = ctx->packet_op};
            static camera_get_packet_outputs_t pkt;

            log_printf("Requesting Packet, Packet Op: %i\r\n", ctx->packet_op);
            if (camera_query_get_packet(&req, &pkt) == SPI_RESP_OK && pkt.dataLen > 0 &&
                ctx->bytes_received + pkt.dataLen <= ctx->buf_cap) {           // if we successfully got an image packet
                memcpy(ctx->buf + ctx->bytes_received, pkt.data, pkt.dataLen); // copy it into the buffer
                ctx->bytes_received += pkt.dataLen;                            // increment the data recieved
                ctx->packet_op = 0;                                            // next packet

                if (ctx->bytes_received >= ctx->image_size) {
                    log_printf("switching to image shutdown state\r\n");
                    next_state(ctx, IMG_SHUTDOWN);
                } else {
                    ctx->last_try = now;
                }
            } else {
                // as we failed to get the packet switch to requesting a resend of packet
                ctx->packet_op = 1;
            }
        }
        break;

    case IMG_SHUTDOWN:
        if (havent_timeout_yet(ctx, now) && camera_query_shutdown() == SPI_RESP_OK) {
            log_printf("switching to state image done\r\n");
            next_state(ctx, IMG_DONE); // image shutdown success, end take image command
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

    log_printf("Starting Image Request\r\n");

    // setup context struct to start request
    take_image_start(&img_ctx, params, img_buf, img_buffer_size);
    while (1) {
        // run the state machine
        if (take_image_tick(&img_ctx)) {
            if (img_ctx.state == IMG_DONE) {
                *bytes_recieved = img_ctx.bytes_received;
                log_printf("Image Request Completed Successfully\r\n");
                return true;
            }
            *bytes_recieved = 0;
            log_printf("Image Request not completed successfully :(\r\n");
            return false;
        }
    }
}