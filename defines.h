#ifndef DEFINES_H
#define DEFINES_H

#define ROWS 6
#define COLS 14
#define MAXLAYERS 10

//Time in ms
#define TAP_TIME 300u

#define IS_UPPER(x) ((x) >= 'A' && (x) <= 'Z')
#define IS_LOWER(x) ((x) >= 'a' && (x) <= 'z')
#define IS_NUMBER(x) ((x) >= '0' && (x) <= '9')
#define TO_LOWER(x) ((x - 'A' + 'a'))

//Media keycodes
#define MEDIA_MUTE          0x00E2
#define MEDIA_VOL_UP        0x00E9
#define MEDIA_VOL_DOWN      0x00EA
#define MEDIA_NEXT_TRACK    0x00B5
#define MEDIA_PREV_TRACK    0x00B6
#define MEDIA_STOP          0x00B7
#define MEDIA_PLAY_PAUSE    0x00CD


#define AL_CC_CONFIG        0x0183
#define APP_EMAIL           0x018A
#define APP_CALCULATOR      0x0192
#define APP_LOCAL_BROWSER   0x0194
#define APP_SEARCH          0x0221
#define APP_HOME            0x0223
#define APP_BACK            0x0224
#define APP_FORWARD         0x0225
#define APP_STOP            0x0226
#define APP_REFRESH         0x0227
#define APP_BOOKMARKS       0x022A

#endif /* end of include guard: DEFINES_H */
