#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"
#include "main.h"
#include "stdlib.h"
static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
static QueueSetHandle_t xQueueSet = NULL;

void vSenderTask1(void *pvParamenters);
void vSenderTask2(void *pvParamenters);
void vReceiverTask(void *pvParamenters);

void task_file_main(void)
{
    printf("INIT TASK\r\n");
    xQueue1 = xQueueCreate(1, sizeof(char*));
    xQueue2 = xQueueCreate(1, sizeof(char*));

    xQueueSet = xQueueCreateSet(2 * 1);

    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);

    xTaskCreate(vSenderTask1, "Sender 1", 1024, NULL, 1, NULL);
    xTaskCreate(vSenderTask2, "Sender 2", 1024, NULL, 1, NULL);
    xTaskCreate(vReceiverTask, "Receiver", 1024, NULL, 1, NULL);
}

void vSenderTask1(void *pvParamenters)
{
    const TickType_t xBlockTime = pdMS_TO_TICKS(100);
    char *msg = "Message from vSenderTask1\r\n";
    while (1) {
        vTaskDelay(xBlockTime);
        xQueueSend(xQueue1, &msg, 0);
    }
}

void vSenderTask2(void *pvParamenters)
{
    const TickType_t xBlockTime = pdMS_TO_TICKS(200);
    char *msg = "Message from vSenderTask2\r\n";
    while (1) {
        vTaskDelay(xBlockTime);
        xQueueSend(xQueue2, &msg, 0);
    }
}

void vReceiverTask(void *pvParamenters)
{
    QueueHandle_t xQueueThatContainsData;
    char *pcReceivedString;

    while (1) {
        xQueueThatContainsData = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
        xQueueReceive(xQueueThatContainsData, &pcReceivedString, 0);
        printf("RECERIVED: %s", pcReceivedString);
    }
}

#endif /* INC_TASKS_H_ */
