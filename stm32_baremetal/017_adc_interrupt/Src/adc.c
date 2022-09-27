#include <stm32f407xx.h>
#include "adc.h"

void pa1_adc_interrupt_init(void)
{
	//clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//SET PA1 pin to analog
	GPIOA->MODER |= GPIO_MODER_MODER1;
	//clock access to ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//conversion sequence start
	ADC1->SQR3 = ADC_SQR3_SQ1_0;
	//conversion sequence length
	ADC1->SQR1 = ~ADC_SQR1_L;
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC_IRQn);
	//enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;
}

void start_conversion(void)
{
	// software start & continuous conversion
	ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_SWSTART;
}
