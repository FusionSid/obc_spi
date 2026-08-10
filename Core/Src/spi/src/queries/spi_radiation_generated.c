// generated for payload RADIATION at time:
#include "queries/spi_radiation_generated.h"
#include "spi_transaction.h"

spi_response_code_t radiation_query_acknowledge(radiation_acknowledge_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ACKNOWLEDGE, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_acknowledge(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

void serialize_radiation_echo(const radiation_echo_inputs_t *inputs, uint8_t *buffer, uint16_t *buffer_len) {
    uint16_t offset = 0;
    for (uint16_t i = 0; i < inputs->dataLen; i++) {
        buffer[offset++] = (uint8_t)(inputs->data[i]);
    }
    *buffer_len = offset;
}

spi_response_code_t radiation_query_echo(const radiation_echo_inputs_t *req, radiation_echo_outputs_t *resp) {
    uint8_t tx_buf[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t tx_len = 0;
    serialize_radiation_echo(req, tx_buf, &tx_len);
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ECHO, tx_buf, tx_len, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_echo(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_event_timestamps(radiation_event_timestamps_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_EVENT_TIMESTAMPS, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_event_timestamps(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_adc_value(radiation_adc_value_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ADC_VALUE, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_adc_value(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}
