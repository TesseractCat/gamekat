#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "bsp/board.h"
#include "tusb.h"

#include "gcReport.hpp"
#include "joybus.hpp"

#include "hid.h"
#include "mapping.h"
#include "logic.h"
#include "matrix.h"

#define LED_PIN 25
#define GC_DATA_PIN 28
#define GC_3V3_PIN 27

const uint32_t us = 125;

extern bool matrix[ROW_COUNT][COLUMN_COUNT];
extern uint8_t keys12[12];
extern uint8_t keys6[6];

void matrix_loop() {
    while (true) {
        gpio_put(LED_PIN, matrix_scan() > 0);
    }
}

int main() {
    board_init();
    matrix_init();

    // Clock at 125MHz
    set_sys_clock_khz(us * 1000, true);

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // for (int i = 0; i < 5000; i++) {
    //     sleep_ms(1);

    //     matrix_scan();
    //     gpio_put(LED_PIN, matrix[1][1]);
    // }
    
    initLogic(ParasolDashing::BAN, SlightSideB::BAN);
    
    // Wait for GC communication, otherwise reboot into BOOTSEL mode
    gpio_init(GC_DATA_PIN);
    gpio_set_dir(GC_DATA_PIN, GPIO_IN);
    gpio_pull_up(GC_DATA_PIN);
    bool reboot_bootsel = true;
    while (to_ms_since_boot(get_absolute_time()) < 2500) {
        if (!gpio_get(GC_DATA_PIN)) {
            reboot_bootsel = false;
            break;
        }
    }
    if (reboot_bootsel) {
        reset_usb_boot(0,0);

        // Keyboard mode
        // Initialize TinyUSB
        tusb_init();
    }

    // Runs on different core because matrix_scan takes ~120 us
    // and the callback for pico-joybus-comms should be instant
    multicore_launch_core1(matrix_loop);
    
    enterMode(GC_DATA_PIN, []() {
        RectangleInput ri = getRectangleInput(keys12, 12);
        GCReport gcReport = makeReport(ri);

        return gcReport;
    });

    return 1;
}
