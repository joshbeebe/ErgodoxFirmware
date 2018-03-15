#include "teensy.h"
#include "../libs/twi/twi.h"
#include "../libs/mcp/mcp23018.h"
#include "../libs/usb/usb_keyboard.h"
#include "../main.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
/*
 *  Initialize teensy things
 */
void teensy_init(void) {
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
}

void hardware_init(void) {
    teensy_init();
    twi_init();
    mcp23018_init();


    PORTB |= (1<<5);
    PORTB |= (1<<6);
    PORTB |= (1<<7);
}

void hardware_loop(void) {
    PORTB |= (1<<5);
    PORTB |= (1<<6);
    PORTB |= (1<<7);

    _delay_ms(5);
}

void hardware_press_modifier(KeyCode mod, bool isPressed) {
    if (isPressed) {
        keyboard_modifier_keys |= mod;
    } else {
        keyboard_modifier_keys &= ~(mod);
    }
}
/*
 * Press or release a key
 * key: USB keycode of the key to be pressed/released
 * isPressed: whether the key is to be pressed or released
 */
void hardware_press(KeyCode key, bool isPressed) {
    // Codes above KEYPAD_PERIOD need to be handled with a special function
    if (key > KEYPAD_PERIOD) {
        if (isPressed) {
            consumer_key = key;
        } else {
            consumer_key = 0;
        }
        usb_extra_consumer_send();
    } else {
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
}
void hardware_momentary_press(KeyCode key, int mods) {
    usb_keyboard_press(key, mods);
}
void hardware_reflash_firmware(void) {
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

void hardware_delay_ms(int ms) {
    _delay_ms(ms);
}
