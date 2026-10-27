C_SOURCES += lv_arc.c
C_SOURCES += lv_bar.c
C_SOURCES += lv_btn.c
C_SOURCES += lv_btnmatrix.c
C_SOURCES += lv_canvas.c
C_SOURCES += lv_checkbox.c
C_SOURCES += lv_dropdown.c
C_SOURCES += lv_img.c
C_SOURCES += lv_label.c
C_SOURCES += lv_line.c
C_SOURCES += lv_roller.c
C_SOURCES += lv_slider.c
C_SOURCES += lv_switch.c
C_SOURCES += lv_table.c
C_SOURCES += lv_textarea.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/widgets
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/widgets

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/widgets"
