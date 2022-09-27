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
