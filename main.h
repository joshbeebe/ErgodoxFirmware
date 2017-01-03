#ifndef MAIN_H
#define MAIN_H

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include "defines.h"
#include "layer.h"

//Each key is represented in the matrix as a function to execute 
//when the key is pressed and data for that function.
//The data will need to be cast in the function to the correct type
struct KeyPress {
    void* const data; 
    void (*const func)(void*, bool);
};

typedef uint16_t KeyCode;
typedef struct KeyPress KeyPress;
//Variables global to whole project
//To keep track of the layers
extern int g_stackLength;
//extern int currentLayer;
extern int g_layerStack[MAXLAYERS];

//position of transparent key press
extern int g_trans_pos[3];

//matrix of current keys pressed
extern bool g_keys_pressed[ROWS][COLS];
//matrix of previous keys pressed
extern bool g_old_keys_pressed[ROWS][COLS];

extern const struct KeyPress g_keys[][ROWS][COLS];

extern bool g_was_key_pressed;

//For normal keys
#define K(x) {x, press_key}
void press_key(void* data, bool);
#define M(x) {x, press_mod}
void press_mod(void* data, bool);
#define S(x) {x, press_special}
void press_special(void*, bool);
#define TRANS {0, press_trans}
void press_trans(void*, bool);
#define L(x) {x, push_layer}
#define P(x) {x, pop_layer}
#define T(x) {x, toggle_layer}
#define U(x) {x, press_upper}
void press_upper(void*, bool);
#define Z(x) {x, press_macro}
void press_macro(void*, bool);
#define C(x) {x, press_ctrl_key}
void press_ctrl_key(void*, bool);
#define H(x) {x, press_shift_key}
void press_shift_key(void*, bool);


void press(KeyCode, bool);
void press_media(KeyCode, bool);




//From official firmware
//src/keyboard/ergodox/matrix.h
#define KB_MATRIX_LAYER(	    			            	    \
        /* for unused positions */	    		                \
        na,						                                \
                                                                \
        /* left hand, spatial positions */		                \
        k50,k51,k52,k53,k54,k55,k56,			                \
        k40,k41,k42,k43,k44,k45,k46,			                \
        k30,k31,k32,k33,k34,k35,		    	                \
        k20,k21,k22,k23,k24,k25,k26,			                \
        k10,k11,k12,k13,k14,			    	                \
                                k05,k06,		                \
                            k15,k16,k04,		                \
                    k03,k02,k01,		                        \
                                                                \
        /* right hand, spatial positions */		                \
            k57,k58,k59,k5A,k5B,k5C,k5D,		                \
            k47,k48,k49,k4A,k4B,k4C,k4D,		                \
                k38,k39,k3A,k3B,k3C,k3D,		                \
            k27,k28,k29,k2A,k2B,k2C,k2D,		                \
                    k19,k1A,k1B,k1C,k1D,		                \
        k07,k08,					                            \
        k09,k17,k18,					                        \
        k0C,k0B,k0A )					                        \
                                                                \
/* matrix positions */						                    \
{{  na,k01,k02,k03,k04,k05,k06, k07,k08,k09,k0A,k0B,k0C, na },	\
 { k10,k11,k12,k13,k14,k15,k16, k17,k18,k19,k1A,k1B,k1C,k1D },	\
 { k20,k21,k22,k23,k24,k25,k26, k27,k28,k29,k2A,k2B,k2C,k2D },	\
 { k30,k31,k32,k33,k34,k35, na,  na,k38,k39,k3A,k3B,k3C,k3D },	\
 { k40,k41,k42,k43,k44,k45,k46, k47,k48,k49,k4A,k4B,k4C,k4D },	\
 { k50,k51,k52,k53,k54,k55,k56, k57,k58,k59,k5A,k5B,k5C,k5D }}


#endif /* end of include guard: MAIN_H */
