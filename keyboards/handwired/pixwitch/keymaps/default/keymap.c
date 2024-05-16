// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SWITCH_ENC0 = SAFE_RANGE,
    SWITCH_ENC1
};

uint8_t encoder_mode[3] = {0};

const char *encoder_modes[][3] = {
    {"fuck", "oof", "yeep"},
    {"F2A", "F2B", "F2C"},
    {"F 3A", "F 3B", "F 3C"}
};

void update_oled_display(void) {
    oled_clear();
    oled_set_cursor(0, 0);
    oled_write("Encoder Modes:\n", false);

    for (uint8_t i = 0; i < 3; i++) {
        oled_write("Encoder ", false);
        oled_write_char(i + '1', false);
        oled_write(": ", false);
        oled_write(encoder_modes[i][encoder_mode[i]], false);
        oled_write("\n", false);
    }

    oled_render();
}

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
    update_oled_display();
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SWITCH_ENC0:
            if (record->event.pressed) {
                encoder_mode[0] = (encoder_mode[0] + 1) % 3;
                update_oled_display();
            }
            return false;
        case SWITCH_ENC1:
            if (record->event.pressed) {
                encoder_mode[1] = (encoder_mode[1] + 1) % 3;
                update_oled_display();
            }
            return false;
        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(
                                    KC_MPRV,      KC_MNXT,      KC_MPLY,
    KC_NUM,  KC_P0,   KC_PDOT,      C(KC_0),      MEH(KC_W),    KC_2,
    KC_P7,   KC_P8,   KC_P9,        KC_V,         KC_F3,        KC_F4,
    KC_P4,   KC_P5,   KC_P6,        KC_F5,        KC_F6,        KC_B,
    KC_P1,   KC_P2,   KC_P3,        SWITCH_ENC0,  SWITCH_ENC1,  SWITCH_ENC0)
};
