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

#define SPI_RESULT_LIST                                                                                                \
    X(SPI_FSM_RESULT_OK)                                                                                               \
    X(SPI_FSM_RESULT_BAD_CRC)                                                                                          \
    X(SPI_FSM_RESULT_START_TIMEOUT)                                                                                    \
    X(SPI_FSM_RESULT_BAD_LENGTH)                                                                                       \
    X(SPI_FSM_RESULT_BUS_ERROR)

#define SPI_RESP_LIST                                                                                                  \
    X(SPI_RESP_OK)                                                                                                     \
    X(SPI_RESP_DEVICE_ERROR)                                                                                           \
    X(SPI_RESP_TIMEOUT)                                                                                                \
    X(SPI_RESP_BAD_CRC)                                                                                                \
    X(SPI_RESP_BUS_ERROR)                                                                                              \
    X(SPI_RESP_BUSY)                                                                                                   \
    X(SPI_RESP_INVALID_ARGS)

#define X(name) name,
typedef enum { SPI_STATUS_LIST } spi_status_t;
typedef enum { SPI_RESULT_LIST } spi_fsm_result_t;
typedef enum { SPI_RESP_LIST } spi_response_code_t;
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