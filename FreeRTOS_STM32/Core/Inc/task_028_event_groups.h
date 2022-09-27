#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "event_groups.h"

#define TASK1_BIT (1UL << 0UL)
#define TASK2_BIT (1UL << 1UL)

EventGroupHandle_t xEventGroup;
void vEventBitReadingTask(void *pvParameters);
void vEventBitSettingTask(void *pvParameters);

const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

void task_file_main(void)
{
    printf("init finished\r\n");

    xEventGroup = xEventGroupCreate();
    xTaskCreate(vEventBitSettingTask, "Bit Setter", 100, NULL, 1, NULL);
    xTaskCreate(vEventBitReadingTask, "Bit Reader", 100, NULL, 1, NULL);
}

void vEventBitSettingTask(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        printf("Setting bit 0\r\n");
        xEventGroupSetBits(xEventGroup, TASK1_BIT);

        vTaskDelay(xDelay500ms);
        printf("Setting bit 1\r\n");
        xEventGroupSetBits(xEventGroup, TASK2_BIT);
    }
}

void vEventBitSettingTask(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        printf("Setting bit 0\r\n");
        xEventGroupSetBits(xEventGroup, TASK1_BIT);

        vTaskDelay(xDelay500ms);
        printf("Setting bit 1\r\n");
        xEventGroupSetBits(xEventGroup, TASK2_BIT);
    }
}

void vEventBitReadingTask(void *pvParameters)
{
    const EventBits_t xBitsToWaitFor = TASK1_BIT | TASK2_BIT;

    while (1) {
        const EventBits_t xEventGroupValue = xEventGroupWaitBits(xEventGroup, xBitsToWaitFor, pdTRUE, pdFALSE, portMAX_DELAY);
        if (xEventGroupValue & TASK1_BIT) {
            printf("Read bit 0\r\n");
        }

        if (xEventGroupValue & TASK2_BIT) {
            printf("Read bit 1\r\n");
        }

        vTaskDelay(xDelay500ms / 2);
    }
}

#endif /* INC_TASK_000_H_ */
