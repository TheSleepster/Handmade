#include "Sugar_Input.h"
#include "Sugar_Intrinsics.h"

internal void
Win32LoadKeyData() 
{ 
    GameInput->KeyCodes[VK_LBUTTON] = KEY_MOUSE_LEFT;
    GameInput->KeyCodes[VK_MBUTTON] = KEY_MOUSE_MIDDLE;
    GameInput->KeyCodes[VK_RBUTTON] = KEY_MOUSE_RIGHT;

    GameInput->KeyCodes['A'] = KEY_A;
    GameInput->KeyCodes['B'] = KEY_B;
    GameInput->KeyCodes['C'] = KEY_C;
    GameInput->KeyCodes['D'] = KEY_D;
    GameInput->KeyCodes['E'] = KEY_E;
    GameInput->KeyCodes['F'] = KEY_F;
    GameInput->KeyCodes['G'] = KEY_G;
    GameInput->KeyCodes['H'] = KEY_H;
    GameInput->KeyCodes['I'] = KEY_I;
    GameInput->KeyCodes['J'] = KEY_J;
    GameInput->KeyCodes['K'] = KEY_K;
    GameInput->KeyCodes['L'] = KEY_L;
    GameInput->KeyCodes['M'] = KEY_M;
    GameInput->KeyCodes['N'] = KEY_N;
    GameInput->KeyCodes['O'] = KEY_O;
    GameInput->KeyCodes['P'] = KEY_P;
    GameInput->KeyCodes['Q'] = KEY_Q;
    GameInput->KeyCodes['R'] = KEY_R;
    GameInput->KeyCodes['S'] = KEY_S;
    GameInput->KeyCodes['T'] = KEY_T;
    GameInput->KeyCodes['U'] = KEY_U;
    GameInput->KeyCodes['V'] = KEY_V;
    GameInput->KeyCodes['W'] = KEY_W;
    GameInput->KeyCodes['X'] = KEY_X;
    GameInput->KeyCodes['Y'] = KEY_Y;
    GameInput->KeyCodes['Z'] = KEY_Z;
    GameInput->KeyCodes['0'] = KEY_0;
    GameInput->KeyCodes['1'] = KEY_1;
    GameInput->KeyCodes['2'] = KEY_2;
    GameInput->KeyCodes['3'] = KEY_3;
    GameInput->KeyCodes['4'] = KEY_4;
    GameInput->KeyCodes['5'] = KEY_5;
    GameInput->KeyCodes['6'] = KEY_6;
    GameInput->KeyCodes['7'] = KEY_7;
    GameInput->KeyCodes['8'] = KEY_8;
    GameInput->KeyCodes['9'] = KEY_9;

    GameInput->KeyCodes[VK_SPACE] = KEY_SPACE,
    GameInput->KeyCodes[VK_OEM_3] = KEY_TICK,
    GameInput->KeyCodes[VK_OEM_MINUS] = KEY_MINUS,

    GameInput->KeyCodes[VK_OEM_PLUS] = KEY_EQUAL,
    GameInput->KeyCodes[VK_OEM_4] = KEY_LEFT_BRACKET,
    GameInput->KeyCodes[VK_OEM_6] = KEY_RIGHT_BRACKET,
    GameInput->KeyCodes[VK_OEM_1] = KEY_SEMICOLON,
    GameInput->KeyCodes[VK_OEM_7] = KEY_QUOTE,
    GameInput->KeyCodes[VK_OEM_COMMA] = KEY_COMMA,
    GameInput->KeyCodes[VK_OEM_PERIOD] = KEY_PERIOD,
    GameInput->KeyCodes[VK_OEM_2] = KEY_FORWARD_SLASH,
    GameInput->KeyCodes[VK_OEM_5] = KEY_BACKWARD_SLASH,
    GameInput->KeyCodes[VK_TAB] = KEY_TAB,
    GameInput->KeyCodes[VK_ESCAPE] = KEY_ESCAPE,
    GameInput->KeyCodes[VK_PAUSE] = KEY_PAUSE,
    GameInput->KeyCodes[VK_UP] = KEY_UP,
    GameInput->KeyCodes[VK_DOWN] = KEY_DOWN,
    GameInput->KeyCodes[VK_LEFT] = KEY_LEFT,
    GameInput->KeyCodes[VK_RIGHT] = KEY_RIGHT,
    GameInput->KeyCodes[VK_BACK] = KEY_BACKSPACE,
    GameInput->KeyCodes[VK_RETURN] = KEY_RETURN,
    GameInput->KeyCodes[VK_DELETE] = KEY_DELETE,
    GameInput->KeyCodes[VK_INSERT] = KEY_INSERT,
    GameInput->KeyCodes[VK_HOME] = KEY_HOME,
    GameInput->KeyCodes[VK_END] = KEY_END,
    GameInput->KeyCodes[VK_PRIOR] = KEY_PAGE_UP,
    GameInput->KeyCodes[VK_NEXT] = KEY_PAGE_DOWN,
    GameInput->KeyCodes[VK_CAPITAL] = KEY_CAPS_LOCK,
    GameInput->KeyCodes[VK_NUMLOCK] = KEY_NUM_LOCK,
    GameInput->KeyCodes[VK_SCROLL] = KEY_SCROLL_LOCK,
    GameInput->KeyCodes[VK_APPS] = KEY_MENU,

    GameInput->KeyCodes[VK_SHIFT] = KEY_SHIFT,
    GameInput->KeyCodes[VK_LSHIFT] = KEY_SHIFT,
    GameInput->KeyCodes[VK_RSHIFT] = KEY_SHIFT,

    GameInput->KeyCodes[VK_CONTROL] = KEY_CONTROL,
    GameInput->KeyCodes[VK_LCONTROL] = KEY_CONTROL,
    GameInput->KeyCodes[VK_RCONTROL] = KEY_CONTROL,

    GameInput->KeyCodes[VK_MENU] = KEY_ALT,
    GameInput->KeyCodes[VK_LMENU] = KEY_ALT,
    GameInput->KeyCodes[VK_RMENU] = KEY_ALT,

    GameInput->KeyCodes[VK_F1] = KEY_F1;
    GameInput->KeyCodes[VK_F2] = KEY_F2;
    GameInput->KeyCodes[VK_F3] = KEY_F3;
    GameInput->KeyCodes[VK_F4] = KEY_F4;
    GameInput->KeyCodes[VK_F5] = KEY_F5;
    GameInput->KeyCodes[VK_F6] = KEY_F6;
    GameInput->KeyCodes[VK_F7] = KEY_F7;
    GameInput->KeyCodes[VK_F8] = KEY_F8;
    GameInput->KeyCodes[VK_F9] = KEY_F9;
    GameInput->KeyCodes[VK_F10] = KEY_F10;
    GameInput->KeyCodes[VK_F11] = KEY_F11;
    GameInput->KeyCodes[VK_F12] = KEY_F12;

    GameInput->KeyCodes[VK_NUMPAD0] = KEY_NUMPAD_0;
    GameInput->KeyCodes[VK_NUMPAD1] = KEY_NUMPAD_1;
    GameInput->KeyCodes[VK_NUMPAD2] = KEY_NUMPAD_2;
    GameInput->KeyCodes[VK_NUMPAD3] = KEY_NUMPAD_3;
    GameInput->KeyCodes[VK_NUMPAD4] = KEY_NUMPAD_4;
    GameInput->KeyCodes[VK_NUMPAD5] = KEY_NUMPAD_5;
    GameInput->KeyCodes[VK_NUMPAD6] = KEY_NUMPAD_6;
    GameInput->KeyCodes[VK_NUMPAD7] = KEY_NUMPAD_7;
    GameInput->KeyCodes[VK_NUMPAD8] = KEY_NUMPAD_8;
    GameInput->KeyCodes[VK_NUMPAD9] = KEY_NUMPAD_9;
}
