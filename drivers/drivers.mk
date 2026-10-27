ifndef DRIVERS
	DRIVERS := ../drivers
endif

C_SOURCES +=\
	$(BSP)/ev_hc32f460_lqfp100_v2/ev_hc32f460_lqfp100_v2.c\
	$(CMSIS)/Device/HDSC/hc32f4xx/Source/system_hc32f460.c\
	$(HC32_LL_DRIVER)/src/hc32_ll.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_clk.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_aos.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_efm.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_fcm.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_fcg.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_gpio.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_icg.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_spi.c\
	$(HC32_LL_DRIVER)/src/hc32_ll_dma.c\
	$(HC32_LL_DRIVER)/src/hc32_ll_qspi.c\
	$(HC32_LL_DRIVER)/src/hc32_ll_interrupts.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_usart.c\
	$(HC32_LL_DRIVER)/src/hc32_ll_tmr0.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_pwc.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_sram.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_utility.c \
	$(HC32_LL_DRIVER)/src/hc32_ll_usb.c

C_INCLUDES +=\
	-I$(BSP)/ev_hc32f460_lqfp100_v2	\
	-I$(CMSIS)/Device/HDSC/hc32f4xx/Include	\
	-I$(CMSIS)/Include	\
	-I$(HC32_LL_DRIVER)/inc	\
	-I$(DRIVERS)/platform

C_SOURCES +=\
	$(DRIVERS)/platform/keyboard_time.c \
	$(DRIVERS)/platform/keyboard_support.c\
	$(DRIVERS)/platform/keyboard_boot.c\
	$(DRIVERS)/usb/keyboard_usb_board.c \
	$(DRIVERS)/usb/keyboard_usb_hid.c\
	$(DRIVERS)/usb/usb_dev_user.c \
	$(DRIVERS)/usb/usb_dev_desc.c \
	$(DRIVERS)/eeprom.c \
	$(DRIVERS)/creat_task.c \
	$(DRIVERS)/keyboard_driver.c\
	$(DRIVERS)/keyboard_matrix.c \
	$(DRIVERS)/flash.c
            

C_INCLUDES += \
	-I$(DRIVERS)/usb \
	-I$(DRIVERS)

OPT_DEFS += -DKEYBOARD_SHARED_EP -DMOUSE_SHARED_EP -DNKRO_SHARED_EP
#使能动态键盘矩阵，满足qmk的宏定义
OPT_DEFS += -DDYNAMIC_KEYMAP_ENABLE -DGRAVE_ESC_ENABLE -DSPACE_CADET_ENABLE -DTRI_LAYER_ENABLE -DREPEAT_KEY_ENABLE \
	        -DBACKING_STORE_WRITE_SIZE=2 \
	        -DWEAR_LEVELING_BACKING_SIZE=8192 \
	        -DWEAR_LEVELING_LOGICAL_SIZE=4096\
            -DEEPROM_DRIVER\
            -DVIAL_INSECURE\
            -DMAGIC_KEYCODE_ENABLE

# OPT_DEFS += -DDYNAMIC_KEYMAP_ENABLE \
#             -DGRAVE_ESC_ENABLE \
#             -DTRI_LAYER_ENABLE \
#             -DSPACE_CADET_ENABLE \
#             -DREPEAT_KEY_ENABLE \
#             -DMAGIC_KEYCODE_ENABLE \
#             -DBACKING_STORE_WRITE_SIZE=2 \
#             -DWEAR_LEVELING_BACKING_SIZE=8192 \
#             -DWEAR_LEVELING_LOGICAL_SIZE=4096 \
#             -DEEPROM_DRIVER

ifeq (yes,$(strip $(VIAL_RGB)))
    C_SOURCES +=$(QMK_CORE)/vialrgb.c
    OPT_DEFS += -DVIAL_RGB_ENABLE
endif

ifeq (yes,$(strip $(TAP_DANCE_ENABLE)))
    OPT_DEFS += -DTAP_DANCE_ENABLE
    OPT_DEFS += -DTAPPING_TERM_PER_KEY
endif	

ifeq (yes,$(strip $(COMBO_ENABLE)))
    OPT_DEFS += -DCOMBO_ENABLE
endif	
ifeq (yes,$(strip $(KEY_OVERRIDE_ENABLE)))
    OPT_DEFS += -DKEY_OVERRIDE_ENABLE
