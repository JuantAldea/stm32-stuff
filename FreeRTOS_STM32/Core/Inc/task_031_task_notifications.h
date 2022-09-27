#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "exti.h"

#define STACK_SIZE 128

void vHandlertask(void *pvParameters);

static TaskHandle_t xHandlerTask = NULL;

void task_file_main(void)
{
    printf("init finished\r\n");
    gpio_init();
    PE4_interrupt_init();
    xTaskCreate(vHandlertask, "Handler task", STACK_SIZE, NULL, 1, &xHandlerTask);
}

void EXTI4_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xHandlerTask, &xHigherPriorityTaskWoken);
    EXTI->PR = EXTI_PR_PR4;
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vHandlertask(void *pvParameters)
{
    const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS(100UL);
    while (1) {
        if (!ulTaskNotifyTake(pdFALSE, xMaxExpectedBlockTime)) {
            continue;
        }
        printf("Handler - Processing event\r\n");
    }
}

#endif /* INC_TASK_000_H_ */
