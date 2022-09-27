#include "stm32f4xx_hal.h"
#include "adc_driver.h"
void adc_init()
{
    ADC1->CR2 = 0;
    // enable clock access
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //PA Analog mode
    GPIOA->MODER |= GPIO_MODER_MODER1;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    //channel 1
    ADC1->SQR3 = ADC_SQR3_SQ1_0;
    //len = 1
    //ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 = 0;
    //adc on
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint32_t read_analog_sensor(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while(!(ADC1->SR & ADC_SR_EOC)) {}
    return ADC1->DR;
}
