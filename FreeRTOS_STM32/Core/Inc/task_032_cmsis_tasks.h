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

osTimerId_t timer1Id, timer2Id;
uint32_t timer1Executed = 0;
uint32_t timer2Executed = 0;


void task_file_main(void)
{
    printf(__PRETTY_FUNCTION__);
    osThreadAttr_t blueThreadAttr = { .name = "Blue Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t redThreadAttr = { .name = "Red Led Controller", .stack_size = 512, .priority = osPriorityNormal, };
    osThreadAttr_t greenThreadAttr = { .name = "Green Led Controller", .stack_size = 512, .priority = osPriorityNormal, };

    blueId = osThreadNew(vBlueLedControllerTask, NULL, &blueThreadAttr);
    greenId = osThreadNew(vGreenLedControllerTask, NULL, &greenThreadAttr);
    redId = osThreadNew(vRedLedControllerTask, NULL, &redThreadAttr);

    timer1Id = osTimerNew(timer1Callback, osTimerOnce, NULL, NULL);
    timer2Id = osTimerNew(timer2Callback, osTimerPeriodic, NULL, NULL);

}

void vBlueLedControllerTask(void *pvParameters)
{
    if (osOK != osTimerStart(timer1Id, 5000)) {
        while (1) {}
    }
    if (osOK != osTimerStart(timer2Id, 1000)) {
        while (1) {}
    }
    while (1) {
        blueTaskProfiler++;
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        if (blueTaskProfiler == 1500000) {
            osThreadResume(greenId);
        }
    }
}

void vGreenLedControllerTask(void *pvParameters)
{
    while (1) {
        greenTaskProfiler++;
        osDelay(100);
        HAL_GPIO_TogglePin(GPIOA, LED2_Pin);
        if (greenTaskProfiler == 1000000) {
            osThreadSuspend(greenId);
        } else if (greenTaskProfiler == 1500000) {
            osThreadTerminate(blueId);
            osThreadTerminate(greenId);
        }
    }
}

void vRedLedControllerTask(void *pvParameters)
{
    while (1) {
        redTaskProfiler++;
        osDelay(100);
        if (redTaskProfiler == 500000) {
            osThreadSetPriority(redId, osPriorityBelowNormal);
        }
    }
}

void timer1Callback(void *pvParameters)
{
    timer1Executed++;
}

void timer2Callback(void *pvParameters)
{
    timer2Executed++;
}

#endif /* INC_TASKS_H_ */
