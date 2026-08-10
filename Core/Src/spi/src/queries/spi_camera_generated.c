// generated for payload CAMERA at time:
#include "queries/spi_camera_generated.h"
#include "spi_transaction.h"

spi_response_code_t camera_query_startup() {
    spi_packet_t response;
    spi_response_code_t status = spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_STARTUP, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    // expected no response but got one
    if (response.length != 0) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t camera_query_img_params(camera_img_params_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_IMG_PARAMS, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_camera_img_params(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t camera_query_get_size() {
    spi_packet_t response;
    spi_response_code_t status = spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_GET_SIZE, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    // expected no response but got one
    if (response.length != 0) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

void serialize_camera_send_image_packet(const camera_send_image_packet_inputs_t *inputs, uint8_t *buffer,
                                        uint16_t *buffer_len) {
    uint16_t offset = 0;
    for (uint16_t i = 0; i < inputs->dataLen; i++) {
        buffer[offset++] = (uint8_t)(inputs->data[i]);
    }
    *buffer_len = offset;
}

spi_response_code_t camera_query_send_image_packet(const camera_send_image_packet_inputs_t *req) {
    uint8_t tx_buf[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t tx_len = 0;
    serialize_camera_send_image_packet(req, tx_buf, &tx_len);
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_SEND_IMAGE_PACKET, tx_buf, tx_len, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    // expected no response but got one
    if (response.length != 0) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}
