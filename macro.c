#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "macro.h"
#include "hardware/teensy.h"
#include "libs\usb\usb_keyboard.h"

void macro_init(void) {
}

uint8_t macroLen = 0;
void macro_clear(void) {
    macroLen = 0;
}

void macro_play(void) {
    for (int i = 0; i <= macroLen; i++) {
        if (macroArray[i].isModifier) {
            hardware_press_modifier(macroArray[i].key, macroArray[i].isPressed);
        } else {
            hardware_press(macroArray[i].key, macroArray[i].isPressed);
        }
        usb_keyboard_send();
        hardware_delay_ms(5);
    }
    //Service the watchdog
    hareware_reset_watchdog();
    hardware_release_all_keys();
}

/* 
    key: keycode of the key
    isPressed: whether the key is pressed or released
    isModifier: if the key is a modifier
*/
void macro_append(KeyCode key, bool isPressed, bool isModifier) {
    if (macroLen < MAX_MACRO_LEN) {
        /*macroArray[macroLen++] = {.key=key, .isPressed=isPressed, .isModifier=isModifier};*/
        macroArray[macroLen].key = key;
        macroArray[macroLen].isModifier = isModifier;
        macroArray[macroLen].isPressed = isPressed;
        macroLen++;
    }
    //If macroLen > 14, the keyboard crashes at this spot after pressing the
    //8th character (15 key events). I don't know why.
    //My best guess is the microcontroller is running out of memory.
    //I don't know of anything else that could cause the problem when returning
    //from a function.
}
