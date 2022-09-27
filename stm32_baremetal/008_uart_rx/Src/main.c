#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
char key;

#define PIN6 (1UL << 6);

#define LED1_PIN PIN6

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER |= (1UL << 12);
	GPIOA->MODER &= ~(1UL << 13);

	uart1_rx_tx_init();

	while (1) {
		printf("Texting\n");
		key = uart_read(USART1);
		if (key == 'a'){
			GPIOA->ODR ^= LED1_PIN;
		}
	}
}
