#pragma once

#include "spi.h"

spi_status_t query_ack(SPI_HandleTypeDef *hspi);
spi_status_t query_echo(SPI_HandleTypeDef *hspi);
spi_status_t query_getRTD(SPI_HandleTypeDef *hspi);
spi_status_t query_getADC(SPI_HandleTypeDef *hspi);
spi_status_t query_getRTC(SPI_HandleTypeDef *hspi);