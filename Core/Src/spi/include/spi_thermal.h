#pragma once
#include "spi_hal.h"
#include "spi_status.h"

spi_status_t thermal_query_ack(const spi_cs_config_t *cs);
spi_status_t thermal_query_echo(const spi_cs_config_t *cs);
spi_status_t thermal_query_get_rtd(const spi_cs_config_t *cs);