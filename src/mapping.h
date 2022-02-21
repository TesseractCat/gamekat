#ifndef MAPPING_H
#define MAPPING_H

#include "pico/stdlib.h"

#define NUMBER_OF_INPUTS 24

typedef union {
    struct {
        bool a;
        bool b;
        bool x;
        bool y;
        bool z;
        bool l;
        bool r;
        bool ls;
        bool ms;
        bool mx;
        bool my;
        bool start;
        bool left;
        bool right;
        bool up;
        bool down;
        bool cLeft;
        bool cRight;
        bool cUp;
        bool cDown;
        bool dLeft;
        bool dRight;
        bool dUp;
        bool dDown;
    };
    bool buffer[NUMBER_OF_INPUTS];
} RectangleInput;

struct Mapping {
    uint8_t keys[NUMBER_OF_INPUTS];
};
const Mapping defaultKeymap {
    {
        /*      a: */ HID_KEY_I,
        /*      b: */ HID_KEY_SPACE,
        /*      x: */ HID_KEY_P,
        /*      y: */ HID_KEY_NONE,
        /*      z: */ HID_KEY_M,
        /*      l: */ HID_KEY_NONE,
        /*      r: */ HID_KEY_O,
        /*     ls: */ HID_KEY_NONE,
        /*     ms: */ HID_KEY_SEMICOLON,
        /*     mx: */ HID_KEY_F15,
        /*     my: */ HID_KEY_NONE,
        /*  start: */ HID_KEY_ENTER,
        /*   left: */ HID_KEY_A,
        /*  right: */ HID_KEY_F,
        /*     up: */ HID_KEY_D,
        /*   down: */ HID_KEY_S,
        /*  cLeft: */ HID_KEY_H,
        /* cRight: */ HID_KEY_L,
        /*    cUp: */ HID_KEY_K,
        /*  cDown: */ HID_KEY_J,
        /*  dLeft: */ HID_KEY_ARROW_LEFT,
        /* dRight: */ HID_KEY_ARROW_RIGHT,
        /*    dUp: */ HID_KEY_ARROW_UP,
        /*  dDown: */ HID_KEY_ARROW_DOWN,
    }
};

RectangleInput getRectangleInput(hid_keyboard_report_t const *report) {
    RectangleInput ri = {0};
    
    for (int i = 0; i < 6; i++) {
        // Keycode (of 6 max keys pressed)
        uint8_t keycode = report->keycode[i];
        if (keycode == HID_KEY_NONE)
            continue;
        
        for (int j = 0; j < NUMBER_OF_INPUTS; j++) {
            // Get mapping from defaultKeymap
            uint8_t mapped_keycode = defaultKeymap.keys[j];
            
            if (keycode == mapped_keycode) {
                // We are pressing this mapping!
                ri.buffer[j] = true;
                break;
            }
        }
    }
    return ri;
}

#endif
