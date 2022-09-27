#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "tim.h"

#define LED1_PIN GPIO_ODR_OD6
#define LED2_PIN GPIO_ODR_OD7
uint32_t asd;
int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	uart1_rx_tx_init();
	printf("%s\n", __FUNCTION__);
	tim2_1hz_interrupt_init();
	while (1) {
		;
	}
}

void tim2_callback(void)
{
	GPIOA->ODR ^= LED1_PIN;
	GPIOA->ODR ^= LED2_PIN;
	printf("A second passed\n");
}

void TIM2_IRQHandler(void)
{
	tim2_callback();
	TIM2->SR &= ~TIM_SR_UIF;
}

