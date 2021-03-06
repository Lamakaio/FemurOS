#include <stdbool.h>
int initKeyboard(void);
enum key {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    SPACE,
    ENTER,
    LSHIFT,
    RSHIFT,
    LCTRL,
    RCTRL,
    LALT,
    RALT,
    VERMAJ,
    TAB,
    DEL,
    VIRGULE,
    PVIRGULE,
    _2POINTS,
    EXCLAMATION,
    U_ACCENT,
    DOLLAR,
    CIRCONFLEXE,
    ETOILE,
    EGALE,
    RPAR,
    A_ACCENT,
    C_CEDILLE,
    UNDERSCORE,
    E_GRAVE,
    TIRET,
    LPAR,
    APOSROPHE,
    GUILLEMETS,
    E_AIGU,
    ESPERLUETTE,
    CARRE,
    ECHAP,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    IMPR_ECRAN,
    INS,
    SUPPR,
    UP,
    LEFT,
    RIGHT,
    DOWN,
    INFSUP
};
void translate(int scancode1, int scancode2, int scancode3);
void writeBuf(void);
char* getBuf(void);
char getChar(void);
void flushBuf(void);
void freeze(void); //wait until keypress
bool getKey(enum key k);
