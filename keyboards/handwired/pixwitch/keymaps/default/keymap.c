// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "dogAnim.h"

#ifndef MAGIC_ENABLE
uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}
#endif

#ifndef MAGIC_ENABLE
uint8_t mod_config(uint8_t mod) {
    return mod;
}
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    //windows layout
[0] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,        KC_MPRV,      KC_MNXT,      KC_MPLY,
    KC_NUM,  KC_P0,   KC_PDOT,      C(KC_0),      MEH(KC_W),    KC_2,
    KC_P7,   KC_P8,   KC_P9,        KC_V,         KC_F3,        KC_F4,
    KC_P4,   KC_P5,   KC_P6,        RGB_TOG,      RGB_HUI,    RGB_MOD,
    KC_P1,   KC_P2,   KC_P3,        KC_1,         KC_2,       KC_3),

    //mac layout
[1] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,        KC_MPRV,      KC_MNXT,      KC_MPLY,
    KC_NUM,  KC_P0,   KC_PDOT,      C(KC_0),      MEH(KC_W),    KC_2,
    KC_P7,   KC_P8,   KC_P9,        KC_V,         KC_F3,        KC_F4,
    KC_P4,   KC_P5,   KC_P6,        RGB_TOG,      RGB_HUI,      RGB_MOD,
    KC_P1,   KC_P2,   KC_P3,        KC_4,         KC_5,         KC_6)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_LBRC, KC_RBRC), ENCODER_CCW_CW(S(KC_LBRC), S(KC_RBRC)), ENCODER_CCW_CW(KC_Z, S(KC_Z)), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_LBRC, KC_RBRC), ENCODER_CCW_CW(S(KC_LBRC), S(KC_RBRC)), ENCODER_CCW_CW(KC_Z, S(KC_Z)), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
#endif
// clang-format on

// OS detection
void keyboard_post_init_user(void) {
    uint32_t get_host_os(uint32_t trigger_time, void *cb_arg) { // Set default layer based on the detected OS after a 500 ms delay.
        switch (detected_host_os()) {
            case OS_UNSURE: // Don't change default layer if unsure.
                break;
            case OS_MACOS: // On Mac, set default layer to BASE_MAC.
            case OS_IOS:
                set_single_persistent_default_layer(1);
                break;
            default: // On Windows and Linux, set to BASE_WIN.
                set_single_persistent_default_layer(0);
                break;
        }
        return 0;
    }
    defer_exec(500, get_host_os, NULL);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { // Assuming you have only one encoder
        if (clockwise) {
            pan_right();
        } else {
            pan_left();
        }
    }
    return false; // Don't further process the encoder event
}

bool oled_task_user(void) {
    render_anim();
    return false;
}
