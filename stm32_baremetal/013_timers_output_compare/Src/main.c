#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "tim.h"

#define PIN5 (1UL << 5);
#define PIN6 (1UL << 6);
#define PIN7 (1UL << 7);

#define LED1_PIN PIN6
#define LED2_PIN PIN7

int main(void) {
	tim2_output_compare_PA5();

	tim3_output_compare_PA6();


	while (1) {

	}
}
