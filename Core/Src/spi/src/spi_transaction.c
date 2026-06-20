#include "spi_transaction.h"
#include "spi_packet.h"
#include "string.h"

static spi_status_t transport_receive_packet(uint8_t *packet_buffer, uint16_t *packet_length_out,
                                             uint32_t timeout_ms) {
    uint8_t byte;
    uint32_t start_time = HAL_GetTick();

    do {
        if ((HAL_GetTick() - start_time) > timeout_ms)
            return SPI_ERR_TIMEDOUT; // we waited and waited and waited and nothing came :(

        if (spi_recieve_byte(&byte) != SPI_WORKED) continue;
    } while (byte != SPI_PACKET_START_BYTE);

    packet_buffer[0] = byte;

    if (spi_recieve_byte(&packet_buffer[1]) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    uint8_t len_low;
    uint8_t len_high;

    if (spi_recieve_byte(&len_low) != SPI_WORKED || spi_recieve_byte(&len_high) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    packet_buffer[2] = len_low;
    packet_buffer[3] = len_high;
    uint16_t data_length = (uint16_t)len_low | ((uint16_t)len_high << 8);

    if (data_length > SPI_PACKET_MAX_DATA_SIZE) {
        return SPI_ERR_OVERFLOW;
    }

    if (spi_recieve(&packet_buffer[SPI_PACKET_HEADER_SIZE], data_length) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    uint16_t crc_index = SPI_PACKET_HEADER_SIZE + data_length;
    if (spi_recieve(&packet_buffer[crc_index], SPI_PACKET_FOOTER_SIZE) != SPI_WORKED) {
        return SPI_ERR_HAL;
    }

    *packet_length_out = SPI_PACKET_HEADER_SIZE + data_length + SPI_PACKET_FOOTER_SIZE;

    return SPI_WORKED;
}

spi_status_t spi_transaction(const spi_cs_config_t *cs, uint8_t query, const uint8_t *tx_data,
                             uint16_t tx_length, uint8_t *rx_data, uint16_t rx_buffer_size, uint16_t *rx_length_out,
                             uint32_t timeout_ms) {
    uint8_t tx_packet[SPI_PACKET_MAX_PACKET_SIZE];
    uint8_t rx_packet[SPI_PACKET_MAX_PACKET_SIZE];

    spi_status_t status;
    uint16_t received_packet_length;

    uint16_t tx_packet_length = SPI_PACKET_HEADER_SIZE + tx_length + SPI_PACKET_FOOTER_SIZE;

    status = spi_packet_build(tx_packet, query, tx_data, tx_length);
    if (status != SPI_WORKED) {
        return status;
    }

    spi_device_select(cs); // pull the line to low

    status = spi_send(tx_packet, tx_packet_length);
    if (status != SPI_WORKED) {
        spi_device_deselect(cs);
        return status;
    }

    status = transport_receive_packet(rx_packet, &received_packet_length, timeout_ms);
    spi_device_deselect(cs);
    if (status != SPI_WORKED) {
        return status;
    }

    spi_packet_t packet;
    status = spi_packet_parse(rx_packet, received_packet_length, &packet);
    if (status != SPI_WORKED) {
        return status;
    }

    if (packet.length > rx_buffer_size) {
        return SPI_ERR_OVERFLOW;
    }

    memcpy(rx_data, packet.data, packet.length);

    if (rx_length_out) {
        *rx_length_out = packet.length;
    }

    return SPI_WORKED;
}