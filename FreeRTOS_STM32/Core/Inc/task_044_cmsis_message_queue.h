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
osMessageQueueId_t messageQueue;

void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);

    osThreadAttr_t blueThreadAttr = { .name = "Blue Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t redThreadAttr = { .name = "Red Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t greenThreadAttr = { .name = "Green Led Controller", .stack_size = 512, .priority = osPriorityNormal, };

    blueId = osThreadNew(vBlueLedControllerTask, NULL, &blueThreadAttr);
    greenId = osThreadNew(vGreenLedControllerTask, NULL, &greenThreadAttr);
    redId = osThreadNew(vRedLedControllerTask, NULL, &redThreadAttr);
    osMessageQueueAttr_t messageQueueAttr = { .name = "Message queue" };
    messageQueue = osMessageQueueNew(5, sizeof(uint16_t), &messageQueueAttr);
}

void vBlueLedControllerTask(void *pvParameters)
{
    while (1) {
        uint16_t value = 2050;
        blueTaskProfiler++;
        osDelay(200);
        //HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        osMessageQueuePut(messageQueue, &value, 0, osWaitForever);
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        //osDelay(100);
        uint16_t buffer;
        osMessageQueueGet(messageQueue, &buffer, 0, osWaitForever);
        printf("%d\r\n", buffer);
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        //osDelay(100);
    }
}

#endif /* INC_TASKS_H_ */
