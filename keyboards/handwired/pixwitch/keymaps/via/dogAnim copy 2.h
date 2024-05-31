// Copyright 2024 evau
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MATH
#    define MATH
#    include <math.h>
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "progmem.h"
#include "timer.h"
#include "oled_driver.h"
#include "util.h"
#include "encoder_custom.h"

#define LOGO_FRAMES 1 // number of frames in logo animation
#define LOGO_SIZE 512 // number of bytes in array, minimize for adequate firmware size, max is 1024

#define ANIM_FRAMES 8 // number of frames in animation array
// #define ANIM_SIZE 672 // number of bytes in array, minimize for adequate firmware size, max is 1024

#define FRAME_WIDTH 168
#define FRAME_HEIGHT 32
#define FRAME_SIZE (FRAME_WIDTH * FRAME_HEIGHT / 8)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 8)

/////////////////////////////////////// INIT A BUNCH OF VARS

// Logo animation vars
static bool     first_start            = true;
static uint8_t  bytes_drawn[LOGO_SIZE] = {0};
static uint16_t drawn_count            = 0;
// static uint8_t  draw_interval          = 1; //draw random logo pixels every ms
static uint16_t logo_time = 0;
// Logo holding vars, lmao this is ridiculous
static uint16_t hold_start_time         = 0;
static uint16_t hold_duration           = 3000; // Duration to hold the completed logo
static bool     draw_complete           = false;
static bool     holding_logo            = false;
static bool     logo_held               = false;
const uint8_t   single_bit_masks_out[8] = {127, 191, 223, 239, 247, 251, 253, 254}; // bit masks (thanks mr.qmk) for the fade out animation

// Main animation vars
static uint8_t  frame_rate_ms   = 200;
static uint8_t  frame           = 0;
static uint16_t anim_timer      = 0;
static uint16_t sleep_timeout   = 5000;
static bool     sleeping        = false;
static bool     going_to_bed    = false;
static uint16_t fade_start_time = 0;

// Pan animation vars
static uint8_t  pan_offset    = 0;  // Where the animation is currently panned to
static uint16_t pan_delay_clk = 0;  // timer for pan delay
static uint8_t  pan_delay     = 10; // delay between pan steps (in ms)
static bool     move_right    = false;
static float    step          = 0.0f;
static int8_t   blend_factor  = 0;

// clang-format off
static const char PROGMEM logo[1][512] = {
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
0xf0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x20,
0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x20, 0x00, 0x10,
0xf0, 0xf0, 0xf0, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0xf0, 0xf0, 0xf0, 0xc0, 0x00, 0x00, 0x00,
0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0xe0, 0xe0,
0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0,
0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0,
0x10, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0xc1,
0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x07, 0x1f, 0x7f, 0xff, 0xfe, 0xfc, 0xff, 0xff, 0x7f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00,
0x01, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0xf0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xe0, 0xfe,
0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x01,
0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x00, 0xf8, 0xfe, 0xff, 0xff, 0xff,
0x0f, 0x03, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x80, 0xe0, 0xfc, 0xff, 0xff, 0x7f, 0x1f, 0x7f, 0xff, 0xff, 0xf8, 0xe0, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x03, 0x00, 0x07, 0x3f, 0xff, 0xff, 0xff,
0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x7f, 0xff, 0xff, 0xff,
0xf0, 0xc0, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x06,
0x07, 0x07, 0x07, 0x07, 0x01, 0x00, 0x00, 0x00, 0x01, 0x07, 0x07, 0x07, 0x07, 0x06, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00,
0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
0x07, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f,
0x08, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
},
};
#include "Z:\_PROJECTS\2024-pixWitch\OUTPUT\dogAnim_E\anim_data_RLE.h"
// clang-format on

