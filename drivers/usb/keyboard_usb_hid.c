/* 三接口 USB HID 类：处理枚举、控制请求、接收回调和底层报告发送。 */
#include "keyboard_usb_hid.h"
#include "usb_desc.h"
#include "usb_dev_driver.h"
#include "usb_dev_ctrleptrans.h"
#include "usb_dev_stdreq.h"
#include "raw_hid.h"
#include <string.h>
extern usb_core_instance usb_dev;
/* 端点地址：最高位 1 表示设备发往电脑（IN），0 表示电脑发往设备（OUT）。 */
#define KEY_IN 0x81U
#define KEY_OUT 0x02U
#define RAW_IN 0x83U
#define RAW_OUT 0x01U
#define SHARED_IN 0x82U
#define SHARED_EPSIZE 32U
/* 三组报告格式直接引用参考宏，描述符决定主机如何解释报告字节。 */
static uint8_t keyboard_descriptor[]=APP_USBD_HID_KBD_REPORT_DSC();
static uint8_t raw_descriptor[]=APP_USBD_VIAL_REPORT_DSC();
static uint8_t shared_descriptor[]=APP_USBD_HID_NRKO_REPORT_DSC();
/* 配置总长 98 字节：接口 0 标准键盘、接口 1 RAW、接口 2 组合 HID。 */
static uint8_t configuration[]={
                    9,2,98,0,3,1,0,0x80,50,
                    9,4,0,0,2,3,1,1,0,
                    9,0x21,0x11,0x01,0,1,0x22,sizeof(keyboard_descriptor),0,
                    7,5,KEY_IN,3,8,0,1, 7,5,KEY_OUT,3,1,0,1,
                    9,4,1,0,2,3,0,0,0,
                    9,0x21,0x11,0x01,0,1,0x22,sizeof(raw_descriptor),0,
                    7,5,RAW_IN,3,RAW_EPSIZE,0,1, 7,5,RAW_OUT,3,RAW_EPSIZE,0,1,
                    9,4,2,0,1,3,0,0,0,
                    9,0x21,0x11,0x01,0,1,0x22,sizeof(shared_descriptor)&0xFF,sizeof(shared_descriptor)>>8,
                    7,5,SHARED_IN,3,SHARED_EPSIZE,0,1};
/* OUT/EP0 接收缓冲区按 4 字节对齐，数据在 USB 完成回调内直接处理。 */
static uint8_t key_rx[4] __attribute__((aligned(4)));
static uint8_t raw_rx[RAW_EPSIZE] __attribute__((aligned(4)));
static uint8_t control_rx[RAW_EPSIZE] __attribute__((aligned(4)));
static uint8_t control_interface,control_pending,control_length;
/* 每接口保存协议、主机设置的 Idle 值和备用设置；Idle 值不触发定时重发。 */
static uint8_t protocol[3]={1,1,1},idle_rate[3],alt[3],led_report;

