#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "adc_driver.h"
#include "exti.h"

SemaphoreHandle_t xSerialSemaphore;
TaskHandle_t xDigitalReadTask, xAnalogReadTask;
void vTaskDigitalSensorTask(void *pvParameters);
void vTaskAnalogSensorTask(void *pvParameters);

void task_file_main(void)
{
    printf("init finished\r\n");
    xSerialSemaphore = xSemaphoreCreateMutex();
    xTaskCreate(vTaskDigitalSensorTask, "Button Read", 128, NULL, 2, &xDigitalReadTask);
    xTaskCreate(vTaskAnalogSensorTask, "Analog Read", 128, NULL, 2, &xAnalogReadTask);
}

void vTaskDigitalSensorTask(void *pvParameters)
{
    gpio_init();

    while (1) {
        uint8_t btn_state = read_digital_sensor();
        if (pdTRUE == xSemaphoreTake(xSerialSemaphore, (TickType_t ) 5)) {
            printf("Button state: %d\r\n", btn_state);
            xSemaphoreGive(xSerialSemaphore);
        }
        vTaskDelay(1);
    }
}

void vTaskAnalogSensorTask(void *pvParameters)
{
    adc_init();

    while (1) {
        uint32_t sensor_value = read_analog_sensor();
        if (pdTRUE == xSemaphoreTake(xSerialSemaphore, (TickType_t ) 5)) {
            printf("Analog value: %ld\r\n", sensor_value);
            xSemaphoreGive(xSerialSemaphore);
        }
        vTaskDelay(1);
    }
}

#endif /* INC_TASK_000_H_ */
