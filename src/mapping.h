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
        /*      a: */ 21,
        /*      b: */ 17,
        /*      x: */ 24,
        /*      y: */ 255,
        /*      z: */ 19,
        /*      l: */ 255,
        /*      r: */ 23,
        /*     ls: */ 255,
        /*     ms: */ 255,
        /*     mx: */ 7,
        /*     my: */ 255,
        /*  start: */ 12,
        /*   left: */ 0,
        /*  right: */ 5,
        /*     up: */ 4,
        /*   down: */ 2,
        /*  cLeft: */ 18,
        /* cRight: */ 22,
        /*    cUp: */ 20,
        /*  cDown: */ 16,
        /*  dLeft: */ 1,
        /* dRight: */ 3,
        /*    dUp: */ 6,
        /*  dDown: */ 8,
    }
};

RectangleInput getRectangleInput(bool const *piano) {
    RectangleInput ri = {0};
    
    for (int j = 0; j < NUMBER_OF_INPUTS; j++) {
        // Get mapping from defaultKeymap
        uint8_t mapped_key = defaultKeymap.keys[j];
        
        if (mapped_key >= 0 && mapped_key < PIANO_KEYS && piano[mapped_key]) {
            // We are pressing this mapping!
            ri.buffer[j] = true;
        }
    }
    return ri;
}

#endif
