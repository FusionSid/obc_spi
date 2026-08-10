// generated for payload THERMAL at time:
#include "queries/spi_thermal_generated.h"
#include "spi_transaction.h"

spi_response_code_t thermal_query_acknowledge(thermal_acknowledge_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ACKNOWLEDGE, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_thermal_acknowledge(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

void serialize_thermal_echo(const thermal_echo_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len) {
    uint16_t offset = 0;
    for (uint16_t i = 0; i < inputs->dataLen; i++) {
        buffer[offset++] = (uint8_t)(inputs->data[i]);
    }
    *buffer_len = offset;
}

spi_response_code_t thermal_query_echo(const thermal_echo_inputs_t *req, thermal_echo_outputs_t *resp) {
    uint8_t tx_buf[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t tx_len = 0;
    serialize_thermal_echo(req, tx_buf, &tx_len);
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ECHO, tx_buf, tx_len, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_thermal_echo(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_rtd_data(thermal_rtd_data_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_RTD_DATA, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_thermal_rtd_data(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}
