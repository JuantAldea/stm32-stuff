#ifndef STM32F407VET6_RTOS_BSP_H_
#define STM32F407VET6_RTOS_BSP_H_
#include <stdint.h>
#define LED1 GPIO_ODR_OD6
#define LED2 GPIO_ODR_OD7

enum probes {
    PROBE0 = 1,
    PROBE1 = 2,
    PROBE2 = 3,
    PROBE3 = 4,
    PROBE4 = 5,
};

void BSP_led_init();
void BSP_led1_on(void);
void BSP_led2_on(void);
void BSP_led1_off(void);
void BSP_led2_off(void);
void BSP_led1_toggle(void);
void BSP_led2_toggle(void);

void BSP_adc1_init(void);
uint32_t BSP_adc1_read();

void BSP_delay_ms(uint32_t ms);

void BSP_buttons_init();
uint32_t BPS_button_WK_UP_read();
uint32_t BPS_button_K0_read();
uint32_t BPS_button_K1_read();

void BPS_probes_init();
void BPS_probe_on(enum probes p);
void BPS_probe_off(enum probes p);
void BPS_probe_toggle(enum probes p);

#endif /* STM32F407VET6_RTOS_BSP_H_ */
