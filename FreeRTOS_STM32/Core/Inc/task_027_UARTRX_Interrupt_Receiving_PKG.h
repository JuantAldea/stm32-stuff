#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"

#define STACK_SIZE 128 //128*4 = 512
#define PACKAGE_LEN 5

static SemaphoreHandle_t rxDone = NULL;

void HandlerTask(void *pvParameters);
int32_t start_rx_interrupt(uint8_t *buffer, uint_fast16_t len);

static int rxInProgress = 0;
static uint16_t rxLen = 0;
static uint8_t *rxBuff = NULL;
static uint16_t rxItr = 0;

void task_file_main(void)
{
    printf("init finished\r\n");
    rxDone = xSemaphoreCreateBinary();
    xTaskCreate(HandlerTask, "uartPrintTask", STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 3,
    NULL);
}

int32_t start_rx_interrupt(uint8_t *buffer, uint_fast16_t len)
{
    if (rxInProgress || !buffer) {
        return -1;
    }

    rxInProgress = 1;
    rxLen = len;
    rxBuff = buffer;
    rxItr = 0;

    USART1->CR1 |= USART_CR1_RXNEIE;
    NVIC_SetPriority(USART1_IRQn, 6);
    NVIC_EnableIRQ(USART1_IRQn);

    return 0;
}

char nextByte;
void USART1_IRQHandler(void)
{
    // do not wake up the highest priority task
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (USART1->SR & USART_SR_RXNE) {
        nextByte = USART1->DR;
    }

    if (!rxInProgress) {
        goto end;
    }
    if (rxItr >= rxLen) {
        goto end;
    }

    rxBuff[rxItr++] = nextByte;

    if (rxItr == rxLen) {
        rxInProgress = 0;
        xSemaphoreGiveFromISR(rxDone, &xHigherPriorityTaskWoken);
    }

    end:

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

char recivedByte;
uint8_t rxData[PACKAGE_LEN] = { 0 };
char rxCode[50] = { 0 };

void HandlerTask(void *pvParameters)
{
    USART1_UART_RX_Init();
    const TickType_t timeout = pdMS_TO_TICKS(5000);

    while (1) {
        start_rx_interrupt((uint8_t*) rxData, sizeof(rxData));
        if (xSemaphoreTake(rxDone, timeout) == pdPASS) {
            sprintf(rxCode, rxItr == PACKAGE_LEN ? "received" : "len mismatch");
        } else {
            sprintf(rxCode, "timeout");
            rxInProgress = 0;
        }
    }
}

#endif /* INC_TASK_000_H_ */
