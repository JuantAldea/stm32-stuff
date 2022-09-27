#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

static void adc_callback(void);

uint32_t sensor_value;

int main(void)
{
	uart1_rx_tx_init();

	pa1_adc_interrupt_init();

	start_conversion();

	while (1) {
		;
	}
}

static void adc_callback(void)
{
	sensor_value = ADC1->DR;
	printf("Sensor value: %lu\n", sensor_value);
}

void ADC_IRQHandler(void)
{
	if (ADC1->SR & ADC_CSR_EOC1) {
		adc_callback();
		//clear: not needed
		//It is cleared by software or by reading the ADC_DR register.
		//ADC1->SR &= ADC_CSR_EOC1;
	}
}
