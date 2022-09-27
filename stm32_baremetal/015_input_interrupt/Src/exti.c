#include <exti.h>

void pa0_exti_init(void)
{
	// disable interrupts
	__disable_irq();
	// enable clock to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// enable clock access to SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// select PORTC for EXTI13
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_PA;
	// unmask EXTI
	EXTI->IMR |= EXTI_IMR_MR0;
	// trigger on falling edge
	EXTI->FTSR |= EXTI_FTSR_TR0;
	// enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
	// enable interrupts
	__enable_irq();
}

void pe3_exti_init(void)
{
	// disable interrupts
	__disable_irq();
	// enable clock to GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	// enable clock access to SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// select PORTC for EXTI13
	GPIOE->MODER &= ~GPIO_MODER_MODER3;

	GPIOE->PUPDR |= GPIO_PUPDR_PUPDR3_0;

	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PE;
	// trigger on falling edge
	EXTI->FTSR |= EXTI_FTSR_TR3;
	// unmask EXTI
	EXTI->IMR |= EXTI_IMR_MR3;

	// enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI3_IRQn);
	// enable interrupts
	__enable_irq();
}

void pe4_exti_init(void)
{
	// disable interrupts
	__disable_irq();
	// enable clock to GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	// enable clock access to SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// select PORTC for EXTI13
	GPIOE->MODER &= ~GPIO_MODER_MODER4;

	GPIOE->PUPDR |= GPIO_PUPDR_PUPDR4_0;

	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PE;
	// trigger on falling edge
	EXTI->FTSR |= EXTI_FTSR_TR4;
	// unmask EXTI
	EXTI->IMR |= EXTI_IMR_MR4;

	// enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI4_IRQn);
	// enable interrupts
	__enable_irq();
}
