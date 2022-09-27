#ifndef LED_H_
#define LED_H_

enum board_leds {
    LED_0 = 6, LED_1 = 7,
};

void leds_init(void);
void led_on(enum board_leds led);
void led_off(enum board_leds led);
void led_toggle(enum board_leds led);

#endif /* LED_H_ */
