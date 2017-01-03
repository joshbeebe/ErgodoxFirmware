#ifndef DETERMINE_KEY_H

#define DETERMINE_KEY_H
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "libs/usb/usb_keyboard.h"
#include "defines.h"
#include "main.h"


/*
 * Determine keycode from string.
 * key: string to parse to find keycode
 */
KeyCode determine_key(char* key);
/*
 * Determine keycode from a single character.
 * This should only be called when you don't want to parse the string for a 
 * keycode.
 * key: char of key to send
 */
KeyCode determine_single_key(char key);


#endif /* end of include guard: DETERMINE_KEY_H */
