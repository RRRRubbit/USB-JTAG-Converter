#ifndef LED_H
#define LED_H

int pico_led_init(void);

void pico_set_led(int led_color);
int led();

#endif // LED_H
