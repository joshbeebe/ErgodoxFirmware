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

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))


uint8_t matrix[ROWS][COLS] = {{0}};

bool g_keys_pressed[ROWS][COLS] = {{0}};
bool g_old_keys_pressed[ROWS][COLS] = {{0}};

int g_stackLength = 0;
int g_layerStack[MAXLAYERS] = {0};

bool g_was_key_pressed = false;
int g_trans_pos[] = {0, 0, 0};

/*
 *  Initialize teensy things
 */
int teensy_init(void) {
    CPU_PRESCALE(0);
    PORTF = 0xFF;
    PORTD &= ~(1<<6); //turn off led

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

int main(void) {

    teensy_init();
    twi_init();
    mcp23018_init();

    while (1) {
        update_cols();
        //send_keys();
        _delay_ms(5);
    }
}


/*
 * Matrix of keys
 */
const struct KeyPress g_keys[][ROWS][COLS] = {KB_MATRIX_LAYER(K("0"),
        K("="), K("1"), K("2"), K("3"), K("4"), K("5"), K("`"),
        S("tab"), K("q"), K("w"), K("e"), K("r"), K("t"), L("1"),
        C("esc"), K("a"), K("s"), K("d"), K("f"), K("g"),
        H("9"), K("z"), K("x"), K("c"), K("v"), K("b"), T("1"),
        M("gl"), K("`"), K("\\"), K("2"), T("1"),
                                                        S("home"), S("end"),
                                                T("1"), T("2"), S("tab"),
                                                S("bs"), M("cl"), M("al"),


           K("5"), K("6"), K("7"), K("8"), K("9"), K("0"), K("-"),
           K("["), K("y"), K("u"), K("i"), K("o"), K("p"), K("]"),
                   K("h"), K("j"), K("k"), K("l"), K(";"), K("\'"),
           T("1"), K("n"), K("m"), K(","), K("."), K("/"), H("0"),
                           S("left"), S("down"), S("up"), S("right"), M("gr"),
            M("ar"), L("2"),
            K("3"), S("pgup"), S("pgdn"),
            S("del"), S("enter"), K(" ")
),
//Function/symbol layer
KB_MATRIX_LAYER(K("0"),
        K("="), K("F1"), K("F2"), K("F3"), K("F4"), K("F5"), K("F11"),
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS, P("1"),
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, Z("/remaining`e"), Z("/hideout`e"), T("1"),
        TRANS, TRANS, TRANS, TRANS, T("1"),
                                                       TRANS, TRANS,
                                                TRANS, TRANS, TRANS,
                                                TRANS, TRANS, S("calc"),


       K("F12"), K("F6"), K("F7"),   K("F8"),   K("F9"),   K("F10"), K("1"),
       TRANS,    TRANS,   TRANS,     TRANS,     TRANS,     TRANS,    TRANS,
                 TRANS,   U("["),    U("]"),    TRANS,     K("="),    S("vup"),
       TRANS,    TRANS,   U("9"),    U("0"),    TRANS,     TRANS,    S("vdn"),
                          S("play"), S("next"), S("prev"), TRANS,    S("mute"),
        TRANS, TRANS,
        TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS
),
//Gaming layer
KB_MATRIX_LAYER(K("0"),
        K("`"), TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        M("sl"), TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        M("cl"), TRANS, TRANS, M("al"), K(" "),
                                                        TRANS, TRANS,
                                                TRANS, TRANS, TRANS,
                                                TRANS, TRANS, TRANS,


               TRANS, TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
               TRANS, TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
               TRANS, TRANS, TRANS, TRANS, TRANS, TRANS, M("sr"),
                             TRANS, TRANS, TRANS, TRANS, TRANS,
                TRANS, P("2"),
                TRANS, TRANS, TRANS,
                TRANS, TRANS, TRANS
)
};

/*
 * Determine key to be pressed then press it
 * key: key to be pressed/released
 * isPressed: Whether the key was changed to pressed or changed to release
 */
void press_key(void* data, bool isPressed) {
    g_was_key_pressed = true;
    KeyCode key = determine_key((char*)data);
    press(key, isPressed);

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
        press(keyCode, isPressed);
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
        usb_keyboard_press(KEY_X, 0);
        return;
    }
    //change the layer
    stackPos--;
    g_trans_pos[2] = stackPos;
    lay = g_layerStack[stackPos];
    //transfer location if next key is also transparent
    KeyPress key = g_keys[lay][row][col];
    if (key.func == press_trans) {
        key.func(v, isPressed);
    }
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
    for (int i = 0; i < len; i++) {
        if (macro[i] == '`') {
            i++;
            key = macro[i];
            switch (key) {
                case 'e':
                    keyCode = KEY_ENTER;
                    break;
            }
        } else {
            keyCode = determine_single_key(macro[i]);
        }
        usb_keyboard_press(keyCode, mods);
    }
    len = 0;
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

/*
 * Press or release a key
 * key: USB keycode of the key to be pressed/released
 * isPressed: whether the key is to be pressed or released
 */
void press(KeyCode key, bool isPressed) {
    for (int i = 0; i < 6; i++) {
        //Put key in array
        //If full, the key is just ignored
        if (isPressed) {
            /*if (num_toggledLayers) {*/
                /*num_toggledKeys++;*/
                /*toggledPressedKeys[num_toggledKeys] = key;*/
            /*}*/
            if (keyboard_keys[i] == 0) {
                keyboard_keys[i] = key;
                return;
            }
        //Remove key from array
        } else {
            /*for (int j = 0; j < 6; j++) {*/
                /*if (key == toggledPressedKeys[j]) {*/
                    /*toggledPressedKeys[j] = 0;*/
                    /*num_toggledKeys--;*/
                /*}*/
            /*}*/
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

