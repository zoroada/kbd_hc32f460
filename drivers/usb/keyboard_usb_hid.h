/* USB HID 类与底层发送接口声明，统一发送回调位于 drivers/keyboard_driver.c。 */
#pragma once
#include "report.h"
#include "usb_dev_core.h"
#include "usb_device_state.h"
/* 注册到 usb_dev_init() 的 HID 类回调表。 */
extern usb_dev_class_func keyboard_usb_class;
/* report 首字节为 Report ID；len 为包含 ID 的总长度，键盘接口实际发送 len-1 字节。 */
void usb_send_key(uint8_t report_id,uint8_t len,uint8_t *report);
/* RAW 发送接口，data 不含 Report ID，参考描述符报告长度为 32 字节。 */
void usb_send_raw(uint8_t *data,uint8_t len);
