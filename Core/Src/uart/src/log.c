#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "stm32h7xx_nucleo.h"
#include "cmsis_os2.h"

#define UART_TIMEOUT 100
#define LOG_BUFFER_SIZE 512
#define HUSART3 hcom_uart[COM1]

static osMutexId_t s_log_mutex;

void log_init(void) {
    s_log_mutex = osMutexNew(NULL);
}

void log_text(char *text) {
    if (s_log_mutex) osMutexAcquire(s_log_mutex, osWaitForever);
    HAL_UART_Transmit(&HUSART3, (uint8_t *)text, strlen(text), UART_TIMEOUT);
    if (s_log_mutex) osMutexRelease(s_log_mutex);
}

void log_printf(const char *fmt, ...) {
    char buffer[LOG_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len < 0) return;
    if (len >= LOG_BUFFER_SIZE) len = LOG_BUFFER_SIZE - 1;

    if (s_log_mutex) osMutexAcquire(s_log_mutex, osWaitForever);
    HAL_UART_Transmit(&HUSART3, (uint8_t *)buffer, len, UART_TIMEOUT);
    if (s_log_mutex) osMutexRelease(s_log_mutex);
}

void log_as_bytes(const void *data, int len) {
    const uint8_t *p = data;

    for (int i = 0; i < len; i++) {
        log_printf("%02X ", p[i]);
    }

    log_printf("\r\n");
}