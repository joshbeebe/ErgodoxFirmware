#ifndef TEENSY_H
#define TEENSY_H

#ifndef __AVR_ATmega32U4__
    #define __AVR_ATmega32U4__
#endif
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "../types.h"

void hardware_reflash_firmware(void);
void hardware_press(KeyCode, bool);
void hardware_press_modifier(Modifier, bool);
void hardware_loop(void);
void hardware_init(void);
void hardware_delay_ms(int);
void hardware_momentary_press(KeyCode, Modifier);
void hardware_release_all_keys(void);

//LED functions
#define _led_on(n) DDRB |= (1<<(n+4))
#define _led_off(n) DDRB &= ~(1<<(n+4))
#define _led_all_off() do {         \
                        _led_off(1);\
                        _led_off(2);\
                        _led_off(3);\
                       } while (0);

#define _led_all_on() do {         \
                        _led_on(1);\
                        _led_on(2);\
                        _led_on(3);\
                       } while (0);

#define _led_blink() do {            \
                        _led_all_on();\
                        hardware_delay_ms(500);\
                        _led_all_off();\
                     } while (0);
// Internal LED
#define __led_off PORTD &= ~(1<<6); hardware_delay_ms(250);//turn off led
#define __led_on PORTD |= 1<<6; hardware_delay_ms(250);
#define __led_hold(n) __led_on; hardware_delay_ms(n); __led_off;
#define __led_blink(n) for (int MACTMP = 0; MACTMP < n; MACTMP++) { \
                        __led_on;\
                        __led_off;\
                    }

// Private functions
void teensy_init(void);
#endif //TEENSY_H
