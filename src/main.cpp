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
    if (reboot_bootsel)
        reset_usb_boot(0,0);
    
    // Initialize TinyUSB
    tusb_init();

    enterMode(GC_DATA_PIN, []() {
        // Poll keyboard
        tuh_task();

        RectangleInput ri = getRectangleInput(&usb_keyboard_report);
        GCReport gcReport = makeReport(ri);

        return gcReport;
    });

    return 1;
}
