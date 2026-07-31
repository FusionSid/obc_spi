#include "spi_thermal.h"
#include "log.h"
#include "spi_query.h"
#include "spi_transaction.h"
#include "string.h"

spi_response_code_t thermal_query_ack(thermal_ack_response_t *resp) {
    spi_packet_t response;

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ACKNOWLEDGE, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (response.length != 1) {
        return SPI_RESP_INVALID_ARGS;
    }

    resp->ack = response.data[0];

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_echo(const thermal_echo_request_t *req, thermal_echo_response_t *resp) {
    spi_packet_t response;

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ECHO, req->data, req->length, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (response.length > sizeof(resp->data)) {
        return SPI_RESP_INVALID_ARGS;
    }

    memcpy(resp->data, response.data, response.length);
    resp->length = response.length;

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_get_rtd(thermal_rtd_response_t *resp) {
    spi_packet_t response;

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_RTD_DATA, NULL, 0, &response, true);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (response.length != 8) {
        return SPI_RESP_INVALID_ARGS;
    }

    for (int i = 0; i < 4; i++) {
        resp->value[i] = ((uint16_t)response.data[2 * i] << 8) | response.data[2 * i + 1];
    }

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_ack2() {
    thermal_ack_response_t resp;

    spi_response_code_t status = thermal_query_ack(&resp);

    if (status != SPI_RESP_OK) {
        log_printf("ACK query failed: %d\r\n", status);
        return status;
    }

    log_printf("ACK: %u\r\n", resp.ack);

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_echo2() {
    thermal_echo_request_t req = {.length = 47};

    memset(req.data, 67, req.length);

    thermal_echo_response_t resp;

    spi_response_code_t status = thermal_query_echo(&req, &resp);

    if (status != SPI_RESP_OK) {
        log_printf("Echo query failed: %d\r\n", status);
        return status;
    }

    log_text("Echo response:\r\n");
    log_as_bytes(resp.data, resp.length);
    log_text("\r\n");

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_get_rtd2() {
    thermal_rtd_response_t resp;

    spi_response_code_t status = thermal_query_get_rtd(&resp);

    if (status != SPI_RESP_OK) {
        log_printf("RTD query failed: %d\r\n", status);
        return status;
    }

    log_text("RTD values:\r\n");

    for (uint8_t i = 0; i < 4; i++) {
        log_printf("value %u: %u\r\n", i + 1, (unsigned int)resp.value[i]);
    }

    log_text("\r\n");

    return SPI_RESP_OK;
}