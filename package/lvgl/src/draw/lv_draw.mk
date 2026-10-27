C_SOURCES += lv_draw_arc.c
C_SOURCES += lv_draw.c
C_SOURCES += lv_draw_img.c
C_SOURCES += lv_draw_label.c
C_SOURCES += lv_draw_line.c
C_SOURCES += lv_draw_mask.c
C_SOURCES += lv_draw_rect.c
C_SOURCES += lv_draw_transform.c
C_SOURCES += lv_draw_layer.c
C_SOURCES += lv_draw_triangle.c
C_SOURCES += lv_img_buf.c
C_SOURCES += lv_img_cache.c
C_SOURCES += lv_img_decoder.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw"

include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/arm2d/lv_draw_arm2d.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/lv_draw_nxp.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/sdl/lv_draw_sdl.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/stm32_dma2d/lv_draw_stm32_dma2d.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/sw/lv_draw_sw.mk
include $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/swm341_dma2d/lv_draw_swm341_dma2d.mk
