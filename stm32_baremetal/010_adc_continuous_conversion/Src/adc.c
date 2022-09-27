#include <stm32f407xx.h>
#include "adc.h"
/*
#define ADC1_ENA	(1U<<8)
#define GPIOA_ENA	(1U<<0)
#define CH1		(1U<<0)
#define SEQ_LEN1 	0x00
#define CR2_ADON	(1U<<0)
#define CR2_SWSTRT	(1U<<30)
#define SR_EOC		(1U<<1)

#define ADC1EN (1UL<<8)
#define ADC_CH1 (1UL<<0)
#define ADC_SEQ_LEN_1 0x00

void pa1_adc_init(void)
{
    //clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //SET PA1 pin to analog
    //GPIOA->MODER |= (3UL << 2);
    GPIOA->MODER |= (1UL << 2);
    GPIOA->MODER |= (1UL << 3);
    //clock access to ADC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    //conversion sequence start
    ADC1->SQR3 = ADC_SQR3_SQ1_0;
    //conversion sequence length
    ADC1->SQR1 = ADC_SEQ_LEN_1;
    //enable ADC
    ADC1->CR2 |= ADC_CR2_ADON;
}
*/
void internal_temperature_sensor_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC123_COMMON->CCR |= ADC_CCR_ADCPRE_0;
    ADC1->CR2 |= ADC_CR2_EOCS;

    ADC123_COMMON->CCR |= ADC_CCR_TSVREFE;
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_2 | ADC_SMPR1_SMP16_1|ADC_SMPR1_SMP16_0;
    ADC1->SQR3 = ADC_SQR3_SQ1_4;

    for(int i = 0; i<0xFFFF; i++){};

    ADC1->CR2 |= ADC_CR2_ADON;

    for(int i = 0; i<0xFFFF; i++){};


}

void start_conversion(void)
{
    // software start & continuous conversion
    //ADC1->CR2 |= ADC_CR2_CONT | ADC_CR2_SWSTART;
    //ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adc_read(void)
{
    //wait for conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
    for(int i = 0; i < 0xFFFF; i++);
    while (!(ADC1->SR & ADC_SR_EOC)) {}
    uint32_t uhADCxConvertedValue = ADC1->DR;
    return uhADCxConvertedValue;
}
uint32_t read(void){
    //RCC->CFGR |= RCC_CFGR_PPRE2_2;
    //for(int i = 0; i<0xFFFF; i++){};
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC123_COMMON->CCR |= ADC_CCR_ADCPRE_1 | ADC_CCR_ADCPRE_0;
    ADC1->CR2 |= ADC_CR2_EOCS;


    ADC123_COMMON->CCR |= ADC_CCR_TSVREFE;
    ADC1->SMPR1 |= ADC_SMPR1_SMP16_2 | ADC_SMPR1_SMP16_1|ADC_SMPR1_SMP16_0;
    ADC1->SQR3 = ADC_SQR3_SQ1_4;

    for(int i = 0; i<0xFFFF; i++){};

    ADC1->CR2 |= ADC_CR2_ADON;

    for(int i = 0; i <0xFFFF; i++){};

    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC)) {}

    uint32_t uhADCxConvertedValue = ADC1->DR;

    return uhADCxConvertedValue;
}