static void fade_out_display(void) {
    // Define the reader structure
    oled_buffer_reader_t reader;
    uint8_t              buff_char;
    if (random() % 100 < 2) {
        srand(timer_read());
        // Fetch a pointer for the buffer byte at index 0. The return structure
        // will have the pointer and the number of bytes remaining from this
        // index position if we want to perform a sequential read by
        // incrementing the buffer pointer
        reader = oled_read_raw(0);
        // Loop over the remaining buffer and erase pixels as we go
        for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
            // Get the actual byte in the buffer by dereferencing the pointer
            buff_char = *reader.current_element;
            if (buff_char != 0) {
                oled_write_raw_byte(buff_char & single_bit_masks_out[rand() % 8], i);
            }
            // increment the pointer to fetch a new byte during the next loop
            reader.current_element++;
        }
    }
}

static void draw_logo_randomly(void) {
    // you already did it! don't keep going, my guy
    if (draw_complete) {
        return;
    }
    if (drawn_count >= (LOGO_SIZE)) {
        draw_complete   = true;
        hold_start_time = timer_read();
        return;
    }

    uint16_t       min_index, max_index;
    uint16_t       index;
    uint16_t       col_mid;
    static uint8_t expand = 1;
    srand(timer_read());
    // Find a random index that hasn't been written and is within the desired range
    bool found = false;
    while (!found) {
        // Select a random center column
        col_mid = (((rand() % 4) + 1) * SCREEN_WIDTH) - (SCREEN_WIDTH / 2);
        // Determine the range around the center column
        min_index = (expand <= 64) ? col_mid - expand : col_mid - 64;
        max_index = (expand <= 63) ? col_mid + expand : col_mid + 63;
        // Generate a random index within the narrowed range
        index = min_index + (rand() % (max_index - min_index + 1));
        if (!bytes_drawn[index]) {
            found = true;
        }
    }
    uint8_t logo_byte = pgm_read_byte(&logo[0][index]);
    oled_write_raw_byte(logo_byte, index);
    logo_time          = timer_read();
    bytes_drawn[index] = 1;
    drawn_count++;
    // Expand the range every 8 draws
    if (expand < 64 && ((drawn_count % 8)) == 0) {
        (drawn_count < 16) ? (expand += 1) : (expand += 2);
    }
}

void check_encoder_changes(bool *move_right, int8_t *blend_factor) {
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t pin_a_state = encoder_read_pin(i, false);
        uint8_t pin_b_state = encoder_read_pin(i, true);
        uint8_t state       = pin_a_state | (pin_b_state << 1);

        if ((encoder_state[i] & 0x3) != state) {
            uint8_t prev_state = encoder_state[i] & 0x3;
            uint8_t new_state  = (encoder_state[i] << 2) | state;
            int8_t  pulse      = encoder_LUT[new_state & 0xF] - encoder_LUT[prev_state];

            encoder_state[i] = new_state;

            if (pulse > 0) {
                if (!move_right) {
                    *blend_factor = 10;
                }
                *move_right = true;
                frame       = 0;
            } else if (pulse < 0) {
                if (move_right) {
                    *blend_factor = 10;
                }
                *move_right = false;
                frame       = 0;
            }
        }
    }
}

float bounce_ease_inc(float step) {
    if (step < 4 / 11.0) {
        return (121 * step * step) / 16.0;
    } else if (step < 8 / 11.0) {
        return (363 / 40.0 * step * step) - (99 / 10.0 * step) + 17 / 5.0;
    } else if (step < 9 / 10.0) {
        return (4356 / 361.0 * step * step) - (35442 / 1805.0 * step) + 16061 / 1805.0;
    } else {
        return (54 / 5.0 * step * step) - (513 / 25.0 * step) + 268 / 25.0;
    }
}

float bounce_ease_dec(float step) {
    return 1 - bounce_ease_inc(1 - step);
}

