#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"
#include "stdlib.h"

void SenderTask(void *pvParameters);
void ReceiverTask(void *pvParameters);

TaskHandle_t Sender1Handle, Sender2Handle, ReceiverHandle;
const TickType_t _250ms = pdMS_TO_TICKS(250);
QueueHandle_t yearQueue;

void task_file_main(void)
{
    yearQueue = xQueueCreate(5, sizeof(int32_t));
    int32_t *params = malloc(2 * sizeof(int32_t));
    params[0] = 2050;
    params[1] = 5050;
    xTaskCreate(SenderTask, "Sender 1", 1024, &params[0], 1, &Sender1Handle);
    xTaskCreate(SenderTask, "Sender 2", 1024, &params[1], 1, &Sender2Handle);
    xTaskCreate(ReceiverTask, "Receiver", 1024, NULL, 2, &ReceiverHandle);
}

void SenderTask(void *pvParameters)
{
    const int32_t value_to_send = *((int32_t*) pvParameters);

    while (1) {
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);

        // 0 -> non-blocking
        BaseType_t qStatus = xQueueSend(yearQueue, &value_to_send, 0);

        if (qStatus != pdPASS) {
            printf("%s Error: Data could not be sent\r\n", __PRETTY_FUNCTION__);
        }
    }
}

void ReceiverTask(void *pvParameters)
{
    const TickType_t wait_time = pdMS_TO_TICKS(100);
    int32_t value_received;
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        BaseType_t qStatus = xQueueReceive(yearQueue, &value_received, wait_time);

        if (qStatus != pdPASS) {
            continue;
        }

        printf("%s Value received: %ld\r\n", __PRETTY_FUNCTION__, value_received);
    }
}

#endif /* INC_TASKS_H_ */
