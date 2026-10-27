# SEGGER RTT backend, only included when CONSOLE_ENABLE=yes.
C_SOURCES += $(ROOT_DIR)/package/RTT/SEGGER_RTT.c
C_SOURCES += $(ROOT_DIR)/package/RTT/SEGGER_RTT_printf.c
C_INCLUDES += -I$(ROOT_DIR)/package/RTT

