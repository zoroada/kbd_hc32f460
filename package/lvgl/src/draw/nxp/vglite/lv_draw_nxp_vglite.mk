C_SOURCES += lv_draw_vglite.c
C_SOURCES += lv_draw_vglite_arc.c
C_SOURCES += lv_draw_vglite_blend.c
C_SOURCES += lv_draw_vglite_line.c
C_SOURCES += lv_draw_vglite_rect.c
C_SOURCES += lv_vglite_buf.c
C_SOURCES += lv_vglite_utils.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/vglite
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/vglite

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/draw/nxp/vglite"
