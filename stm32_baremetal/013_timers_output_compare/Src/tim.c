#include <stm32f4xx.h>
#include "tim.h"
void tim2_1hz_init(void)
{
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// prescaler
	TIM2->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	TIM2->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	// clear
	TIM2->CNT = 0;
	//enable
	TIM2->CR1 |= TIM_CR1_CEN;
}

void tim3_1hz_init(void)
{
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// prescaler
	TIM3->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	TIM3->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	// clear
	TIM3->CNT = 0;
	//enable
	TIM3->CR1 |= TIM_CR1_CEN;
}

void tim3_output_compare_PA6(void)
{
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set LED1 to alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER6_0;
	GPIOA->MODER |= GPIO_MODER_MODER6_1;

	// set alternate function to TIM3_CH1
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL6_0|GPIO_AFRL_AFSEL6_2);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// pre-scaler
	TIM3->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	TIM3->ARR = 10000 - 1; // 10.000/10.000 = 1Hz

	// compare mode in ch1
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
	// enable capture compare
	TIM3->CCER |= TIM_CCER_CC1E;
	// clear
	TIM3->CNT = 0;
	//enable
	TIM3->CR1 |= TIM_CR1_CEN;
}


void tim2_output_compare_PA5(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER5_0;
	GPIOA->MODER |= GPIO_MODER_MODER5_1;
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5_2 | GPIO_AFRL_AFSEL5_1);
	GPIOA->AFR[0]|= GPIO_AFRL_AFSEL5_0;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = 10000 - 1;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
	TIM2->CCER  |= TIM_CCER_CC1E;
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}
