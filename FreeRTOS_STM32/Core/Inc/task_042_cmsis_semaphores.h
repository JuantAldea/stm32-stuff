#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);
void timer1Callback(void *pvParameters);
void timer2Callback(void *pvParameters);

uint32_t blueTaskProfiler, redTaskProfiler, greenTaskProfiler;
TaskHandle_t blueHandle, redHandle, greenHandle;
osThreadId_t blueId, redId, greenId;
osSemaphoreId_t uartSemaphore;

void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);

    osThreadAttr_t blueThreadAttr = { .name = "Blue Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t redThreadAttr = { .name = "Red Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t greenThreadAttr = { .name = "Green Led Controller", .stack_size = 512, .priority = osPriorityNormal, };

    blueId = osThreadNew(vBlueLedControllerTask, NULL, &blueThreadAttr);
    greenId = osThreadNew(vGreenLedControllerTask, NULL, &greenThreadAttr);
    redId = osThreadNew(vRedLedControllerTask, NULL, &redThreadAttr);
    osSemaphoreAttr_t uartSemaphoreAttr = { .name = "UART Semaphore" };
    uartSemaphore = osSemaphoreNew(1, 1, &uartSemaphoreAttr);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        blueTaskProfiler++;
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        osSemaphoreAcquire(uartSemaphore, osWaitForever);
        printf("%s\r\n", __PRETTY_FUNCTION__);
        osSemaphoreRelease(uartSemaphore);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        osSemaphoreAcquire(uartSemaphore, osWaitForever);
        printf("%s\r\n", __PRETTY_FUNCTION__);
        osSemaphoreRelease(uartSemaphore);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        osDelay(100);
        osSemaphoreAcquire(uartSemaphore, osWaitForever);
        printf("%s\r\n", __PRETTY_FUNCTION__);
        osSemaphoreRelease(uartSemaphore);
    }
}

#endif /* INC_TASKS_H_ */
