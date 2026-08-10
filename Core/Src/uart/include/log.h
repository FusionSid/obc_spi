#pragma once

void log_init(void);
void log_text(char *text);
void log_printf(const char *fmt, ...);
void log_as_bytes(const void *data, int len);