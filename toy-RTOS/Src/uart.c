#include <stm32f407xx.h>
#include <system_stm32f4xx.h>
#include "config.h"
#include <stdio.h>

#include "uart.h"

int __io_putchar(int ch)
{
    return uart1_write(ch);
}

int indexA = 0;
int len = 3;
char s[10] = "1\r\n";
int __io_getchar(void)
{
    int ch = uart1_read();
    //indexA = (indexA + 1) % len;
    //int ch = s[indexA];
    uart1_write(ch);
    return ch;
}

void uart1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER9_Msk | GPIO_MODER_MODER10_Msk);
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;

    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_Msk | GPIO_AFRH_AFSEL10_Msk;
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9_3 | GPIO_AFRH_AFSEL10_3);

    USART1->BRR &= ~(USART_BRR_DIV_Fraction_Msk | USART_BRR_DIV_Mantissa_Msk);
    USART1->BRR |= SYS_FREQ / UART_BAUDRATE;

    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

uint8_t uart1_write(uint8_t byte)
{
    while (!(USART1->SR & USART_SR_TXE)) {};
    USART1->DR = byte;
    return byte;
}

uint8_t uart1_read(void)
{
    USART1->SR &= ~USART_SR_ORE;
    while (!(USART1->SR & USART_SR_RXNE)) {};
    return USART1->DR;
}
