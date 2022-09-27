#include <stm32f407xx.h>

#define SYSTICK_LOAD_VAL 16000 //ticks/ms
#define SYSTICK_CTRL_ENABLE (1UL << 0)
#define SYSTICK_CTRL_CLKSRC (1UL << 2)
#define SYSTICK_CTRL_COUNTFLAG (1UL << 16)

void systick_delay_ms(uint32_t ms) {
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	for (int i = 0; i < ms; ++i) {
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
			;
		}
	}

	SysTick->CTRL = 0;
}
