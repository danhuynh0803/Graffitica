#include <SDL3/SDL_keycode.h>
#include <core/types.h>

namespace gr
{

enum class ScanCode : U32
{
    UNKNOWN = SDL_SCANCODE_UNKNOWN,

    A = SDL_SCANCODE_A,
    B = SDL_SCANCODE_B,
    C = SDL_SCANCODE_C,
    D = SDL_SCANCODE_D,
    E = SDL_SCANCODE_E,
    F = SDL_SCANCODE_F,
    G = SDL_SCANCODE_G,
    H = SDL_SCANCODE_H,
    I = SDL_SCANCODE_I,
    J = SDL_SCANCODE_J,
    K = SDL_SCANCODE_K,
    L = SDL_SCANCODE_L,
    M = SDL_SCANCODE_M,
    N = SDL_SCANCODE_N,
    O = SDL_SCANCODE_O,
    P = SDL_SCANCODE_P,
    Q = SDL_SCANCODE_Q,
    R = SDL_SCANCODE_R,
    S = SDL_SCANCODE_S,
    T = SDL_SCANCODE_T,
    U = SDL_SCANCODE_U,
    V = SDL_SCANCODE_V,
    W = SDL_SCANCODE_W,
    X = SDL_SCANCODE_X,
    Y = SDL_SCANCODE_Y,
    Z = SDL_SCANCODE_Z,

    _1 = SDL_SCANCODE_1,
    _2 = SDL_SCANCODE_2,
    _3 = SDL_SCANCODE_3,
    _4 = SDL_SCANCODE_4,
    _5 = SDL_SCANCODE_5,
    _6 = SDL_SCANCODE_6,
    _7 = SDL_SCANCODE_7,
    _8 = SDL_SCANCODE_8,
    _9 = SDL_SCANCODE_9,
    _0 = SDL_SCANCODE_0,

    RETURN = SDL_SCANCODE_RETURN,
    ESCAPE = SDL_SCANCODE_ESCAPE,
    BACKSPACE = SDL_SCANCODE_BACKSPACE,
    TAB = SDL_SCANCODE_TAB,
    SPACE = SDL_SCANCODE_SPACE,

    MINUS = SDL_SCANCODE_MINUS,
    EQUALS = SDL_SCANCODE_EQUALS,
    LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
    RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
    BACKSLASH = SDL_SCANCODE_BACKSLASH,
    NONUSHASH = SDL_SCANCODE_NONUSHASH,
    SEMICOLON = SDL_SCANCODE_SEMICOLON,
    APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    GRAVE = SDL_SCANCODE_GRAVE,
    COMMA = SDL_SCANCODE_COMMA,
    PERIOD = SDL_SCANCODE_PERIOD,
    SLASH = SDL_SCANCODE_SLASH,

    CAPSLOCK = SDL_SCANCODE_CAPSLOCK,

    F1 = SDL_SCANCODE_F1,
    F2 = SDL_SCANCODE_F2,
    F3 = SDL_SCANCODE_F3,
    F4 = SDL_SCANCODE_F4,
    F5 = SDL_SCANCODE_F5,
    F6 = SDL_SCANCODE_F6,
    F7 = SDL_SCANCODE_F7,
    F8 = SDL_SCANCODE_F8,
    F9 = SDL_SCANCODE_F9,
    F10 = SDL_SCANCODE_F10,
    F11 = SDL_SCANCODE_F11,
    F12 = SDL_SCANCODE_F12,

    PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
    SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
    PAUSE = SDL_SCANCODE_PAUSE,
    INSERT = SDL_SCANCODE_INSERT,
    HOME = SDL_SCANCODE_HOME,
    PAGEUP = SDL_SCANCODE_PAGEUP,
#undef DELETE // TODO
    DELETE = SDL_SCANCODE_DELETE,
    END = SDL_SCANCODE_END,
    PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
    RIGHT = SDL_SCANCODE_RIGHT,
    LEFT = SDL_SCANCODE_LEFT,
    DOWN = SDL_SCANCODE_DOWN,
    UP = SDL_SCANCODE_UP,

    NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR,
    KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
    KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
    KP_MINUS = SDL_SCANCODE_KP_MINUS,
    KP_PLUS = SDL_SCANCODE_KP_PLUS,
    KP_ENTER = SDL_SCANCODE_KP_ENTER,
    KP_1 = SDL_SCANCODE_KP_1,
    KP_2 = SDL_SCANCODE_KP_2,
    KP_3 = SDL_SCANCODE_KP_3,
    KP_4 = SDL_SCANCODE_KP_4,
    KP_5 = SDL_SCANCODE_KP_5,
    KP_6 = SDL_SCANCODE_KP_6,
    KP_7 = SDL_SCANCODE_KP_7,
    KP_8 = SDL_SCANCODE_KP_8,
    KP_9 = SDL_SCANCODE_KP_9,
    KP_0 = SDL_SCANCODE_KP_0,
    KP_PERIOD = SDL_SCANCODE_KP_PERIOD,

    NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH,
    APPLICATION = SDL_SCANCODE_APPLICATION,
    POWER = SDL_SCANCODE_POWER,

    KP_EQUALS = SDL_SCANCODE_KP_EQUALS,

    F13 = SDL_SCANCODE_F13,
    F14 = SDL_SCANCODE_F14,
    F15 = SDL_SCANCODE_F15,
    F16 = SDL_SCANCODE_F16,
    F17 = SDL_SCANCODE_F17,
    F18 = SDL_SCANCODE_F18,
    F19 = SDL_SCANCODE_F19,
    F20 = SDL_SCANCODE_F20,
    F21 = SDL_SCANCODE_F21,
    F22 = SDL_SCANCODE_F22,
    F23 = SDL_SCANCODE_F23,
    F24 = SDL_SCANCODE_F24,

    EXECUTE = SDL_SCANCODE_EXECUTE,
    HELP = SDL_SCANCODE_HELP,
    MENU = SDL_SCANCODE_MENU,
    SELECT = SDL_SCANCODE_SELECT,
    STOP = SDL_SCANCODE_STOP,
    AGAIN = SDL_SCANCODE_AGAIN,
    UNDO = SDL_SCANCODE_UNDO,
    CUT = SDL_SCANCODE_CUT,
    COPY = SDL_SCANCODE_COPY,
    PASTE = SDL_SCANCODE_PASTE,
    FIND = SDL_SCANCODE_FIND,

    MUTE = SDL_SCANCODE_MUTE,
    VOLUMEUP = SDL_SCANCODE_VOLUMEUP,
    VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN,

    KP_COMMA = SDL_SCANCODE_KP_COMMA,
    KP_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400,

    INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1,
    INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2,
    INTERNATIONAL3 = SDL_SCANCODE_INTERNATIONAL3,
    INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4,
    INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5,
    INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6,
    INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7,
    INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8,
    INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9,

    LANG1 = SDL_SCANCODE_LANG1,
    LANG2 = SDL_SCANCODE_LANG2,
    LANG3 = SDL_SCANCODE_LANG3,
    LANG4 = SDL_SCANCODE_LANG4,
    LANG5 = SDL_SCANCODE_LANG5,
    LANG6 = SDL_SCANCODE_LANG6,
    LANG7 = SDL_SCANCODE_LANG7,
    LANG8 = SDL_SCANCODE_LANG8,
    LANG9 = SDL_SCANCODE_LANG9,

    ALTERASE = SDL_SCANCODE_ALTERASE,
    SYSREQ = SDL_SCANCODE_SYSREQ,
    CANCEL = SDL_SCANCODE_CANCEL,
    CLEAR = SDL_SCANCODE_CLEAR,
    PRIOR = SDL_SCANCODE_PRIOR,
    RETURN2 = SDL_SCANCODE_RETURN2,
    SEPARATOR = SDL_SCANCODE_SEPARATOR,
#undef OUT // TODO
    OUT = SDL_SCANCODE_OUT,
    OPER = SDL_SCANCODE_OPER,
    CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN,
    CRSEL = SDL_SCANCODE_CRSEL,
    EXSEL = SDL_SCANCODE_EXSEL,

    KP_00 = SDL_SCANCODE_KP_00,
    KP_000 = SDL_SCANCODE_KP_000,
    THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR,
    DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR,
    CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT,
    CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT,

