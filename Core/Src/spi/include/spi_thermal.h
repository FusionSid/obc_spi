#pragma once

#include "spi_transaction.h"

spi_response_code_t thermal_query_ack(void);
spi_response_code_t thermal_query_echo(void);
spi_response_code_t thermal_query_get_rtd(void);