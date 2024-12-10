#include <stdio.h>
#include "pico/stdlib.h"
#include "Tusb.h"

#include "pico/binary_info.h"
#include "led.h"
// LED 引脚定义
#define LED_PIN_RGB_POWER 11
#define LED_PIN_RGB_DATA 12
#define LED_PIN_GREEN 16
#define LED_PIN_RED 17
#define LED_PIN_BLUE 25
#define LED_TYPE 0
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 500
#endif
int pico_led_init(void) {
    if (LED_TYPE == 0) {
        // A device like Pico that uses a GPIO for the LED so we can
        // use normal GPIO functionality to turn the led on and off
        gpio_init(LED_PIN_GREEN);
        gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
        gpio_init(LED_PIN_RED);
        gpio_set_dir(LED_PIN_RED, GPIO_OUT);
        gpio_init(LED_PIN_BLUE);
        gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
        gpio_init(LED_PIN_RGB_POWER);
        gpio_set_dir(LED_PIN_RGB_POWER, GPIO_OUT);
        return PICO_OK;
    } else {
        return PICO_ERROR_INVALID_DATA;
    }
}
void pico_set_led(int led_color) {
    switch (led_color)
    {
    case 0: //关闭灯
        gpio_put(LED_PIN_BLUE,0);
        gpio_put(LED_PIN_RED,0);
        gpio_put(LED_PIN_GREEN,0);
        break;
    case LED_PIN_GREEN:
        gpio_put(LED_PIN_BLUE,0);
        gpio_put(LED_PIN_RED,0);
        gpio_put(LED_PIN_GREEN,1);
        break;
    case LED_PIN_RED:
        gpio_put(LED_PIN_BLUE,0);
        gpio_put(LED_PIN_RED,1);
        gpio_put(LED_PIN_GREEN,0);
        break;
    case LED_PIN_BLUE:
        gpio_put(LED_PIN_BLUE,1);
        gpio_put(LED_PIN_RED,0);
        gpio_put(LED_PIN_GREEN,0);
        break;
    default:
        break;
    }
}
int led(int led_mode) {
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    switch (led_mode)
    {
    case 0: //蓝灯持续闪烁
            while (true) {
        pico_set_led(LED_PIN_BLUE);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
        break;
    case 1: //蓝灯闪烁两次
            for (size_t i = 0; i < 2; i++)
            {
                pico_set_led(LED_PIN_BLUE);
                sleep_ms(LED_DELAY_MS);
                pico_set_led(false);
                sleep_ms(LED_DELAY_MS);
            }
        break;
    case 2: //红灯闪烁两次
            for (size_t i = 0; i < 2; i++)
            {
                pico_set_led(LED_PIN_RED);
                sleep_ms(LED_DELAY_MS);
                pico_set_led(false);
                sleep_ms(LED_DELAY_MS);
            }
        break;
    case 3: //绿灯闪烁两次
            for (size_t i = 0; i < 2; i++)
            {
                pico_set_led(LED_PIN_GREEN);
                sleep_ms(LED_DELAY_MS);
                pico_set_led(false);
                sleep_ms(LED_DELAY_MS);
            }
        break;
    default:
        break;
    }

}