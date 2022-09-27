#ifndef INC_TASK_000_H_
#define INC_TASK_000_H_

#include "cmsis_os.h"
#include "main.h"
#include "uart_driver.h"
#include "adc_driver.h"
#include "exti.h"
uint8_t btn_state;
uint32_t sensor_value;

void task_file_main(void)
{
    gpio_init();
    adc_init();
    while(1){
        btn_state = read_digital_sensor();
        sensor_value = read_analog_sensor();
    }
}


#endif /* INC_TASK_000_H_ */
