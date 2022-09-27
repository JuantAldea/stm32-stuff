#if 0
#include "timebase.h"

#include <stm32f407xx.h>
#include "config.h"

volatile uint32_t g_curr_tick;
volatile uint32_t tick_freq = 1;

#define MAX_DELAY      0xFFFFFFFFU

void timebase_init()
{
    SysTick->LOAD = SYS_FREQ - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    __enable_irq();
}

uint32_t get_tick()
{
    __disable_irq(); // HAL does not do this
    const uint32_t g_curr_tick_prime = g_curr_tick;
    __enable_irq(); // HAL does not do this
    return g_curr_tick_prime;
}

void delay(uint32_t delay)
{
    const uint32_t tick_begin = get_tick();
    if (delay < MAX_DELAY) {
        delay += tick_freq;
    }

    while ((get_tick() - tick_begin) < delay) {};
}

/*
static void tick_increment(void)
{
    g_curr_tick += tick_freq;
}

void SysTick_Handler(void)
{
    tick_increment();
}
*/
#endif

