// Copyright 2018 Jack Humbert <jack.humb@gmail.com>
// Copyright 2018-2023 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later

// Constants and variables needed from encoder_quadrature.c
static int8_t  encoder_LUT[]    = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
static uint8_t encoder_state[4] = {0};

// Function to read the encoder pin state
uint8_t encoder_read_pin(uint8_t index, bool pin_b) {
    pin_t encoders_pad_a[] = ENCODERS_PAD_A;
    pin_t encoders_pad_b[] = ENCODERS_PAD_B;
    pin_t pin              = pin_b ? encoders_pad_b[index] : encoders_pad_a[index];
    return gpio_read_pin(pin);
}
