#include <stm32f407xx.h>

#define HSE_VALUE              8000000U /*!< Value of the External oscillator in Hz */
#define HSI_VALUE              16000000U /*!< Value of the Internal oscillator in Hz */
#define LSI_VALUE               32000U    /*!< LSI Typical Value in Hz */
#define LSE_VALUE               32768U    /*!< Value of the External Low Speed oscillator in Hz */
#define USE_FULL_LL_DRIVER
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;


double adc_reading_to_celsius(uint32_t value)
{
    double volts = (3.3 * value) / 4096;
    return 25 + (volts - 0.76) / 0.0025;
    //return (float){2.5};
}


int main(void)
{
    /* set CP10 Full Access */
    /* set CP11 Full Access */
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    uart1_rx_tx_init();
    printf("%s\n", __FUNCTION__);
    //pa1_adc_init();
    //internal_temperature_sensor_init();

    //start_conversion();
    while (1) {
        sensor_value = read();
        double celsius = adc_reading_to_celsius(sensor_value);
        printf("sensor = %lu\t output = %f\n", sensor_value, celsius);
    }
}
