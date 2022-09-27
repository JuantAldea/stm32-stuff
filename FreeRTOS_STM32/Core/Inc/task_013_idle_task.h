#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);

uint32_t blueTaskProfiler, redTaskProfiler, greenTaskProfiler;
uint32_t IdleTaskProfiler;

TaskHandle_t blueHandle, redHandle, greenHandle;
uint32_t GreenPriority;
uint32_t ExecutionMonitor;
const TickType_t _250ms = pdMS_TO_TICKS(250);

void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);
    xTaskCreate(vBlueLedControllerTask, "Blue Led Controller", 100, NULL, 2, &blueHandle);
    xTaskCreate(vRedLedControllerTask, "Red Led Controller", 100, NULL, 2, &redHandle);
    xTaskCreate(vGreenLedControllerTask, "Green Led Controller", 100, NULL, 2, &greenHandle);
}

void vApplicationIdleHook(void)
{
    //requires enabling idle hook in FreeRTOS config
    IdleTaskProfiler++;
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        blueTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        vTaskDelay(_250ms);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        vTaskDelay(_250ms);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        vTaskDelay(_250ms);
    }
}

#endif /* INC_TASKS_H_ */
