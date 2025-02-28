// https://github.com/a-h/pico_usb_kbd_mouse
#ifndef HID_H
#define HID_H

#include "pico/multicore.h"
#include "bsp/board_api.h"
#include "tusb.h"

hid_keyboard_report_t usb_keyboard_report;
int keyboard_connected = 0;

static inline bool is_key_held(hid_keyboard_report_t const *report, uint8_t keycode)
{
    for(uint8_t i=0; i<6; i++) {
        if(report->keycode[i] == keycode) {
            return true;
        }
    }
    return false;
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
	uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
	if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
        keyboard_connected = 1;
	}
    
    // Request report
    if (!tuh_hid_receive_report(dev_addr, instance)) { }
}
void process_kbd_report(hid_keyboard_report_t const *report) {
	static hid_keyboard_report_t prev_report = { 0, 0, {0} };
    
    usb_keyboard_report = {0};
    usb_keyboard_report = *report;

	prev_report = *report;
}
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
	switch (tuh_hid_interface_protocol(dev_addr, instance)) {
        case HID_ITF_PROTOCOL_KEYBOARD:
            process_kbd_report((hid_keyboard_report_t const*) report);
            break;
	}
    
    // Request report
    if (!tuh_hid_receive_report(dev_addr, instance)) { }
}

// tuh_hid_umount_cb is executed when a device is unmounted.
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
	printf("Device with address %d, instance %d was unmounted.\r\n", dev_addr, instance);
}

#endif
