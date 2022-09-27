#ifndef UART_H_
#define UART_H_

#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>

#define UART_BAUDRATE 115200


void uart1_rx_tx_init();
void uart_write(USART_TypeDef *uartx, int ch);
char uart_read(USART_TypeDef *uartx);
void uart1_rx_tx_interrupt_init(void);
#endif /* UART_H_ */
