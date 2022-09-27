#include "uart.h"
#include "misc.h"

// Oscillator values in Hz
#define HSE_VALUE (8000000)
#define LSE_VALUE (32768)
#define EXTERNAL_CLOCK_VALUE (12288000)

// Oscillator timeouts in ms
#define HSE_STARTUP_TIMEOUT (100)
#define LSE_STARTUP_TIMEOUT (5000)

#define SYS_FREQ (16000000L)

#define APB1_CLK SYS_FREQ

static void set_uart_baudrate(USART_TypeDef *USARTx, uint32_t perif_clk, uint32_t baudrate);
static uint16_t compute_uart_bd(uint32_t perif_clk, uint32_t baudrate);

void dma2_channel4_init(uint32_t src, uint32_t dst, uint32_t len)
{
    // enable clock access to DMA2
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    // disable DMA2
    DMA2_Stream7->CR &= DMA_SxCR_EN;

    // wait until it is disabled
    register_wait_flags_reset(&DMA2_Stream7->CR, DMA_SxCR_EN);

    // clear all interrupt flags of stream 7
    // but why...?
    DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
    DMA2->HIFCR |= DMA_HIFCR_CHTIF7;
    DMA2->HIFCR |= DMA_HIFCR_CTEIF7;
    DMA2->HIFCR |= DMA_HIFCR_CFEIF7;
    DMA2->HIFCR |= DMA_HIFCR_CDMEIF7;
    // destination buffer
    DMA2_Stream7->PAR = dst;
    // set source buffer Memory = Address Register
    DMA2_Stream7->M0AR = src;
    // set length (Number of DaTa Register
    DMA2_Stream7->NDTR = len;
    // select channel 4
    DMA2_Stream7->CR = DMA_SxCR_CHSEL_2;
    // set memory increment
    DMA2_Stream7->CR |= DMA_SxCR_MINC;
    // set transfer direction: memory to peripheral
    DMA2_Stream7->CR |= DMA_SxCR_DIR_0;
    // enable transfer complete interrupt
    DMA2_Stream7->CR |= DMA_SxCR_TCIE;
    // enable direct mode and disable FIFO
    DMA2_Stream7->FCR = 0;
    // enable DMA2 stream 7
    DMA2_Stream7->CR |= DMA_SxCR_EN;
    // enable USART1 transmitter DMA
    USART1->CR3 |= USART_CR3_DMAT;
    // enable DMA interrupt in NVIC
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

void uart1_tx_init()
{
    //UART1_TX -> PA9
    // pass clock to GPIO_A
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //pass clock to USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //set mode to alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER9;
    GPIOA->MODER |= GPIO_MODER_MODER9_1;

    // set alternate function to USART_TX AF07 (0111 -> 0x7 :D)
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_2 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_0;
    // configure UART module
    // baud rate
    set_uart_baudrate(USART1, APB1_CLK, UART_BAUDRATE);
    // transfer direction & enable UART module
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart1_rx_tx_init()
{
    //UART1_TX -> PA9
    //UART1_RX -> PA10
    // pass clock to GPIO_A
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //pass clock to USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //set mode to alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER9;
    GPIOA->MODER |= GPIO_MODER_MODER9_1;

    GPIOA->MODER &= ~GPIO_MODER_MODER10;
    GPIOA->MODER |= GPIO_MODER_MODER10_1;

    // set alternate function to USART_TX AF07 (0111 -> 0x7 :D)
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_2 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_0;
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_0;
    // configure UART module
    // baud rate
    set_uart_baudrate(USART1, APB1_CLK, UART_BAUDRATE);
    // transfer direction & enable UART module
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void uart1_rx_tx_interrupt_init()
{
    //UART1_TX -> PA9
    //UART1_RX -> PA10
    // pass clock to GPIO_A
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    //pass clock to USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //set mode to alternate function
    GPIOA->MODER &= ~GPIO_MODER_MODER9;
    GPIOA->MODER |= GPIO_MODER_MODER9_1;

    GPIOA->MODER &= ~GPIO_MODER_MODER10;
    GPIOA->MODER |= GPIO_MODER_MODER10_1;

    // set alternate function to USART_TX AF07 (0111 -> 0x7 :D)
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_2 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_0;
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_0;
    // configure UART module
    // baud rate
    set_uart_baudrate(USART1, APB1_CLK, UART_BAUDRATE);
    // transfer direction & enable UART module
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART1_IRQn);
}

char uart_read(USART_TypeDef *uartx)
{
    register_wait_flags_set(&uartx->SR, USART_SR_RXNE);
    return uartx->DR;
}

void uart_write(USART_TypeDef *uartx, int ch)
{
    // ensure transmit data register is empty
    register_wait_flags_set(&uartx->SR, USART_SR_TXE);

    // write to transmit data register
    uartx->DR = ch;

    register_wait_flags_set(&uartx->SR, USART_SR_TXE);
}

static uint16_t compute_uart_bd(uint32_t perif_clk, uint32_t baudrate)
{
    /*
     * See RM0090 (r19) -> 30.3.4 Fractional baud rate generation
     *
     * USARTDIV = 16Mhz/(16*115200) = 1M/115200 = 8.680555..
     * fraction:
     * 		16*0.6805555 = 10.88 -> round(10.88) = 11 = 0xB -> no carry
     * mantissa:
     * 		8 + carry = 8
     * 	Note:
     * 		baud rate register (USART_BRR) - 12-bit mantissa and 4-bit fraction.
     * 	so:
     * 	BRR = mantissa << 4 | fraction
     */

    //USARTx->BRR = (8 << 4) | 11;
    // integer round up equivalent to round(x.y) = X.Y + 0.5
    // CLK/BR + 1/2
    return (perif_clk + (baudrate / 2U)) / baudrate;
}

static void set_uart_baudrate(USART_TypeDef *USARTx, uint32_t perif_clk, uint32_t baudrate)
{
    USARTx->BRR = compute_uart_bd(perif_clk, baudrate);
}

int __io_putchar(int ch)
{
    uart_write(USART1, ch);
    return ch;
}

