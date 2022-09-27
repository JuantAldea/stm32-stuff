#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "systick.h"
#include "uart.h"

#define PA6 (1UL << 6)
#define PA7 (1UL << 7)

#define LED1_PIN PA6
#define LED2_PIN PA7

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	uart1_rx_tx_init();
	printf("%s\n", __FUNCTION__);
	systick_1hz_interrupt();

	while (1) {
		;
	}
}

static void systick_callback(void)
{
	printf("A second passed\n");
	GPIOA->ODR ^= LED1_PIN;
	GPIOA->ODR ^= LED2_PIN;
}

void SysTick_Handler(void)
{
	systick_callback();
}
