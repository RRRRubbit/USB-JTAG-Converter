#include <stdio.h>
#include "pico/stdlib.h"
#include "Tusb.h"
#include "usb_task.h"
#include "pico/binary_info.h"
#include "jtag_task.h"




int main() {
    stdio_init_all();
    tusb_init();
    jtag_init();

    while (true) {
        tud_task();  // 处理 USB 事件
        usb_task();  // 处理 USB 数据
    }
}

