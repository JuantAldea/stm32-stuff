#include <stm32f407xx.h>
#include "config.h"
#include "STM32F407VET6_RTOS_BSP.h"
#define LED1 GPIO_ODR_OD6
#define LED2 GPIO_ODR_OD7

void BSP_led_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
    GPIOA->MODER &= ~(GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
}

void BSP_led1_on(void)
{
    GPIOA->BSRR |= GPIO_BSRR_BR6;
}

void BSP_led2_on(void)
{
    GPIOA->BSRR |= GPIO_BSRR_BR7;
}

void BSP_led1_off(void)
{
    GPIOA->BSRR |= GPIO_BSRR_BS6;
}

void BSP_led2_off(void)
{
    GPIOA->BSRR |= GPIO_BSRR_BS7;
}

void BSP_led1_toggle(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD6;
}
void BSP_led2_toggle(void)
{
    GPIOA->ODR ^= GPIO_ODR_OD7;
}

void BSP_adc1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER1_Msk;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->CR2 = 0;
    //channel 1
    ADC1->SQR3 = ADC_SQR3_SQ1_0;
    // length = 1 (L[3:0] = 0)
    ADC1->SQR1 = 0;
    //enable
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint32_t BSP_adc1_read()
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}

void BSP_delay_ms(uint32_t ms)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 160 - 1; //100 000 //100kHz
    TIM3->ARR = ms * 100 - 1; // 100 000 / 100 = 1kHz;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
    //wait for flag
    while (!(TIM3->SR & TIM_SR_UIF));
    //clear flag
    TIM3->SR &= ~TIM_SR_UIF;
}

void BSP_buttons_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    //WK_UP -> PA0, active high
    GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_1;
    //K0 = PE4, active low
    GPIOE->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD4_Msk;
    GPIOE->PUPDR |= GPIO_PUPDR_PUPD4_0;
    //K1 = PE3, active low
    GPIOE->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD3_Msk;
    GPIOE->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

uint32_t BPS_button_WK_UP_read()
{
    return (GPIOA->IDR & GPIO_IDR_ID0) == 0;
}

uint32_t BPS_button_K0_read()
{
    return (GPIOE->IDR & GPIO_IDR_ID4) != 0;
}

uint32_t BPS_button_K1_read()
{
    return (GPIOE->IDR & GPIO_IDR_ID3) != 0;
}

void BPS_probes_init()
{
    __disable_irq();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~(GPIO_MODER_MODE1_Msk | GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk | GPIO_MODER_MODE4_Msk | GPIO_MODER_MODE5_Msk);
    GPIOA->MODER |= GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0;
    __enable_irq();
}

void BPS_probe_on(enum probes p)
{
    GPIOA->BSRR |= 1 << (GPIO_BSRR_BS0_Pos + p);
}

void BPS_probe_off(enum probes p)
{
    GPIOA->BSRR |= 1 << (GPIO_BSRR_BR0_Pos + p);
}

void BPS_probe_toggle(enum probes p)
{
    GPIOA->ODR ^= 1 << (GPIO_ODR_OD0_Pos + p);
}
