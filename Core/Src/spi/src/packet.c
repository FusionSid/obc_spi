#include "packet.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"

// maybe in the future i can use an enum, rn its just true/false for error
bool spi_packet_build(uint8_t* packet_out, spi_query_code_t query,
                      const uint8_t* data, uint16_t data_length) {
    if (packet_out == NULL || (data_length != 0 && data == NULL) ||
        data_length > (SPI_PACKET_MAX_SIZE - SPI_PACKET_START_AND_END_SIZE)) {
        return false;
    }

    packet_out[0] = SPI_PACKET_START_BYTE;
    packet_out[1] = query;

    packet_out[2] = (uint8_t)(data_length & 0xFF);
    packet_out[3] = (uint8_t)((data_length >> 8) & 0xFF);

    if (data_length > 0 && data != NULL) {
        // i think this might be better than the for loop
        memcpy(&packet_out[SPI_PACKET_START_SIZE], data, data_length);
    }

    uint16_t crc =
        spi_packet_crc16(packet_out, SPI_PACKET_START_SIZE + data_length);
    uint16_t crc_index = SPI_PACKET_START_SIZE + data_length;

    packet_out[crc_index] = (uint8_t)(crc & 0xFF);
    packet_out[crc_index + 1] = (uint8_t)((crc >> 8) & 0xFF);

    return true;
}

uint16_t spi_packet_crc16(uint8_t* pData, uint16_t length) {
    uint8_t i;  // copied from rad payload just changed to not add 4, instead do
                // that in packet_build
    uint16_t wCrc = 0xffff;
    while (length--) {
        wCrc ^= *(unsigned char*)pData++ << 8;
        for (i = 0; i < 8; i++)
            wCrc = wCrc & 0x8000 ? (wCrc << 1) ^ 0x1021 : wCrc << 1;
    }
    return wCrc & 0xffff;
}
