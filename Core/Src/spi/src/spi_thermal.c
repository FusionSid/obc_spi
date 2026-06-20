#include "spi_thermal.h"
#include "log.h"
#include "spi_status.h"
#include "spi_transaction.h"

#define SPI_TIMEOUT_MS 1000

spi_status_t thermal_query_ack(spi_handle_t *spi, const spi_cs_config_t *cs) {
    uint8_t rx_data[1];
    uint16_t rx_length;

    return spi_transaction(spi, cs, QUERY_ACKNOWLEDGE, NULL, 0, rx_data, sizeof(rx_data), &rx_length, SPI_TIMEOUT_MS);
}

spi_status_t thermal_query_echo(spi_handle_t *spi, const spi_cs_config_t *cs) {
    uint8_t tx_data[] = {67, 67, 67, 67};

    uint8_t rx_data[sizeof(tx_data)];
    uint16_t rx_length;

    spi_status_t status =
        spi_transaction(spi, cs, QUERY_ECHO, tx_data, sizeof(tx_data), rx_data, sizeof(rx_data), &rx_length, SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("Data that we got back:\r\n");
    log_as_bytes(rx_data, rx_length);
    log_text("\r\n");

    return SPI_WORKED;
}

spi_status_t thermal_query_get_rtd(spi_handle_t *spi, const spi_cs_config_t *cs) {
    uint8_t rx_data[8];
    uint16_t rx_length;

    spi_status_t status = spi_transaction(spi, cs, QUERY_RTD_DATA, NULL, 0, rx_data, sizeof(rx_data), &rx_length, SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    if (rx_length != 8) {
        return SPI_ERR_OVERFLOW;
    }

    log_text("rtd values:\r\n");
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t value = ((uint16_t)rx_data[i * 2] << 8) | ((uint16_t)rx_data[i * 2 + 1]);
        log_printf("value %i: %u\r\n", i + 1, (unsigned int)value);
    }
    log_text("\r\n");

    return SPI_WORKED;
}