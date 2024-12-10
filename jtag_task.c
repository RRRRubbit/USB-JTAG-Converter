#include <stdio.h>
#include "pico/stdlib.h"
#include "jtag_task.h"
#include "Tusb.h"
// 初始化 JTAG GPIO
void jtag_init() {
    gpio_init(PIN_TDI);
    gpio_init(PIN_TMS);
    gpio_init(PIN_TCK);
    gpio_set_dir(PIN_TDI, GPIO_OUT);
    gpio_set_dir(PIN_TMS, GPIO_OUT);
    gpio_set_dir(PIN_TCK, GPIO_OUT);
    gpio_init(PIN_TDO);
    gpio_set_dir(PIN_TDO, GPIO_IN);
}
// 设置 JTAG 信号
void jtag_write(uint8_t tms, uint8_t tdi, uint8_t tck) {
    gpio_put(PIN_TMS, tms);
    gpio_put(PIN_TDI, tdi);
    gpio_put(PIN_TCK, tck);
}

// 读取 TDO 信号
uint8_t jtag_read() {
    return gpio_get(PIN_TDO);
}
//设置TAP状态机TMS逻辑
void tap_state_transition(uint8_t tms) {
    switch (current_state) {
        case TEST_LOGIC_RESET:
            current_state = tms ? TEST_LOGIC_RESET : RUN_TEST_IDLE;
            break;
        case RUN_TEST_IDLE:
            current_state = tms ? SELECT_DR_SCAN : RUN_TEST_IDLE;
            break;
        case SELECT_DR_SCAN:
            current_state = tms ? SELECT_IR_SCAN : CAPTURE_DR;
            break;
        case CAPTURE_DR:
            current_state = tms ? EXIT1_DR : SHIFT_DR;
            break;
        case SHIFT_DR:
            current_state = tms ? EXIT1_DR : SHIFT_DR;
            break;
        case EXIT1_DR:
            current_state = tms ? UPDATE_DR : PAUSE_DR;
            break;
        case PAUSE_DR:
            current_state = tms ? EXIT2_DR : PAUSE_DR;
            break;
        case EXIT2_DR:
            current_state = tms ? UPDATE_DR : SHIFT_DR;
            break;
        case UPDATE_DR:
            current_state = tms ? SELECT_DR_SCAN : RUN_TEST_IDLE;
            break;
        case SELECT_IR_SCAN:
            current_state = tms ? TEST_LOGIC_RESET : CAPTURE_IR;
            break;
        case CAPTURE_IR:
            current_state = tms ? EXIT1_IR : SHIFT_IR;
            break;
        case SHIFT_IR:
            current_state = tms ? EXIT1_IR : SHIFT_IR;
            break;
        case EXIT1_IR:
            current_state = tms ? UPDATE_IR : PAUSE_IR;
            break;
        case PAUSE_IR:
            current_state = tms ? EXIT2_IR : PAUSE_IR;
            break;
        case EXIT2_IR:
            current_state = tms ? UPDATE_IR : SHIFT_IR;
            break;
        case UPDATE_IR:
            current_state = tms ? SELECT_DR_SCAN : RUN_TEST_IDLE;
            break;
    }
}
//设置单次TMS逻辑，同步TAP状态
void jtag_tap_move(uint8_t tms) {
    jtag_write(tms, 0, 1);  // 设置 TMS，触发 TCK 上升沿
    sleep_ms(500);
    jtag_write(tms, 0, 0);
    tap_state_transition(tms);
}

void move_to_shift_ir() {
    while (current_state != SHIFT_IR) {
        if (current_state == TEST_LOGIC_RESET) {
            jtag_tap_move(0);  // 从 RESET 到 IDLE
        } else if (current_state == RUN_TEST_IDLE) {
            jtag_tap_move(1);  // 从 IDLE 到 SELECT-DR
            jtag_tap_move(1);  // SELECT-DR 到 SELECT-IR
        } else if (current_state == SELECT_IR_SCAN) {
            jtag_tap_move(0);  // SELECT-IR 到 CAPTURE-IR
            jtag_tap_move(0);  // CAPTURE-IR 到 SHIFT-IR
        }
        if (current_state == SHIFT_IR){
            led(1);
        }
    }
}
void move_to_shift_dr() {
    while (current_state != SHIFT_DR) {
        if (current_state == TEST_LOGIC_RESET) {
            jtag_tap_move(0);  // 从 RESET 到 IDLE
        } else if (current_state == RUN_TEST_IDLE) {
            jtag_tap_move(1);  // 从 IDLE 到 SELECT-DR
        } else if (current_state == SELECT_DR_SCAN) {
            jtag_tap_move(0);  // SELECT-DR 到 CAPTURE-DR
            jtag_tap_move(0);  // CAPTURE-DR 到 SHIFT-DR
        }
        if (current_state == SHIFT_IR){
            led(1);
        }
    }
}

void jtag_shift_data(uint64_t data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        // 提取当前位：右移 i 位并取最低位
        uint64_t bit = (data >> i) & 1;
        
        // 调用 jtag_write 函数，写入每一位
        jtag_write(0, bit, 1);  // 假设 0 是寄存器地址，bit 是要写的位，1 是写入的位数
        sleep_ms(500);
        jtag_write(1, bit, 0);  // 假设 0 是寄存器地址，bit 是要写的位，1 是写入的位数
    }
}

void send_riscv_step() {
    // 进入 SHIFT-IR 状态
    while (current_state != SHIFT_IR) {
        jtag_tap_move(current_state == TEST_LOGIC_RESET ? 0 : 1);
    }

    // 发送指令寄存器数据 (例如步进命令的指令寄存器值为 0x10)
    uint8_t ir_command = 0x10;
    for (int i = 0; i < 5; i++) {  // 假设 IR 长度为 8 位
        jtag_write(0, ir_command & 1, 1);  // 发送 TDI
        ir_command >>= 1;
    }
    jtag_tap_move(1);  // 从 SHIFT-IR 进入 UPDATE-IR

    // 进入 SHIFT-DR 状态
    while (current_state != SHIFT_DR) {
        jtag_tap_move(current_state == RUN_TEST_IDLE ? 1 : 0);
    }

    // 发送步进数据 (假设 DR 数据为 0x01)
    uint8_t dr_command = 0x01;
    for (int i = 0; i < 8; i++) {  // 假设 DR 长度为 8 位
        jtag_write(0, dr_command & 1, 1);  // 发送 TDI
        dr_command >>= 1;
    }
    jtag_tap_move(1);  // 从 SHIFT-DR 进入 UPDATE-DR
}

void show_current_state(){
}
void read_riscv_reg(uint8_t address){
    move_to_shift_ir();
    //jtag_shift_data(0b101010101010101010101,21);
    //led(1);
    //jtag_shift_data(IDCODE,5);
    //move_to_shift_dr();
    /*uint64_t command = 0;
    uint64_t value =0x15552aaa;
    uint64_t addr = 0xAA;
    command |= (uint64_t)value;            // 数据部分 (32 bits)
    command |= ((uint64_t)addr << 32); // 地址部分 (7 bits)
    command |= ((uint64_t)0b10 << 39);    // 操作码: Write
    jtag_shift_data(command,41);  // 通过 JTAG 接口发送命令
    led(3);*/
}
void send_riscv_step_by_func(){}
void send_riscv_run(){}
void send_riscv_set_breakpoint(){}