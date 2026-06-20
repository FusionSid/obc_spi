#pragma once

#define SPI_STATUS_LIST                                                                                                \
    X(SPI_WORKED)                                                                                                      \
    X(SPI_ERR_HAL)                                                                                                     \
    X(SPI_ERR_TIMEDOUT)                                                                                                \
    X(SPI_ERR_CRCBAD)                                                                                                  \
    X(SPI_ERR_OVERFLOW)                                                                                                \
    X(SPI_ERR_INVALID_ARGS)                                                                                            \
    X(SPI_ERR_BUILD_FAIL)

#define QUERY_LIST                                                                                                     \
    X(QUERY_NOT_USED)                                                                                                  \
    X(QUERY_ACKNOWLEDGE)                                                                                               \
    X(QUERY_ECHO)                                                                                                      \
    X(QUERY_RTD_DATA)                                                                                                  \
    X(QUERY_THERMISTOR_DATA)                                                                                           \
    X(QUERY_TIME)                                                                                                      \
    X(QUERY_STORE_ERRORS)                                                                                              \
    X(QUERY_SEND_ERRORS)

#define X(name) name,
typedef enum { SPI_STATUS_LIST } spi_status_t;
typedef enum { QUERY_LIST } spi_query_code_t;
#undef X

static inline const char *spi_status_to_string(spi_status_t status) {
    switch (status) {
#define X(name)                                                                                                        \
    case name:                                                                                                         \
        return #name;
        SPI_STATUS_LIST
#undef X
    default:
        return "invalid status type";
    }
}
static inline const char *spi_query_to_string(spi_query_code_t query) {
    switch (query) {
#define X(name)                                                                                                        \
    case name:                                                                                                         \
        return #name;
        QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}