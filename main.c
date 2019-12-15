#ifdef __INTELLISENSE__
    #define __AVR_ATmega32U4__
    #include <avr/io.h>
#endif
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "update_keys.h"
#include "defines.h"
#include "determine_key.h"
#include "macro.h"
#include "hardware/teensy.h"

#include "layoutBigDM.h"

int g_stackLength = 0;
int g_layerStack[MAXLAYERS] = {0};

static bool isRecording = false;


//used for determining whether key was pressed when a mod key/ tap key is released
static bool was_key_pressed = false;
int g_trans_pos[] = {0, 0, 0};


#define test(fn,in,out) (fn(in) != out)
bool testing(void) {
    _led_blink();
    //{{{Test determineSingleKey
    if( test(determine_single_key, 'a', KEY_A)
    ||  test(determine_single_key, 'z', KEY_Z)
    ||  test(determine_single_key, '0', KEY_0)
    ||  test(determine_single_key, '9', KEY_9)
    ||  test(determine_single_key, '=', KEY_EQUAL)
    ||  test(determine_single_key, '[', KEY_LEFT_BRACE) ) {
        _led_blink();
        _led_on(1);
        hardware_delay_ms(2000);
        return true;
    }
    //}}}

    //{{{Test determine_key
    if (test(determine_key, "F10", KEY_F10)
    ||  test(determine_key, "F1" , KEY_F1)
    ||  test(determine_key, "N1" , KEYPAD_1)
    ||  test(determine_key, "N0" , KEYPAD_0)
    ||  test(determine_key, "N/" , KEYPAD_SLASH)
    ||  test(determine_key, "enter" , KEY_ENTER)
    ||  test(determine_key, "play" , MEDIA_PLAY_PAUSE)
    ||  test(determine_key, "a" , KEY_A)) {
        _led_blink();
        _led_on(2);
        hardware_delay_ms(2000);
        return true;
    }
    //}}}


    return false;
}

ms timer_elapsed(ms start_time) {
    return (start_time - hardware_read_time());
}

int main(void) {

    hardware_init();
    __led_blink(4);

    /*if (testing()) return 1;*/
    while (1) {
        hardware_loop();
        hardware_delay_ms(5);
        update_cols();
    }
}



/*
 * Determine key to be pressed then press it
 * key: key to be pressed/released
 * isPressed: Whether the key was changed to pressed or changed to release
 */
void press_key(void* data, bool isPressed) {
    was_key_pressed = true;
    KeyCode key = determine_key((char*)data);
    if (isRecording) {
        macro_append(key, isPressed, false);
    }
    hardware_press(key, isPressed);
}

void press_num_lock(void* data, bool isPressed) {
    if (!isPressed) {
        hardware_momentary_press(KEY_NUM_LOCK, 0);
        push_layer(data, isPressed);
    }
}
/*
 *  Press a modifier key
 *  data: 2 char string, first letter representing the modifier, 2nd letter rep. left or right
 *  isPressed: whether the key is being pressed or released
 */
void press_mod(void* data, bool isPressed) {
    char* key = (char*)data;
    static bool leftShiftPressed = false;
    Modifier mod = 0;
    //determine the modifier
    if (key[0] == 'c') {
        //control
        mod = key[1] == 'l' ? KEY_LEFT_CTRL : KEY_RIGHT_CTRL;
    } else if (key[0] == 'a') {
        //alt
        mod = key[1] == 'l' ? KEY_LEFT_ALT : KEY_RIGHT_ALT;
    } else if (key[0] == 's') {
        //shift
        if (isPressed) {
            if (key[1] == 'l') leftShiftPressed = true;
            if (key[1] == 'r' && leftShiftPressed) {
                hardware_momentary_press(KEY_CAPS_LOCK, 0);
            }
        } else {
            if (key[1] == 'l') leftShiftPressed = false;
        }
        mod = key[1] == 'l' ? KEY_LEFT_SHIFT : KEY_RIGHT_SHIFT;
    } else if (key[0] == 'g' || key[0] == 'w') {
        //gui/windows key
        mod = key[1] == 'l' ? KEY_LEFT_GUI : KEY_RIGHT_GUI;
    }

    //press the modifier
    if (isRecording) {
        macro_append(mod, isPressed, true);
    }
    hardware_press_modifier(mod, isPressed);
}

/*
 * Send keys with shift held. Mostly for symbols
 * data: USB keycode to be sent
 * isPressed: whether the key is pressed or released
 */
void press_upper(void* data, bool isPressed) {
    /*hardware_press_modifier(KEY_LEFT_SHIFT, isPressed);*/
    press_mod("sl", isPressed);
    press_key(data, isPressed);
}