endif	
#启用编码器
# -DMATRIX_FORIGN_KEY 
ifeq (yes,$(strip $(ROTARY_ENCODER)))
    C_SOURCES += $(DRIVERS)/ENCODER/encode.c

    C_INCLUDES += -I$(DRIVERS)/ENCODER/

    OPT_DEFS += -DENCODER_ENABLED -DMATRIX_FORIGN_KEY
endif
#RGB底灯
# ifeq (yes,$(strip $(RGB_BOTTOM_ENABLE)))
#     OPT_DEFS += -DNRFX_PWM0_ENABLED=1  -DPWM0_ENABLED=1  -DNRFX_PWM_ENABLED=1  -DPWM_ENABLED=1
#     OPT_DEFS += -DRGB_BOTTOM_ENABLE
#     # OPT_DEFS += -DSPI_ENABLED=1  -DSPI0_ENABLED=1 -DSPI0_USE_EASY_DMA=1 -DNRFX_SPIM_MISO_PULL_CFG=0
#     # OPT_DEFS += -DRGB_BOTTOM_ENABLE

#     C_INCLUDES += \
#                 $(DRIVERS)/RGB\
# 	    		$(DRIVERS)/RGB/RGB_bottom
                

#     C_SOURCES +=$(DRIVERS)/RGB/ws2812.c \
# 	            $(DRIVERS)/RGB/RGB_bottom/rgblight.c
				
#     CIE1931_CURVE = yes
    
# ifeq (yes,$(strip $(BOTTOM_LIGHT_AS_STATUS)))
#     OPT_DEFS += -DBOTTOM_LIGHT_AS_STATUS
# endif

# endif

#RGB轴灯WS2812_SPI
ifeq (yes,$(strip $(RGB_TOP_ENABLE)))
    OPT_DEFS += -DRGB_TOP_ENABLE -DWS2812_PWM

    C_INCLUDES += \
				-I$(DRIVERS)/RGB/RGB_top

    C_SOURCES +=$(DRIVERS)/RGB/color.c \
                $(DRIVERS)/RGB/ws2812.c \
				$(DRIVERS)/RGB/RGB_top/rgb_matrix_drivers.c\
				$(DRIVERS)/RGB/RGB_top/rgb_matrix.c\
                $(DRIVERS)/RGB/RGB_top/lib8tion/lib8tion.c \

    CIE1931_CURVE = yes
endif

ifeq ($(strip $(CIE1931_CURVE)), yes)
    OPT_DEFS += -DUSE_CIE1931_CURVE

    C_INCLUDES += -I$(DRIVERS)/RGB\
	
    C_SOURCES += $(DRIVERS)/RGB/led_tables.c

endif

ifeq ($(strip $(QMK_SETTINGS)), yes)
    AUTO_SHIFT_ENABLE := yes
    C_SOURCES += $(QMK_CORE)/qmk_settings.c

    OPT_DEFS += -DQMK_SETTINGS \
        -DAUTO_SHIFT_NO_SETUP \
		-DAUTO_SHIFT_REPEAT_PER_KEY \
		-DAUTO_SHIFT_NO_AUTO_REPEAT_PER_KEY \
        -DPERMISSIVE_HOLD_PER_KEY \
		-DHOLD_ON_OTHER_KEY_PRESS_PER_KEY \
		-DTAPPING_FORCE_HOLD_PER_KEY \
		-DRETRO_TAPPING_PER_KEY \
        -DCOMBO_TERM_PER_COMBO\
        -DCHORDAL_HOLD \
        -DFLOW_TAP_TERM=150
endif

ifeq (yes,$(strip $(DEBUG_ENABLE)))
    OPT_DEFS += -DDEBUG=1
endif	

include $(RTOS)/rtos.mk
include $(USBLIB)/usblib.mk
include $(TMK_CORE)/protocol.mk

# Use RTT as the QMK console backend only when console logging is enabled.
ifeq ($(strip $(CONSOLE_ENABLE)), yes)
    include $(ROOT_DIR)/package/RTT/rtt.mk
endif
include $(QMK_CORE)/quantum.mk
include $(DRIVERS)/wear_leveling/wear_leveling.mk
include $(ROOT_DIR)/app.mk