/** @brief 主机配置设备时调用：初始化 HID 状态、打开五个端点并准备 OUT 接收。 */
static void class_init(void *pdev)
{
    control_pending=0;
    memset(protocol,1,sizeof(protocol));
    memset(idle_rate,0,sizeof(idle_rate));
    memset(alt,0,sizeof(alt));
    usb_device_state_set_protocol(USB_PROTOCOL_REPORT);
    usb_device_state_set_idle_rate(0);
    usb_device_state_set_leds(0);
    usb_opendevep(pdev,KEY_IN,8,EP_TYPE_INTR);
    usb_opendevep(pdev,KEY_OUT,1,EP_TYPE_INTR);
    usb_opendevep(pdev,RAW_IN,RAW_EPSIZE,EP_TYPE_INTR);
    usb_opendevep(pdev,RAW_OUT,RAW_EPSIZE,EP_TYPE_INTR);
    usb_opendevep(pdev,SHARED_IN,SHARED_EPSIZE,EP_TYPE_INTR);
    usb_readytorx(pdev,KEY_OUT,key_rx,1);
    usb_readytorx(pdev,RAW_OUT,raw_rx,RAW_EPSIZE);
    usb_device_state_set_configuration(true,1);
}
/** @brief 设备取消配置时调用：关闭各端点，清除控制接收标志及配置状态。 */
static void class_deinit(void *pdev)
{
    usb_shutdevep(pdev,KEY_IN); usb_shutdevep(pdev,KEY_OUT);
    usb_shutdevep(pdev,RAW_IN); usb_shutdevep(pdev,RAW_OUT);
    usb_shutdevep(pdev,SHARED_IN);
    control_pending=0;
    usb_device_state_set_reset();
}
/** @brief 处理主机 SETUP 请求：HID 报告/协议/Idle 设置、描述符读取和接口选择。 */
static uint8_t class_setup(void *pdev,USB_SETUP_REQ *req)
{
    uint8_t interface=req->wIndex;
    if(req->wIndex>2) goto invalid;
    if((req->bmRequest & USB_REQ_TYPE_MASK)==USB_REQ_TYPE_CLASS) {
        switch(req->bRequest) {
        case 0x01: /* GET_REPORT */
            if(interface==0 && (req->wValue>>8)==2) {
                led_report=usb_device_state_get_leds();
                usb_ctrldatatx(pdev,&led_report,LL_MIN(req->wLength,1));
            } else usb_ctrldatatx(pdev,control_rx,0);
            break;
        case 0x09: /* SET_REPORT */
            if(interface==2 || req->wLength==0 || req->wLength>(interface?RAW_EPSIZE:1)) goto invalid;
            control_interface=interface; control_pending=1; control_length=req->wLength;
            usb_ctrldatarx(pdev,control_rx,req->wLength); break;
        case 0x0B: /* SET_PROTOCOL：选择 Boot 或 Report 协议。 */
            if(req->wValue>1)goto invalid;
            protocol[interface]=req->wValue;
            if(interface==0)usb_device_state_set_protocol(protocol[0]);
            break;
        case 0x03: /* GET_PROTOCOL：返回当前协议。 */
            usb_ctrldatatx(pdev,&protocol[interface],LL_MIN(req->wLength,1)); break;
        case 0x0A: /* SET_IDLE：只记录主机要求，不添加周期发送。 */
            idle_rate[interface]=req->wValue>>8;
            if(interface==0)usb_device_state_set_idle_rate(idle_rate[0]);
            break;
        case 0x02: /* GET_IDLE：返回保存的 Idle 值。 */
            usb_ctrldatatx(pdev,&idle_rate[interface],LL_MIN(req->wLength,1)); break;
        default: goto invalid;
        }
    } else if((req->bmRequest & USB_REQ_TYPE_MASK)==USB_REQ_TYPE_STANDARD) {
        switch(req->bRequest) {
        case USB_REQ_GET_DESCRIPTOR:
            if((req->wValue>>8)==0x22) {
                uint8_t *d=interface==0?keyboard_descriptor:(interface==1?raw_descriptor:shared_descriptor);
                uint16_t n=interface==0?sizeof(keyboard_descriptor):(interface==1?sizeof(raw_descriptor):sizeof(shared_descriptor));
                usb_ctrldatatx(pdev,d,LL_MIN(req->wLength,n));
            } else if((req->wValue>>8)==0x21) {
                usb_ctrldatatx(pdev,configuration+(interface==0?18:(interface==1?50:82)),LL_MIN(req->wLength,9));
            } else goto invalid;
            break;
        case USB_REQ_GET_INTERFACE: usb_ctrldatatx(pdev,&alt[interface],LL_MIN(req->wLength,1)); break;
        case USB_REQ_SET_INTERFACE: if(req->wValue)goto invalid; alt[interface]=0; break;
        default: goto invalid;
        }
    } else goto invalid;
    return USB_DEV_OK;
invalid:
    usb_ctrlerr(pdev); return USB_DEV_FAIL;
}
/** @brief 返回包含三个 HID 接口的配置描述符，并填写实际字节长度。 */
static uint8_t *class_descriptor(uint16_t *len) { *len=sizeof(configuration); return configuration; }
/** @brief IN 发送完成回调。 */
static void class_datain(void *pdev,uint8_t ep)
{
    (void)pdev;
    (void)ep;
}
/** @brief OUT 接收完成回调：更新键盘 LED，或直接调用 raw_hid_receive() 后重新准备接收。 */
static void class_dataout(void *pdev,uint8_t ep)
{
    if(ep==(KEY_OUT & 15)) {
        if(usb_getrxcnt(pdev,ep))usb_device_state_set_leds(key_rx[0]);
        usb_readytorx(pdev,KEY_OUT,key_rx,1);
    } else if(ep==(RAW_OUT & 15)) {
        uint8_t length=LL_MIN(usb_getrxcnt(pdev,ep),RAW_EPSIZE);
        raw_hid_receive(raw_rx,length);
        usb_readytorx(pdev,RAW_OUT,raw_rx,RAW_EPSIZE);
    }
}
/** @brief EP0 的 SET_REPORT 数据收齐后调用：直接处理 LED 或 RAW 数据。 */
static void class_control_ready(void *pdev)
{
    (void)pdev;
    if(!control_pending)return;
    control_pending=0;
    if(control_interface==0)usb_device_state_set_leds(control_rx[0]);
    else if(control_interface==1)raw_hid_receive(control_rx,control_length);
}
/* HC32 USB 核心通过此表调用初始化、控制请求及端点收发回调。 */
usb_dev_class_func keyboard_usb_class={class_init,class_deinit,class_setup,NULL,class_control_ready,class_descriptor,NULL,class_datain,class_dataout,NULL,NULL};
/** @brief 判断 USB 核心当前是否已完成主机配置，可用于发送报告。 */
static bool configured(void) { return usb_dev.dev.device_cur_status==USB_DEV_CONFIGURED; }
/** @brief 发送 RAW 报告：data 为不带 Report ID 的数据，len 为字节数；直接提交 RAW IN 端点。 */
void usb_send_raw(uint8_t *data,uint8_t len)
{
    if(configured())usb_deveptx(&usb_dev,RAW_IN,data,len);
}

/** @brief 按 report_id 选择键盘或组合 IN 端点；len 包含首字节 ID，键盘发送时去掉该 ID。 */
void usb_send_key(uint8_t report_id,uint8_t len,uint8_t *report)
{
    if(!configured())return;
    switch(report_id) {
    case REPORT_ID_KEYBOARD:
        usb_deveptx(&usb_dev,KEY_IN,&report[1],len-1);
        break;
    case REPORT_ID_MOUSE: {
        /* 参考描述符的鼠标 ID 固定为 1，内部 REPORT_ID_MOUSE 为 2；仅适配此 ID。 */
        static report_mouse_t mouse_report;
        mouse_report=*(report_mouse_t *)report;
        mouse_report.report_id=1;
        usb_deveptx(&usb_dev,SHARED_IN,(uint8_t *)&mouse_report,len);
        break;
    }
    case REPORT_ID_SYSTEM:
    case REPORT_ID_CONSUMER:
    case REPORT_ID_NKRO:
    case REPORT_ID_WHEEL:
        usb_deveptx(&usb_dev,SHARED_IN,report,len);
        break;
    default:
        return;
    }
}
