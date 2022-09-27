#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"

#define STACK_SIZE 128 //128*4 = 512

static QueueHandle_t uart_BytesReceived = NULL;
void polledUartReceive(void *pvParameters);
void HandlerTask(void *pvParameters);

void task_file_main(void)
{
    printf("init finished\r\n");


    xTaskCreate(polledUartReceive, "polled uart rx",
    STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 2,
    NULL);

    xTaskCreate(HandlerTask, "uartPrintTask", STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 3,
    NULL);

    uart_BytesReceived = xQueueCreate(10, sizeof(char));
}

uint8_t nextByte;
void polledUartReceive(void *pvParameters)
{
    printf("pulled\r\n");
    USART1_UART_RX_Init();

    while (1) {
        nextByte = USART1_read();
        xQueueSend(uart_BytesReceived, &nextByte, 0);
    }
}

char recivedByte;

void HandlerTask(void *pvParameters)
{
    while (1) {
        xQueueReceive(uart_BytesReceived, &recivedByte, portMAX_DELAY);
    }
}

#endif /* INC_TASK_000_H_ */
