#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

void USART1_UART_TX_Init(void);
void USART1_UART_RX_Init(void);
int USART1_write(int ch);
int USART1_read();

#endif /* INC_UART_DRIVER_H_ */
