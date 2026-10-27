C_SOURCES += lv_draw_sw.c
C_SOURCES += lv_draw_sw_arc.c
C_SOURCES += lv_draw_sw_blend.c
C_SOURCES += lv_draw_sw_dither.c
C_SOURCES += lv_draw_sw_gradient.c
C_SOURCES += lv_draw_sw_img.c
C_SOURCES += lv_draw_sw_letter.c
C_SOURCES += lv_draw_sw_line.c
C_SOURCES += lv_draw_sw_polygon.c
C_SOURCES += lv_draw_sw_rect.c
C_SOURCES += lv_draw_sw_transform.c
C_SOURCES += lv_draw_sw_layer.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/sw
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/sw

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/sw"
