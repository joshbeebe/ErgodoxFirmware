#include "macro.h"
void macro_init(void) {
    /*macroKeysLen = -1;*/
    /*for (int i = 0; i < MACRO_LEN - 1; i++) {*/
        /*for (int j = 0; j < 6; j++) {*/
            /*macroKeys[i].keys[j] = 0;*/
        /*}*/
        /*macroKeys[i].modifiers = 0;*/
    /*}*/
}

int16_t macroKeysLen = -1;
void macro_clear(void) {
    macroKeysLen = -1;
}

void macro_append(void) {
    static bool wroteZero = false;
    // check for empty array
    for (int i = 0; i < 6; ++i) {
        //The array is zero
        if (keyboard_keys[i] != 0) {
            wroteZero = false;
            break;
        }
        if (keyboard_modifier_keys != 0) {
            wroteZero = false;
            break;
        }
        if (wroteZero == true) {
            return;
        }
        wroteZero = true;
    }
    macroKeysLen++;
    // write to the macro
    for (int i = 0; i < 6; ++i) {
        macroKeys[macroKeysLen].keys[i] = keyboard_keys[i];
    }
    macroKeys[macroKeysLen].modifiers = keyboard_modifier_keys;
}

void macro_playback(void) {
    for (int i = 0; i <= macroKeysLen; i++) {
        //Load keyboard_keys
        for (int j = 0; j < 6; j++) {
            keyboard_keys[j] = macroKeys[i].keys[j];
        }
        keyboard_modifier_keys = macroKeys[i].modifiers;
        usb_keyboard_send();
    }
}
