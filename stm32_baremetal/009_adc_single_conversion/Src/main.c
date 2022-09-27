#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;
int main(void) {

	uart1_rx_tx_init();
	pa1_adc_init();

	while (1) {
		start_conversion();
		sensor_value = adc_read();
		printf("Sensor value: %lu\n", sensor_value);
	}
}