#include <stdio.h>
#include <stddef.h>
#include "bme280.h"
#include "i2c.h"
#include "spi.h"
#include "misc.h"

void BME280_wait_initialization()
{
    int retries;
    for (retries = 0; retries < 0xffff; retries++) {
        uint8_t buf;
        I2C_read_perif(BME280_ADDR, BME280_REG_STATUS, 1U, &buf);
        //printf("Status: %x\n", buf[0]);
        if (!(buf & BME280_STATUS_IM_UPDATE)) {
            printf("NVM copied\n");
            break;
        }
    }

    if (retries == 0xFFFF) {
        printf("NVM copy timed out");
        trap();
    }
}

int32_t BME280_read_adc_T()
{
    uint8_t buf[3];
    // Register 0xFA...0xFC “temp” (_msb, _lsb, _xlsb)
    I2C_read_perif(BME280_ADDR, BME280_REG_TEMP, 3U, buf);

    return (int32_t) ((buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4));
}

int32_t BME280_compensate_T_int32(int32_t adc_T, const struct BME280_calib *calibration_data)
{
    int32_t var1 = ((((adc_T >> 3) - ((int32_t) calibration_data->dig_T1 << 1))) * ((int32_t) calibration_data->dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t) calibration_data->dig_T1)) * ((adc_T >> 4) - ((int32_t) calibration_data->dig_T1))) >> 12)
            * ((int32_t) calibration_data->dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;

    return (t_fine * 5 + 128) >> 8;
}

void BME280_read_calibration_data(struct BME280_calib *calibration_data)
{
    I2C_read_perif(BME280_ADDR, BME280_REG_CALIB_00, offsetof(struct BME280_calib, dig_H1), (void*) calibration_data);

    uint8_t buf[9];
    I2C_read_perif(BME280_ADDR, BME280_REG_CALIB_25, sizeof(struct BME280_calib) - offsetof(struct BME280_calib, dig_H1), buf);

    calibration_data->dig_H2 = (int16_t) ((((int8_t) buf[1]) << 8) | buf[0]);
    calibration_data->dig_H3 = buf[2];
    calibration_data->dig_H4 = (int16_t) ((((int8_t) buf[3]) << 4) | (buf[4] & 0x0f));
    calibration_data->dig_H5 = (int16_t) ((((int8_t) buf[5]) << 4) | (buf[4] >> 4));
    calibration_data->dig_H6 = (int8_t) buf[6];
}

uint8_t BME280_sensor_ID()
{
    uint8_t id;
    I2C_read_perif(BME280_ADDR, BME280_REG_ID, 1U, &id);
    return id;
}

void BME280_SPI_wait_initialization()
{
    int retries;
    for (retries = 0; retries < 0xffff; retries++) {
        uint8_t buf;
        I2C_read_perif(BME280_ADDR, BME280_REG_STATUS, 1U, &buf);
        //printf("Status: %x\n", buf[0]);
        if (!(buf & BME280_STATUS_IM_UPDATE)) {
            printf("NVM copied\n");
            break;
        }
    }

    if (retries == 0xFFFF) {
        printf("NVM copy timed out");
        trap();
    }
}
