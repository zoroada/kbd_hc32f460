/* USB 全速设备配置：接口数量、端点包长及硬件 FIFO 分配。 */
#ifndef __USB_APP_CONF_H__
#define __USB_APP_CONF_H__
#define USB_FS_MODE
#define USE_DEVICE_MODE
/* RAW HID 的 IN/OUT 端点及 32 字节报告长度。 */
#define HID_IN_EP 0x83U
#define HID_OUT_EP 0x01U
#define HID_IN_PACKET 32U
#define HID_OUT_PACKET 32U
/* FIFO 大小以 32 位字为单位：接收 128，EP0 发送 64，EP1/2/3 各发送 32。 */
#define RX_FIFO_FS_SIZE 128U
#define TX0_FIFO_FS_SIZE 64U
#define TX1_FIFO_FS_SIZE 32U
#define TX2_FIFO_FS_SIZE 32U
#define TX3_FIFO_FS_SIZE 32U
#define TX4_FIFO_FS_SIZE 0U
#define TX5_FIFO_FS_SIZE 0U
/* 一个配置、三个接口，字符串描述符缓冲区最大 128 字节。 */
#define DEV_MAX_CFG_NUM 1U
#define USBD_ITF_MAX_NUM 3U
#define USB_MAX_STR_DESC_SIZ 128U
/* 通过 VBUS 感知 USB 连接供电状态。 */
#define VBUS_SENSING_ENABLED
#if RX_FIFO_FS_SIZE+TX0_FIFO_FS_SIZE+TX1_FIFO_FS_SIZE+TX2_FIFO_FS_SIZE+TX3_FIFO_FS_SIZE > 320
#error USB FIFO allocation exceeds hardware capacity
#endif
#endif
