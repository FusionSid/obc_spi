// this is sorta a test (which im doing with the thermal payload) to see if the spi stuff works, i'll rewrite it later

#include "test_payload.h"
#include "log.h"

spi_status_t query_ack(SPI_HandleTypeDef *hspi) {
    uint8_t data_out[1];
    uint16_t data_length_out;

    return spi_transaction(hspi, QUERY_ACKNOWLEDGE, NULL, 0, data_out, &data_length_out, sizeof(data_out),
                           SPI_TIMEOUT_MS);
}

spi_status_t query_echo(SPI_HandleTypeDef *hspi) {
    uint8_t send_data[] = {67, 67, 67, 67};

    uint8_t data_out[sizeof(send_data)];
    uint16_t data_length_out;

    spi_status_t status = spi_transaction(hspi, QUERY_ECHO, send_data, sizeof(send_data), data_out, &data_length_out,
                                          sizeof(data_out), SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("Data that we got back:\n");
    log_as_bytes(data_out, data_length_out);
    log_text("\n");

    return SPI_WORKED;
}

spi_status_t query_getRTD(SPI_HandleTypeDef *hspi) {
    uint8_t data_out[8];
    uint16_t data_length_out;

    spi_status_t status =
        spi_transaction(hspi, QUERY_RTD_DATA, NULL, 0, data_out, &data_length_out, sizeof(data_out), SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("rtd values:\r\n");
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t value = ((uint16_t)data_out[i * 2] << 8) | ((uint16_t)data_out[i * 2 + 1]);
        log_printf("value %i: %u\r\n", i + 1, (unsigned int)value);
    }
    log_text("\r\n");

    return SPI_WORKED;
}

spi_status_t query_getADC(SPI_HandleTypeDef *hspi) {
    // adc count is 8 i think from the adc.h header file
    uint8_t data_out[8 * 2];
    uint16_t data_length_out;

    spi_status_t status = spi_transaction(hspi, QUERY_THERMISTOR_DATA, NULL, 0, data_out, &data_length_out,
                                          sizeof(data_out), SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    log_text("ADC data:\r\n");
    log_as_bytes(data_out, data_length_out);

    return SPI_WORKED;
}

spi_status_t query_getRTC(SPI_HandleTypeDef *hspi) {
    uint8_t data_out[4];
    uint16_t data_length_out;

    spi_status_t status =
        spi_transaction(hspi, QUERY_TIME, NULL, 0, data_out, &data_length_out, sizeof(data_out), SPI_TIMEOUT_MS);

    if (status != SPI_WORKED) {
        return status;
    }

    uint32_t seconds = (uint32_t)data_out[0] | ((uint32_t)data_out[1] << 8) | ((uint32_t)data_out[2] << 16) |
                       ((uint32_t)data_out[3] << 24);
    log_printf("RTC Seconds: %lu\r\n", (unsigned long)seconds);

    return SPI_WORKED;
}