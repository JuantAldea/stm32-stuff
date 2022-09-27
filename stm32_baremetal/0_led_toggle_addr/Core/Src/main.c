// PA6
// PA7

#define PERIPH_BASE 		(0x40000000UL)
#define AHB1PERIPH_OFFSET 	(0x00020000UL)
#define AHB1PERIPH_BASE 	(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET 		(0x0000UL)

#define GPIOA_BASE			(AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x3800UL)
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET)

#define GPIOAEN				(1UL)

#define PIN6				(1UL<<6)
#define PIN7				(1UL<<7)

#define LED1_PIN			(PIN6)
#define LED2_PIN			(PIN7)

#include <stdint.h>

typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct
{
	volatile uint32_t dummy[12];
	volatile uint32_t AHB1ENR;

} RCC_TypeDef;

#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define RCC ((RCC_TypeDef *)RCC_BASE)

//GPIO_TypeDef * const GPIOA = (GPIO_TypeDef *) GPIOA_BASE;

int main(void)
{
	// enable clock access go GPIOA
	RCC->AHB1ENR |=  GPIOAEN;
	GPIOA->MODER |=  (1U << 12);
	GPIOA->MODER &= ~(1U << 13);
	GPIOA->MODER |=  (1U << 14);
	GPIOA->MODER &= ~(1U << 15);
	GPIOA->ODR ^= LED1_PIN;
	while(1){
		//set PA5 high
		GPIOA->ODR ^= LED1_PIN;
		GPIOA->ODR ^= LED2_PIN;
		int i;
		for(i = 0; i < 1000000; i++) {

		}
	}
}


