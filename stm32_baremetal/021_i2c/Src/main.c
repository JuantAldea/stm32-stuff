#include <stm32f407xx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "uart.h"
#include "i2c.h"
#include "bme280.h"
#include "misc.h"

#define PIN6 (1UL << 6)
#define PIN7 (1UL << 7)

#define LED1_PIN PIN6
#define LED2_PIN PIN7

int main(void)
{
    uart1_tx_init();

    printf("%s\n", __FUNCTION__);

    i2c1_init();

    BME280_wait_initialization();

    const uint8_t id = BME280_sensor_ID();
    printf("Sensor ID: %x\n", id);

    struct BME280_calib cal_param;
    BME280_read_calibration_data(&cal_param);

    I2C_write_register(BME280_ADDR, BME280_REG_CTRL_HUM, 0x0);
    I2C_write_register(BME280_ADDR, BME280_REG_CTRL_MEAS, BME280_CTRL_MEAS_OSRS_T_0 | BME280_CTRL_MEAS_MODE_1 | BME280_CTRL_MEAS_MODE_0);

    while (true) {
        int32_t temperature = BME280_compensate_T_int32(BME280_read_adc_T(), &cal_param);
        int32_t integer_part = temperature / 100;
        printf("Temperature: %ldd.%02ldC\n", integer_part, temperature - integer_part * 100);
    }
}

