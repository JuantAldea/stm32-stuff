#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);

uint32_t blueTaskProfiler, redTaskProfiler, greenTaskProfiler;
TaskHandle_t blueHandle, redHandle, greenHandle;
uint32_t GreenPriority;
uint32_t ExecutionMonitor;

SemaphoreHandle_t xBinarySemaphore;

void task_file_main(void)
{
    printf("INIT TASK\r\n");

    xBinarySemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore);
    xTaskCreate(vBlueLedControllerTask, "Sender 1", 1024, NULL, 1, NULL);
    xTaskCreate(vRedLedControllerTask, "Sender 2", 1024, NULL, 1, NULL);
    xTaskCreate(vGreenLedControllerTask, "Receiver", 1024, NULL, 1, NULL);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        blueTaskProfiler++;
        //HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        xSemaphoreGive(xBinarySemaphore);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        greenTaskProfiler++;
        //HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        xSemaphoreGive(xBinarySemaphore);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        redTaskProfiler++;
        xSemaphoreGive(xBinarySemaphore);
    }
}

#endif /* INC_TASK_000_H_ */
