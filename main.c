#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "libs/usb/usb_keyboard.h"
#include "main.h"
#include "update_keys.h"
#include "defines.h"
#include "libs/twi/twi.h"
#include "libs/mcp/mcp23018.h"
#include "determine_key.h"

#include "layout.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))


// Recordable macro variables
/*KeyCode recordedMacro[5][255] = {{0}};*/
/*uint8_t macroBufferLength[5] = {0};*/
/*int activeMacroBuffer = 0;*/
/*bool recordingMacro = false;*/
/*bool retrieveMacroBuffer = false;*/

uint8_t matrix[ROWS][COLS] = {{0}};

bool g_keys_pressed[ROWS][COLS] = {{0}};
bool g_old_keys_pressed[ROWS][COLS] = {{0}};

int g_stackLength = 0;
int g_layerStack[MAXLAYERS] = {0};

bool g_is_recording_macro = false;


//used for determining whether key was pressed when a mod key/ tap key is released
bool g_was_key_pressed = false;
//used to keep track of key when a key is transparent
/*struct {*/
    /*int row;*/
    /*int col;*/
    /*int stack_pos;*/
/*} transparant_position;*/
int g_trans_pos[] = {0, 0, 0};

/*
 *  Initialize teensy things
 */
int teensy_init(void) {
    CPU_PRESCALE(0);
    PORTF = 0xFF;
    PORTD &= ~(1<<6); //turn off led
    /*PORTD |= 1<<6;*/

    //Set each pin for input
    DDRB = 0;
    DDRC = 0;
    //DDRD = 0;
    DDRE = 0;
    DDRF = 0;

    //Set outputs to low
    PORTC = 0;
    PORTB = 0;

    usb_init();
    while (!usb_configured());
    _delay_ms(500);
    return 0;
}

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
        _delay_ms(2000);
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
        _delay_ms(2000);
        return true;
    }
    //}}}


    return false;
}

int main(void) {

    teensy_init();
    twi_init();
    mcp23018_init();
    macro_init();


    PORTB |= (1<<5);
    PORTB |= (1<<6);
    PORTB |= (1<<7);
    if (testing()) return 1;
    //For LEDs


    while (1) {
        PORTB |= (1<<5);
        PORTB |= (1<<6);
        PORTB |= (1<<7);
        update_cols();
        //send_keys();
        _delay_ms(5);
    }
}



/*
 * Determine key to be pressed then press it
 * key: key to be pressed/released
 * isPressed: Whether the key was changed to pressed or changed to release
 */
void press_key(void* data, bool isPressed) {
    g_was_key_pressed = true;
    KeyCode key = determine_key((char*)data);
    _press(key, isPressed);

}
/*
 *  Press a non-printing key
 *  data: string with the name of the key to be pressed
 *  isPressed: whether the key is being pressed or released
 */
void press_special(void* data, bool isPressed) {
    g_was_key_pressed = true;
    KeyCode keyCode = determine_key((char*)data);
    //KEYPAD_PERIOD is the highest USB keycode
    if (keyCode > KEYPAD_PERIOD) {
        press_media(keyCode, isPressed);
    } else {
        _press(keyCode, isPressed);
    }

}

void press_num_lock(void* data, bool isPressed) {
    if (!isPressed) {
        usb_keyboard_press(KEY_NUM_LOCK, 0);
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
    int mod = 0;
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
                usb_keyboard_press(KEY_CAPS_LOCK, 0);
            }
        } else {
            if (key[1] == 'l') leftShiftPressed = false;
        }
        mod = key[1] == 'l' ? KEY_LEFT_SHIFT : KEY_RIGHT_SHIFT;
    } else if (key[0] == 'g') {
        //gui
        mod = key[1] == 'l' ? KEY_LEFT_GUI : KEY_RIGHT_GUI;
    }

    //press the modifier
    if (isPressed) {
        keyboard_modifier_keys |= mod;
    } else {
        keyboard_modifier_keys &= ~(mod);
    }
}

