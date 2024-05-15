// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
  SWITCH_ENC = SAFE_RANGE
};

uint8_t encoder_mode = 1;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(
                                    SWITCH_ENC, KC_MPRV,    KC_MNXT,
    KC_NUM,  KC_P0,   KC_PDOT,      C(KC_0),    KC_1,       KC_2,
    KC_P7,   KC_P8,   KC_P9,        KC_V,       KC_F3,      KC_F4,
    KC_P4,   KC_P5,   KC_P6,        KC_F5,      KC_F6,      KC_B,
    KC_P1,   KC_P2,   KC_P3,        MEH(KC_W),  C(KC_J),    KC_H)
};

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { // Replace 0 with the index of your encoder if you have multiple encoders
    switch (encoder_mode) {
      case 0:
        // Encoder function for mode 0 (e.g., volume control)
        if (clockwise) {
          tap_code16_delay(KC_G, 10);
        } else {
          tap_code16_delay(KC_L, 10);
        }
        break;;
      case 1:
        // Encoder function for mode 1 (e.g., brush size)
        if (clockwise) {
          // Increase brush size
          // Replace with the appropriate keycode or function
          tap_code16_delay(KC_VOLU, 10);
        } else {
          // Decrease brush size
          // Replace with the appropriate keycode or function
          tap_code16_delay(KC_VOLD, 10);
        }
        break;
      // Add more cases for additional modes
    }
  }
  return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case SWITCH_ENC:
      if (record->event.pressed) {
        encoder_mode = (encoder_mode + 1) % 2; // Change 2 to the number of encoder modes you have
      }
      return false;
    default:
      return true;
  }
}
