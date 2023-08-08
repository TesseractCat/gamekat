#ifndef MATRIX_H
#define MATRIX_H

#include "hardware/gpio.h"

#define ROW_COUNT 4
#define COLUMN_COUNT 12

// ESCAPE Q W E R T Y U I O P BACKSPACE
// TAB A S D F G H J K L SEMICOLON APOSTROPHE
// SHIFT_LEFT Z X C V B N M COMMA PERIOD SLASH ENTER
// NONE NONE GUI_LEFT ALT_LEFT CONTROL_LEFT SPACE NONE CONTROL_RIGHT ARROW_LEFT ARROW_DOWN ARROW_UP ARROW_RIGHT
const int KEYMAP[ROW_COUNT][COLUMN_COUNT] = {
    { HID_KEY_ESCAPE, HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T, HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BACKSPACE },
    { HID_KEY_TAB, HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE },
    { HID_KEY_SHIFT_LEFT, HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B, HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, HID_KEY_ENTER },
    { HID_KEY_NONE, HID_KEY_NONE, HID_KEY_GUI_LEFT, HID_KEY_ALT_LEFT, HID_KEY_CONTROL_LEFT, HID_KEY_SPACE, HID_KEY_NONE, HID_KEY_CONTROL_RIGHT, HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP, HID_KEY_ARROW_RIGHT }
};

const int ROWS[] = {21, 20, 19, 18};
const int COLUMNS[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 22};

bool matrix[ROW_COUNT][COLUMN_COUNT] = { 0 };

uint8_t keys12[12] = { 0 };
uint8_t keys6[6] = { 0 };

void matrix_init() {
    for (int i = 0; i < ROW_COUNT; i++) {
        gpio_init(ROWS[i]);
        gpio_set_dir(ROWS[i], GPIO_OUT);
        gpio_put(ROWS[i], 1);
    }
    for (int i = 0; i < COLUMN_COUNT; i++) {
        gpio_init(COLUMNS[i]);
        gpio_set_dir(COLUMNS[i], GPIO_IN);
        gpio_pull_up(COLUMNS[i]);
    }
}

int matrix_scan() {
    int count = 0;

    uint8_t temp_keys[12] = { 0 };

    for (int row = 0; row < ROW_COUNT; row++) {
        // Set row, read columns
        gpio_put(ROWS[row], 0);
        sleep_us(30); // QMK has a delay here, if it's missing causes overlapping keys sometimes

        // For each column
        for (int column = 0; column < COLUMN_COUNT; column++) { 
            bool pressed = !gpio_get(COLUMNS[column]);
            matrix[row][column] = pressed;
            if (pressed) {
                if (count < 12) temp_keys[count] = KEYMAP[row][column];
                count++;
            }
        }
        gpio_put(ROWS[row], 1);
    }

    memcpy(keys12, temp_keys, sizeof(keys12));
    memcpy(keys6, temp_keys, sizeof(keys6));

    return count;
}

#endif