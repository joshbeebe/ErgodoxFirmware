#ifndef LAYER_H

#define LAYER_H

#include "main.h"
void push_layer(void*, bool);
void pop_layer(void*, bool);
void toggle_layer(void*, bool);
void hold_layer(void*, bool);
void release_toggled_keys(void);
extern uint8_t num_toggledLayers; 
extern uint8_t num_toggledKeys;
extern uint16_t toggledPressedKeys[6];
#endif /* end of include guard: LAYER_H */
