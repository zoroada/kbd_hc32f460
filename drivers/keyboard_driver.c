/* 键盘统一发送层：注册 TMK host_driver_t 回调，并将报告转交 USB 底层。 */
#include "keyboard_driver.h"
#include "keyboard_usb_hid.h"
#include "raw_hid.h"

void raw_hid_send(uint8_t *data, uint8_t length)
{
    usb_send_raw(data, length);
}
/** @brief 返回电脑通过 USB 输出报告设置的 Num Lock、Caps Lock 等键盘指示灯状态。 */
uint8_t keyboard_leds(void)
{
    return usb_device_state_get_leds();
}

/** @brief 统一报告发送入口；当前走 USB，后续传输方式选择可集中放在这里。 */
static void send_key_package(uint8_t report_id,uint8_t len,uint8_t *report)
{
    usb_send_key(report_id,len,report);
}

/** @brief TMK 标准键盘发送回调，将含 Report ID 的键盘结构体交给统一入口。 */
void send_keyboard(report_keyboard_t *report)
{
    send_key_package(report->report_id,sizeof(*report),(uint8_t *)report);
}

/** @brief TMK 鼠标发送回调，将含 Report ID 的鼠标结构体交给统一入口。 */
void send_mouse(report_mouse_t *report)
{
    send_key_package(report->report_id,sizeof(*report),(uint8_t *)report);
}

/** @brief TMK 系统控制/消费控制发送回调，报告类型由结构体的 report_id 指定。 */
void send_extra(report_extra_t *report)
{
    send_key_package(report->report_id,sizeof(*report),(uint8_t *)report);
}

/** @brief TMK NKRO 发送回调，发送修饰键和按键位图。 */
void send_nkro(report_nkro_t *report)
{
    send_key_package(report->report_id,sizeof(*report),(uint8_t *)report);
}

/* 在 main() 中用 host_set_driver(&driver) 注册，按键处理经此表调用发送函数。 */
host_driver_t driver={
    .keyboard_leds=keyboard_leds,
    .send_keyboard=send_keyboard,
    .send_nkro=send_nkro,
    .send_mouse=send_mouse,
    .send_extra=send_extra
};
