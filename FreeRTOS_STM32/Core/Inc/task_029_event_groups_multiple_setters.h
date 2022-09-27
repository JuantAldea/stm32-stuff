#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "event_groups.h"

#define TASK0_BIT (1UL << 0UL)
#define TASK1_BIT (1UL << 1UL)
#define TASK2_BIT (1UL << 2UL)

EventGroupHandle_t xEventGroup;
void vEventBitSettingTask0(void *pvParameters);
void vEventBitSettingTask1(void *pvParameters);
void vEventBitSettingTask2(void *pvParameters);
void vEventBitReadingTask(void *pvParameters);

const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

void task_file_main(void)
{
    printf("init finished\r\n");

    xEventGroup = xEventGroupCreate();
    xTaskCreate(vEventBitSettingTask0, "Bit Setter0", 100, NULL, 1, NULL);
    xTaskCreate(vEventBitSettingTask1, "Bit Setter1", 100, NULL, 1, NULL);
    xTaskCreate(vEventBitSettingTask2, "Bit Setter2", 100, NULL, 1, NULL);

    xTaskCreate(vEventBitReadingTask, "Bit Reader", 100, NULL, 1, NULL);
}

void vEventBitSettingTask0(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        //printf("Setting bit 0\r\n");
        xEventGroupSetBits(xEventGroup, TASK0_BIT);
    }
}

void vEventBitSettingTask1(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        //printf("Setting bit 1\r\n");
        xEventGroupSetBits(xEventGroup, TASK1_BIT);
    }
}

void vEventBitSettingTask2(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        //printf("Setting bit 2\r\n");
        xEventGroupSetBits(xEventGroup, TASK2_BIT);
    }
}

void vEventBitReadingTask(void *pvParameters)
{
    const EventBits_t xBitsToWaitFor = TASK0_BIT | TASK1_BIT | TASK2_BIT;

    while (1) {
        const EventBits_t xEventGroupValue = xEventGroupWaitBits(xEventGroup, xBitsToWaitFor, pdTRUE, pdFALSE, portMAX_DELAY);
        if (xEventGroupValue & TASK0_BIT) {
            printf("Task 0 has done its stuff\r\n");
        }

        if (xEventGroupValue & TASK1_BIT) {
            printf("Task 1 has done its stuff\r\n");
        }

        if (xEventGroupValue & TASK2_BIT) {
            printf("Task 2 has done its stuff\r\n");
        }

        vTaskDelay(xDelay500ms / 2);
    }
}

#endif /* INC_TASK_000_H_ */
