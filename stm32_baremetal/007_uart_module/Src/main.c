#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"

int main(void) {

    uart1_rx_tx_init();
	while (1) {
		printf("Testing printf\n");
	}
}
