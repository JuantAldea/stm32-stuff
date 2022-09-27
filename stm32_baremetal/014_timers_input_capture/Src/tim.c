#include <stm32f4xx.h>
#include "tim.h"
#include <stdio.h>

void output_compare_PA0_TIM5_CH1(void) {
	printf("%s\n", __FUNCTION__);
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set LED1 to alternate function
	GPIOA->MODER |= GPIO_MODER_MODER0_1;
	// set alternate function to TIM3_CH1
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	// pre-scaler
	TIM5->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	TIM5->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	// compare mode -> toggle OC1REF on match
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;

	// enable capture compare
	TIM5->CCER |= TIM_CCER_CC1E;
	// clear
	TIM5->CNT = 0;
	TIM5->SMCR |= TIM_SMCR_MSM;
	//TIM5->CR2 |= TIM_CR2_MMS_1;
	//TIM5->CR2 |= TIM_CR2_MMS_0;
	//enable
	TIM5->CR1 = TIM_CR1_CEN;
}

void input_capture_PA5_TIM2_CH1(void) {
	//printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//TIM2->PSC = 16000;
	//TIM2->ARR = 10000 -1; //3 - 1; // 10.000/10.000 = 1Hz
	//TIM2->ARR = TIM_ARR_ARR;
	TIM2->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 | TIM_SMCR_SMS_2;
	TIM2->CNT = 0;
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP;
	TIM2->CR1 = TIM_CR1_CEN;
}

void input_capture_PA7_TIM3_CH2(void) {
	printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM5->CNT = 0;
	TIM3->PSC = 1 - 1;
	TIM3->ARR = TIM_ARR_ARR;
	TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;
	TIM3->CCER |= TIM_CCER_CC2E;

	TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;
	TIM3->CR1 = TIM_CR1_CEN;
}


void output_compare_PA2_TIM2_CH3(void) {
	printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	//TIM2->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;

	// enable capture compare
	TIM2->CCER |= TIM_CCER_CC3E;
	// clear
	TIM2->CNT = 0;
	//enable
	TIM2->CR1 = TIM_CR1_CEN;
}
void output_compare_PA5_TIM2_CH1(void) {
	printf("%s\n", __FUNCTION__);
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set LED1 to alternate function
	GPIOA->MODER |= GPIO_MODER_MODER5_1;
	// set alternate function to TIM3_CH1
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// pre-scaler
	TIM2->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	//TIM2->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	// compare mode in ch1
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;

	// enable capture compare
	TIM2->CCER |= TIM_CCER_CC1E;
	// clear
	TIM2->CNT = 0;
	//enable
	TIM2->CR1 = TIM_CR1_CEN;
}

void output_compare_PA6_TIM3_CH1(void) {
	//printf("%s\n", __FUNCTION__);
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set LED1 to alternate function
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	// set alternate function to TIM3_CH1
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// pre-scaler
	TIM3->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	TIM3->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	//TIM3->ARR = 10;
	// compare mode in ch1
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
	//master mode enable signal as trigger
	TIM3->CR2 |=TIM_CR2_MMS_0;
	// enable capture compare
	TIM3->CCER |= TIM_CCER_CC1E;
	// clear
	TIM3->CNT = 0;
	//enable
	//TIM3->CR1 = TIM_CR1_CEN;
}

void output_compare_PA7_TIM3_CH2(void) {
	printf("%s\n", __FUNCTION__);
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set LED1 to alternate function
	GPIOA->MODER |= GPIO_MODER_MODER7_1;
	// set alternate function to TIM3_CH1
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_1;
	// clock access
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// pre-scaler
	TIM3->PSC = 1600 - 1; // 16 000 000Hz / 1600 = 10.000Hz
	// autoreload
	//TIM3->ARR = 10000 - 1; // 10.000/10.000 = 1Hz
	// compare mode in ch1
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;

	// enable capture compare
	TIM3->CCER |= TIM_CCER_CC2E;
	// clear
	TIM3->CNT = 0;
	//enable
	TIM3->CR1 = TIM_CR1_CEN;
}

void input_capture_PA0_TIM5_CH1(void) {
	printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER0_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->PSC = 16000;
	TIM5->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM5->CCER |= TIM_CCER_CC1E;
	TIM5->CR1 = TIM_CR1_CEN;
}

void input_capture_PA2_TIM2_CH3(void) {
	printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 16000;
	TIM2->CCMR2 |= TIM_CCMR2_CC3S_0;
	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->CR1 = TIM_CR1_CEN;
}

void input_capture_PA6_TIM3_CH1(void) {
	printf("%s\n", __FUNCTION__);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 16000;
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CR1 = TIM_CR1_CEN;
}

