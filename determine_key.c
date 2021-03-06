#include <string.h>
#include "determine_key.h"
KeyCode determine_key(char* key) {
    KeyCode keyCode = 0;
    if (strlen(key) == 1) {
        return determine_single_key(key[0]);
    }
    //check for F keys
    if (key[0] == 'F') { 
        //this determines which F key
        if (!strcmp(key, "F10")) {
            keyCode = KEY_F10;
        } else if (!strcmp(key, "F11")) {
            keyCode = KEY_F11;
        } else if (!strcmp(key, "F12")) {
            keyCode = KEY_F12;
        } else {
            keyCode = key[1];
            keyCode -= '1';
            keyCode = KEY_F1 + keyCode;
        }
    } else if (key[0] == 'N') {
        if (IS_NUMBER(key[1])) {
            //key[1] -'1' turns ASCII to #, + KEYPAD_1 to convert to USB keycode
            keyCode = (key[1] - '1') + KEYPAD_1;
        } else if (key[1] == '0') {
            //0 is at the end in USB keycodes, but at the beginning of ASCII
            keyCode = KEYPAD_0;
        } else {
            switch (key[1]) {
                case '/':
                    keyCode = KEYPAD_SLASH;
                    break;
                case '*':
                    keyCode = KEYPAD_ASTERIX;
                    break;
                case '-':
                    keyCode = KEYPAD_MINUS;
                    break;
                case '+':
                    keyCode = KEYPAD_PLUS;
                    break;
                case 'e':
                    keyCode = KEYPAD_ENTER;
                    break;
                case '.':
                    keyCode = KEYPAD_PERIOD;
                    break;
            }
        }
    } else if (!strcmp(key, "enter")) {
        keyCode = KEY_ENTER;
    } else if (!strcmp(key, "tab")) {
        keyCode = KEY_TAB;
    } else if (!strcmp(key, "bs") || !strcmp(key, "backspace")) {
        keyCode = KEY_BACKSPACE;
    } else if (!strcmp(key, "up")) {
        keyCode = KEY_UP;
    } else if (!strcmp(key, "down")) {
        keyCode = KEY_DOWN;
    } else if (!strcmp(key, "right")) {
        keyCode = KEY_RIGHT;
    } else if (!strcmp(key, "left")) {
        keyCode = KEY_LEFT;
    } else if (!strcmp(key, "esc")) {
        keyCode = KEY_ESC;
    } else if (!strcmp(key, "home")) {
        keyCode = KEY_HOME;
    } else if (!strcmp(key, "end")) {
        keyCode = KEY_END;
    } else if (!strcmp(key, "del")) {
        keyCode = KEY_DELETE;
    } else if (!strcmp(key, "pgup")) {
        keyCode = KEY_PAGE_UP;
    } else if (!strcmp(key, "pgdn")) {
        keyCode = KEY_PAGE_DOWN;
    } else if (!strcmp(key, "numl")) {
        keyCode = KEY_NUM_LOCK;


        //Media keys need different treatment
    } else if (!strcmp(key, "vup")) {
        keyCode = MEDIA_VOL_UP;
    } else if (!strcmp(key, "vdn")) {
        keyCode = MEDIA_VOL_DOWN;
    } else if (!strcmp(key, "mute")) {
        /*keyCode = MEDIA_MUTE;*/
        keyCode = AUDIO_MUTE;
    } else if (!strcmp(key, "next")) {
        keyCode = MEDIA_NEXT_TRACK;
    } else if (!strcmp(key, "prev")) {
        keyCode = MEDIA_PREV_TRACK;
    } else if (!strcmp(key, "play")) {
        keyCode = MEDIA_PLAY_PAUSE;
    } else if (!strcmp(key, "calc")) {
        keyCode = APP_CALCULATOR;
    }
    /*if (keyCode) return keyCode;*/
    /*else return determine_single_key(key[0]);*/
    return keyCode;
}
#define USB_KEYCODE_OFFSET KEY_A
KeyCode determine_single_key(char data) {
    //key is used both for determining the key and returning the keycode
    KeyCode key = data;


    //convert from ASCII value to USB keycode
    //Key needs to come in as lowercase
    if (IS_LOWER(key)) {
        //set 'a' as 0, then add in the usb offset
        key -= 'a';
        key += USB_KEYCODE_OFFSET;
    } else if (key == '0') {
        //0 comes after the other numbers in USB, but before them in ASCII
        key = KEY_0;
    } else if (IS_NUMBER(key)) {
        //numbers 1-9
        key -= '1';
        key += KEY_1;
    } else {
        //Key is a special character, so it needs special treatment
        switch (key) {
            case '=':
                key = KEY_EQUAL;
                break;
            case '`':
                key = KEY_TILDE;
                break;
            case '-':
                key = KEY_MINUS;
                break;
            case '[':
                key = KEY_LEFT_BRACE;
                break;
            case ']':
                key = KEY_RIGHT_BRACE;
                break;
            case '/':
                key = KEY_SLASH;
                break;
            case ' ':
                key = KEY_SPACE;
                break;
            case '\\':
                key = KEY_BACKSLASH;
                break;
            case ';':
                key = KEY_SEMICOLON;
                break;
            case '\'':
                key = KEY_QUOTE;
                break;
            case ',':
                key = KEY_COMMA;
                break;
            case '.':
                key = KEY_PERIOD;
                break;

        }
    }
    return key;
}
