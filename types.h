#pragma once
//Each key is represented in the matrix as a function to execute 
//when the key is pressed and data for that function.
//The data will need to be cast in the function to the correct type
struct KeyPress {
    void* const data; 
    void (*const func)(void*, bool);
};

typedef uint16_t KeyCode;
typedef uint8_t Modifier;
typedef struct KeyPress KeyPress;
typedef uint16_t ms;