/*
 * Transparant keys. This is used when a key is not defined on a layer. It stops from having to 
 * define the same key on multiple layers.
 * This may be called recursively if the key is transparent on multiple layers.
 * TODO: Figure out why lower layers cannot press keys from higher layers even if they are positioned
 *       that way in the layer stack.
 * v: Not used.
 * isPressed: whether or not the key should be pressed or released
 *
 * Globals:
 * g_trans_pos: position array of the key to press. 
 *      g_trans_pos[0]: row
 *      g_trans_pos[1]: col
 *      g_trans_pos[2]: current location in the layer stack
 */
void press_trans(void* v, bool isPressed) {
    int row = g_trans_pos[0];
    int col = g_trans_pos[1];
    int stackPos = g_trans_pos[2];
    int lay = g_layerStack[stackPos];
    //Do nothing if called from the main layer
    if (g_stackLength <= 0) {
        return;
    }
    //update current layer and the global transparent position array
    g_trans_pos[2]--;
    lay = g_layerStack[g_trans_pos[2]];   

    KeyPress key = g_keys[lay][row][col];
    key.func(key.data, isPressed);
}

/*
 * Send a series of keys specified by data
 * data: string of characters to send
 *       ` is used as an escape character for special keys
 * isPressed: 
 */
void press_macro(void* data, bool isPressed) {
    //only execute when key is released
    if (isPressed) return;

    char* macro = (char*)data;
    int len = strlen(macro);
    KeyCode keyCode = 0;
    char key;
    uint8_t mods = 0;
    uint8_t time;
    for (int i = 0; i < len; i++) {
        if (macro[i] == '`') {
            i++;
            key = macro[i];
            switch (key) {
                case 'a':
                    mods |= KEY_LEFT_ALT;
                    continue;
                    //This will keep control pressed for the next key press
                case 'b':
                    keyCode = KEY_BACKSPACE;
                    break;
                case 't':
                    keyCode = KEY_TAB;
                    break;
                case 'c':
                    mods |= KEY_LEFT_CTRL;
                    //This will keep control pressed for the next key press
                    continue;
                case 'e':
                    keyCode = KEY_ENTER;
                    break;
                case 'w':
                    //wait for x ms
                    time = 0;
                    i++;
                    key = macro[i];
                    while (IS_NUMBER(key)) {
                        time *= 10;
                        time += key - '0';
                        i++;
                        key = macro[i];
                    }
                    hardware_delay_ms(time);
                    //make sure we interpret the next command
                    i--;
                    continue;
            }
        } else {
            key = macro[i];

            //Handle uppercase keys
            if (IS_UPPER(key)) {
                key = TO_LOWER(key);
                mods |=  KEY_LEFT_SHIFT;  
            }

            keyCode = determine_single_key(key);
        }
        hardware_momentary_press(keyCode, mods);
        //make sure we wait long enough for things to register key presses
        hardware_delay_ms(35);
        mods = 0;
    }
}

/*
 * Press ctrl key. If released before any other key was pressed, send esc
 *
 * key: key to be sent on press
 * isPressed: whether the key is pressed
 */
void press_ctrl_key(void* key, bool isPressed) {
    if (isPressed) {
        was_key_pressed = false;
        press_mod("cl", isPressed);
    } else {
        press_mod("cl", isPressed);
        if (!was_key_pressed) {
            hardware_momentary_press(KEY_ESC, 0);
        }
    }
}
/*
 * Press shift key. If called when shift is already held, caps lock is
 * turned on. If released before any other key was pressed, send key.
 *
 * key: key to be sent on press
 * isPressed: whether the key is pressed
 */
void press_shift_key(void* key, bool isPressed) {
    static bool isShiftPressed = false;
    static uint16_t start_time = 0u;
    if (isPressed) {
        was_key_pressed = false;
        if (!isShiftPressed) {
            isShiftPressed = true;
        }
        else {
            was_key_pressed = true;
            /*hardware_momentary_press(determine_key((char*)key), KEY_LEFT_SHIFT);*/
            hardware_momentary_press(KEY_CAPS_LOCK, 0);
        }
        start_time = hardware_read_time();
        press_mod("sl", isPressed);
        
    } else {
        press_mod("sl", false);
        if ((!was_key_pressed) && (timer_elapsed(start_time) < TAP_TIME)) {
            hardware_momentary_press(determine_key((char*)key), KEY_LEFT_SHIFT);
        }
        isShiftPressed = false;
    }
}

void reflash_firmware(void* x, bool isPressed) {
    if (!isPressed) {
        hardware_reflash_firmware();
    }
}

void press_macro_record(void* key, bool isPressed) {
    if (!isPressed) {
        if (isRecording) {
            isRecording = false;
        } else {
            macro_clear();
            isRecording = true;
        }
    }
}

void press_macro_play(void* key, bool isPressed) {
    if (!isPressed) {
        isRecording = false;
        macro_play();
    }
}
