#include "layer.h"
#include "libs/usb/usb_keyboard.h"
#include "libs/mcp/mcp23018.h"
#include "hardware/teensy.h"
#include <string.h>

static bool isSecLayerSet = false;
static bool isSecLayerPushed = false;
static uint8_t secLayerNumber = 0;


/*
 * Push layer onto the stack
 * TODO?: remove the layer from its current postition in the stack if it is there.
 * lay: char of the number of the layer is to be pushed onto the stack
 *  NOTE: currently only supports layers 0-9. Function will have to be changed to allow for multi-digit
 *        numbers to support more layers
 * isPressed: really only used to keep the layer from being pushed multiple times.
 *            Also needed for the function pointer
 */
static inline void _push_layer(int layer) {
    g_stackLength++;
    g_layerStack[g_stackLength] = layer;
}
void push_layer(void* lay, bool isPressed) {
    int layer = ((char*)lay)[0];
    //push when key is released so only one push actually happens
    if (!isPressed) {
        //convert to int since layer is passed in as a character
        layer -= '0';
        _push_layer(layer);

        //Secondary layer
        if (strlen((char*)lay) > 1) {
            uint8_t secLayer = ((char*)lay)[1];
            if (isSecLayerSet && secLayer == secLayerNumber) {
                _push_layer(secLayer);
                isSecLayerPushed = true;
            } else {
                isSecLayerSet = true;
                secLayerNumber = secLayer;
            }
        }
        //The LED to turn on is the first # of the string
        /*_led_on((((char*)g_keys[layer][0][0].data)[0] - '0'));*/
    }
}
/*
 * Remove layer from the stack.
 * Currently only removed the top-most apperance in the stack.
 * lay: char of number of the layer to remove
 *  NOTE: currently only supports layers 0-9. Function will have to be changed to allow for multi-digit
 *        numbers to support more layers
 * isPressed: really only used to keep the layer from being popped multiple times.
 *            Also needed for the function pointer
 */
static bool _pop_layer(int layer) {
    int i = g_stackLength;
    while (g_layerStack[i] != layer) {
        i--;
        //layer doesn't exist
        if (i <= 0) {
            return false;
        }
    }
    //delete the element and move the elements to fill the gap
    for (int j = i; j < g_stackLength; j++) {
        g_layerStack[j] = g_layerStack[j+1];
        g_layerStack[j+1] = 0;
    }
    g_stackLength--;
    return true;
    /*_led_off((((char*)g_keys[layer][0][0].data)[0] - '0'));*/
}
void pop_layer(void* lay, bool isPressed) {
    int layer = ((char*)lay)[0];
    //convert to int since layer is passed in as a character
    layer -= '0';
    //only pop on release of button
    if (!isPressed) {
        _pop_layer(layer);


        if (strlen((char*)lay) > 1) {
            uint8_t secLayer = ((char*)lay)[1];
            if (isSecLayerPushed && secLayer == secLayerNumber) {
                _pop_layer(secLayer);
                isSecLayerPushed = false;
            } else {
                isSecLayerSet = false;
            }
        }
    }
}
void toggle_layer(void* lay, bool isPressed) {
    int layer = ((char*)lay)[0] - '0';
    if (isPressed) {
        if (!_pop_layer(layer)) {
            _push_layer(layer);
        }
    }
}

//Used to keep track of which keys need to be released when layer is toggled off
uint8_t num_toggledLayers = 0;
uint8_t num_toggledKeys = 0;
uint16_t toggledPressedKeys[6] = {0};

/*
 * Toggle layer only while key is held.
 * IMPORTANT: this key needs to be present on both layers. Otherwise you will be stuck on the layer.
 *            (popping the layer might fix things, but is considered undefined behavior)
 * lay: char of number to toggle
 * isPressed: whether the layer is pushed or popped. Also needed for function pointer.
 *
 * TODO: determine whether this pushes the layer multiple times. If it does, a
 *       static bool may need to be used to stop that. That solution would only
 *       allow 1 layer to be toggled at a time, though.
 * TODO: only release keys which were pressed while a layer was being toggled
 */
void hold_layer(void* lay, bool isPressed) {
    if (isPressed) {
        //need to pass false because layer only pushes on release
        push_layer(lay, false);
        //num_toggledLayers++;
    } else {
        //release_toggled_keys();
        //num_toggledLayers--;
        hardware_release_all_keys();
        pop_layer(lay, false);
    }

}
/*
 * Release all currently pressed keys from a toggled layer
 */
