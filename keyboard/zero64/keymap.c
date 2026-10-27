/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "keymap_common.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
		KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
		KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_TRNS,KC_ENT,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, ACTION_MODS_KEY(MOD_LCTL, KC_C),
		KC_LCTL, KC_LGUI, KC_LALT, KC_TRNS,KC_TRNS, KC_SPC, KC_TRNS,KC_TRNS,KC_TRNS,KC_SPC, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT)};

#ifdef CHORDAL_HOLD
__attribute__((weak)) const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    'L', 'L', 'L', 'L', 'L','L', 'R','R', 'R', 'R','R','R','R','R', 
	'L', 'L', 'L', 'L', 'L','L', 'R','R', 'R', 'R','R','R','R','R', 
	'L', 'L', 'L', 'L', 'L','L', 'R','R', 'R', 'R','R','R','R','R', 
	'L', 'L', 'L', 'L', 'L','L', 'R','R', 'R', 'R','R','R','R','R', 
	'L', 'L', 'L', 'L', 'L','L', 'R','R', 'R', 'R','R','R','R','R'
);
#endif
