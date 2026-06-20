#pragma once
#include "spi_hal.h"
#include "spi_status.h"

spi_status_t thermal_query_ack(spi_handle_t *spi, const spi_cs_config_t *cs);
spi_status_t thermal_query_echo(spi_handle_t *spi, const spi_cs_config_t *cs);
spi_status_t thermal_query_get_rtd(spi_handle_t *spi, const spi_cs_config_t *cs);