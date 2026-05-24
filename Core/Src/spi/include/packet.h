#pragma once

#include "spi.h"
#include <stdbool.h>
#include <stdint.h>

#define SPI_PACKET_START_BYTE 0xD8
#define SPI_PACKET_MAX_DATA_SIZE 6767 // ill probably change later
#define SPI_PACKET_START_SIZE 4
#define SPI_PACKET_END_SIZE 2 // because crc

#define SPI_PACKET_START_AND_END_SIZE SPI_PACKET_START_SIZE + SPI_PACKET_END_SIZE
#define SPI_PACKET_MAX_SIZE SPI_PACKET_START_AND_END_SIZE + SPI_PACKET_MAX_DATA_SIZE

uint16_t spi_packet_crc16(uint8_t *pData, uint16_t length);
bool spi_packet_build(uint8_t *packet_out, spi_query_code_t query, const uint8_t *data, uint16_t data_length);
