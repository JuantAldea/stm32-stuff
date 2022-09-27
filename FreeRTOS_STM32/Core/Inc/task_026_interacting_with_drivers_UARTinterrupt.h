#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"

#define STACK_SIZE 128 //128*4 = 512

static QueueHandle_t uart_BytesReceived = NULL;

void HandlerTask(void *pvParameters);
void start_rx_interrupt(void);

static int rxInProcess = 0;
void task_file_main(void)
{
    printf("init finished\r\n");

    xTaskCreate(HandlerTask, "uartPrintTask", STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 3,
    NULL);

    uart_BytesReceived = xQueueCreate(10, sizeof(char));
}

void start_rx_interrupt(void)
{
    rxInProcess = 1;
    USART1->CR1 |= USART_CR1_RXNEIE;
    NVIC_SetPriority(USART1_IRQn, 6);
    NVIC_EnableIRQ(USART1_IRQn);
}

char nextByte;
void USART1_IRQHandler(void)
{
    // do not wake up the highest priority task
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (USART1->SR & USART_SR_RXNE) {
        nextByte = USART1->DR;
        xQueueSendFromISR(uart_BytesReceived, &nextByte, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

char recivedByte;
void HandlerTask(void *pvParameters)
{
    USART1_UART_RX_Init();
    start_rx_interrupt();

    while (1) {
        xQueueReceive(uart_BytesReceived, &recivedByte, portMAX_DELAY);
    }
}

#endif /* INC_TASK_000_H_ */
