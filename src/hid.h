// https://github.com/a-h/pico_usb_kbd_mouse
#ifndef HID_H
#define HID_H

#include "pico/multicore.h"
#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_midi_host.h"

#define PIANO_START 48
#define PIANO_KEYS 25
bool piano[PIANO_KEYS];

// static inline bool is_key_held(hid_keyboard_report_t const *report, uint8_t keycode)
// {
//     for(uint8_t i=0; i<6; i++) {
//         if(report->keycode[i] == keycode) {
//             return true;
//         }
//     }
//     return false;
// }

// void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
// {
// 	uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
// 	if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD) {
//         keyboard_connected = 1;
// 	}
    
//     // Request report
//     if (!tuh_hid_receive_report(dev_addr, instance)) { }
// }
// void tuh_midi_mount_cb(uint8_t dev_addr, uint8_t in_ep, uint8_t out_ep, uint8_t num_cables_rx, uint16_t num_cables_tx) {
//     if (keyboard_connected == 0) {
//         // then no MIDI device is currently connected
//         keyboard_connected = dev_addr;
//     }
// }

/*void process_kbd_report(hid_keyboard_report_t const *report) {
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
}*/

void tuh_midi_rx_cb(uint8_t dev_addr, uint32_t num_packets) {
    if (num_packets != 0) {
        uint8_t cable_num;
        uint8_t buffer[48];
        while (1) {
            uint32_t bytes_read = tuh_midi_stream_read(dev_addr, &cable_num, buffer, sizeof(buffer));
            if (bytes_read == 0)
                return;

            // for (uint32_t idx = 0; idx < bytes_read; idx++) {
            for (uint32_t idx = 0; idx < bytes_read - 1; idx++) {
                uint8_t val = buffer[idx];
                uint8_t key = buffer[idx + 1];

                // All we care about is note-on/note-off
                // https://qcpages.qc.cuny.edu/~howe/music726.1/MIDI.html
                // Note on is 1001XXXX, note off is 1000XXXX
                // Keyboard is 48 -> 72
                int key_idx = (int)key - PIANO_START;
                if (key_idx >= 0 && key_idx < PIANO_KEYS) {
                    if ((val & 0xF0) == 0x90) {
                        piano[key_idx] = true;
                    }
                    if ((val & 0xF0) == 0x80) {
                        piano[key_idx] = false;
                    }
                }
            }
        }
    }
}

// tuh_hid_umount_cb is executed when a device is unmounted.
// void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
// {
// 	printf("Device with address %d, instance %d was unmounted.\r\n", dev_addr, instance);
// }
// void tuh_midi_umount_cb(uint8_t dev_addr, uint8_t instance) {
//     if (dev_addr == keyboard_connected) {
//         keyboard_connected = 0;
//         printf("MIDI device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
//     }
//     else {
//         printf("Unused MIDI device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
//     }
// }

#endif
