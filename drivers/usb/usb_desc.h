/* 参考文件的三组 HID 报告描述符：RAW、标准键盘和组合报告；保留原始描述符字节。 */
#pragma once
#include "report.h"

#define HID_VALUE_16(v) ((uint8_t)(v & 0xFF)), ((uint8_t)(v >> 8))

/////////////////////
// RAW Usage page and ID configuration

#ifndef RAW_USAGE_PAGE
#define RAW_USAGE_PAGE  0xFF60
#endif

#ifndef RAW_USAGE_ID
#define RAW_USAGE_ID    0x61
#endif

#define RAW_EPSIZE      0x20


/* RAW 报告：厂商页 0xFF60、Usage 0x61，输入和输出均为 32 字节。 */
#define APP_USBD_VIAL_REPORT_DSC()                          \
    {                                                          \
        0x06, HID_VALUE_16(RAW_USAGE_PAGE),     /* Usage Page (Vendor Defined)*/ \
        0x09, RAW_USAGE_ID,                      /* Usage Page (Vendor Defined)*/ \
        0xA1, 0x01,                             /* COLLECTION (Application)*/    \
        0x75, 0x08,         \
        0x15, 0x00,         \
        0x25, 0xFF,         \
        0x09, 0x62,         \
        0x95, RAW_EPSIZE,   \
        0x81, 0x02,         \
        0x09, 0x63,         \
        0x95, RAW_EPSIZE,   \
        0x91, 0x02,         \
        0xC0                \
    }

/* 标准键盘：8 位修饰键、保留字节、6 个键值及 5 个 LED 输出位。 */
#define APP_USBD_HID_KBD_REPORT_DSC()                                       \
    {                                                                        \
        0x05, 0x01,     /* USAGE_PAGE (Generic Desktop)                   */ \
            0x09, 0x06, /* USAGE (Keyboard)                               */ \
            0xa1, 0x01, /* COLLECTION (Application)                       */ \
            0x05, 0x07, /*   USAGE_PAGE (Keyboard)                        */ \
            0x19, 0xE0, /*   USAGE_MINIMUM (Keyboard LeftControl)         */ \
            0x29, 0xE7, /*   USAGE_MAXIMUM (Keyboard Right GUI)           */ \
            0x15, 0x00, /*   LOGICAL_MINIMUM (0)                          */ \
            0x25, 0x01, /*   LOGICAL_MAXIMUM (1)                          */ \
            0x95, 0x08, /*   REPORT_SIZE (1)                              */ \
            0x75, 0x01, /*   REPORT_COUNT (8)                             */ \
            0x81, 0x02, /*   INPUT (Data,Var,Abs)                         */ \
            0x95, 0x01, /*   REPORT_COUNT (1)                             */ \
            0x75, 0x08, /*   REPORT_SIZE (8)                              */ \
            0x81, 0x03, /*   INPUT (Cnst,Var,Abs)                         */ \
            0x05, 0x07,       /*   REPORT_COUNT (5)                             */ \
            0x19, 0x00,       /*   REPORT_SIZE (1)                              */ \
            0x29, 0xFF,       /*   USAGE_PAGE (LEDs)                            */ \
            0x15, 0x00,       /*   USAGE_MINIMUM (Num Lock)                     */ \
            0x26, 0xFF, 0x00, /*   USAGE_MAXIMUM (Kana)                         */ \
            0x95, 0x06,       /*   OUTPUT (Data,Var,Abs)                        */ \
            0x75, 0x08,       /*   REPORT_COUNT (1)                             */ \
            0x81, 0x00,       /*   REPORT_SIZE (3)                              */ \
            0x05, 0x08, /*   OUTPUT (Cnst,Var,Abs)                        */ \
            0x19, 0x01, /*   REPORT_COUNT (6)                             */ \
            0x29, 0x05, /*   REPORT_SIZE (8)                              */ \
            0x15, 0x00, /*   LOGICAL_MINIMUM (0)                          */ \
            0x25, 0x01, /*   LOGICAL_MAXIMUM (101)                        */ \
            0x95, 0x05, /*   USAGE_PAGE (Keyboard)                        */ \
            0x75, 0x01, /*   USAGE_MINIMUM (Reserved (no event indicated))*/ \
            0x91, 0x02, /*   USAGE_MAXIMUM (Keyboard Application)         */ \
            0x95, 0x01, /*   INPUT (Data,Ary,Abs)                         */ \
            0x75, 0x03, /* END_COLLECTION                                 */ \
            0x91, 0x03, /* END_COLLECTION                                 */ \
            0xC0,       /* END_COLLECTION                                 */ \
    }

