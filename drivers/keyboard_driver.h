/* 统一键盘驱动声明：LED 状态、各类报告发送回调和全局 driver。 */
#pragma once

#include "report.h"
#include "host_driver.h"

extern host_driver_t driver;

uint8_t keyboard_leds(void);
void send_keyboard(report_keyboard_t *report);
void send_mouse(report_mouse_t *report);
void send_extra(report_extra_t *report);
void send_nkro(report_nkro_t *report);