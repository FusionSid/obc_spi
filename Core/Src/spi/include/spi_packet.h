#pragma once

#include "spi_status.h"
#include <stdbool.h>
#include <stdint.h>

#define SPI_PACKET_START_BYTE 0xD8

#ifdef NEW_PACKET_FORMAT
#define SPI_PACKET_HEADER_SIZE 5
#else
#define SPI_PACKET_HEADER_SIZE 4 // start byte + query + 2 for length
#endif

#define SPI_PACKET_FOOTER_SIZE 2    // crc
#define SPI_PACKET_MAX_DATA_SIZE 67 // probably change to smth more reasonable later

#define SPI_PACKET_MAX_PACKET_SIZE (SPI_PACKET_HEADER_SIZE + SPI_PACKET_MAX_DATA_SIZE + SPI_PACKET_FOOTER_SIZE)

// start and crc dont really need to be included here
typedef struct {
#ifdef NEW_PACKET_FORMAT
    uint8_t payload;
#endif
    uint8_t query;
    uint16_t length;
    const uint8_t *data;
} spi_packet_t;

#ifdef NEW_PACKET_FORMAT
spi_status_t spi_packet_build(uint8_t *packet_out, uint8_t payload, uint8_t query, const uint8_t *data,
                              uint16_t data_length);
#else
spi_status_t spi_packet_build(uint8_t *packet_out, uint8_t query, const uint8_t *data, uint16_t data_length);
#endif

spi_status_t spi_packet_parse(const uint8_t *raw_packet, uint16_t raw_packet_length, spi_packet_t *packet_out);

uint16_t spi_packet_compute_crc16(const uint8_t *data, uint16_t length);