    KP_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN,
    KP_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN,
    KP_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE,
    KP_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE,
    KP_TAB = SDL_SCANCODE_KP_TAB,
    KP_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE,
    KP_A = SDL_SCANCODE_KP_A,
    KP_B = SDL_SCANCODE_KP_B,
    KP_C = SDL_SCANCODE_KP_C,
    KP_D = SDL_SCANCODE_KP_D,
    KP_E = SDL_SCANCODE_KP_E,
    KP_F = SDL_SCANCODE_KP_F,
    KP_XOR = SDL_SCANCODE_KP_XOR,
    KP_POWER = SDL_SCANCODE_KP_POWER,
    KP_PERCENT = SDL_SCANCODE_KP_PERCENT,
    KP_LESS = SDL_SCANCODE_KP_LESS,
    KP_GREATER = SDL_SCANCODE_KP_GREATER,
    KP_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND,
    KP_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND,
    KP_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR,
    KP_DBLVERTICALBAR = SDL_SCANCODE_KP_DBLVERTICALBAR,
    KP_COLON = SDL_SCANCODE_KP_COLON,
    KP_HASH = SDL_SCANCODE_KP_HASH,
    KP_SPACE = SDL_SCANCODE_KP_SPACE,
    KP_AT = SDL_SCANCODE_KP_AT,
    KP_EXCLAM = SDL_SCANCODE_KP_EXCLAM,
    KP_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE,
    KP_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL,
    KP_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR,
    KP_MEMADD = SDL_SCANCODE_KP_MEMADD,
    KP_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT,
    KP_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY,
    KP_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE,
    KP_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS,
    KP_CLEAR = SDL_SCANCODE_KP_CLEAR,
    KP_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY,
    KP_BINARY = SDL_SCANCODE_KP_BINARY,
    KP_OCTAL = SDL_SCANCODE_KP_OCTAL,
    KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
    KP_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL,

    LCTRL = SDL_SCANCODE_LCTRL,
    LSHIFT = SDL_SCANCODE_LSHIFT,
    LALT = SDL_SCANCODE_LALT,
    LGUI = SDL_SCANCODE_LGUI,
    RCTRL = SDL_SCANCODE_RCTRL,
    RSHIFT = SDL_SCANCODE_RSHIFT,
    RALT = SDL_SCANCODE_RALT,
    RGUI = SDL_SCANCODE_RGUI,

    MODE = SDL_SCANCODE_MODE,

    SLEEP = SDL_SCANCODE_SLEEP,
    WAKE = SDL_SCANCODE_WAKE,
    CHANNEL_INCREMENT = SDL_SCANCODE_CHANNEL_INCREMENT,
    CHANNEL_DECREMENT = SDL_SCANCODE_CHANNEL_DECREMENT,
    MEDIA_PLAY = SDL_SCANCODE_MEDIA_PLAY,
    MEDIA_PAUSE = SDL_SCANCODE_MEDIA_PAUSE,
    MEDIA_RECORD = SDL_SCANCODE_MEDIA_RECORD,
    MEDIA_FAST_FORWARD = SDL_SCANCODE_MEDIA_FAST_FORWARD,
    MEDIA_REWIND = SDL_SCANCODE_MEDIA_REWIND,
    MEDIA_NEXT_TRACK = SDL_SCANCODE_MEDIA_NEXT_TRACK,
    MEDIA_PREVIOUS_TRACK = SDL_SCANCODE_MEDIA_PREVIOUS_TRACK,
    MEDIA_STOP = SDL_SCANCODE_MEDIA_STOP,
    MEDIA_EJECT = SDL_SCANCODE_MEDIA_EJECT,
    MEDIA_PLAY_PAUSE = SDL_SCANCODE_MEDIA_PLAY_PAUSE,
    MEDIA_SELECT = SDL_SCANCODE_MEDIA_SELECT,

    AC_NEW = SDL_SCANCODE_AC_NEW,
    AC_OPEN = SDL_SCANCODE_AC_OPEN,
    AC_CLOSE = SDL_SCANCODE_AC_CLOSE,
    AC_EXIT = SDL_SCANCODE_AC_EXIT,
    AC_SAVE = SDL_SCANCODE_AC_SAVE,
    AC_PRINT = SDL_SCANCODE_AC_PRINT,
    AC_PROPERTIES = SDL_SCANCODE_AC_PROPERTIES,
    AC_SEARCH = SDL_SCANCODE_AC_SEARCH,
    AC_HOME = SDL_SCANCODE_AC_HOME,
    AC_BACK = SDL_SCANCODE_AC_BACK,
    AC_FORWARD = SDL_SCANCODE_AC_FORWARD,
    AC_STOP = SDL_SCANCODE_AC_STOP,
    AC_REFRESH = SDL_SCANCODE_AC_REFRESH,
    AC_BOOKMARKS = SDL_SCANCODE_AC_BOOKMARKS,

