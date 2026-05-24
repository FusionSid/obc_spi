#include "spi.h"

#include <string.h>

#include "log.h"
#include "packet.h"

spi_status_t spi_receive_packet(SPI_HandleTypeDef *hspi, uint8_t *data_out,
                                uint16_t *data_length_out, uint16_t data_buf_size, uint32_t timeout_ms) {

    uint8_t byte;
    uint32_t start = HAL_GetTick();

    do {
        if (HAL_SPI_Receive(hspi, &byte, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK) continue;

        if (HAL_GetTick() - start > timeout_ms)
            return SPI_ERR_TIMEDOUT; // we waited and waited and waited and nothing came :(

    } while (byte != SPI_PACKET_START_BYTE);

    if (HAL_SPI_Receive(hspi, &byte, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK) return SPI_ERR_HAL;
    spi_query_code_t query = (spi_query_code_t)byte;

    uint8_t len_low, len_high;
    if (HAL_SPI_Receive(hspi, &len_low, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK ||
        HAL_SPI_Receive(hspi, &len_high, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK) {
        return SPI_ERR_HAL;
    }
    uint16_t data_length = (uint16_t)len_low | ((uint16_t)len_high << 8);

    // the buffer isnt big enough to store what we recieve
    if (data_length > data_buf_size) {
        return SPI_ERR_OVERFLOW;
    }

    for (uint16_t i = 0; i < data_length; i++) {
        if (HAL_SPI_Receive(hspi, &data_out[i], 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK) return SPI_ERR_HAL;
    }

    uint8_t crc_low, crc_high;
    if (HAL_SPI_Receive(hspi, &crc_low, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK ||
        HAL_SPI_Receive(hspi, &crc_high, 1, SPI_BYTE_TIMEOUT_MS) != HAL_OK) {
        return SPI_ERR_HAL;
    }

    uint16_t received_crc = (uint16_t)crc_low | ((uint16_t)crc_high << 8);

    // make the actual packet
    uint8_t tmp[SPI_PACKET_MAX_SIZE];
    tmp[0] = SPI_PACKET_START_BYTE;
    tmp[1] = (uint8_t)query;
    tmp[2] = len_low;
    tmp[3] = len_high;
    memcpy(tmp + SPI_PACKET_START_SIZE, data_out, data_length);

    uint16_t computed_crc = spi_packet_crc16(tmp, SPI_PACKET_START_SIZE + data_length);
    if (computed_crc != received_crc) return SPI_ERR_CRCBAD;

    if (data_length_out) *data_length_out = data_length;

    return SPI_WORKED;
}

spi_status_t spi_transaction(SPI_HandleTypeDef *hspi, spi_query_code_t query_code, const uint8_t *sending_data,
                             uint16_t sending_data_length, uint8_t *recieved_data,
                             uint16_t *recieved_data_length, uint16_t recieving_buffer_size, uint32_t timeout_ms) {
    uint8_t tx_buf[SPI_PACKET_MAX_SIZE];
    uint16_t tx_buf_len = SPI_PACKET_START_AND_END_SIZE + sending_data_length;

    if (!spi_packet_build(tx_buf, query_code, sending_data, sending_data_length)) return SPI_ERR_BUILD_FAIL;

    spi_cs_low();

    log_as_bytes(tx_buf, tx_buf_len);

    // maybe use inturupts but idk yet
    if (HAL_SPI_Transmit(hspi, tx_buf, tx_buf_len, timeout_ms) != HAL_OK) {
        spi_cs_high();
        return SPI_ERR_HAL;
    }

    spi_status_t status = spi_receive_packet(hspi, recieved_data, recieved_data_length,
                                             recieving_buffer_size, timeout_ms);

    spi_cs_high();
    return status;
}
