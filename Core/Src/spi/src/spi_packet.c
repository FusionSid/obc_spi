#include "spi_packet.h"

#include <string.h>

// this should be useful when we chnge the packet structure to have extra byte
#define SPI_PACKET_OFFSET_START 0
#define SPI_PACKET_OFFSET_QUERY 1
#define SPI_PACKET_OFFSET_LEN_L 2
#define SPI_PACKET_OFFSET_LEN_H 3

// #define SPI_PACKET_OFFSET_START   0
// #define SPI_PACKET_OFFSET_PAYLOAD 1
// #define SPI_PACKET_OFFSET_QUERY   2 
// #define SPI_PACKET_OFFSET_LEN_L   3 
// #define SPI_PACKET_OFFSET_LEN_H   4

spi_status_t spi_packet_build(uint8_t *packet_out, uint8_t query, const uint8_t *data, uint16_t data_length) {
    if (packet_out == NULL || (data_length != 0 && data == NULL) ||
        (data_length > (SPI_PACKET_MAX_PACKET_SIZE - SPI_PACKET_HEADER_SIZE - SPI_PACKET_FOOTER_SIZE))) {
        return SPI_ERR_INVALID_ARGS;
    }

    packet_out[SPI_PACKET_OFFSET_START] = SPI_PACKET_START_BYTE;
    // packet_out[SPI_PACKET_OFFSET_PAYLOAD] = payload;
    packet_out[SPI_PACKET_OFFSET_QUERY] = query;

    packet_out[SPI_PACKET_OFFSET_LEN_L] = (uint8_t)(data_length & 0xFF);
    packet_out[SPI_PACKET_OFFSET_LEN_H] = (uint8_t)((data_length >> 8) & 0xFF);

    if (data_length > 0) {
        memcpy(&packet_out[SPI_PACKET_HEADER_SIZE], data, data_length);
    }

    uint16_t crc = spi_packet_compute_crc16(packet_out, SPI_PACKET_HEADER_SIZE + data_length);
    uint16_t crc_index = SPI_PACKET_HEADER_SIZE + data_length;

    packet_out[crc_index] = (uint8_t)(crc & 0xFF);
    packet_out[crc_index + 1] = (uint8_t)((crc >> 8) & 0xFF);

    return SPI_WORKED;
}

spi_status_t spi_packet_parse(const uint8_t *raw_packet, uint16_t raw_packet_length, spi_packet_t *packet_out) {
    if (!raw_packet || !packet_out) {
        return SPI_ERR_INVALID_ARGS;
    }

    if (raw_packet_length < (SPI_PACKET_HEADER_SIZE + SPI_PACKET_FOOTER_SIZE)) {
        return SPI_ERR_OVERFLOW;
    }

    if (raw_packet[SPI_PACKET_OFFSET_START] != SPI_PACKET_START_BYTE) {
        return SPI_ERR_INVALID_ARGS;
    }

    uint16_t data_length =
        ((uint16_t)raw_packet[SPI_PACKET_OFFSET_LEN_L]) | ((uint16_t)raw_packet[SPI_PACKET_OFFSET_LEN_H] << 8);
    uint16_t expected_size = SPI_PACKET_HEADER_SIZE + data_length + SPI_PACKET_FOOTER_SIZE;

    if (expected_size != raw_packet_length) {
        return SPI_ERR_OVERFLOW;
    }

    uint16_t crc_index = SPI_PACKET_HEADER_SIZE + data_length;
    uint16_t received_crc = ((uint16_t)raw_packet[crc_index]) | ((uint16_t)raw_packet[crc_index + 1] << 8);
    uint16_t calculated_crc = spi_packet_compute_crc16(raw_packet, expected_size - 2);

    if (received_crc != calculated_crc) {
        return SPI_ERR_CRCBAD;
    }

    // packet_out->payload = raw_packet[SPI_PACKET_OFFSET_PAYLOAD];
    packet_out->query = raw_packet[SPI_PACKET_OFFSET_QUERY];
    packet_out->length = data_length;
    packet_out->data = &raw_packet[SPI_PACKET_HEADER_SIZE];

    return SPI_WORKED;
}

uint16_t spi_packet_compute_crc16(const uint8_t *data, uint16_t length) {
    uint8_t i;
    uint16_t wCrc = 0xffff;

    while (length--) {
        wCrc ^= *(unsigned char *)data++ << 8;
        for (i = 0; i < 8; i++) wCrc = wCrc & 0x8000 ? (wCrc << 1) ^ 0x1021 : wCrc << 1;
    }

    return wCrc & 0xffff;
}