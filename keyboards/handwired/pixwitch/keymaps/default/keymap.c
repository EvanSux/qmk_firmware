// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>
#include QMK_KEYBOARD_H
#include "quantum.h"
#include "kb.h"

enum custom_keycodes {
    SWITCH_ENC0 = SAFE_RANGE,
    SWITCH_ENC1,
    SWITCH_ENC2
};

uint8_t encoder_mode[3] = {0};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (encoder_mode[index]) {
            case 0:
                if (clockwise) {
                    tap_code_delay(KC_1, 10);
                } else {
                    tap_code_delay(KC_2, 10);
                }
                break;
            case 1:
                if (clockwise) {
                    tap_code_delay(KC_3, 10);
                } else {
                    tap_code_delay(KC_4, 10);
                }
                break;
            case 2:
                if (clockwise) {
                    tap_code_delay(KC_5, 10);
                } else {
                    tap_code_delay(KC_6, 10);
                }
                break;
        }
    }
    else if (index == 1) {
        switch (encoder_mode[index]) {
            case 0:
                if (clockwise) {
                    tap_code_delay(KC_7, 10);
                } else {
                    tap_code_delay(KC_8, 10);
                }
                break;
            case 1:
                if (clockwise) {
                    tap_code_delay(KC_9, 10);
                } else {
                    tap_code_delay(KC_0, 10);
                }
                break;
            case 2:
                if (clockwise) {
                    tap_code_delay(KC_A, 10);
                } else {
                    tap_code_delay(KC_B, 10);
                }
                break;
        }
    }
    else if (index == 2) {
        switch (encoder_mode[index]) {
            case 0:
                if (clockwise) {
                    tap_code_delay(KC_C, 10);
                } else {
                    tap_code_delay(KC_D, 10);
                }
                break;
            case 1:
                if (clockwise) {
                    tap_code_delay(KC_E, 10);
                } else {
                    tap_code_delay(KC_F, 10);
                }
                break;
            case 2:
                if (clockwise) {
                    tap_code_delay(KC_G, 10);
                } else {
                    tap_code_delay(KC_H, 10);
                }
                break;
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SWITCH_ENC0:
            if (record->event.pressed) {
                encoder_mode[0] = (encoder_mode[0] + 1) % 3;
            }
            return false;
        case SWITCH_ENC1:
            if (record->event.pressed) {
                encoder_mode[1] = (encoder_mode[1] + 1) % 3;
            }
            return false;
        case SWITCH_ENC2:
            if (record->event.pressed) {
                encoder_mode[2] = (encoder_mode[2] + 1) % 3;
            }
            return false;
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,        KC_MPRV,      KC_MNXT,      KC_MPLY,
    KC_NUM,  KC_P0,   KC_PDOT,      C(KC_0),      MEH(KC_W),    KC_2,
    KC_P7,   KC_P8,   KC_P9,        KC_V,         KC_F3,        KC_F4,
    KC_P4,   KC_P5,   KC_P6,        RGB_TOG,        RGB_HUI,        RGB_MOD,
    KC_P1,   KC_P2,   KC_P3,        SWITCH_ENC0,  SWITCH_ENC1,  SWITCH_ENC2)
};


void matrix_init_user(void) {}

void matrix_scan_user(void) {}


typedef struct {
    char word1[10];
    char word2[10];
} encoder_mode_name_t;

const encoder_mode_name_t encoder_modes[][3] = {
    {{"brush  ", "size   "}, {"brush  ", "hardnes"}, {"brush  ", "jitter "}},
    {{"undo   ", "redo   "}, {"zoom-  ", "zoom+  "}, {"next   ", "prev   "}},
    {{"1234567", "tabPrev"}, {"pan-lf ", "pan-rt "}, {"pan-up ", "pan-dn "}}
};

void oled_enc_switch(void) {
    for (uint8_t i = 0; i < 32; i++) {
        oled_write_pixel(42, i, true);
        oled_write_pixel(84, i, true);
    }

    // Display encoder modes
    for (uint8_t i = 0; i < 3; i++) {
        // Set the cursor position for each encoder section
        uint8_t col = i * 7;
        uint8_t row = 1;
        oled_set_cursor(col, row);

        // Display the first word
        oled_write(encoder_modes[i][encoder_mode[i]].word1, false);

        // Move to the next line
        row = 2;
        oled_set_cursor(col, row);

        // Display the second word
        oled_write(encoder_modes[i][encoder_mode[i]].word2, false);
    }
}

bool oled_task_user(void) {
    // oled_clear();
    oled_enc_switch();
    return false;  // Return false to indicate that the OLED display should not be updated again
}

void oled_task(void);
