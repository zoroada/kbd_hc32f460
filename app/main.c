/* 固件入口：完成板级、键盘和 USB 初始化，然后启动 FreeRTOS 调度器。 */
#include "quantum.h"
#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_bsp.h"
#include "eeprom.h"
#include "keyboard_usb_hid.h"
#include "keyboard_driver.h"
#include "creat_task.h"
#include "FreeRTOS.h"
#include "task.h"
usb_core_instance usb_dev;
/** @brief 初始化硬件、键盘及 USB，注册统一发送驱动，然后交由 FreeRTOS 调度。 */
int main(void)
{
    stc_usb_port_identify port={0}; port.u8CoreID=USBFS_CORE_ID;
    bsp_init();
    usb_device_state_init();
    host_set_driver(&driver);
    keyboard_setup();
    keyboard_init();
    usb_dev_init(&usb_dev,&port,&user_desc,&keyboard_usb_class,&user_cb);
    Creat_app_task();
    vTaskStartScheduler();
    for(;;) {}
}