/* 组合接口：系统控制、消费控制、NKRO 位图、鼠标和径向控制器。 */
#define APP_USBD_HID_NRKO_REPORT_DSC()                                                                                                \
    {                                                                           /* system control */                                  \
        0x05, 0x01,                                                             /* USAGE_PAGE (Generic Desktop)*/                     \
            0x09, 0x80,                                                         /* USAGE (System Control)*/                           \
            0xa1, 0x01,                                                         /* COLLECTION (Application)*/                         \
            0x85, REPORT_ID_SYSTEM,                                             /*   REPORT_ID (2)*/                                  \
            0x15, 0x01,                                                         /*   LOGICAL_MINIMUM (0x1)*/                          \
            0x26, 0xb7, 0x00,                                                   /*   LOGICAL_MAXIMUM (0xb7)*/                         \
            0x19, 0x01,                                                         /*   USAGE_MINIMUM (0x1)*/                            \
            0x29, 0xb7,                                                         /*   USAGE_MAXIMUM (0xb7)*/                           \
            0x75, 0x10,                                                         /*   REPORT_SIZE (16)*/                               \
            0x95, 0x01,                                                         /*   REPORT_COUNT (1)*/                               \
            0x81, 0x00,                                                         /*   INPUT (Data,Array,Abs)*/                         \
            0xc0, /* END_COLLECTION*/                                           /* consumer */                                        \
            0x05, 0x0c,                                                         /* USAGE_PAGE (Consumer Devices)*/                    \
            0x09, 0x01,                                                         /* USAGE (Consumer Control)*/                         \
            0xa1, 0x01,                                                         /* COLLECTION (Application)*/                         \
            0x85, REPORT_ID_CONSUMER,                                           /*   REPORT_ID (3)*/                                  \
            0x15, 0x01,                                                         /*   LOGICAL_MINIMUM (0x1)*/                          \
            0x26, 0x9c, 0x02,                                                   /*   LOGICAL_MAXIMUM (0x29c)*/                        \
            0x19, 0x01,                                                         /*   USAGE_MINIMUM (0x1)*/                            \
            0x2a, 0x9c, 0x02,                                                   /*   USAGE_MAXIMUM (0x29c)*/                          \
            0x75, 0x10,                                                         /*   REPORT_SIZE (16)*/                               \
            0x95, 0x01,                                                         /*   REPORT_COUNT (1)*/                               \
            0x81, 0x00,                                                         /*   INPUT (Data,Array,Abs)*/                         \
            0xc0, /* END_COLLECTION*/                                           /* nkro */                                            \
            0x05, 0x01,                                                         /* Usage Page (Generic Desktop),*/                    \
            0x09, 0x06,                                                         /* Usage (Keyboard),*/                                \
            0xA1, 0x01,                                                         /* Collection (Application),*/                        \
            0x85, REPORT_ID_NKRO, /*   REPORT_ID (128)*/                                  /* bitmap of modifiers   */                 \
            0x75, 0x01,                                                         /*   Report Size (1),*/                               \
            0x95, 0x08,                                                         /*   Report Count (8),*/                              \
            0x05, 0x07,                                                         /*   Usage Page (Key Codes),*/                        \
            0x19, 0xE0,                                                         /*   Usage Minimum (224),*/                           \
            0x29, 0xE7,                                                         /*   Usage Maximum (231),*/                           \
            0x15, 0x00,                                                         /*   Logical Minimum (0),*/                           \
            0x25, 0x01,                                                         /*   Logical Maximum (1),*/                           \
            0x81, 0x02, /*   Input (Data, Variable, Absolute), ;Modifier byte*/ /* bitmap of keys*/                                   \
            0x95, NKRO_REPORT_BITS * 8,                                         /*   Report Count (),*/                               \
            0x75, 0x01,                                                         /*   Report Size (1),*/                               \
            0x15, 0x00,                                                         /*   Logical Minimum (0),*/                           \
            0x25, 0x01,                                                         /*   Logical Maximum(1),*/                            \
            0x05, 0x07,                                                         /*   Usage Page (Key Codes),*/                        \
            0x19, 0x00,                                                         /*   Usage Minimum (0),*/                             \
            0x29, (NKRO_REPORT_BITS * 8 - 1) & 0xFF,                            /*   Usage Maximum (),*/                              \
            0x81, 0x02,                                                         /*   Input (Data, Variable, Absolute),*/              \
            0xc0, /* End Collection*/                                           /* mouse */                                           \
            0x05, 0x01,                                                         /* USAGE_PAGE (Generic Desktop)*/                     \
            0x09, 0x02,                                                         /* USAGE (Mouse)*/                                    \
            0xa1, 0x01,                                                         /* COLLECTION (Application)*/                         \
            0x85, 0x01,                                                         /*   REPORT_ID (1)*/                                  \
            0x09, 0x01,                                                         /*   USAGE (Pointer)*/                                \
            0xa1, 0x00, /*   COLLECTION (Physical)*/                            /* ----------------------------  Buttons*/            \
            0x05, 0x09,                                                         /*     USAGE_PAGE (Button)*/                          \
            0x19, 0x01,                                                         /*     USAGE_MINIMUM (Button 1)*/                     \
            0x29, 0x05,                                                         /*     USAGE_MAXIMUM (Button 5)*/                     \
            0x15, 0x00,                                                         /*     LOGICAL_MINIMUM (0)*/                          \
            0x25, 0x01,                                                         /*     LOGICAL_MAXIMUM (1)*/                          \
            0x75, 0x01,                                                         /*     REPORT_SIZE (1)*/                              \
            0x95, 0x05,                                                         /*     REPORT_COUNT (5)*/                             \
            0x81, 0x02,                                                         /*     INPUT (Data,Var,Abs)*/                         \
            0x75, 0x03,                                                         /*     REPORT_SIZE (3)*/                              \
            0x95, 0x01,                                                         /*     REPORT_COUNT (1)*/                             \
            0x81, 0x03, /*     INPUT (Cnst,Var,Abs)*/                           /* ----------------------------  X,Y position*/       \
            0x05, 0x01,                                                         /*     USAGE_PAGE (Generic Desktop)*/                 \
            0x09, 0x30,                                                         /*     USAGE (X)*/                                    \
            0x09, 0x31,                                                         /*     USAGE (Y)*/                                    \
            0x15, 0x81,                                                         /*     LOGICAL_MINIMUM (-127)*/                       \
            0x25, 0x7f,                                                         /*     LOGICAL_MAXIMUM (127)*/                        \
            0x75, 0x08,                                                         /*     REPORT_SIZE (8)*/                              \
            0x95, 0x02,                                                         /*     REPORT_COUNT (2)*/                             \
            0x81, 0x06, /*     INPUT (Data,Var,Rel)*/                           /* ----------------------------  Vertical wheel*/     \
            0x09, 0x38,                                                         /*     USAGE (Wheel)*/                                \
            0x15, 0x81,                                                         /*     LOGICAL_MINIMUM (-127)*/                       \
            0x25, 0x7f,                                                         /*     LOGICAL_MAXIMUM (127)*/                        \
            0x35, 0x00,                                                         /*     PHYSICAL_MINIMUM (0)        - reset physical*/ \
            0x45, 0x00,                                                         /*     PHYSICAL_MAXIMUM (0)*/                         \
            0x75, 0x08,                                                         /*     REPORT_SIZE (8)*/                              \
            0x95, 0x01,                                                         /*     REPORT_COUNT (1)*/                             \
            0x81, 0x06, /*     INPUT (Data,Var,Rel)*/                           /* ----------------------------  Horizontal wheel*/   \
            0x05, 0x0c,                                                         /*     USAGE_PAGE (Consumer Devices)*/                \
            0x0a, 0x38, 0x02,                                                   /*     USAGE (AC Pan)*/                               \
            0x15, 0x81,                                                         /*     LOGICAL_MINIMUM (-127)*/                       \
            0x25, 0x7f,                                                         /*     LOGICAL_MAXIMUM (127)*/                        \
            0x75, 0x08,                                                         /*     REPORT_SIZE (8)*/                              \
            0x95, 0x01,                                                         /*     REPORT_COUNT (1)*/                             \
            0x81, 0x06,                                                         /*     INPUT (Data,Var,Rel)*/                         \
            0xc0,                                                               /*   END_COLLECTION*/                                 \
            0xc0,                                                               /* END_COLLECTION*/                                   \
            0x05, 0x01, /* 径向控制器*/                                                                                                \
            0x09, 0x0e,                                                                                                               \
            0xa1, 0x01,                                                                                                               \
            0x85, REPORT_ID_WHEEL, /* REPORT_ID (Radial Controller)*/                                                                 \
            0x05, 0x0d,                                                                                                               \
            0x09, 0x21,                                                                                                               \
            0xa1, 0x00,                                                                                                               \
            0x05, 0x09,                                                                                                               \
            0x09, 0x01,                                                                                                               \
            0x95, 0x01,                                                                                                               \
            0x75, 0x01,                                                                                                               \
            0x15, 0x00,                                                                                                               \
            0x25, 0x01,                                                                                                               \
            0x81, 0x02,                                                                                                               \
            0x05, 0x01,                                                                                                               \
            0x09, 0x37,                                                                                                               \
            0x95, 0x01,                                                                                                               \
            0x75, 0x0f,                                                                                                               \
            0x55, 0x0f,                                                                                                               \
            0x65, 0x14,                                                                                                               \
            0x36, 0xf0, 0xf1,                                                                                                         \
            0x46, 0x10, 0x0e,                                                                                                         \
            0x16, 0xf0, 0xf1,                                                                                                         \
            0x26, 0x10, 0x0e,                                                                                                         \
            0x81, 0x06,                                                                                                               \
            0x09, 0x30,                                                                                                               \
            0x75, 0x10,                                                                                                               \
            0x55, 0x0d,                                                                                                               \
            0x65, 0x13,                                                                                                               \
            0x35, 0x00,                                                                                                               \
            0x46, 0xc0, 0x5d,                                                                                                         \
            0x15, 0x00,                                                                                                               \
            0x26, 0xff, 0x7f,                                                                                                         \
            0x81, 0x02,                                                                                                               \
            0x09, 0x31,                                                                                                               \
            0x46, 0xb0, 0x36,                                                                                                         \
            0x81, 0x02,                                                                                                               \
            0x05, 0x0d,                                                                                                               \
            0x09, 0x48,                                                                                                               \
            0x36, 0xb8, 0x0b,                                                                                                         \
            0x46, 0xb8, 0x0b,                                                                                                         \
            0x16, 0xb8, 0x0b,                                                                                                         \
            0x26, 0xb8, 0x0b,                                                                                                         \
            0x81, 0x03,                                                                                                               \
            0xc0,                                                                                                                     \
            0xc0,\
}
