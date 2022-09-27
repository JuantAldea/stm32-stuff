#include "stm32f407xx.h"

#define GPIOAEN (1U << 0)
#define UART6EN (1U << 5)

#define PIN0 (1UL << 0)
#define PIN6 (1UL << 6)
#define LED1_PIN PIN6
#define BTN_PIN PIN0

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;


	GPIOA->MODER |= (1UL << 12);
	GPIOA->MODER &= ~(1UL << 13);

	GPIOA->MODER &= ~(1UL << 0);
	GPIOA->MODER &= ~(1UL << 1);

	while (1) {
		if (GPIOA->IDR & GPIO_IDR_ID0) {
			GPIOA->BSRR = PIN6;
		} else {
			GPIOA->BSRR = GPIO_BSRR_BR6;
		}
	}
}

