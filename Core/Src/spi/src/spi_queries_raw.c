#include "spi_queries_raw.h"
#include "spi_transaction.h"

spi_response_code_t spi_thermal_query_acknowledge(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ACKNOWLEDGE, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_ECHO, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_rtd_data(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status = spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_RTD_DATA, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_thermistor_data(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_THERMISTOR_DATA, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_time(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status = spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_TIME, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_store_all_errors(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_STORE_ALL_ERRORS, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_thermal_query_get_errors(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_THERMAL, SPI_THERMAL_QUERY_GET_ERRORS, NULL, 0, response, false);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_acknowledge(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ACKNOWLEDGE, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ECHO, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_event_timestamps(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_EVENT_TIMESTAMPS, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_adc_value(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_ADC_VALUE, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_tmp112_temp(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_TMP112_TEMP, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_event_stats(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_EVENT_STATS, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_radiation_query_waveform_chunk(const uint8_t *tx_data, uint16_t tx_len,
                                                       spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_RADIATION, SPI_RADIATION_QUERY_WAVEFORM_CHUNK, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_camera_query_startup(spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status = spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_STARTUP, NULL, 0, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_camera_query_img_params(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_IMG_PARAMS, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_camera_query_echo(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_ECHO, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_camera_query_send_size(const uint8_t *tx_data, uint16_t tx_len, spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_SEND_SIZE, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}

spi_response_code_t spi_camera_query_send_image_packet(const uint8_t *tx_data, uint16_t tx_len,
                                                       spi_packet_t *response) {
    // TODO: get data from user as a struct not raw bufer
    // TODO: serialise the data they gave into a buffer

    spi_response_code_t status =
        spi_transact(SPI_PAYLOAD_CAMERA, SPI_CAMERA_QUERY_SEND_IMAGE_PACKET, tx_data, tx_len, response, true);
    if (status != SPI_RESP_OK) {
        return status;
    }

    // TODO: check data if its correct size or anything like that
    // TODO: parse data into an appropriate struct

    return status;
}
