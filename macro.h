#ifndef MACRO_H
#define MACRO_H
#include <stdbool.h>
#include "main.h"
#include "libs/usb/usb_keyboard.h"

#define MACRO_LEN 25
typedef struct {
    uint8_t keys[6];
    uint8_t modifiers;
} MacroKey;

MacroKey macroKeys[MACRO_LEN];
//int16_t macroKeysLen;

void macro_init(void);
void macro_playback(void);
void macro_append(void);
void macro_clear(void);

#endif /* end of include guard: MACRO_H */

