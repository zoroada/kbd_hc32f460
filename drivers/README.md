# HC32F460 Quantum/TMK 固件

应用入口为 app/main.c，负责板级初始化、Quantum/TMK 初始化、USB 初始化和启动 FreeRTOS。drivers/creat_task.c 创建自动重载软件定时器：每 5 ms 调用 keyboard_task() 和 housekeeping_task()，每 10 ms 检测 PC13 Boot 按键。不定时重发 USB 报告。

## 目录

- app/main.c：实际固件入口。
- app/config：芯片库及 FreeRTOS 配置头文件。
- drivers/keyboard_config.h：矩阵大小和键盘配置。
- drivers/keyboard_keymap.c：键位映射。
- drivers/keyboard_driver.c/.h：统一发送入口、发送回调及全局 host_driver_t driver，app/main.c 通过 host_set_driver(&driver) 注册。
- drivers/keyboard_matrix.c/.h：矩阵读取和消抖。
- drivers/keyboard_storage.c：RAM EEPROM 后端。
- drivers/platform：基于 FreeRTOS tick 的计时、启动、平台兼容头文件和支持函数。
- drivers/usb：USB 板级代码、描述符、HID 收发、Boot 按键和 USB 配置。
- drivers/drivers.mk：Quantum/TMK 及平台源文件列表。
- keyboard/zero64/Makefile：构建入口，调用根目录 app.mk 中的应用构建规则。
- drivers 下其余原应用文件保留，仅明确列入 app.mk 和 drivers/drivers.mk 的源文件参与编译。

## USB

接口 0 为标准键盘（IN 0x81、OUT 0x02），接口 1 为 32 字节 RAW（IN 0x83、OUT 0x01），接口 2 为系统控制、消费控制、NKRO、鼠标和径向控制器组合 HID（IN 0x82）。

三组报告描述符位于 drivers/usb/usb_desc.h，采用参考文件的原始宏。报告 ID 取自 tmk_core/protocol/report.h。

## 发送接口

drivers/usb/keyboard_usb_hid.h 提供 usb_send_key(report_id, len, report)，report 必须包含首字节报告 ID，len 为包含 ID 的总长度。使用 REPORT_ID_KEYBOARD、REPORT_ID_MOUSE、REPORT_ID_SYSTEM、REPORT_ID_CONSUMER、REPORT_ID_NKRO 或 REPORT_ID_WHEEL，不使用示例注释中的旧数值。

host_driver_t 的键盘、鼠标、系统/消费控制及 NKRO 发送回调统一调用 send_key_package()，再转到 usb_send_key()。当前仅接入 USB；后续传输选择可在 send_key_package() 集中添加。标准键盘的内部结构包含 ID，USB 键盘接口发送时去除该 ID，保持参考描述符的 8 字节报告。鼠标使用内部 REPORT_ID_MOUSE，USB 发送时转换为参考描述符固定的 ID 1。径向控制器可通过 usb_send_key() 发送包含 ID 的 9 字节报告。RAW 继续由 raw_hid_send() 发送。

产品名称在 drivers/usb/usb_dev_desc.c 中配置为 HC32F460 Keyboard。USB 类和板级实现分别为 keyboard_usb_hid.c、keyboard_usb_board.c；未使用的旧自定义 HID 示例及旧 usb_bsp.c 已移除。usb_bsp.h 仍供 HC32 USB 底层使用，保留必要接口。

## 构建和烧录

在 keyboard/zero64 目录执行 make -j4，或在工程根目录执行 make -C keyboard/zero64 -j4。固件仍输出根目录的 hc32f460_pinmu.elf、hc32f460_pinmu.hex 和 hc32f460_pinmu.bin，中间文件仍在根目录 build。清理命令为 make -C keyboard/zero64 clean。HEX 携带应用地址，BIN 必须写入 0x10000。

不包含测试目录、模拟器测试入口和测试构建目标，固件直接从 app/main.c 启动。

## 当前硬件配置

系统时钟 200 MHz，USB 时钟 48 MHz。PC13 保留进入 Boot 的功能，启动标志约定由 drivers/usb/usb_boot.c 和 drivers/platform/keyboard_boot.c 实现。

默认矩阵仍为 1 行 1 列、KC_NO；实际接线需在 drivers/keyboard_matrix.c 的 keyboard_matrix_read() 实现，并设置键盘配置和键位。EEPROM 为 1024 字节 RAM 后端，复位后配置丢失。SysTick 由 FreeRTOS 接管，Quantum 计时读取 RTOS tick。USB RAW 接收在 USB 中断回调中直接处理。
