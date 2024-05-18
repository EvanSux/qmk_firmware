// Copyright 2024 evau (@EvanSux)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#define ENCODER_DEFAULT_POS 0x3

#define OLED_DISPLAY_WIDTH 128
#define OLED_DISPLAY_HEIGHT 32
#define OLED_MATRIX_SIZE (OLED_DISPLAY_HEIGHT / 8 * OLED_DISPLAY_WIDTH) // 512 (compile time mathed)
#define OLED_BLOCK_TYPE uint16_t // Type to use for segmenting the oled display for smart rendering, use unsigned types only
#define OLED_BLOCK_COUNT (sizeof(OLED_BLOCK_TYPE) * 8) // 16 (compile time mathed)
#define OLED_BLOCK_SIZE (OLED_MATRIX_SIZE / OLED_BLOCK_COUNT) // 32 (compile time mathed)
#define OLED_COM_PINS COM_PINS_SEQ

#define OLED_DISPLAY_ADDRESS 0x3C
#define OLED_FONT_H "glcdfont.c"
#define OLED_FONT_START 0 //The starting characer index for custom fonts
#define OLED_FONT_END 223 // The ending characer index for custom fonts
#define OLED_FONT_WIDTH 6 //The font width
#define OLED_FONT_HEIGHT 8 //The font height (untested)
#define OLED_IC OLED_IC_SSD1306 //Set to OLED_IC_SH1106 if you're using the SH1106 OLED controller.
#define OLED_DISPLAY_128X32
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
// #define NO_ACTION_LAYER
// #define NO_ACTION_TAPPING
// #define NO_ACTION_ONESHOT
