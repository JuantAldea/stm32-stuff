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

void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);
    xTaskCreate(vBlueLedControllerTask, "Blue Led Controller", 100, NULL, 2, &blueHandle);
    xTaskCreate(vRedLedControllerTask, "Red Led Controller", 100, NULL, 2, &redHandle);
    xTaskCreate(vGreenLedControllerTask, "Green Led Controller", 100, NULL, 2, &greenHandle);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        blueTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        for (int i = 0; i < 100000; i++) {}
        //vTaskSuspend(NULL); //self-suspend
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        for (int i = 0; i < 100000; i++) {}
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        for (int i = 0; i < 100000; i++) {}
        ExecutionMonitor++;
        if (ExecutionMonitor >= 50) {
            vTaskSuspend(blueHandle);
        }

        if (ExecutionMonitor >= 100) {
            ExecutionMonitor = 0;
            vTaskResume(blueHandle);
            eTaskState state = eTaskGetState(redHandle);
            if (state != eDeleted && state != eInvalid) {
                vTaskDelete(NULL);
            }
        }
    }
}

#endif /* INC_TASKS_H_ */
