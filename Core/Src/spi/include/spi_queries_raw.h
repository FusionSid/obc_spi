#pragma once

#include "spi_packet.h"
#include "spi_query.h"
#include "spi_status.h"
#include <stdint.h>

// (THERMAL) NOT_USED (query 0x00) — placeholder for now
spi_response_code_t spi_thermal_query_not_used(spi_packet_t *response);

// (THERMAL) ACKNOWLEDGE (query 0x01) — placeholder for now
spi_response_code_t spi_thermal_query_acknowledge(spi_packet_t *response);

// (THERMAL) ECHO (query 0x02) — placeholder for now
spi_response_code_t spi_thermal_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (THERMAL) RTD_DATA (query 0x03) — placeholder for now
spi_response_code_t spi_thermal_query_rtd_data(spi_packet_t *response);

// (THERMAL) THERMISTOR_DATA (query 0x04) — placeholder for now
spi_response_code_t spi_thermal_query_thermistor_data(spi_packet_t *response);

// (THERMAL) TIME (query 0x05) — placeholder for now
spi_response_code_t spi_thermal_query_time(spi_packet_t *response);

// (THERMAL) STORE_ALL_ERRORS (query 0x06) — placeholder for now
spi_response_code_t spi_thermal_query_store_all_errors(spi_packet_t *response);

// (THERMAL) GET_ERRORS (query 0x07) — placeholder for now
spi_response_code_t spi_thermal_query_get_errors(spi_packet_t *response);

// (RADIATION) NOT_USED (query 0x00) — placeholder for now
spi_response_code_t spi_radiation_query_not_used(spi_packet_t *response);

// (RADIATION) ACKNOWLEDGE (query 0x01) — placeholder for now
spi_response_code_t spi_radiation_query_acknowledge(spi_packet_t *response);

// (RADIATION) ECHO (query 0x02) — placeholder for now
spi_response_code_t spi_radiation_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (RADIATION) EVENT_TIMESTAMPS (query 0x03) — placeholder for now
spi_response_code_t spi_radiation_query_event_timestamps(spi_packet_t *response);

// (RADIATION) ADC_VALUE (query 0x04) — placeholder for now
spi_response_code_t spi_radiation_query_adc_value(spi_packet_t *response);

// (RADIATION) TMP112_TEMP (query 0x05) — placeholder for now
spi_response_code_t spi_radiation_query_tmp112_temp(spi_packet_t *response);

// (RADIATION) EVENT_STATS (query 0x06) — placeholder for now
spi_response_code_t spi_radiation_query_event_stats(spi_packet_t *response);

// (RADIATION) WAVEFORM_CHUNK (query 0x07) — placeholder for now
spi_response_code_t spi_radiation_query_waveform_chunk(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (CAMERA) STARTUP (query 0x00) — placeholder for now
spi_response_code_t spi_camera_query_startup(spi_packet_t *response);

// (CAMERA) IMG_PARAMS (query 0x01) — placeholder for now
spi_response_code_t spi_camera_query_img_params(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (CAMERA) ECHO (query 0x02) — placeholder for now
spi_response_code_t spi_camera_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (CAMERA) SEND_SIZE (query 0x03) — placeholder for now
spi_response_code_t spi_camera_query_send_size(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);

// (CAMERA) SEND_IMAGE_PACKET (query 0x04) — placeholder for now
spi_response_code_t spi_camera_query_send_image_packet(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response);
