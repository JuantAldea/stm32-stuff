#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "event_groups.h"

#define TASK0_BIT (1UL << 0UL)
#define TASK1_BIT (1UL << 1UL)
#define TASK2_BIT (1UL << 2UL)

const EventBits_t uxAllSyncBits = TASK0_BIT | TASK1_BIT | TASK2_BIT;

EventGroupHandle_t xEventGroup;

void vTask0(void *pvParameters);
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

void task_file_main(void)
{
    printf("init finished\r\n");

    xEventGroup = xEventGroupCreate();

    xTaskCreate(vTask0, "Bit Setter0", 100, NULL, 1, NULL);
    xTaskCreate(vTask1, "Bit Setter1", 100, NULL, 1, NULL);
    xTaskCreate(vTask2, "Bit Setter2", 100, NULL, 1, NULL);
}

void vTask0(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        EventBits_t uxReturn = xEventGroupSync(xEventGroup, TASK0_BIT, uxAllSyncBits, portMAX_DELAY);
        if ((uxReturn & uxAllSyncBits) == uxAllSyncBits) {
            printf("Tasks synced\r\n");
        }
    }
}

void vTask1(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        EventBits_t uxReturn = xEventGroupSync(xEventGroup, TASK1_BIT, uxAllSyncBits, portMAX_DELAY);
        if ((uxReturn & uxAllSyncBits) == uxAllSyncBits) {
            //
        }
    }
}

void vTask2(void *pvParameters)
{
    while (1) {
        vTaskDelay(xDelay500ms);
        EventBits_t uxReturn = xEventGroupSync(xEventGroup, TASK2_BIT, uxAllSyncBits, portMAX_DELAY);
        if ((uxReturn & uxAllSyncBits) == uxAllSyncBits) {
            //
        }
    }
}

void vEventBitReadingTask(void *pvParameters)
{

}

#endif /* INC_TASK_000_H_ */
