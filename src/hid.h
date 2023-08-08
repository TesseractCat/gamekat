// https://github.com/a-h/pico_usb_kbd_mouse
#ifndef HID_H
#define HID_H

#include "pico/multicore.h"
#include "bsp/board.h"
#include "tusb.h"

static inline bool is_key_held(hid_keyboard_report_t const *report, uint8_t keycode)
{
    for(uint8_t i=0; i<6; i++) {
        if(report->keycode[i] == keycode) {
            return true;
        }
    }
    return false;
}

#endif
