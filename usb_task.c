#include <stdio.h>
#include "pico/stdlib.h"
#include "Tusb.h"
#include "pico/binary_info.h"
#include "usb_task.h"
#include "jtag_task.h"
#include "led.h" 
// USB 数据处理
void usb_task() {
    if (tud_cdc_available()) {
        uint8_t buffer[64];
        uint32_t count = tud_cdc_read(buffer, sizeof(buffer));
        //read_riscv_reg(0b10000001);
        if (count > 0) {
            process_usb_command(buffer, count);  // 交给 process_usb_command 解析和处理
        }
        else if (count < 0){
            tud_cdc_write_str("Error: Insufficient data\n");
            tud_cdc_write_flush();
        }
    }
}

void process_usb_command(uint8_t *buffer, uint32_t length) {
    uint8_t command = buffer[0];  // 命令字节
    uint8_t command_2 = buffer[1]; // 数据长度
    uint8_t END_CR = 0x0D; //回车符
    uint8_t data=0;
    for (size_t i = 0; i < length; i++)
    {
        if (buffer[i] == END_CR) {
                break;
        }
        else{
        uint8_t data = data + buffer[i];  // 命令内容
        }

    }
    printf("%s\n",data);
    switch (command) {
        case 0x42:  // B Breakpoint
            switch (command_2)
            {
            case 0x53:
                //BS function Breakpoint_set
                read_riscv_reg(0b10000001);
                break;
            case 0x4C:
                //BL function Breakpoint_list
                        led(1);
                break;
            case 0x4B:
                //BK function Breakpoint_clean
                break;
            case 0x45:
                //BE function Breakpoint_enable
                break;
            case 0x44:
                //BD function Breakpoint_disable
                break;
            default:
                break;
            }
        //tud_cdc_write(buffer[0:1],2);
        tud_cdc_write_flush();
            break;
        case 0x44:  // D Inter Data
        tud_cdc_write_str("D\n");
        tud_cdc_write_flush();
            break;
        case 0x45:  // E Extra Data
        tud_cdc_write_str("E\n");
        tud_cdc_write_flush();
            break;
        case 0x47:  // G Execute 
        tud_cdc_write_str("G\n");
        tud_cdc_write_flush();
            break;
        case 0x54:  // T
        tud_cdc_write_str("T\n");
        tud_cdc_write_flush();
            break;
        case 0x50:  // P
        tud_cdc_write_str("P\n");
        tud_cdc_write_flush();
            break;
        default:
            // 未知命令
            tud_cdc_write_str("Unknown command\n");
            break;
    }
}
