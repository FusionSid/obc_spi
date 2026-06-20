#pragma once

#define SPI_QUERY_LIST \
    X(QUERY_NOT_USED) \
    X(QUERY_ACKNOWLEDGE) \
    X(QUERY_ECHO) \
    X(QUERY_RTD_DATA) \
    X(QUERY_THERMISTOR_DATA) \
    X(QUERY_TIME) \
    X(QUERY_STORE_ERRORS) \
    X(QUERY_SEND_ERRORS) \

#define X(name) name,
typedef enum { SPI_QUERY_LIST } spi_query_code_t;
#undef X

static inline const char *spi_query_to_string(spi_query_code_t query) {
    switch (query) {
#define X(name)                                                                                                        \
    case name:                                                                                                         \
        return #name;
        SPI_QUERY_LIST
#undef X
    default:
        return "invalid query type";
    }
}