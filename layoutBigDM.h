#include "passwords.h"
/*
 * Matrix of keys
 * TODO: Make an auto-formatter for this file
 */
#define HOM "0"
#define FUN "1"
#define GAM "2"
#define NUM "3"
#define MAC "4"
const struct PROGMEM KeyPress g_keys[][ROWS][COLS] = {
DACTYL_BIG_MANU_LAYER(K("0"),
        K("=")   , K("1") , K("2")  , K("3") , K("4") , K("5") ,
        K("tab") , K("q") , K("w")  , K("e") , K("r") , K("t") ,
        K("esc") , K("a") , K("s")  , K("d") , K("f") , K("g") ,
        H("9")   , K("z") , K("x")  , K("c") , K("v") , K("b") ,
                            K("\\") , M("wl") ,
                                               K("bs") , HL("1") ,
                                                            TL("2"),K("tab"),
                                                            M("cl"),M("al"),


                  K("6"), K("7"), K("8"), K("9"), K("0"), K("-"),
                   K("y"), K("u"), K("i"), K("o"), K("p"), K("="),
                   K("h"), K("j"), K("k"), K("l"), K(";"), K("\'"),
                   K("n"), K("m"), K(","), K("."), K("/"), H("0"),
                                   K("["), K("]"),
               K("enter"), K(" "),
    M("cr"), K("del"),
    K("del"), HL("1")
),
//Function/symbol layer
DACTYL_BIG_MANU_LAYER(K("0"),
        K("F11"), K("F1"), K("F2"), K("F3"), K("F4"), K("F5"),
        TRANS, RM(""), TRANS, K("end"), TRANS, PM(""),
        TRANS, K("home"), TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, RF(0), TRANS,
                     K("`"),TRANS,
                                           TRANS, TRANS,
                                                        TRANS,   TRANS,
                                                      M("cl"),   M("al"),


                 K("F6"), K("F7"),   K("F8"),   K("F9"),   K("F10"), K("F12"),
                 TRANS,   TRANS,     TRANS,     TRANS,     TRANS,    TRANS,
                 K("left"),K("down"),K("up"),   K("right"),TRANS,    K("vup"),
                 TRANS,   TRANS,     U("9"),    U("0"),    TRANS,    K("vdn"),
                                     K("play"), K("next"),
                TRANS, U("-"),
        TRANS, TRANS,
        TRANS, TRANS
),
//Gaming layer
DACTYL_BIG_MANU_LAYER(K("1"),
        TRANS,    TRANS,  TRANS,  TRANS,   TRANS,  TRANS,
        TRANS,    K("t"), K("q"), K("w"),  K("e"), K("r"),
        K("esc"), K("f"), K("a"), K("s"), K("d"),  TRANS,
        M("sl"),  TRANS,  TRANS,  TRANS,   TRANS,  TRANS,
                          TRANS,  M("al"),
                                           K(" "), TRANS,
                                                        TRANS, TRANS, 
                                                        TRANS, TRANS,


                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, M("sr"),
                                    TRANS, TRANS,
                        K("bs"), PL(GAM),
            TRANS, TRANS,
            TRANS, TRANS
)
};
