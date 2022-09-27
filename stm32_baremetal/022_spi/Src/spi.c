#include <stm32f407xx.h>

#include "spi.h"
#include "misc.h"
#include <stddef.h>

#ifdef HALF_DUPLEX

void SPI1_half_duplex_send(uint8_t *buffer, uint32_t n)
{
    /*
     * In bidirectional mode, when transmitting (BIDIMODE=1 and BIDIOE=1)
     *  - The sequence begins when data are written into the SPI_DR register (Tx buffer).
     *  - The data are then parallel loaded from the Tx buffer into the 8-bit shift register
     *      during the first bit transmission and then shifted out serially to the MOSI pin.
     *  - No data are received
     */

    SPI1->CR1 |= SPI_CR1_BIDIMODE;
    SPI1->CR1 |= SPI_CR1_BIDIOE;

    SPI1->CR1 |= SPI_CR1_SPE;

    while (n) {
        SPI1->DR = *buffer++;
        register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);
        n--;
    }

    register_wait_flags_reset(&SPI1->SR, SPI_SR_BSY);

    SPI1->CR1 &= ~SPI_CR1_SPE;
}

void SPI1_half_duplex_recv(uint8_t *buffer, uint32_t n)
{
    /*
     * Start sequence in master mode
     * In bidirectional mode, when receiving (BIDIMODE=1 and BIDIOE=0)
     * - The sequence begins as soon as SPE=1 and BIDIOE=0.
     * - The received data on the MOSI pin are shifted in serially to the 8-bit shift register
     *      and then parallel loaded into the SPI_DR register (Rx buffer).
     * - The transmitter is not activated and no data are shifted out serially to the MOSI pin.
     */

    SPI1->CR1 |= SPI_CR1_BIDIMODE;
    SPI1->CR1 &= ~SPI_CR1_BIDIOE;
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    __disable_irq();

    SPI1->CR1 |= SPI_CR1_SPE;

    while (n > 1) {
        register_wait_flags_set(&SPI1->SR, SPI_SR_RXNE);
        *buffer++ = SPI1->DR;
        n--;
    }

    /*
     * crazy idea?
     * SCK pre-scaler is 4 so 1 bit is transfered per 4 clock cycles (SPI SCK runs 4 times slower than CLK)
     * For receiving a single/the last byte, SPI should be disabled while such last byte is being transfered.
     *
     * So I guess, ~4 DSB should execute per SCK cycle, although the first one might take longer[1] as DSB
     * takes a total of:
     *      1 + "The number of cycles required to perform the barrier operation". For DSB and DMB, the minimum number of cycles is zero."
     * cycles.
     *
     * So ~4 DSB/byte * 8bytes = ~32 DSB should fit during a byte transfer at the given SCK speed.
     *
     * I could not find any real documentation about this, but there are some undocumented examples[2][3][4].
     * In [3], SCK pre-scaler is set to 16 so there are ~16 DSB are executed per SPI-bit-transfer
     * which accounts for 128 DSBs (16 * 8) fitting in a full byte transmission,
     * and there are 67 DSB in [4] so a bit more than half an SCK-cyle worth of DSB
     *
     * But this is STM, HAL, and its examples, and there are a lot of complains regarding STM deliverables.
     *
     * Dark clouds on the horizon.
     *
     * So lets just drop more than one 1 bit, but less than 8 worth of DSBs.
     *
     * [1] https://developer.arm.com/documentation/ddi0439/b/CHDDIGAC
     * [2] https://github.com/STMicroelectronics/STM32CubeL4/blob/5e1553e07706491bd11f4edd304e093b6e4b83a4/Drivers/BSP/STM32L476G-Discovery/stm32l476g_discovery.c#L709
     * [3] https://github.com/STMicroelectronics-CentralLabs/ST_Drone_FCU_F401/blob/f6e3e75629b122fdd82df3de7510e28f05534948/STM32%20FW%20Project/BLE%20Remocon%20Beta%20release%20301117/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1.c#L254
     * [4] https://github.com/STMicroelectronics-CentralLabs/ST_Drone_FCU_F401/blob/f6e3e75629b122fdd82df3de7510e28f05534948/STM32%20FW%20Project/BLE%20Remocon%20Beta%20release%20301117/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1.c#L533
     */

    __asm volatile("dsb");
    __asm volatile("dsb");
    __asm volatile("dsb");
    __asm volatile("dsb");

    __asm volatile("dsb");
    __asm volatile("dsb");
    __asm volatile("dsb");
    __asm volatile("dsb");

    SPI1->CR1 &= ~SPI_CR1_SPE;

    __enable_irq();

    register_wait_flags_set(&SPI1->SR, SPI_SR_RXNE);
    *buffer = SPI1->DR;
}

#endif

void SPI1_gpio_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    //alternate function
    GPIOB->MODER |= GPIO_MODER_MODER3_1;
    GPIOB->MODER |= GPIO_MODER_MODER4_1;
    GPIOB->MODER |= GPIO_MODER_MODER5_1;

    GPIOB->AFR[0] |= GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_0;
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL4_2 | GPIO_AFRL_AFRL4_0;
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL5_0;

    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_1 | GPIO_OSPEEDR_OSPEED4_1;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1;

    __asm volatile("dsb");
}

void SPI1_config()
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // clock phase and polarity
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;

    SPI1->CR1 |= SPI_CR1_MSTR;

    // baudrate 16Mhz / 4 = 4Mhz
    SPI1->CR1 |= SPI_CR1_BR_0;
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    SPI1->CR1 |= SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SSM;

    SPI1->CR1 &= ~SPI_CR1_RXONLY;
    SPI1->CR1 &= ~SPI_CR1_DFF;

    SPI1->CR2 = 0;

    __asm volatile("dsb");
}

void SPI1_enable()
{
    SPI1->CR1 |= SPI_CR1_SPE;
    __asm volatile("dsb");
}

void SPI1_disable()
{
    SPI1->CR1 &= ~SPI_CR1_SPE;
    __asm volatile("dsb");
}

void SPI1_send(uint8_t *buffer, uint32_t n)
{
    while (n) {
        register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);
        SPI1->DR = *buffer++;
        n--;
    }

    register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);
    register_wait_flags_reset(&SPI1->SR, SPI_SR_BSY);

    {
        __IO uint16_t _reset_overflow = SPI1->DR;
        _reset_overflow = SPI1->SR;
        (void) _reset_overflow;
    }
}

void SPI1_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t n)
{
    //SPI1_enable();

    while (n--) {
        SPI1->DR = *tx_buffer++;

        register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);
        register_wait_flags_set(&SPI1->SR, SPI_SR_RXNE);

        if (rx_buffer != NULL) {
            *rx_buffer++ = SPI1->DR;
        }
    }

    register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);
    register_wait_flags_reset(&SPI1->SR, SPI_SR_BSY);
    //SPI1_disable();
}

void SPI1_recv(uint8_t *buffer, uint32_t n)
{
    register_wait_flags_set(&SPI1->SR, SPI_SR_TXE);

    while (n) {
        register_wait_flags_reset(&SPI1->SR, SPI_SR_BSY);
        // send dummy data
        SPI1->DR = 0;
        register_wait_flags_reset(&SPI1->SR, SPI_SR_RXNE);
        int8_t byte = SPI1->DR;
        *buffer++ = byte;
        n--;
    }

    register_wait_flags_reset(&SPI1->SR, SPI_SR_RXNE);
    register_wait_flags_reset(&SPI1->SR, SPI_SR_BSY);
}

