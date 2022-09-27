#include <stdio.h>
#include <stm32f4xx.h>

#include "i2c.h"
#include "misc.h"

#define I2C_100KHZ 80
#define STANDARD_MODE_MAX_RISE_TIME 17

void i2c1_init(void)
{
    // i2c1_SCL = PB8
    // i2c1_SDA = PB9

    // clock access
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    // clock access to I2C
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR1 &= ~I2C_CR1_PE;

    // set pins to alternate functions
    GPIOB->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;
    // open drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9;
    // enable pull up resistors
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0;

    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2;

    // suppose
    //RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    //RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    // enter reset mode
    I2C1->CR1 |= I2C_CR1_SWRST;
    // exit reset mode
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    // set peripheral clock frequency -> PCLK1
    I2C1->CR2 |= 16; // 16 Mhz

    // set i2c clock to standard mode
    /* I2C_100KHZ = 80 explanation:
     * CLK signal period is hence 1/100.000hz = 10^-5 s
     * In standard mode 50% duty cycle (T_high = T_low)
     * Thus:
     * 	T_h = 5*10^-6s (5000ns)
     * Given:
     * 	T_high = CCR * T_PCLK1
     * 	CCR = T_high/T_PCLK1
     * 	CCR = 5*10^-6 / (1/16000000) = 80
     */

    I2C1->CCR |= I2C_100KHZ;

    /* Rise Time:
     * T_PCLK1 = 1/16000000 = 62.5*10^-9s = 62.5ns
     * In standard mode, max rise time is 1000ns
     * 1000ns/62.5ns = 16
     * Rise time should be set to 16 + 1
     */
    I2C1->TRISE = STANDARD_MODE_MAX_RISE_TIME;

    I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_PE;

    register_wait_flags_reset(&I2C1->SR2, I2C_SR2_BUSY);
}

void I2C_start()
{
    // generate start condition
    I2C1->CR1 |= I2C_CR1_START;
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_SB);
}

uint8_t I2C_send_address(uint8_t addr_rw_mode)
{
    I2C_start();

    I2C1->DR = addr_rw_mode;

    // EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_ADDR);

    {
        __IO uint32_t _tmp = I2C1->SR2;
        (void) _tmp;
    }

    /*
     * The TRA bit indicates whether the master is in Receiver or Transmitter mode.
     * 0 -> Receiver
     * 1 -> Transmitter
     *
     * The opposite the R/W address bit
     */
    const uint8_t tra_bit = (~addr_rw_mode & 1) << I2C_SR2_TRA_Pos;
    while ((I2C1->SR2 & I2C_SR2_TRA) != tra_bit);

    return I2C1->SR1 & I2C_SR1_AF;
}

void I2C_read_byte(uint8_t *data)
{
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_RXNE);
    *data = I2C1->DR;
}

uint32_t I2C_read(uint8_t addr, uint32_t n, uint8_t *data)
{

    // What a jumpy mess is to have all three cases merged together! Never again.

    I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1  &= ~I2C_CR1_POS;

    if (I2C_send_address((addr << 1) | 1)) {
        printf("ERROR\n");
        trap();
    }

    if (n <= 2) {
        /*
         *  In single byte has to be received, the Acknowledge disable is made during EV6
         * (before ADDR flag is cleared) and the STOP condition generation is made after EV6.
         *
         * For 2-byte reception:
         *  - Wait until ADDR = 1 (above)I2C_wait_bits_set
         *  - Set ACK low
         */
        I2C1->CR1 &= ~I2C_CR1_ACK;
    }

    if (n == 2) {
        //For 2-byte reception: set POS high
        I2C1->CR1 |= I2C_CR1_POS;
    }

    {
        // clear ADDR flag
        __IO int __clear_addr = I2C1->SR2;
        (void) __clear_addr;
    }

    uint32_t bytes_left = n;

    if (bytes_left == 1) {
        goto single_byte;
    }

    while (bytes_left > 3) {
        I2C_read_byte(data++);
        bytes_left--;
    }

    if (bytes_left >= 2) {
        /*
         *
         * For EITHER case:
         *  - 2-byte reception:
         *  - N > 2-byte reception, from N-2 data reception
         *     * Wait until BTF = 1 -> DR and Shift Register filled
         */

        register_wait_flags_set(&I2C1->SR1, I2C_SR1_BTF);

        // Either (Data0 and Data1) OR (DataN-2 and DataN-1) are in DR and SR
    }

    if (bytes_left == 3) {
        /*
         * For N >2 -byte reception, from N-2 data reception
         *  - Set ACK low
         *  - Read data N-2
         *  - Wait until BTF = 1
         */

        I2C1->CR1 &= ~I2C_CR1_ACK;

        // read DataN-2
        I2C_read_byte(data++);
        //DR is now empty, clears BTF, and moves the N-1 from the sift register to DR

        bytes_left--;

        // wait until the last byte is stored in the shift register.
        register_wait_flags_set(&I2C1->SR1, I2C_SR1_BTF);
        // from this point both, DR and the sift register, are filled, DR = DataN-1, SR = DataN
    }

    // N <= 2 bytes are left to be read

    single_byte:

    I2C1->CR1 |= I2C_CR1_STOP;

    while (bytes_left) {
        I2C_read_byte(data++);
        bytes_left--;
    }

    register_wait_flags_reset(&I2C1->CR1, I2C_CR1_STOP);

    return n;
}

uint32_t I2C_write_perif(uint8_t addr, uint8_t reg, uint32_t n, uint8_t *data, uint8_t send_stop)
{
    if (I2C_send_address(addr << 1)) {
        printf("ERROR\n");
        trap();
    }

    //EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_TXE);

    I2C1->DR = reg;

    //register_wait_flags_set(&I2C1->SR1, I2C_SR1_BTF);
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_TXE);

    for (uint32_t i = 0; i < n; i++) {
        I2C1->DR = *data++;
        // EV8: TxE=1, shift register not empty,.data register empty, cleared by writing DR register
        register_wait_flags_set(&I2C1->SR1, I2C_SR1_TXE);
    }
    // EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
    register_wait_flags_set(&I2C1->SR1, I2C_SR1_BTF);

    if (send_stop) {
        I2C1->CR1 |= I2C_CR1_STOP;
    }

    register_wait_flags_reset(&I2C1->CR1, I2C_CR1_STOP);

    return n;
}

uint32_t I2C_write_register(uint8_t addr, uint8_t reg, uint8_t data)
{
    return I2C_write_perif(addr, reg, 1, &data, 1);
}

uint32_t I2C_read_perif(uint8_t addr, uint8_t reg, uint32_t n, uint8_t *data)
{
    I2C_write_perif(addr, reg, 0, NULL, 0);

    return I2C_read(addr, n, data);
}
