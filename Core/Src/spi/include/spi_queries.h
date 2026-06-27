#pragma once
#include "spi_query.h"
#include "spi_transaction.h"

#define SPI_TIMEOUT_MS 1000

static inline spi_status_t spi_thermal_query_acknowledge(const spi_cs_config_t *cs, uint8_t *rx_data,
                                                         uint16_t *rx_length) {
    return spi_transaction(cs, SPI_THERMAL_QUERY_ACKNOWLEDGE, NULL, 0, rx_data, 1, rx_length, SPI_TIMEOUT_MS);
}
static inline spi_status_t spi_thermal_query_echo(const spi_cs_config_t *cs, uint8_t *tx_data, uint8_t *rx_data,
                                                  uint16_t *rx_length) {
    return spi_transaction(cs, SPI_THERMAL_QUERY_ECHO, tx_data, 4, rx_data, 4, rx_length, SPI_TIMEOUT_MS);
}
static inline spi_status_t spi_thermal_query_rtd_data(const spi_cs_config_t *cs, uint8_t *rx_data,
                                                      uint16_t *rx_length) {
    return spi_transaction(cs, SPI_THERMAL_QUERY_RTD_DATA, NULL, 0, rx_data, 8, rx_length, SPI_TIMEOUT_MS);
}
static inline spi_status_t spi_thermal_query_thermistor_data(const spi_cs_config_t *cs, uint8_t *rx_data,
                                                             uint16_t *rx_length) {
    return spi_transaction(cs, SPI_THERMAL_QUERY_THERMISTOR_DATA, NULL, 0, rx_data, 8, rx_length, SPI_TIMEOUT_MS);
}
static inline spi_status_t spi_thermal_query_time(const spi_cs_config_t *cs, uint8_t *rx_data, uint16_t *rx_length) {
    return spi_transaction(cs, SPI_THERMAL_QUERY_TIME, NULL, 0, rx_data, 4, rx_length, SPI_TIMEOUT_MS);
}
