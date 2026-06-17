#pragma once

#include <stdbool.h>
#include <stdint.h>

#define SPI_PACKET_START_BYTE 0xD8

#define SPI_PACKET_HEADER_SIZE 4      // start byte + query + 2 for length
#define SPI_PACKET_FOOTER_SIZE 2      // crc
#define SPI_PACKET_MAX_DATA_SIZE 6767 // probably change to smth more reasonable later

#define SPI_PACKET_MAX_PACKET_SIZE SPI_PACKET_HEADER_SIZE + SPI_PACKET_MAX_DATA_SIZE + SPI_PACKET_FOOTER_SIZE

typedef struct {
    uint8_t start;
    uint8_t query;
    uint16_t length;
    const uint8_t *data;
} spi_packet_t;
