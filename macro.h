#ifndef MACRO_H
#define MACRO_H
#include <stdbool.h>
#include "main.h"
#define MAX_MACRO_LEN 52

void macro_append(KeyCode, bool, bool);
void macro_play(void);
void macro_clear(void);

typedef struct {
    KeyCode key;
    bool isPressed;
    bool isModifier;
} MacroKey;
MacroKey macroArray[MAX_MACRO_LEN];
#endif /* end of include guard: MACRO_H */

