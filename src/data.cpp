#include "qnamespace.h"
#include "usb_hid_keys.h"
#include <cstdint>
#include <QList>
#include <QMap>
class DataReference {
public:
    inline static QMap<int, uint16_t> ui_to_special_action {
        {0, 0x00ea},
        {1, 0x00e9},
        {2, 0x0223},
        {3, 0x0183},
        {4, 0x018a},
        {5, 0x00b6},
        {6, 0x00b5},
        {7, 0x00e2},
        {8, 0x00cd},
        {9, 0x0192}
    };

    inline static QMap<Qt::Key, unsigned char> qt_key_to_hid_keycode {
        {Qt::Key_0, KEY_0},
        {Qt::Key_1, KEY_1},
        {Qt::Key_2, KEY_2},
        {Qt::Key_3, KEY_3},
        {Qt::Key_4, KEY_4},
        {Qt::Key_5, KEY_5},
        {Qt::Key_6, KEY_6},
        {Qt::Key_7, KEY_7},
        {Qt::Key_8, KEY_8},
        {Qt::Key_9, KEY_9},
        {Qt::Key_A, KEY_A},
        {Qt::Key_B, KEY_B},
        {Qt::Key_C, KEY_C},
        {Qt::Key_D, KEY_D},
        {Qt::Key_E, KEY_E},
        {Qt::Key_F, KEY_F},
        {Qt::Key_G, KEY_G},
        {Qt::Key_H, KEY_H},
        {Qt::Key_I, KEY_I},
        {Qt::Key_J, KEY_J},
        {Qt::Key_K, KEY_K},
        {Qt::Key_L, KEY_L},
        {Qt::Key_M, KEY_M},
        {Qt::Key_N, KEY_N},
        {Qt::Key_O, KEY_O},
        {Qt::Key_P, KEY_P},
        {Qt::Key_Q, KEY_Q},
        {Qt::Key_R, KEY_R},
        {Qt::Key_S, KEY_S},
        {Qt::Key_T, KEY_T},
        {Qt::Key_U, KEY_U},
        {Qt::Key_V, KEY_V},
        {Qt::Key_W, KEY_W},
        {Qt::Key_X, KEY_X},
        {Qt::Key_Y, KEY_Y},
        {Qt::Key_Z, KEY_Z},
        {Qt::Key_Control, KEY_LEFTCTRL},
        {Qt::Key_Alt, KEY_LEFTALT},
        {Qt::Key_AltGr, KEY_RIGHTALT},
        {Qt::Key_Escape, KEY_ESC},
        {Qt::Key_Backslash, KEY_BACKSLASH},
        {Qt::Key_Backspace, KEY_BACKSPACE},
        {Qt::Key_Return, KEY_ENTER},
        {Qt::Key_Insert, KEY_INSERT},
        {Qt::Key_Delete, KEY_DELETE},
        {Qt::Key_Pause, KEY_PAUSE},
        {Qt::Key_Print, KEY_SYSRQ},
        {Qt::Key_Home, KEY_HOME},
        {Qt::Key_End, KEY_END},
        {Qt::Key_Left, KEY_LEFT},
        {Qt::Key_Up, KEY_UP},
        {Qt::Key_Right, KEY_RIGHT},
        {Qt::Key_Down, KEY_DOWN},
        {Qt::Key_PageUp, KEY_PAGEUP},
        {Qt::Key_PageDown, KEY_PAGEDOWN},
        {Qt::Key_Shift, KEY_LEFTSHIFT},
        {Qt::Key_CapsLock, KEY_CAPSLOCK},
        {Qt::Key_NumLock, KEY_NUMLOCK},
        {Qt::Key_ScrollLock, KEY_SCROLLLOCK},
        {Qt::Key_F1, KEY_F1},
        {Qt::Key_F2, KEY_F2},
        {Qt::Key_F3, KEY_F3},
        {Qt::Key_F4, KEY_F4},
        {Qt::Key_F5, KEY_F5},
        {Qt::Key_F6, KEY_F6},
        {Qt::Key_F7, KEY_F7},
        {Qt::Key_F8, KEY_F8},
        {Qt::Key_F9, KEY_F9},
        {Qt::Key_F10, KEY_F10},
        {Qt::Key_F11, KEY_F11},
        {Qt::Key_F12, KEY_F12},
        {Qt::Key_F13, KEY_F13},
        {Qt::Key_F14, KEY_F14},
        {Qt::Key_F15, KEY_F15},
        {Qt::Key_F16, KEY_F16},
        {Qt::Key_F17, KEY_F17},
        {Qt::Key_F18, KEY_F18},
        {Qt::Key_F19, KEY_F19},
        {Qt::Key_F20, KEY_F20},
        {Qt::Key_F21, KEY_F21},
        {Qt::Key_F22, KEY_F22},
        {Qt::Key_F23, KEY_F23},
        {Qt::Key_F24, KEY_F24},
        {Qt::Key_Super_L, KEY_LEFTMETA},
        {Qt::Key_Super_R, KEY_RIGHTMETA},
        {Qt::Key_Menu, KEY_COMPOSE},
        {Qt::Key_Space, KEY_SPACE},
        {Qt::Key_Equal, KEY_EQUAL},
        {Qt::Key_Plus, KEY_KPPLUS},
        {Qt::Key_Minus, KEY_MINUS},
        {Qt::Key_Semicolon, KEY_SEMICOLON},
        {Qt::Key_Comma, KEY_COMMA},
        {Qt::Key_Period, KEY_DOT},
        {Qt::Key_BracketLeft, KEY_LEFTBRACE},
        {Qt::Key_BracketRight, KEY_RIGHTBRACE},
        {Qt::Key_Tab, KEY_TAB}
    };

    static QList<unsigned char> getModifiers(Qt::KeyboardModifiers modifier) {
        QList<unsigned char> list = {};
        if(modifier.testFlag(Qt::KeyboardModifier::ControlModifier)) {
            list.append(KEY_LEFTCTRL);
        }
        if(modifier.testFlag(Qt::KeyboardModifier::AltModifier)) {
            list.append(KEY_LEFTALT);
        }
        if(modifier.testFlag(Qt::KeyboardModifier::MetaModifier)) {
            list.append(KEY_LEFTMETA);
        }
        if(modifier.testFlag(Qt::KeyboardModifier::ShiftModifier)) {
            list.append(KEY_LEFTSHIFT);
        }
        return list;
    }
};
