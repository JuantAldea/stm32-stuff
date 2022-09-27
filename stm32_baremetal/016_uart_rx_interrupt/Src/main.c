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

	uart1_rx_tx_interrupt_init();

	printf("%s\n", __FUNCTION__);

	while (1) {
	}
}



void USART1_IRQHandler(void) {
	if (USART1->SR & USART_SR_RXNE) {
		key = USART1->DR;
		if (key == 'a') {
			GPIOA->ODR ^= LED1_PIN;
		}
		return;
	}
}
