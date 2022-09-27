#include "spi.h"
#include "ds3234.h"
#include <stm32f407xx.h>

static void ds3234_enable_slave();
static void ds3234_disable_slave();

static void ds3234_read_bytes(uint8_t reg, uint8_t *buffer, uint32_t n);
//static void ds3234_write_bytes(uint8_t reg, uint8_t *buffer, uint32_t n);

int8_t ds3234_read_temperature()
{
    uint8_t buffer[2];
    ds3234_read_bytes(0x11, buffer, 2);
    //int8_t sign = (buffer[0] & 0x80) ? -1 : 1;
    //int8_t a = buffer[0];
    //int8_t b = buffer[1] >> 6;
    //float number = a + 0.25 * b;
    return buffer[0];
    //return sign * ((buffer[0] & ~0x80) + 0.25 * (buffer[1] >> 6));
}

void ds3234_read_bytes(uint8_t reg, uint8_t *buffer, uint32_t n)
{
    ds3234_enable_slave();

    SPI1_transfer(&reg, buffer, 1);
    SPI1_transfer(&(uint8_t) { 0 }, buffer, n);
    ds3234_disable_slave();
}

void ds3234_init()
{
    //general ouput mode
    GPIOB->MODER |= GPIO_MODER_MODER6_0;
    __asm volatile("dsb");
    ds3234_disable_slave();
}

void ds3234_enable_slave()
{
    GPIOB->ODR &= ~GPIO_ODR_OD6;
    __asm volatile("dsb");
}

void ds3234_disable_slave()
{
    GPIOB->ODR |= GPIO_ODR_OD6;
    __asm volatile("dsb");
}
