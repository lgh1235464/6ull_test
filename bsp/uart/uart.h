#ifndef __UART_H
#define __UART_H

#include "imx6ul.h"


void uart_init(void);
void uart_io_init(void);
void uart_disable(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_softreset(UART_Type *base);
void putc(uint8_t byte);
void puts(uint8_t *str);
uint8_t getc(void);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);




#endif 










