#include "passwords.h"
/*
 * Matrix of keys
 * TODO: Make an auto-formatter for this file
 */
const struct KeyPress g_keys[][ROWS][COLS] = {DACTYL_MATRIX_LAYER(K("0"),
        K("=")   , K("1") , K("2")  , K("3") , K("4") , K("5") ,
        S("tab") , K("q") , K("w")  , K("e") , K("r") , K("t") ,
        C("esc") , K("a") , K("s")  , K("d") , K("f") , K("g") ,
        H("9")   , K("z") , K("x")  , K("c") , K("v") , K("b") ,
        M("gl")  , K("`") , K("\\") , K("2") , HL("1") ,
                                                        HL("1"), HL("4"),
                                                                S("tab"),
                                                S("bs"),M("cl"),M("al"),


                  K("6"), K("7"), K("8"), K("9"), K("0"), K("-"),
                   K("y"), K("u"), K("i"), K("o"), K("p"), K("="),
                   K("h"), K("j"), K("k"), K("l"), K(";"), K("\'"),
                   K("n"), K("m"), K(","), K("."), K("/"), H("0"),
                           K("["), K("]"), S("up"), S("right"), M("gr"),
            HL("1"), S("bs"),
            M("cr"),                      
            S("del"), S("enter"), K(" ")
),
//Function/symbol layer
DACTYL_MATRIX_LAYER(K("0"),
        K("F11"), K("F1"), K("F2"), K("F3"), K("F4"), K("F5"),          
        TRANS, RM(""), TRANS, S("end"), TRANS, TRANS,        
        TRANS, S("home"), TRANS, TRANS, TRANS, TRANS,
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,        
        RF(""), TRANS, TRANS, TRANS, HL("1"),
                                                       TRANS, TRANS,
                                                              TRANS,
                                                TRANS, LL("2"), S("calc"),


                 K("F6"), K("F7"),   K("F8"),   K("F9"),   K("F10"), K("F12"),
                 TRANS,   TRANS,     TRANS, TRANS,     TRANS,    TRANS,
                 S("left"), S("down"), S("up"), S("right"),  TRANS, S("vup"),
                 TRANS,   U("9"),    U("0"),    TRANS,     TRANS,    S("vdn"),
                          S("play"), S("next"), S("prev"), TRANS,    S("mute"),
        TRANS, TRANS,
        TRANS,              
        TRANS, PL("2"), TL("2")
),
//Gaming layer
DACTYL_MATRIX_LAYER(K("1"),
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,       
        TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,       
        S("esc"), TRANS, TRANS, TRANS, TRANS, TRANS,
        M("sl"), TRANS, TRANS, TRANS, TRANS, TRANS,       
        M("cl"), TRANS, TRANS, M("al"), K(" "),
                                                        TRANS, TRANS,
                                                               TRANS,
                                                K(" "), TRANS, TRANS,


                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, TRANS,
                      TRANS, TRANS, TRANS, TRANS, TRANS, M("sr"),
                             TRANS, TRANS, TRANS, TRANS, TRANS,
                S("bs"), PL("2"),
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
)
};
