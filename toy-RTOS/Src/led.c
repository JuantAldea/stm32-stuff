#include <stm32f407xx.h>
#include "led.h"

void leds_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER6_1;
    GPIOA->MODER |= GPIO_MODER_MODER7_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER7_1;
}

void led_on(enum board_leds led)
{
    if (led == LED_0) {
        GPIOA->ODR &= ~GPIO_ODR_OD6;
    } else if (led == LED_1) {
        GPIOA->ODR &= ~GPIO_ODR_OD7;
    }
}

void led_off(enum board_leds led)
{
    if (led == LED_0) {
        GPIOA->ODR |= GPIO_ODR_OD6;
    } else if (led == LED_1) {
        GPIOA->ODR |= GPIO_ODR_OD7;
    }
}

void led_toggle(enum board_leds led)
{
    if (led == LED_0) {
        GPIOA->ODR ^= GPIO_ODR_OD6;
    } else if (led == LED_1) {
        GPIOA->ODR ^= GPIO_ODR_OD7;
    }
}
