#include <stm32f407xx.h>

#define GPIOAEN (1U << 0)
#define PIN6 	(1U << 6)
#define PIN7 	(1U << 7)

#define LED1_PIN PIN6
#define LED2_PIN PIN7



int main(void)
{
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1UL << 12);
	GPIOA->MODER &= ~(1UL << 13);
	GPIOA->MODER |= (1UL << 14);
	GPIOA->MODER &= ~(1UL << 15);

	while(1){
		GPIOA->BSRR = LED1_PIN;
		GPIOA->BSRR = LED2_PIN;
		for(int i=0; i<100000; i++){};
		GPIOA->BSRR = (1UL << 22);
		GPIOA->BSRR = (1UL << 23);
		for(int i=0; i<100000; i++){};
	}
}
