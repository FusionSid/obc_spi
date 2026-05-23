#include "log.h"

#include <string.h>
#include <stdio.h>

#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"

#define UART_TIMEOUT 100
#define HUSART3 hcom_uart[COM1]

void log_array(uint8_t *data, int size) {
	HAL_UART_Transmit(&HUSART3, data, size, UART_TIMEOUT);
};

void log_text(char* text) {
	HAL_UART_Transmit(&HUSART3, (uint8_t*)text, strlen(text), UART_TIMEOUT);
};

void log_as_bytes(uint8_t *data, int size) {
	// probably a better way to do this but idk rn
    unsigned char *p = (unsigned char *)data;
    char buffer[4];

    for (int i = 0; i < size; i++) {
    	sprintf(buffer, "%02X ", p[i]);
    	log_text(buffer);
    }

    log_text("\r\n");
}