void incremental_pan(uint8_t *pan_offset, int8_t *blend_factor) {
    // Check if we're allowed to pan
    if (timer_elapsed(pan_delay_clk) < pan_delay) {
        return;
    }
    // check if we need to pan

    uint8_t to   = (uint8_t)round(40 * bounce_ease_inc(step));
    uint8_t from = (uint8_t)round(40 * bounce_ease_dec(step));

    uint8_t blendA = 10 - *blend_factor;
    uint8_t blendB = 0 + *blend_factor;

    if (move_right) {
        *pan_offset = ((blendA * to) + (blendB * from)) / (blendA + blendB);
        if (step < 1.0f) {
            step += 0.01f;
        } else {
            step = 1.0f;
        }
        *blend_factor = MAX(*blend_factor - 1, 0);
        pan_delay_clk = timer_read();
    } else {
        *pan_offset = ((blendB * to) + (blendA * from)) / (blendA + blendB);
        if (step > 0.0f) {
            step -= 0.01f;
        } else {
            step = 0.0f;
        }
        *blend_factor = MAX(*blend_factor - 1, 0);
        pan_delay_clk = timer_read();
    }
}

void update_frame(void) {
    int            RLE_index          = 0;
    uint16_t       uncompressed_index = 0;
    const char    *compressed_frame   = (const char *)pgm_read_ptr(&compressed_anim[frame]);
    static uint8_t byte2write         = 0;
    uint16_t       cumulative_run     = 0;

    // first figure out where on the screen we want to draw
    for (uint8_t row = 0; row < (SCREEN_HEIGHT / 8); row++) {
        for (uint8_t col = 0; col < SCREEN_WIDTH; col++) {
            if ((row == 0) && (col == 0)) {
                RLE_index      = 0;
                cumulative_run = 0;
            }
            // now find where in the uncompressed frame the byte we want is
            uncompressed_index = (row * FRAME_WIDTH) + col + pan_offset;

            // next find how far along the cumulitive run that index relates to
            while (cumulative_run <= uncompressed_index) {
                cumulative_run += (int)pgm_read_byte(&compressed_frame[RLE_index]); // add the run-count at [RLE_index] to the run counter
                RLE_index += 2;                                                     // increment the rle_index
            }
            byte2write = pgm_read_byte(&compressed_frame[RLE_index - 1]); // tell mr.druino that this byte is the one we want to write
            oled_write_raw_byte(byte2write, (row * SCREEN_WIDTH) + col);  // write the byte and hope it do be
        }
    }
    frame++;
    anim_timer = timer_read();
    if (frame >= ANIM_FRAMES) {
        frame = 0;
    }
}

void render_anim(void) {
    // draws the logo if first boot
    if (first_start) {
        // oled_clear();
        logo_time     = timer_read();
        pan_delay_clk = timer_read();
        first_start   = false;
        // continue drawing logo if incomplete
    } else if (!draw_complete) {
        draw_logo_randomly();
        // starts the timer for holding the logo on screen; delays the start of main animation
    } else if (draw_complete && !holding_logo && !logo_held) {
        hold_start_time = timer_read();
        holding_logo    = true;
    } else if (holding_logo && timer_elapsed(hold_start_time) < hold_duration) {
        return;
        // waits for the logo hold timer to finish, then sets a flag to skip logo stuff on the next loop
    } else if (holding_logo && timer_elapsed(hold_start_time) >= hold_duration) {
        holding_logo = false;
        logo_held    = true;
        // calls the dog animation function and begins the main animation
    } else if (timer_elapsed(anim_timer) > frame_rate_ms && frame < ANIM_FRAMES) {
        update_frame();
    } else if (!sleeping && (timer_elapsed(anim_timer) > sleep_timeout)) {
        if (!going_to_bed) {
            going_to_bed    = true;
            fade_start_time = timer_read();
        }
        if (timer_elapsed(fade_start_time) < 3000) {
            fade_out_display();
        } else {
            oled_off();
            going_to_bed = false;
        }
    } else {
        return;
    }
}
