#ifndef BME280_H_
#define BME280_H_

#include <stdint.h>

#define BME280_ADDR 0x76

#define BME280_REG_ID 0xD0
#define BME280_REG_CALIB_00 0x88
#define BME280_REG_CALIB_25 0xE1
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_STATUS 0xF3
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_TEMP 0xFA


#define BME280_STATUS_IM_UPDATE (1 << 0)
#define BME280_STATUS_IM_MEASURING (1 << 3)

#define BME280_CTRL_MEAS_OSRS_T_0 (1 << 5)
#define BME280_CTRL_MEAS_OSRS_T_1 (1 << 6)
#define BME280_CTRL_MEAS_OSRS_T_2 (1 << 7)

#define BME280_CTRL_MEAS_MODE    0x3
#define BME280_CTRL_MEAS_MODE_0 (1 << 0)
#define BME280_CTRL_MEAS_MODE_1 (1 << 1)

struct BME280_calib {
    unsigned short dig_T1;
    signed short dig_T2;
    signed short dig_T3;
    unsigned short dig_P1;
    signed short dig_P2;
    signed short dig_P3;
    signed short dig_P4;
    signed short dig_P5;
    signed short dig_P6;
    signed short dig_P7;
    signed short dig_P8;
    signed short dig_P9;
    unsigned char dig_H1;
    signed short dig_H2;
    unsigned char dig_H3;
    signed short dig_H4;
    signed short dig_H5;
    signed char dig_H6;
} __attribute__((packed));

int32_t BME280_read_adc_T();
int32_t BME280_compensate_T_int32(int32_t adc_T, const struct BME280_calib *calibration_data);
uint8_t BME280_sensor_ID();
void BME280_read_calibration_data(struct BME280_calib *calibration_data);
void BME280_wait_initialization();

#endif /* BME280_H_ */
