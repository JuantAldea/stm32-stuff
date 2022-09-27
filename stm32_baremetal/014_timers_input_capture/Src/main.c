#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "systick.h"
#include "tim.h"

volatile uint32_t timestamp = 0;
volatile float period;
int main(void) {
	uart1_rx_tx_init();
	//printf("Started\n");
	//printf("%s\n", __FUNCTION__);

	//output_compare_PA0_TIM5_CH1();
	//output_compare_PA2_TIM2_CH3();
	//output_compare_PA5_TIM2_CH1();
	output_compare_PA6_TIM3_CH1();
	//output_compare_PA7_TIM3_CH2();

	//input_capture_PA2_TIM2_CH3(); //mal
	input_capture_PA5_TIM2_CH1();
	//input_capture_PA6_TIM3_CH1();
	//input_capture_PA7_TIM3_CH2();
	//TIM5->CR1 = TIM_CR1_CEN;
	//uint32_t last = 0;
	while (1) {
		do {
			timestamp = TIM2->CCR1;
			//printf("CNT %lu\n", TIM2->CNT);
		} while (!(TIM2->SR & TIM_SR_CC1IF));

		printf("PERIOD %lu\n", timestamp);
		printf("PERIOD2 %lu\n", TIM2->PSC+1);
		//printf("ASAD");
		period = timestamp/(16000000.0*TIM2->PSC + 1);


		/*
		volatile uint8_t overflow = last > timestamp;
		volatile uint32_t period;
		if (overflow) {
			period = TIM2->ARR - last + timestamp;
		} else {
			period = timestamp - last;
		}
		printf("last: %lu\nTS: %lu\nPeriod: %lu, %d\n", last, timestamp, period, overflow);
		//printf("%lu\n", period);
		last = timestamp;
		*/
	}
}
