
#ifndef USB_TASK_H
#define USB_TASK_H

void usb_task();
void process_usb_command(uint8_t *buffer, uint32_t length);

#endif // USB_TASK_H