    SOFTLEFT = SDL_SCANCODE_SOFTLEFT,
    SOFTRIGHT = SDL_SCANCODE_SOFTRIGHT,
    CALL = SDL_SCANCODE_CALL,
    ENDCALL = SDL_SCANCODE_ENDCALL,

    RESERVED = SDL_SCANCODE_RESERVED,
    COUNT = SDL_SCANCODE_COUNT
};

#define KEY_UNKNOWN                 ScanCode::UKNOWN

#define KEY_A                       ScanCode::A
#define KEY_B                       ScanCode::B
#define KEY_C                       ScanCode::C
#define KEY_D                       ScanCode::D
#define KEY_E                       ScanCode::E
#define KEY_F                       ScanCode::F
#define KEY_G                       ScanCode::G
#define KEY_H                       ScanCode::H
#define KEY_I                       ScanCode::I
#define KEY_J                       ScanCode::J
#define KEY_K                       ScanCode::K
#define KEY_L                       ScanCode::L
#define KEY_M                       ScanCode::M
#define KEY_N                       ScanCode::N
#define KEY_O                       ScanCode::O
#define KEY_P                       ScanCode::P
#define KEY_Q                       ScanCode::Q
#define KEY_R                       ScanCode::R
#define KEY_S                       ScanCode::S
#define KEY_T                       ScanCode::T
#define KEY_U                       ScanCode::U
#define KEY_V                       ScanCode::V
#define KEY_W                       ScanCode::W
#define KEY_X                       ScanCode::X
#define KEY_Y                       ScanCode::Y
#define KEY_Z                       ScanCode::Z

#define KEY_1                       ScanCode::_1
#define KEY_2                       ScanCode::_2
#define KEY_3                       ScanCode::_3
#define KEY_4                       ScanCode::_4
#define KEY_5                       ScanCode::_5
#define KEY_6                       ScanCode::_6
#define KEY_7                       ScanCode::_7
#define KEY_8                       ScanCode::_8
#define KEY_9                       ScanCode::_9
#define KEY_0                       ScanCode::_0

#define KEY_RETURN                  ScanCode::RETURN
#define KEY_ESCAPE                  ScanCode::ESCAPE
#define KEY_BACKSPACE               ScanCode::BACKSPACE
#define KEY_TAB                     ScanCode::TAB
#define KEY_SPACE                   ScanCode::SPACE

#define KEY_MINUS                   ScanCode::MINUS
#define KEY_EQUALS                  ScanCode::EQUALS
#define KEY_LEFTBRACKET             ScanCode::LEFTBRACKET
#define KEY_RIGHTBRACKET            ScanCode::RIGHTBRACKET
#define KEY_BACKSLASH               ScanCode::BACKSLASH
#define KEY_NONUSHASH               ScanCode::NONUSHASH
#define KEY_SEMICOLON               ScanCode::SEMICOLON
#define KEY_APOSTROPHE              ScanCode::APOSTROPHE
#define KEY_GRAVE                   ScanCode::GRAVE
#define KEY_COMMA                   ScanCode::COMMA
#define KEY_PERIOD                  ScanCode::PERIOD
#define KEY_SLASH                   ScanCode::SLASH

#define KEY_CAPSLOCK                ScanCode::CAPSLOCK

#define KEY_F1                      ScanCode::F1
#define KEY_F2                      ScanCode::F2
#define KEY_F3                      ScanCode::F3
#define KEY_F4                      ScanCode::F4
#define KEY_F5                      ScanCode::F5
#define KEY_F6                      ScanCode::F6
#define KEY_F7                      ScanCode::F7
#define KEY_F8                      ScanCode::F8
#define KEY_F9                      ScanCode::F9
#define KEY_F10                     ScanCode::F10
#define KEY_F11                     ScanCode::F11
#define KEY_F12                     ScanCode::F12

#define KEY_PRINTSCREEN             ScanCode::PRINTSCREEN
#define KEY_SCROLLLOCK              ScanCode::SCROLLLOCK
#define KEY_PAUSE                   ScanCode::PAUSE
#define KEY_INSERT                  ScanCode::INSERT
#define KEY_HOME                    ScanCode::HOME
#define KEY_PAGEUP                  ScanCode::PAGEUP
#define KEY_DELETE                  ScanCode::DELETE
#define KEY_END                     ScanCode::END
#define KEY_PAGEDOWN                ScanCode::PAGEDOWN
#define KEY_RIGHT                   ScanCode::RIGHT
#define KEY_LEFT                    ScanCode::LEFT
#define KEY_DOWN                    ScanCode::DOWN
#define KEY_UP                      ScanCode::UP

#define KEY_NUMLOCKCLEAR            ScanCode::NUMLOCKCLEAR
#define KEY_KP_DIVIDE               ScanCode::KP_DIVIDE
#define KEY_KP_MULTIPLY             ScanCode::KP_MULTIPLY
#define KEY_KP_MINUS                ScanCode::KP_MINUS
#define KEY_KP_PLUS                 ScanCode::KP_PLUS
#define KEY_KP_ENTER                ScanCode::KP_ENTER
#define KEY_KP_1                    ScanCode::KP_1
#define KEY_KP_2                    ScanCode::KP_2
#define KEY_KP_3                    ScanCode::KP_3
#define KEY_KP_4                    ScanCode::KP_4
#define KEY_KP_5                    ScanCode::KP_5
#define KEY_KP_6                    ScanCode::KP_6
#define KEY_KP_7                    ScanCode::KP_7
#define KEY_KP_8                    ScanCode::KP_8
#define KEY_KP_9                    ScanCode::KP_9
#define KEY_KP_0                    ScanCode::KP_0
#define KEY_KP_PERIOD               ScanCode::KP_PERIOD

#define KEY_NONUSBACKSLASH          ScanCode::NONUSBACKSLASH
#define KEY_APPLICATION             ScanCode::APPLICATION
#define KEY_POWER                   ScanCode::POWER

#define KEY_KP_EQUALS               ScanCode::KP_EQUALS

#define KEY_F13                     ScanCode::F13
#define KEY_F14                     ScanCode::F14
#define KEY_F15                     ScanCode::F15
#define KEY_F16                     ScanCode::F16
#define KEY_F17                     ScanCode::F17
#define KEY_F18                     ScanCode::F18
#define KEY_F19                     ScanCode::F19
#define KEY_F20                     ScanCode::F20
#define KEY_F21                     ScanCode::F21
#define KEY_F22                     ScanCode::F22
#define KEY_F23                     ScanCode::F23
#define KEY_F24                     ScanCode::F24

#define KEY_EXECUTE                 ScanCode::EXECUTE
#define KEY_HELP                    ScanCode::HELP
#define KEY_MENU                    ScanCode::MENU
#define KEY_SELECT                  ScanCode::SELECT
#define KEY_STOP                    ScanCode::STOP
#define KEY_AGAIN                   ScanCode::AGAIN
#define KEY_UNDO                    ScanCode::UNDO
#define KEY_CUT                     ScanCode::CUT
#define KEY_COPY                    ScanCode::COPY
#define KEY_PASTE                   ScanCode::PASTE
#define KEY_FIND                    ScanCode::FIND

#define KEY_MUTE                    ScanCode::MUTE
#define KEY_VOLUMEUP                ScanCode::VOLUMEUP
#define KEY_VOLUMEDOWN              ScanCode::VOLUMEDOWN

#define KEY_KP_COMMA                ScanCode::KP_COMMA
#define KEY_KP_EQUALSAS400          ScanCode::KP_EQUALSAS400

#define KEY_INTERNATIONAL1          ScanCode::INTERNATIONAL1
#define KEY_INTERNATIONAL2          ScanCode::INTERNATIONAL2
#define KEY_INTERNATIONAL3          ScanCode::INTERNATIONAL3
#define KEY_INTERNATIONAL4          ScanCode::INTERNATIONAL4
#define KEY_INTERNATIONAL5          ScanCode::INTERNATIONAL5
#define KEY_INTERNATIONAL6          ScanCode::INTERNATIONAL6
#define KEY_INTERNATIONAL7          ScanCode::INTERNATIONAL7
#define KEY_INTERNATIONAL8          ScanCode::INTERNATIONAL8
#define KEY_INTERNATIONAL9          ScanCode::INTERNATIONAL9

#define KEY_LANG1                   ScanCode::LANG1
#define KEY_LANG2                   ScanCode::LANG2
#define KEY_LANG3                   ScanCode::LANG3
#define KEY_LANG4                   ScanCode::LANG4
#define KEY_LANG5                   ScanCode::LANG5
#define KEY_LANG6                   ScanCode::LANG6
#define KEY_LANG7                   ScanCode::LANG7
#define KEY_LANG8                   ScanCode::LANG8
#define KEY_LANG9                   ScanCode::LANG9

#define KEY_ALTERASE                ScanCode::ALTERASE
#define KEY_SYSREQ                  ScanCode::SYSREQ
#define KEY_CANCEL                  ScanCode::CANCEL
#define KEY_CLEAR                   ScanCode::CLEAR
#define KEY_PRIOR                   ScanCode::PRIOR
#define KEY_RETURN2                 ScanCode::RETURN2
#define KEY_SEPARATOR               ScanCode::SEPARATOR
#define KEY_OUT                     ScanCode::OUT
#define KEY_OPER                    ScanCode::OPER
#define KEY_CLEARAGAIN              ScanCode::CLEARAGAIN
#define KEY_CRSEL                   ScanCode::CRSEL
#define KEY_EXSEL                   ScanCode::EXSEL

#define KEY_KP_00                   ScanCode::KP_00
#define KEY_KP_000                  ScanCode::KP_000
#define KEY_THOUSANDSSEPARATOR      ScanCode::THOUSANDSSEPARATOR
#define KEY_DECIMALSEPARATOR        ScanCode::DECIMALSEPARATOR
#define KEY_CURRENCYUNIT            ScanCode::CURRENCYUNIT
#define KEY_CURRENCYSUBUNIT         ScanCode::CURRENCYSUBUNIT

#define KEY_KP_LEFTPAREN            ScanCode::KP_LEFTPAREN
#define KEY_KP_RIGHTPAREN           ScanCode::KP_RIGHTPAREN
#define KEY_KP_LEFTBRACE            ScanCode::KP_LEFTBRACE
#define KEY_KP_RIGHTBRACE           ScanCode::KP_RIGHTBRACE
#define KEY_KP_TAB                  ScanCode::KP_TAB
#define KEY_KP_BACKSPACE            ScanCode::KP_BACKSPACE
#define KEY_KP_A                    ScanCode::KP_A
#define KEY_KP_B                    ScanCode::KP_B
#define KEY_KP_C                    ScanCode::KP_C
#define KEY_KP_D                    ScanCode::KP_D
#define KEY_KP_E                    ScanCode::KP_E
#define KEY_KP_F                    ScanCode::KP_F
#define KEY_KP_XOR                  ScanCode::KP_XOR
#define KEY_KP_POWER                ScanCode::KP_POWER
#define KEY_KP_PERCENT              ScanCode::KP_PERCENT
#define KEY_KP_LESS                 ScanCode::KP_LESS
#define KEY_KP_GREATER              ScanCode::KP_GREATER
#define KEY_KP_AMPERSAND            ScanCode::KP_AMPERSAND
#define KEY_KP_DBLAMPERSAND         ScanCode::KP_DBLAMPERSAND
#define KEY_KP_VERTICALBAR          ScanCode::KP_VERTICALBAR
#define KEY_KP_DBLVERTICALBAR       ScanCode::KP_DBLVERTICALBAR
#define KEY_KP_COLON                ScanCode::KP_COLON
#define KEY_KP_HASH                 ScanCode::KP_HASH
#define KEY_KP_SPACE                ScanCode::KP_SPACE
#define KEY_KP_AT                   ScanCode::KP_AT
#define KEY_KP_EXCLAM               ScanCode::KP_EXCLAM
#define KEY_KP_MEMSTORE             ScanCode::KP_MEMSTORE
#define KEY_KP_MEMRECALL            ScanCode::KP_MEMRECALL
#define KEY_KP_MEMCLEAR             ScanCode::KP_MEMCLEAR
#define KEY_KP_MEMADD               ScanCode::KP_MEMADD
#define KEY_KP_MEMSUBTRACT          ScanCode::KP_MEMSUBTRACT
#define KEY_KP_MEMMULTIPLY          ScanCode::KP_MEMMULTIPLY
#define KEY_KP_MEMDIVIDE            ScanCode::KP_MEMDIVIDE
#define KEY_KP_PLUSMINUS            ScanCode::KP_PLUSMINUS
#define KEY_KP_CLEAR                ScanCode::KP_CLEAR
#define KEY_KP_CLEARENTRY           ScanCode::KP_CLEARENTRY
#define KEY_KP_BINARY               ScanCode::KP_BINARY
#define KEY_KP_OCTAL                ScanCode::KP_OCTAL
#define KEY_KP_DECIMAL              ScanCode::KP_DECIMAL
#define KEY_KP_HEXADECIMAL          ScanCode::KP_HEXADECIMAL

#define KEY_LCTRL                   ScanCode::LCTRL
#define KEY_LSHIFT                  ScanCode::LSHIFT
#define KEY_LALT                    ScanCode::LALT
#define KEY_LGUI                    ScanCode::LGUI
#define KEY_RCTRL                   ScanCode::RCTRL
#define KEY_RSHIFT                  ScanCode::RSHIFT
#define KEY_RALT                    ScanCode::RALT
#define KEY_RGUI                    ScanCode::RGUI

#define KEY_MODE                    ScanCode::MODE

#define KEY_SLEEP                   ScanCode::SLEEP
#define KEY_WAKE                    ScanCode::WAKE
#define KEY_CHANNEL_INCREMENT       ScanCode::CHANNEL_INCREMENT
#define KEY_CHANNEL_DECREMENT       ScanCode::CHANNEL_DECREMENT
#define KEY_MEDIA_PLAY              ScanCode::MEDIA_PLAY
#define KEY_MEDIA_PAUSE             ScanCode::MEDIA_PAUSE
#define KEY_MEDIA_RECORD            ScanCode::MEDIA_RECORD
#define KEY_MEDIA_FAST_FORWARD      ScanCode::MEDIA_FAST_FORWARD
#define KEY_MEDIA_REWIND            ScanCode::MEDIA_REWIND
#define KEY_MEDIA_NEXT_TRACK        ScanCode::MEDIA_NEXT_TRACK
#define KEY_MEDIA_PREVIOUS_TRACK    ScanCode::MEDIA_PREVIOUS_TRACK
#define KEY_MEDIA_STOP              ScanCode::MEDIA_STOP
#define KEY_MEDIA_EJECT             ScanCode::MEDIA_EJECT
#define KEY_MEDIA_PLAY_PAUSE        ScanCode::MEDIA_PLAY_PAUSE
#define KEY_MEDIA_SELECT            ScanCode::MEDIA_SELECT

#define KEY_AC_NEW                  ScanCode::AC_NEW
#define KEY_AC_OPEN                 ScanCode::AC_OPEN
#define KEY_AC_CLOSE                ScanCode::AC_CLOSE
#define KEY_AC_EXIT                 ScanCode::AC_EXIT
#define KEY_AC_SAVE                 ScanCode::AC_SAVE
#define KEY_AC_PRINT                ScanCode::AC_PRINT
#define KEY_AC_PROPERTIES           ScanCode::AC_PROPERTIES
#define KEY_AC_SEARCH               ScanCode::AC_SEARCH
#define KEY_AC_HOME                 ScanCode::AC_HOME
#define KEY_AC_BACK                 ScanCode::AC_BACK
#define KEY_AC_FORWARD              ScanCode::AC_FORWARD
#define KEY_AC_STOP                 ScanCode::AC_STOP
#define KEY_AC_REFRESH              ScanCode::AC_REFRESH
#define KEY_AC_BOOKMARKS            ScanCode::AC_BOOKMARKS

#define KEY_SOFTLEFT                ScanCode::SOFTLEFT
#define KEY_SOFTRIGHT               ScanCode::SOFTRIGHT
#define KEY_CALL                    ScanCode::CALL
#define KEY_ENDCALL                 ScanCode::ENDCALL

#define KEY_RESERVED                ScanCode::RESERVED
#define KEY_COUNT                   ScanCode::COUNT

}
