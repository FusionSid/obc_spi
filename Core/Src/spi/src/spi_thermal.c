#include "spi_thermal.h"
#include "log.h"
#include "spi_query.h"
#include "spi_transaction.h"

spi_response_code_t thermal_query_ack(void) {
    uint8_t rx_data[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t rx_length;
    return spi_bus_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ACKNOWLEDGE, NULL, 0, rx_data, &rx_length);
}

spi_response_code_t thermal_query_echo(void) {
    uint8_t tx_data[] = {67, 67, 67, 67};
    uint8_t rx_data[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t rx_length;

    spi_response_code_t status =
        spi_bus_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ECHO, tx_data, sizeof(tx_data), rx_data, &rx_length);

    if (status != SPI_RESP_OK) {
        return status;
    }

    log_text("Data that we got back:\r\n");
    log_as_bytes(rx_data, rx_length);
    log_text("\r\n");

    return SPI_RESP_OK;
}

spi_response_code_t thermal_query_get_rtd(void) {
    uint8_t rx_data[SPI_PACKET_MAX_DATA_SIZE];
    uint16_t rx_length;

    spi_response_code_t status =
        spi_bus_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_RTD_DATA, NULL, 0, rx_data, &rx_length);

    if (status != SPI_RESP_OK) {
        return status;
    }

    if (rx_length != 8) {
        return SPI_RESP_INVALID_ARGS;
    }

    log_text("rtd values:\r\n");
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t value = ((uint16_t)rx_data[i * 2] << 8) | ((uint16_t)rx_data[i * 2 + 1]);
        log_printf("value %i: %u\r\n", i + 1, (unsigned int)value);
    }
    log_text("\r\n");

    return SPI_RESP_OK;
}