#include "update_keys.h"
#include "defines.h"
#include "main.h"
#include "libs/mcp/mcp23018.h"
#include "libs/usb/usb_keyboard.h"
#include "macro.h"
#include <avr/pgmspace.h>
/*#include <avr/interrupt.h>*/
#include <util/delay.h>

#include <stdbool.h>
//Give time for pixies to stop moving
#define debounce() _delay_us(1)


static bool keys_pressed[ROWS][COLS] = {{0}};
void update_cols(void) {
    bool old_keys_pressed[ROWS][COLS] = {{0}};
    //Move press array to old press array
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            old_keys_pressed[row][col] = keys_pressed[row][col];
        }
    }

    //Update keypress matrix
    update_right_hand();
    update_left_hand();
    //interpret results
    send_keys(old_keys_pressed);
}

#define _UP_COL(port, pin, col)  \
    {                           \
    DDR##port |= (1 << pin);     \
    debounce();                 \
    update_col(col);            \
    DDR##port &= ~(1 << pin);              \
    }
#define UP_COL(x) _UP_COL(x)

#define COL_D C, 6, 0x0d
#define COL_C D, 3, 0x0c
#define COL_B D, 2, 0x0b
#define COL_A B, 3, 0x0a
#define COL_9 B, 2, 0x09
#define COL_8 B, 1, 0x08
#define COL_7 B, 0, 0x07
void update_right_hand(void) {
    //Col D
    //Set pin to output
    UP_COL(COL_D);
    /*DDRC = (1 << 6);*/
    /*debounce();*/
    /*update_col(0x0d);*/
    /*//Set pin back to input*/
    /*DDRC = 0;*/

    //Col C
    UP_COL(COL_C);
    /*DDRD = (1 << 3);*/
    /*debounce();*/
    /*update_col(0x0c);*/
    /*DDRD = 0;*/

    //Col B
    UP_COL(COL_B);
    /*DDRD = (1 << 2);*/
    /*debounce();*/
    /*update_col(0x0b);*/
    /*DDRD = 0;*/

    //Col A
    UP_COL(COL_A);
    /*DDRB = (1 << 3);*/
    /*debounce();*/
    /*update_col(0x0a);*/
    /*DDRB = 0;*/

    //Col 9
    UP_COL(COL_9);
    /*DDRB = (1 << 2);*/
    /*debounce();*/
    /*update_col(0x09);*/
    /*DDRB = 0;*/

    //Col 8
    UP_COL(COL_8);
    /*DDRB = (1 << 1);*/
    /*debounce();*/
    /*update_col(0x08);*/
    /*DDRB = 0;*/

    //Col 7
    UP_COL(COL_7);
    /*DDRB = (1 << 0);*/
    /*debounce();*/
    /*update_col(0x07);*/
    /*DDRB = 0;*/
}

void update_left_hand(void) {
    int ret;
    ret = mcp23018_update_matrix(keys_pressed);
    }

//Check if pin is powered
#define IS_ROW_PRESSED(x) (!(PINF & 1 << x))

#define DETECT_PRESSED(x) keys_pressed[x][colNum] = IS_ROW_PRESSED(ROW##x);
/*
 * Update each key of a particular column
 * colNum: Column that is to be updated
 */
void update_col(int colNum) {

    //detect presses

    DETECT_PRESSED(0);
    DETECT_PRESSED(1);
    DETECT_PRESSED(2);
    DETECT_PRESSED(3);
    DETECT_PRESSED(4);
    DETECT_PRESSED(5);
}

/*
 * Find which keys are pressed and send them
 */
void send_keys(bool old_keys_pressed[ROWS][COLS]) {
    //int num_keys_pressed = 0;
    bool key_pressed;       //Whether or not the key at the current position is pressed
    bool old_key_pressed;   //Whether or not the key at the current position was pressed

    int currLayer = g_layerStack[g_stackLength];
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            key_pressed = keys_pressed[row][col];
            old_key_pressed = old_keys_pressed[row][col];

            //Update position in case of a transparent key
            g_trans_pos[0] = row;
            g_trans_pos[1] = col;
            g_trans_pos[2] = g_stackLength;

            KeyPress kp = g_keys[currLayer][row][col];
            if (key_pressed && !old_key_pressed) {
                kp.func(kp.data, true);
            } else if (!key_pressed && old_key_pressed) {
                kp.func(kp.data, false);
            }
        }
    }
    usb_keyboard_send();
}

