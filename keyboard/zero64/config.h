/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2019 Jim Jiang <jim@lotlab.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* USB Device descriptor parameter */
#define VENDORS_ID 0x0520 /* USB VID */
#define PRODUCT_ID 0x0061 /* USB PID */
#define DEVICE_VER 0x0100 /* 硬件版本 */
#define MANUFACTURER "Zorokb" /* 硬件制造商，用于蓝牙显示 */
#define PRODUCT "Zero64-vial"/* 硬件名词，用于蓝牙显示 */

/* USB HID report parameter */
#define KEYBOARD_EPSIZE 8 /* 键盘上传端点大小，请不要修改 */
#define NKRO_EPSIZE 28 /* 键盘NKRO端点大小，请不要修改 */

/* key matrix size */
#define MATRIX_ROWS 5 /* 硬件阵列行数 */
#define MATRIX_COLS 14 /* 硬件阵列列数 */

#define VIAL_KEYBOARD_UID {0xAF, 0x15, 0xD4, 0xDC, 0x0B, 0x9D, 0x26, 0x00}
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 9}
/* define if matrix has ghost */
// #define MATRIX_HAS_GHOST /* 按键阵列是否出现Ghost Key，若没有加二极管则需要启用这个项目 */

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5 /* 硬件消抖次数 */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#ifdef COMMAND_ENABLE
/* key combination for command */
#define IS_COMMAND() (     keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)))
#endif


#define RETRO_TAPPING
#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_RIGHT_CTRL
// LED 配置
#define CAPS_LED        15

// ws2812 RGB 配置
#define RGB_M_LED_NUM               5 // 8
#define RGB_M_DI_PIN                10 // 10
#define RGB_M_PWR_PIN_REVERSE       11 // P-mos
#define DRIVER_LED_TOTAL            RGB_M_LED_NUM
// #define RGB_M_PWR_PIN_REVERSE 20 // N-mos
//ENCODER
// #define ROTARY_ENCODER_A  23
// #define ROTARY_ENCODER_B  41
// #define NUM_ENCODERS    1
// // 旋钮正向按钮映射
// #define ROTARY_ENCODER_POS 3,0
// // 旋钮负向按钮映射
// #define ROTARY_ENCODER_NEG 3,1  

#define RGB_MATRIX_STARTUP_ENABLE  1    //默认开启还是关闭EGB轴灯
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
// #define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)释放
#define RGB_MATRIX_KEYPRESSES // reacts to keypresses 按下
#define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#define RGB_DISABLE_WHEN_USB_SUSPENDED false // turn off effects when suspended
#define RGB_MATRIX_LED_PROCESS_LIMIT (DRIVER_LED_TOTAL + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
#define RGB_MATRIX_LED_FLUSH_LIMIT 16 // limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200 // limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 255
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT // Sets the default mode, if none has been set
#define RGB_MATRIX_STARTUP_HUE 0 // Sets the default hue value, if none has been set
#define RGB_MATRIX_STARTUP_SAT 255 // Sets the default saturation value, if none has been set
#define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
#define RGB_MATRIX_STARTUP_SPD 127 // Sets the default animation speed, if none has been set

// 按键阵列配置
static const uint8_t MATRIX_ROW_PINS[MATRIX_ROWS] = { 7, 16, 17, 18, 26};
static const uint8_t MATRIX_COL_PINS[MATRIX_COLS] = { 15, 46, 20, 24, 25, 23, 22, 21, 82, 30, 31, 0, 1, 2 };
#define COL_IN // 键盘阵列的二极管方向是从COL->ROW

#define LED_POSITIVE // LED上拉驱动

#endif

