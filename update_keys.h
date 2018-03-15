#ifndef UPDATE_KEYS_H
#define UPDATE_KEYS_H

//These are defined by physical pins on the Teensy
#define ROW5 0
#define ROW4 1
#define ROW3 4
#define ROW2 5
#define ROW1 6
#define ROW0 7


void update_cols(void);
void update_col(int colNum);
void update_left_hand(void);
void send_keys(void);
//void press_key(char key, bool isPressed);
void update_right_hand(void);

#endif /* end of include guard: UPDATE_KEYS_H */
