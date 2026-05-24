#pragma once

#include <stdint.h>

void log_text(char *text);
void log_as_bytes(uint8_t *data, int size);
void log_array(uint8_t *data, int size);
