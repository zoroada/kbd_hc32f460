#pragma once
#include "stdint.h"
#include "matrix.h"

#define KEYBOARD_DEBOUNCE_MS 1U

void matrix_init(void);
void matrix_wakeup_prepare(void);
void matrix_uninit(void);
void matrix_scan_zoro(void);
void matrix_scan_kb(void);
void matrix_set_row(uint8_t row,uint8_t col);
void matrix_uset_row(uint8_t row, uint8_t col);
void keyboard_debounce(void);
void keyboard_debounce_timer_reset(void);
uint8_t matrix_get_col(uint8_t col);
