#include "stm32f4xx_hal.h"
#include "exti.h"

void PE4_interrupt_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    //enable System configuration controller clock enable
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    GPIOE->MODER &= ~(GPIO_MODER_MODE4_1 | GPIO_MODER_MODE4_0);
    //clear EXTI4
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4;
    //set EXTI4 to PE4
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PE;
    //unmask EXTI4
    EXTI->IMR |= EXTI_IMR_IM4;
    EXTI->FTSR |= EXTI_FTSR_TR4;
    NVIC_SetPriority(EXTI4_IRQn, 6);
    NVIC_EnableIRQ(EXTI4_IRQn);
}

void gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    //K0 & K1 are active-low -> Enable Pull-UP
    GPIOE->PUPDR |= GPIO_PUPDR_PUPD4_0;
    GPIOE->PUPDR |= GPIO_PUPDR_PUPD3_0;

    //K-UP is active high -> Enable Pull-DOWN
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_1;

    GPIOA->MODER |= (1UL << 12);
    GPIOA->MODER &= ~(1UL << 13);

    GPIOA->MODER &= ~(1UL << 0);
    GPIOA->MODER &= ~(1UL << 1);
}

uint8_t read_digital_sensor(void)
{
    volatile uint32_t odr = GPIOA->ODR;
    odr &= ~GPIO_ODR_OD6;
    odr |= ((GPIOE->IDR & GPIO_IDR_ID4) != 0) << GPIO_ODR_OD6_Pos;
    GPIOA->ODR = odr;
/*
    if ((GPIOE->IDR & GPIO_IDR_ID4) ) {
        //GPIOA->BSRR = GPIO_BSRR_BS6;
        GPIOA->ODR |= GPIO_ODR_OD6;
    } else {
        //GPIOA->BSRR = GPIO_BSRR_BR6;
        GPIOA->ODR &= ~GPIO_ODR_OD6;
    }
*/
    return (GPIOE->IDR & GPIO_IDR_ID4) == 0;
}
