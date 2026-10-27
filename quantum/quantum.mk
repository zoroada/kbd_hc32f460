ifndef QMK_CORE
	QMK_CORE := ../quantum
endif

ifeq ($(strip $(MOUSEKEY_ENABLE)), yes)
    OPT_DEFS += -DMOUSEKEY_ENABLE -DMOUSE_ENABLE 
    C_SOURCES +=$(QMK_CORE)/mousekey.c
endif

C_SOURCES +=\
        ${QMK_CORE}/keymap_introspection.c \
        ${QMK_CORE}/quantum.c \
        ${QMK_CORE}/bitwise.c \
        ${QMK_CORE}/led.c \
        ${QMK_CORE}/action.c \
        ${QMK_CORE}/action_layer.c \
        ${QMK_CORE}/action_tapping.c \
        ${QMK_CORE}/action_util.c	\
        ${QMK_CORE}/keyboard.c \
        ${QMK_CORE}/keymap_common.c	\
        ${QMK_CORE}/keycode_config.c \
        ${QMK_CORE}/sync_timer.c \
        ${QMK_CORE}/matrix_common.c \
        ${QMK_CORE}/matrix.c \
        ${QMK_CORE}/dynamic_keymap.c \
        ${QMK_CORE}/send_string/send_string.c \
        ${QMK_CORE}/process_keycode/process_magic.c \
        ${QMK_CORE}/process_keycode/process_combo.c \
        ${QMK_CORE}/process_keycode/process_grave_esc.c \
        ${QMK_CORE}/process_keycode/process_key_override.c \
        ${QMK_CORE}/process_keycode/process_space_cadet.c \
        ${QMK_CORE}/process_keycode/process_tap_dance.c \
        ${QMK_CORE}/process_keycode/process_tri_layer.c \
        ${QMK_CORE}/process_keycode/process_repeat_key.c \
        ${QMK_CORE}/process_keycode/process_default_layer.c \
        ${QMK_CORE}/tri_layer.c \
        ${QMK_CORE}/repeat_key.c \
        ${QMK_CORE}/logging/debug.c \
        ${QMK_CORE}/logging/sendchar.c \
        ${QMK_CORE}/logging/print.c \
        ${QMK_CORE}/eeconfig.c \
        ${QMK_CORE}/wear_leveling/wear_leveling.c \
        ${QMK_CORE}/nvm/eeprom/nvm_dynamic_keymap.c \
        ${QMK_CORE}/nvm/eeprom/nvm_via.c \
        ${QMK_CORE}/via.c \
        ${QMK_CORE}/vial.c


C_INCLUDES +=-I$(QMK_CORE)							\
			-I$(QMK_CORE)/wear_leveling				\
			-I$(QMK_CORE)/bootmagic					\
			-I$(QMK_CORE)/debounce   					\
			-I$(QMK_CORE)/send_string            		\
			-I$(QMK_CORE)/process_keycode				\
			-I$(QMK_CORE)/keymap_extras				\
			-I$(QMK_CORE)/sequencer					\
			-I$(QMK_CORE)/logging					\
			-I$(QMK_CORE)/nvm/eeprom


include $(QMK_CORE)/nvm/nvm.mk

ifeq ($(strip $(AUTO_SHIFT_ENABLE)), yes)
    C_SOURCES += $(QMK_CORE)/process_keycode/process_auto_shift.c
    OPT_DEFS += -DAUTO_SHIFT_ENABLE
    ifeq ($(strip $(AUTO_SHIFT_MODIFIERS)), yes)
        OPT_DEFS += -DAUTO_SHIFT_MODIFIERS
    endif
endif

