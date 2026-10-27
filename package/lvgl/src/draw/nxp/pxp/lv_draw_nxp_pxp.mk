C_SOURCES += lv_draw_pxp.c
C_SOURCES += lv_draw_pxp_blend.c
C_SOURCES += lv_gpu_nxp_pxp_osa.c
C_SOURCES += lv_gpu_nxp_pxp.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/pxp
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/pxp

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/pxp"
