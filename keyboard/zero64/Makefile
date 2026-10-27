# 此工程的根目录
ROOT_DIR := ../..
KEYMAP_PATH := .
APP := $(ROOT_DIR)/app
CORE := $(ROOT_DIR)/Core
DRIVERS := $(ROOT_DIR)/drivers
BSP := $(DRIVERS)/bsp
CMSIS := $(DRIVERS)/cmsis
HC32_LL_DRIVER := $(DRIVERS)/hc32_ll_driver
RTOS :=$(ROOT_DIR)/FreeRTOS
USBLIB := $(ROOT_DIR)/midwares/hc32/usb
LVGL_DIR :=$(ROOT_DIR)/package/lvgl
FATFS :=$(ROOT_DIR)/package/Fatfs
RTT_DIR :=$(ROOT_DIR)/package/RTT
WEAR_LEG :=$(ROOT_DIR)/drivers/wear_leveling
TMK_CORE := $(ROOT_DIR)/tmk_core
PROTOCOL_DIR:= $(TMK_CORE)/protocol
QMK_CORE := $(ROOT_DIR)/quantum

#使用HC32F460xE型号
DEVICE = HC32F460xE

CURRENT_DIR := $(CURDIR)
# 使用basename命令提取文件夹名称  
CURRENT_DIR_NAME := $(notdir $(CURRENT_DIR)) 
TARGET           := $(strip $(CURRENT_DIR_NAME))

CONFIG_H = config.h

KEYMAP_C := $(KEYMAP_PATH)/keymap.c
OPT_DEFS += -DKEYMAP_C=\"$(KEYMAP_C)\"
C_SOURCES += keymap_rgb.c
C_INCLUDES += -I$(KEYMAP_PATH) -I$(ROOT_DIR)

include rules.mk
include $(DRIVERS)/drivers.mk

# 在编译 quantum/vial.c 前，由 vial.json 生成键盘定义头文件。
VIAL_JSON := $(KEYMAP_PATH)/vial.json
VIAL_DEFINITION := $(KEYMAP_PATH)/vial_generated_keyboard_definition.h
VIAL_GENERATOR := $(ROOT_DIR)/python/vial_generate_definition.py

.PHONY: FORCE_VIAL_DEFINITION
FORCE_VIAL_DEFINITION:

$(VIAL_DEFINITION): FORCE_VIAL_DEFINITION $(VIAL_JSON) $(VIAL_GENERATOR)
	python3 "$(VIAL_GENERATOR)" "$(VIAL_JSON)" "$@"

$(BUILD_DIR)/vial.o: $(VIAL_DEFINITION)
