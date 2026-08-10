#pragma once

typedef enum {
    SPI_PAYLOAD_THERMAL = 1,
    SPI_PAYLOAD_RADIATION = 2,
    SPI_PAYLOAD_CAMERA = 3,
    SPI_DEVICE__COUNT
} spi_payload_t;

#define SPI_THERMAL_QUERY_LIST                                                                                         \
    X(SPI_THERMAL_QUERY_ACKNOWLEDGE, 1)                                                                                \
    X(SPI_THERMAL_QUERY_ECHO, 2)                                                                                       \
    X(SPI_THERMAL_QUERY_RTD_DATA, 3)

#define SPI_RADIATION_QUERY_LIST                                                                                       \
    X(SPI_RADIATION_QUERY_ACKNOWLEDGE, 1)                                                                              \
    X(SPI_RADIATION_QUERY_ECHO, 2)                                                                                     \
    X(SPI_RADIATION_QUERY_EVENT_TIMESTAMPS, 3)                                                                         \
    X(SPI_RADIATION_QUERY_ADC_VALUE, 4)

#define SPI_CAMERA_QUERY_LIST                                                                                          \
    X(SPI_CAMERA_QUERY_STARTUP, 0)                                                                                     \
    X(SPI_CAMERA_QUERY_IMG_PARAMS, 1)                                                                                  \
    X(SPI_CAMERA_QUERY_GET_SIZE, 3)                                                                                    \
    X(SPI_CAMERA_QUERY_SEND_IMAGE_PACKET, 4)

#define X(name, value) name = value,
typedef enum { SPI_THERMAL_QUERY_LIST } spi_thermal_query_code_t;
typedef enum { SPI_RADIATION_QUERY_LIST } spi_radiation_query_code_t;
typedef enum { SPI_CAMERA_QUERY_LIST } spi_camera_query_code_t;
#undef X

static inline const char *spi_thermal_query_to_string(spi_thermal_query_code_t query) {
    switch (query) {
#define X(name, value)                                                                                                 \
    case name:                                                                                                         \
        return #name;
        SPI_THERMAL_QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}

static inline const char *spi_radiation_query_to_string(spi_radiation_query_code_t query) {
    switch (query) {
#define X(name, value)                                                                                                 \
    case name:                                                                                                         \
        return #name;
        SPI_RADIATION_QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}

static inline const char *spi_camera_query_to_string(spi_camera_query_code_t query) {
    switch (query) {
#define X(name, value)                                                                                                 \
    case name:                                                                                                         \
        return #name;
        SPI_CAMERA_QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}
