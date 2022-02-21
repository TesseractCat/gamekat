#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "bsp/board.h"
#include "tusb.h"

#include "joybusComms.hpp"

#include "hid.h"
#include "mapping.h"
#include "logic.h"

#define LED_PIN 25
#define GC_DATA_PIN 4

const uint32_t us = 125;

extern hid_keyboard_report_t usb_keyboard_report;
extern int keyboard_connected;

int main() {
    board_init();

    // Clock at 125MHz
    set_sys_clock_khz(us * 1000, true);

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    
    initLogic(ParasolDashing::BAN, SlightSideB::BAN);

    // Initialize joybusComms
    initComms(GC_DATA_PIN, us);
    // Initialize TinyUSB
    tusb_init();

    GCReport gcReport; // GameCube controller data
    while (1) {
        int board_seconds = board_millis()/1000;
        
        // Poll gamecube
        awaitPoll();
        // Poll keyboard
        tuh_task();
        
        RectangleInput ri = getRectangleInput(&usb_keyboard_report);
        gcReport = makeReport(ri);

        respondToPoll(&gcReport); // Send controller data
    }

    return 1;
}
