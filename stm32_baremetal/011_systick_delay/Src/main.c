#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "systick.h"
#include "uart.h"

#define PIN6 (1UL << 6);

#define LED1_PIN PIN6

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER6_1;

	uart1_rx_tx_init();

	while (1) {
		GPIOA->ODR ^= LED1_PIN;
		printf("1000ms\n");
		systick_delay_ms(1000);
	}
}
