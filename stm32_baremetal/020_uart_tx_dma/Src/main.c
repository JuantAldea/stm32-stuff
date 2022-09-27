#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"

#define PIN6 (1UL << 6)
#define PIN7 (1UL << 7)

#define LED1_PIN PIN6
#define LED2_PIN PIN7

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	GPIOA->ODR |= LED1_PIN;
	GPIOA->ODR |= LED2_PIN;

	uart1_tx_init();

	printf("%s\n", __FUNCTION__);

	char msg[] = "DMA TRANSFER\r\n";

	dma2_channel4_init((uint32_t) msg, (uint32_t) &USART1->DR, sizeof(msg));

	while (1) {
		;
	}
}

void DMA2_Stream7_IRQHandler(void)
{
	if (DMA2->HISR & DMA_HISR_TCIF7) {
		GPIOA->ODR &= ~LED1_PIN;
		GPIOA->ODR &= ~LED2_PIN;
		// clear flag
		DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
	}
}
