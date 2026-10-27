# USB HID 固件

实际应用入口为 app/main.c，由 keyboard/zero64/Makefile 调用根目录 app.mk 构建。

## 当前构建

- keyboard_usb_board.c：USB 时钟、引脚和中断。
- keyboard_usb_hid.c：三个 HID 接口、控制请求和报告收发。
- usb_desc.h：参考文件的键盘、RAW 和组合 HID 报告描述符。
- usb_app_conf.h：三个接口、端点包长和 FIFO 分配。
- usb_dev_desc.c/.h：设备及字符串描述符。
- usb_dev_user.c/.h：设备事件回调。
- usb_boot.c/.h：PC13 Boot 按键检测。

未使用的旧 usb_bsp.c、usb_dev_custom_hid_class.c/.h 已删除。usb_bsp.h 仍被 HC32 USB 库使用，声明由 keyboard_usb_board.c 实现的接口。

## 接口

- 接口 0：标准键盘，IN 0x81，OUT 0x02。
- 接口 1：32 字节 RAW，IN 0x83，OUT 0x01。
- 接口 2：系统控制、消费控制、NKRO、鼠标和径向控制器组合 HID，IN 0x82。

产品名称为 HC32F460 Keyboard，VID/PID 沿用 0x2E88/0x4606。报告 ID 使用 tmk_core/protocol/report.h；参考中的鼠标报告固定为 ID 1。

发送入口为 usb_send_key(report_id, len, report)，报告缓冲区和长度均包含首字节 ID。键盘、鼠标、系统/消费控制及 NKRO 的 host_driver_t 回调统一经过 send_key_package()。USB 发送时去除标准键盘的内部 ID，并将鼠标内部 ID 转为参考描述符的 ID 1。USB 数据仅在调用发送接口时发送，不进行 Idle 定时重发。RAW 接收继续在 USB 中断回调中直接处理。

## 更新

根目录执行 make -C keyboard/zero64 -j4，固件输出仍在根目录。使用生成的 HEX 更新应用，或将 BIN 写入 0x10000，随后重新连接 USB。

PC13 按键和 QK_BOOT 沿用现有启动标志约定：擦除 0xE000 所在扇区，使 0xFFFC 恢复为 0xFFFFFFFF，确认成功后软件复位。
