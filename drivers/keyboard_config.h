#pragma once
/* No matrix wiring has been supplied. The default map is deliberately inert. */
#ifndef MATRIX_ROWS
#define MATRIX_ROWS 1
#endif
#ifndef MATRIX_COLS
#define MATRIX_COLS 1
#endif
#define TAPPING_TERM 200
#define TAPPING_TOGGLE 5
#define ONESHOT_TAP_TOGGLE 5
#define ONESHOT_TIMEOUT 5000
#define DEBOUNCE 5
#define EECONFIG_KB_DATA_SIZE 0
#define EECONFIG_USER_DATA_SIZE 0
#define EEPROM_SIZE 1024
#define KEYBOARD_SHARED_EP
#define MOUSE_SHARED_EP
