#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);

uint32_t blueTaskProfiler, redTaskProfiler, greenTaskProfiler;
TaskHandle_t blueHandle, redHandle, greenHandle;
osThreadId_t blueId, redId, greenId;
osEventFlagsId_t eventFlagsId;
void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);

    osThreadAttr_t blueThreadAttr = { .name = "Blue Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t redThreadAttr = { .name = "Red Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t greenThreadAttr = { .name = "Green Led Controller", .stack_size = 512, .priority = osPriorityNormal, };

    blueId = osThreadNew(vBlueLedControllerTask, NULL, &blueThreadAttr);
    greenId = osThreadNew(vGreenLedControllerTask, NULL, &greenThreadAttr);
    redId = osThreadNew(vRedLedControllerTask, NULL, &redThreadAttr);
    osEventFlagsAttr_t eventFlagsAttr = { .name = "Event flags LED" };
    eventFlagsId = osEventFlagsNew(&eventFlagsAttr);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        blueTaskProfiler++;
        osDelay(500);
        osEventFlagsSet(eventFlagsId, 0x10);
        //HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        //osDelay(100);
        osEventFlagsWait(eventFlagsId, 0x10, osFlagsWaitAll, osWaitForever);
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        osDelay(100);
    }
}

#endif /* INC_TASKS_H_ */
