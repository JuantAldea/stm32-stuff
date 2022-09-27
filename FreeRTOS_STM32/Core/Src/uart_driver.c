#include "stm32f4xx_hal.h"
#include "uart_driver.h"

static UART_HandleTypeDef huart1;

static void USART1_UART_Init(void);

void USART1_UART_TX_Init(void)
{
    huart1.Init.Mode = UART_MODE_TX;
    USART1_UART_Init();
}

void USART1_UART_RX_Init(void)
{
    huart1.Init.Mode = UART_MODE_RX;
    USART1_UART_Init();
}

static void USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        //Error_Handler();
        while (1) {}
    }
}

int USART1_write(int ch)
{
    //HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    while (!(USART1->SR & 0x80)) {}
    USART1->DR = (ch & 0xFF);
    return ch;
}

int USART1_read()
{
    char ch;
    HAL_UART_Receive(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    return ch;
    //while (!(USART1->SR & USART_SR_RXNE)) {}
    //return USART1->DR;
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    //HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    //return ch;
    return USART1_write(ch);
}

__attribute__((weak))
int _write(__attribute__((unused)) int fd, char *ptr, int len)
{
    UNUSED(fd);
    HAL_UART_Transmit(&huart1, (uint8_t*) ptr, len, HAL_MAX_DELAY);
    return len;
}