/*
 * Send keys with shift held. Mostly for symbols
 * data: USB keycode to be sent
 * isPressed: whether the key is pressed or released
 */
void press_upper(void* data, bool isPressed) {
    if (isPressed) keyboard_modifier_keys |= KEY_LEFT_SHIFT;
    else keyboard_modifier_keys &= ~(KEY_LEFT_SHIFT);
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
 * Press a media/application key
 * mediakey: key to be pressed
 * isPressed: whether to press or release the key
 */
void press_media(KeyCode mediakey, bool isPressed) {
    if (isPressed) {
        consumer_key = mediakey;
    } else {
        consumer_key = 0;
    }
    usb_extra_consumer_send();
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
                    while (key >= '0' && key <= '9') {
                        time *= 10;
                        time += key - '0';
                        i++;
                        key = macro[i];
                    }
                    _delay_ms(time);
                    //make sure we interpret the next command
                    i--;
                    continue;
            }
        } else {
            key = macro[i];

            //Handle uppercase keys
            if (key >= 'A' && key <= 'Z') {
                key -= 'A';
                key += 'a';
                mods |=  KEY_LEFT_SHIFT;  
            }

            keyCode = determine_single_key(key);
        }
        usb_keyboard_press(keyCode, mods);
        //make sure we wait long enough for things to register key presses
        _delay_ms(35);
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
        g_was_key_pressed = false;
        press_mod("cl", isPressed);
    } else {
        press_mod("cl", isPressed);
        if (!g_was_key_pressed) {
            usb_keyboard_press(KEY_ESC, 0);
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
    if (isPressed) {
        g_was_key_pressed = false;
        if (!isShiftPressed) {
            isShiftPressed = true;
        }
        else {
            g_was_key_pressed = true;
            usb_keyboard_press(KEY_CAPS_LOCK, 0);
        }
        press_mod("sl", isPressed);
        
    } else {
        press_mod("sl", isPressed);
        if (!g_was_key_pressed) {
            usb_keyboard_press(determine_key((char*)key), KEY_LEFT_SHIFT);
        }
        isShiftPressed = false;
    }
}

void record_macro(void* x, bool isPressed) {
    if (!isPressed) {
        g_is_recording_macro = !g_is_recording_macro;
        if (g_is_recording_macro) {
            macro_clear();
        }
    }
}

void play_macro(void* x, bool isPressed) {
    if (!isPressed) {
        g_is_recording_macro = false;
        macro_playback();
    }
}

void reflash_firmware(void* x, bool isPressed) {
    if (!isPressed) {
		// --- for all Teensy boards ---

		cli();

		// disable watchdog, if enabled
		// disable all peripherals
		UDCON = 1;
		USBCON = (1<<FRZCLK);  // disable USB
		UCSR1B = 0;
		_delay_ms(5);

		// --- Teensy 2.0 specific ---

		EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
		TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
		DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
		PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
		asm volatile("jmp 0x7E00");
    }
}

/*
 * Press or release a key
 * key: USB keycode of the key to be pressed/released
 * isPressed: whether the key is to be pressed or released
 */
void _press(KeyCode key, bool isPressed) {
    for (int i = 0; i < 6; i++) {
        //Put key in array
        //If full, the key is just ignored
        if (isPressed) {
            if (keyboard_keys[i] == 0) {
                keyboard_keys[i] = key;
                return;
            }

        //Remove key from array
        } else {
            if (keyboard_keys[i] == key) {
                keyboard_keys[i] = 0;
                return;
            }
        }
    }
}
void release_toggled_keys(void) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (keyboard_keys[i] == toggledPressedKeys[j]) {
                keyboard_keys[i] = 0;
                toggledPressedKeys[j] = 0;
                num_toggledKeys--;
            }
        }
    }
    keyboard_modifier_keys = 0;
    usb_keyboard_send();
}

