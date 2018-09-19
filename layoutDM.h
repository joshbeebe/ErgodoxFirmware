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
DACTYL_MANU_MATRIX_LAYER(K("0"),
        K("q") , K("w")  , K("e") , K("r") , K("t") ,
        K("a") , K("s")  , K("d") , K("f") , K("g") ,
        H("z") , K("x")  , K("c") , K("v") , K("b") ,
                 K("\\") , K("`") ,          
                                                        HL("2"), K("esc"), 
                                                        HL("1"), M("al"), 
                                                K("bs"),M("cl"),


                   K("y"), K("u"), K("i"), K("o"), K("p"),
                   K("h"), K("j"), K("k"), K("l"), K("'"),
                   K("n"), K("m"), K(","), K("."), H("/"),
                                    K("["), K("]"),
            HL("2"), K("bs"),
            HL("1"), M("cr"),
                      K("enter"), K(" ")
),
DACTYL_MANU_MATRIX_LAYER(K("0"),
        K("1") , K("2")  , K("3") , K("4") , K("5") ,
       K("tab"),  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
        M("sl"),  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
                  M("gl"),  TRANS ,          
                                                        HL("2"), TRANS,
                                                        HL("1"), TRANS,
                                                TRANS,TRANS,


                      K("6"),  K("7"),    K("8"),     K("9"), K("0"),
                   K("left"), K("down"), K("up"), K("right"), K(";"),
                    TRANS,  TRANS,  TRANS,  TRANS,  M("sr"),
                                    K("="),  K("-"),
            HL("2"), TRANS,
            HL("1"), TRANS,
                      TRANS, TRANS
),
DACTYL_MANU_MATRIX_LAYER(K("0"),
        K("F1") , K("F2")  , K("F3") , K("F4") , K("F5") ,
         TRANS ,  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
         TRANS ,  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
                  TRANS  ,  TRANS ,          
                                                        HL("2"), TRANS,
                                                        HL("1"), TRANS,
                                                TRANS,TRANS,


                   K("F6"), K("F7"), K("F8"), K("F9"), K("F10"),
                  K("F11"), K("F12"),  TRANS,  TRANS,  TRANS,
                    TRANS,  TRANS,  TRANS,  TRANS,  TRANS,
                                    TRANS,  TRANS,
            HL("2"), TRANS,
            HL("1"), TRANS,
                      TRANS, TRANS
),
DACTYL_MANU_MATRIX_LAYER(K("0"),
        K("1") , K("2")  , K("3") , K("4") , K("5") ,
         TRANS ,  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
         TRANS ,  TRANS  ,  TRANS ,  TRANS ,  TRANS ,
                  TRANS  ,  TRANS ,          
                                                        HL("2"), TRANS,
                                                        HL("1"), TRANS,
                                                TRANS,TRANS,


                   K("6"), K("7"), K("8"), K("9"), K("0"),
                    TRANS,  TRANS,  TRANS,  TRANS,  TRANS,
                    TRANS,  TRANS,  TRANS,  TRANS,  TRANS,
                                    TRANS,  TRANS,
            HL("2"), TRANS,
            HL("1"), TRANS,
                      TRANS, TRANS
),
//Function/symbol layer
/* DACTYL_MATRIX_LAYER(K("0"),
        K("F11"), K("F1"), K("F2"), K("F3"), K("F4"), K("F5"),
        TRANS, RM(""), TRANS, K("end"), TRANS, PM(""),
        TRANS, K("home"), TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        RF(""), TRANS, TRANS, TRANS, HL("1"),
                                                       TRANS, TRANS,
                                                              TRANS,
                                                TRANS, M("cl"), K("calc"),


                 K("F6"), K("F7"),   K("F8"),   K("F9"),   K("F10"), K("F12"),
                 TRANS,   TRANS,     TRANS,     TRANS,     TRANS,    TRANS,
                 K("left"),K("down"),K("up"),   K("right"),TRANS,    K("vup"),
                 TRANS,   TRANS,     U("9"),    U("0"),    TRANS,    K("vdn"),
                          HL("1"),   K("play"), K("next"), K("prev"),K("mute"),
        TRANS, TRANS,
        TRANS,
        TRANS, TRANS, TL(GAM)
),
//Gaming layer
DACTYL_MATRIX_LAYER(K("1"),
        TRANS,    TRANS,  TRANS,  TRANS,   TRANS,  TRANS,
        TRANS,    K("t"), K("q"), K("w"),  K("e"), K("r"),
        K("esc"), K("f"), K("a"), K("s"), K("d"),  TRANS,
        M("sl"),  TRANS,  TRANS,  TRANS,   TRANS,  TRANS,
        M("cl"),  TRANS,  TRANS,  M("al"), K(" "),
                                                        TRANS, TRANS,
                                                               TRANS,
                                                K(" "), TRANS, TRANS,


                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, M("sr"),
                             TRANS, TRANS, TRANS, TRANS, TRANS,
                K("bs"), PL(GAM),
                TRANS,
                TRANS, TRANS, TRANS
),
//NumPad
DACTYL_MATRIX_LAYER(K("3"),
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS,
                                                        TRANS, TRANS,
                                                              TRANS,
                                                TRANS, TRANS, TRANS,


                       TRANS,   TRANS, K("N*"), K("N*"), K("N/"), K("N-"),
                       TRANS, K("N7"), K("N8"), K("N9"),   TRANS,   TRANS,
                       TRANS, K("N4"), K("N5"), K("N6"),   TRANS, K("N+"),
                       TRANS, K("N1"), K("N2"), K("N3"),   TRANS, K("Ne"),
                             K("N0"),   TRANS,  K("N."),   TRANS,   TRANS,
                TRANS, TRANS,
                TRANS,
                TRANS, TRANS, TRANS
),
//Macro layer
DACTYL_MATRIX_LAYER(K("0"),
        TRANS , Z(VANILLA_WOW), TRANS , TRANS , TRANS               , TRANS             ,
        TRANS , TRANS        , TRANS , TRANS , TRANS               , Z("\\Theta")      ,
        TRANS , Z("`atgg`e") , TRANS , TRANS , Z("`e/oos`e")       , TRANS             ,
        TRANS , TRANS        , TRANS , TRANS , Z("`e/remaining`e") , Z("`e/hideout`e") ,
        TRANS , TRANS        , TRANS , TRANS , TRANS               ,
                                                       TRANS, TRANS,
                                                              TRANS,
                                            Z("`a4i"), TRANS, TRANS,


                 TRANS          , TRANS , TRANS     , TRANS , TRANS , TRANS ,
                 TRANS          , TRANS , Z("`nei") , TRANS , TRANS , TRANS ,
                 TRANS          , TRANS , TRANS     , TRANS , TRANS , TRANS ,
                 TRANS          , TRANS , TRANS     , TRANS , TRANS , TRANS ,
                                  TRANS , TRANS     , TRANS , TRANS , TRANS ,
        TRANS, TRANS,
        TRANS,
        TRANS, TRANS, TRANS
) */
};
