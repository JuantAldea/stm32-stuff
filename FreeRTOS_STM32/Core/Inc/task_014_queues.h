#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);

uint32_t blueTaskProfiler, redTaskProfiler, greenTaskProfiler;

TaskHandle_t blueHandle, redHandle, greenHandle;
uint32_t GreenPriority;
uint32_t ExecutionMonitor;
const TickType_t _250ms = pdMS_TO_TICKS(250);
QueueHandle_t yearQueue;

void task_file_main(void)
{
    yearQueue = xQueueCreate(5, sizeof(int32_t));
    printf(__PRETTY_FUNCTION__);
    xTaskCreate(vBlueLedControllerTask, "Blue Led Controller", 1024, NULL, 2, &blueHandle);
    xTaskCreate(vRedLedControllerTask, "Red Led Controller", 1024, NULL, 2, &redHandle);
    xTaskCreate(vGreenLedControllerTask, "Green Led Controller", 1024, NULL, 2, &greenHandle);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        blueTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        const int32_t value_to_send = 2050;
        // 0 -> non-blocking
        BaseType_t qStatus = xQueueSend(yearQueue, &value_to_send, 0);

        if (qStatus != pdPASS) {
            printf("%s Error: Data could not be sent\r\n", __PRETTY_FUNCTION__);
        }

        vTaskDelay(_250ms);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    const TickType_t wait_time = pdMS_TO_TICKS(100);
    int32_t value_received;
    while (1) {
        greenTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        BaseType_t qStatus = xQueueReceive(yearQueue, &value_received, wait_time);

        if (qStatus != pdPASS) {
            printf("%s Error: Could not received\r\n", __PRETTY_FUNCTION__);
            continue;
        }

        printf("%s Value received: %ld\r\n", __PRETTY_FUNCTION__, value_received);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    vTaskDelete(NULL);
}

#endif /* INC_TASKS_H_ */
