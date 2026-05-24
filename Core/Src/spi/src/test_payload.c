#include "test_payload.h"
#include "log.h"

spi_status_t query_ack(SPI_HandleTypeDef *hspi) {
    spi_query_code_t rx_query;

    uint8_t data_out[1];
    uint16_t data_length_out;

    return spi_transaction(hspi, QUERY_ACKNOWLEDGE, NULL, 0, &rx_query, data_out, &data_length_out, 1, SPI_TIMEOUT_MS);
}

spi_status_t query_echo(SPI_HandleTypeDef *hspi) {
    spi_query_code_t rx_query;

    uint8_t send_data[] = {67, 67, 67, 67};
    uint16_t data_sending_len = sizeof(send_data);

    uint8_t data_out[67];
    uint16_t data_length_out;

    spi_status_t status = spi_transaction(hspi, QUERY_ECHO, send_data, data_sending_len, &rx_query, data_out,
                                          &data_length_out, 67, SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("Data that we got back:\n");
    log_as_bytes(data_out, data_length_out);
    log_text("\n");

    return SPI_WORKED;
}

spi_status_t query_getRTD(SPI_HandleTypeDef *hspi) {
    spi_query_code_t rx_query;

    uint8_t data_out[8];
    uint16_t data_length_out;

    spi_status_t status = spi_transaction(hspi, QUERY_ECHO, NULL, 0, &rx_query, data_out, &data_length_out,
                                          sizeof(data_out), SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("Data that we got back:\n");
    log_as_bytes(data_out, data_length_out);
    log_text("\n");

    return SPI_WORKED;
}
