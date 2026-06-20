#pragma once

#define SPI_STATUS_LIST                                                                                                \
    X(SPI_WORKED)                                                                                                      \
    X(SPI_ERR_HAL)                                                                                                     \
    X(SPI_ERR_TIMEDOUT)                                                                                                \
    X(SPI_ERR_CRCBAD)                                                                                                  \
    X(SPI_ERR_OVERFLOW)                                                                                                \
    X(SPI_ERR_NOT_INITALISED)                                                                                          \
    X(SPI_ERR_INVALID_ARGS)                                                                                            \
    X(SPI_ERR_BUILD_FAIL)

#define X(name) name,
typedef enum { SPI_STATUS_LIST } spi_status_t;
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
