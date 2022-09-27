#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "ds3234.h"
#include "misc.h"
#include "spi.h"

#define PIN6 (1UL << 6)
#define PIN7 (1UL << 7)

#define LED1_PIN PIN6
#define LED2_PIN PIN7

int main(void)
{
    uart1_tx_init();

    printf("%s\n", __FUNCTION__);

    SPI1_gpio_init();
    SPI1_config();
    ds3234_init();
    SPI1_enable();

    while (true){
        int8_t temperature = ds3234_read_temperature();
        (void) temperature;
        printf("Temperature: %d\n", temperature);
    }

    SPI1_disable();
}

