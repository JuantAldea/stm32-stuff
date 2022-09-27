#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "adc_driver.h"
#include "exti.h"
QueueHandle_t xPrintQueue;
TaskHandle_t xDigitalReadTask, xAnalogReadTask, xSerialGatekeeperTask;
void vTaskDigitalSensorTask(void *pvParameters);
void vTaskAnalogSensorTask(void *pvParameters);
void vTaskSerialGatekeeper(void *pvParameters);
//uint32_t sensor_value;
//uint8_t btn_state;
void task_file_main(void)
{
    printf("init finished\r\n");
    xPrintQueue = xQueueCreate(2, sizeof(uint32_t));


    xTaskCreate(vTaskDigitalSensorTask, "Button Read", 128, NULL, 1, &xDigitalReadTask);
    xTaskCreate(vTaskAnalogSensorTask, "Analog Read", 128, NULL, 1, &xAnalogReadTask);
    xTaskCreate(vTaskSerialGatekeeper, "Gatekeeper", 128, NULL, 0, &xSerialGatekeeperTask);
}

void vTaskDigitalSensorTask(void *pvParameters)
{
    gpio_init();

    while (1) {
        uint32_t btn_state = read_digital_sensor();
        xQueueSend(xPrintQueue, &btn_state, 0);
        vTaskDelay(1);
    }
}

void vTaskAnalogSensorTask(void *pvParameters)
{
    adc_init();

    while (1) {
        uint32_t sensor_value = read_analog_sensor();
        xQueueSend(xPrintQueue, &sensor_value, 0);
        vTaskDelay(10);
    }
}

void vTaskSerialGatekeeper(void *pvParameters)
{
    int buffer;
    while (1) {
        xQueueReceive(xPrintQueue, &buffer, portMAX_DELAY);
        printf("Value: %d\r\n", buffer);
    }
}

#endif /* INC_TASK_000_H_ */
