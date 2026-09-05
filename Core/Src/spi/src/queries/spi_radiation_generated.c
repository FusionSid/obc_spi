// generated for payload RADIATION at time:
#include "queries/spi_radiation_generated.h"
#include "spi_transaction.h"

spi_response_code_t radiation_query_health_check(radiation_health_check_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_HEALTH_CHECK, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_health_check(resp, response.data, response.length)) {
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

spi_response_code_t radiation_query_timestamp_rad_data(radiation_timestamp_rad_data_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_TIMESTAMP_RAD_DATA, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_timestamp_rad_data(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_raw_adc_data(radiation_raw_adc_data_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_RAW_ADC_DATA, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_raw_adc_data(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_read_temp(radiation_read_temp_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_READ_TEMP, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_read_temp(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_read_event_stats(radiation_read_event_stats_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_READ_EVENT_STATS, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_read_event_stats(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

void serialize_radiation_set_schmitt_calibration(const radiation_set_schmitt_calibration_inputs_t *inputs,
                                                 uint8_t *buffer, uint16_t *buffer_len) {
    uint16_t offset = 0;
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Rise)) >> 0);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Rise)) >> 8);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Rise)) >> 16);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Rise)) >> 24);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Rise)) >> 0);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Rise)) >> 8);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Rise)) >> 16);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Rise)) >> 24);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Fall)) >> 0);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Fall)) >> 8);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Fall)) >> 16);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTH_Fall)) >> 24);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Fall)) >> 0);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Fall)) >> 8);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Fall)) >> 16);
    buffer[offset++] = (uint8_t)(((uint32_t)(inputs->VTL_Fall)) >> 24);
    *buffer_len = offset;
}

spi_response_code_t radiation_query_set_schmitt_calibration(const radiation_set_schmitt_calibration_inputs_t *req,
                                                            radiation_set_schmitt_calibration_outputs_t *resp) {
    uint8_t tx_buf[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t tx_len = 0;
    serialize_radiation_set_schmitt_calibration(req, tx_buf, &tx_len);
    spi_packet_t response;
    spi_response_code_t status = spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_SET_SCHMITT_CALIBRATION,
                                              tx_buf, tx_len, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_set_schmitt_calibration(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}

spi_response_code_t radiation_query_log_event(radiation_log_event_outputs_t *resp) {
    spi_packet_t response;
    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_LOG_EVENT, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (!deserialize_radiation_log_event(resp, response.data, response.length)) {
        return SPI_RESP_INVALID_ARGS;
    }

    return SPI_RESP_OK;
}
