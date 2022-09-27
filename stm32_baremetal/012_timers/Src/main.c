#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "tim.h"


#define PIN6 (1UL << 6);
#define PIN7 (1UL << 7);

#define LED1_PIN PIN6
#define LED2_PIN PIN7

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	uart1_rx_tx_init();
	printf("%s\n", __FUNCTION__);

	//tim2_1hz_init();

	while (1) {
		if (TIM2->SR & TIM_SR_UIF){
			GPIOA->ODR ^= LED1_PIN;
			GPIOA->ODR ^= LED2_PIN;
			TIM2->SR &= ~TIM_SR_UIF;
			printf("A second passed\n");
		}
	}
}
