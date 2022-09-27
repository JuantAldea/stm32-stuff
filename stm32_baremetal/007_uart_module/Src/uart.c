#include "uart.h"
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

void uart1_rx_tx_init() {
	//UART1_TX -> PA9
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

char uart_read(USART_TypeDef *uartx){
	while (!(uartx->SR & USART_SR_RXNE)) {

	}
	return uartx->DR;
}

void uart_write(USART_TypeDef *uartx, int ch) {
	// ensure transmit data register is empty
	while (!(uartx->SR & USART_SR_TXE)) {

	}
	// write to transmit data register
	uartx->DR = ch;

	while (!(uartx->SR & USART_SR_TXE)) {
	}
}

static uint16_t compute_uart_bd(uint32_t perif_clk, uint32_t baudrate) {
	// integer round up equivalent to round(x.y) = X.Y + 0.5
	// CLK/BR + 1/2
	return (perif_clk + (baudrate / 2U)) / baudrate;
}

static void set_uart_baudrate(USART_TypeDef *USARTx, uint32_t perif_clk, uint32_t baudrate) {
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
	USARTx->BRR = compute_uart_bd(perif_clk, baudrate);
}

int __io_putchar(int ch) {
	uart_write(USART1, ch);
	return ch;
}

