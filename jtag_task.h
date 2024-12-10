
#ifndef JTAG_TASK_H
#define JTAG_TASK_H
//TAP register 定义
#define IDCODE 0b00001
#define DTMCS  0b10000
#define DMI    0b10001
// JTAG 引脚定义
#define PIN_TDI 17 //A0 D0 26
#define PIN_TDO 27 //A1 D1 27
#define PIN_TMS 16 //A2 D2 28
#define PIN_TCK 25 //A3 D3 29
void jtag_init();
void jtag_write(uint8_t tms, uint8_t tdi, uint8_t tck);
uint8_t jtag_read();
void tap_state_transition(uint8_t tms);
void jtag_tap_move(uint8_t tms);
void move_to_shift_ir();
//void jtag_shift_data(uint8_t *data, size_t len);
void jtag_shift_data(uint64_t data, size_t len);
void send_riscv_step();
void show_current_state();
void read_riscv_reg(uint8_t address);
void send_riscv_step_by_func();
void send_riscv_run();
void send_riscv_set_breakpoint();
// 枚举TAP类型
typedef enum {
    TEST_LOGIC_RESET,
    RUN_TEST_IDLE,
    SELECT_DR_SCAN,
    CAPTURE_DR,
    SHIFT_DR,
    EXIT1_DR,
    PAUSE_DR,
    EXIT2_DR,
    UPDATE_DR,
    SELECT_IR_SCAN,
    CAPTURE_IR,
    SHIFT_IR,
    EXIT1_IR,
    PAUSE_IR,
    EXIT2_IR,
    UPDATE_IR
} tap_state_t;
//设置初始状态为复位状态
static tap_state_t current_state = TEST_LOGIC_RESET;
#endif // JTAG_TASK_H