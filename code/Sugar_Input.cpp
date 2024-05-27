#include "Sugar_Input.h"
#include "Sugar_Intrinsics.h"

internal void 
Win32LoadKeyData() 
{ 
    KeyCodeLookup[VK_LBUTTON] = KEY_MOUSE_LEFT;
    KeyCodeLookup[VK_MBUTTON] = KEY_MOUSE_MIDDLE;
    KeyCodeLookup[VK_RBUTTON] = KEY_MOUSE_RIGHT;

    KeyCodeLookup['A'] = KEY_A;
    KeyCodeLookup['B'] = KEY_B;
    KeyCodeLookup['C'] = KEY_C;
    KeyCodeLookup['D'] = KEY_D;
    KeyCodeLookup['E'] = KEY_E;
    KeyCodeLookup['F'] = KEY_F;
    KeyCodeLookup['G'] = KEY_G;
    KeyCodeLookup['H'] = KEY_H;
    KeyCodeLookup['I'] = KEY_I;
    KeyCodeLookup['J'] = KEY_J;
    KeyCodeLookup['K'] = KEY_K;
    KeyCodeLookup['L'] = KEY_L;
    KeyCodeLookup['M'] = KEY_M;
    KeyCodeLookup['N'] = KEY_N;
    KeyCodeLookup['O'] = KEY_O;
    KeyCodeLookup['P'] = KEY_P;
    KeyCodeLookup['Q'] = KEY_Q;
    KeyCodeLookup['R'] = KEY_R;
    KeyCodeLookup['S'] = KEY_S;
    KeyCodeLookup['T'] = KEY_T;
    KeyCodeLookup['U'] = KEY_U;
    KeyCodeLookup['V'] = KEY_V;
    KeyCodeLookup['W'] = KEY_W;
    KeyCodeLookup['X'] = KEY_X;
    KeyCodeLookup['Y'] = KEY_Y;
    KeyCodeLookup['Z'] = KEY_Z;
    KeyCodeLookup['0'] = KEY_0;
    KeyCodeLookup['1'] = KEY_1;
    KeyCodeLookup['2'] = KEY_2;
    KeyCodeLookup['3'] = KEY_3;
    KeyCodeLookup['4'] = KEY_4;
    KeyCodeLookup['5'] = KEY_5;
    KeyCodeLookup['6'] = KEY_6;
    KeyCodeLookup['7'] = KEY_7;
    KeyCodeLookup['8'] = KEY_8;
    KeyCodeLookup['9'] = KEY_9;

    KeyCodeLookup[VK_SPACE] = KEY_SPACE,
    KeyCodeLookup[VK_OEM_3] = KEY_TICK,
    KeyCodeLookup[VK_OEM_MINUS] = KEY_MINUS,

    KeyCodeLookup[VK_OEM_PLUS] = KEY_EQUAL,
    KeyCodeLookup[VK_OEM_4] = KEY_LEFT_BRACKET,
    KeyCodeLookup[VK_OEM_6] = KEY_RIGHT_BRACKET,
    KeyCodeLookup[VK_OEM_1] = KEY_SEMICOLON,
    KeyCodeLookup[VK_OEM_7] = KEY_QUOTE,
    KeyCodeLookup[VK_OEM_COMMA] = KEY_COMMA,
    KeyCodeLookup[VK_OEM_PERIOD] = KEY_PERIOD,
    KeyCodeLookup[VK_OEM_2] = KEY_FORWARD_SLASH,
    KeyCodeLookup[VK_OEM_5] = KEY_BACKWARD_SLASH,
    KeyCodeLookup[VK_TAB] = KEY_TAB,
    KeyCodeLookup[VK_ESCAPE] = KEY_ESCAPE,
    KeyCodeLookup[VK_PAUSE] = KEY_PAUSE,
    KeyCodeLookup[VK_UP] = KEY_UP,
    KeyCodeLookup[VK_DOWN] = KEY_DOWN,
    KeyCodeLookup[VK_LEFT] = KEY_LEFT,
    KeyCodeLookup[VK_RIGHT] = KEY_RIGHT,
    KeyCodeLookup[VK_BACK] = KEY_BACKSPACE,
    KeyCodeLookup[VK_RETURN] = KEY_RETURN,
    KeyCodeLookup[VK_DELETE] = KEY_DELETE,
    KeyCodeLookup[VK_INSERT] = KEY_INSERT,
    KeyCodeLookup[VK_HOME] = KEY_HOME,
    KeyCodeLookup[VK_END] = KEY_END,
    KeyCodeLookup[VK_PRIOR] = KEY_PAGE_UP,
    KeyCodeLookup[VK_NEXT] = KEY_PAGE_DOWN,
    KeyCodeLookup[VK_CAPITAL] = KEY_CAPS_LOCK,
    KeyCodeLookup[VK_NUMLOCK] = KEY_NUM_LOCK,
    KeyCodeLookup[VK_SCROLL] = KEY_SCROLL_LOCK,
    KeyCodeLookup[VK_APPS] = KEY_MENU,

    KeyCodeLookup[VK_SHIFT] = KEY_SHIFT,
    KeyCodeLookup[VK_LSHIFT] = KEY_SHIFT,
    KeyCodeLookup[VK_RSHIFT] = KEY_SHIFT,

    KeyCodeLookup[VK_CONTROL] = KEY_CONTROL,
    KeyCodeLookup[VK_LCONTROL] = KEY_CONTROL,
    KeyCodeLookup[VK_RCONTROL] = KEY_CONTROL,

    KeyCodeLookup[VK_MENU] = KEY_ALT,
    KeyCodeLookup[VK_LMENU] = KEY_ALT,
    KeyCodeLookup[VK_RMENU] = KEY_ALT,

    KeyCodeLookup[VK_F1] = KEY_F1;
    KeyCodeLookup[VK_F2] = KEY_F2;
    KeyCodeLookup[VK_F3] = KEY_F3;
    KeyCodeLookup[VK_F4] = KEY_F4;
    KeyCodeLookup[VK_F5] = KEY_F5;
    KeyCodeLookup[VK_F6] = KEY_F6;
    KeyCodeLookup[VK_F7] = KEY_F7;
    KeyCodeLookup[VK_F8] = KEY_F8;
    KeyCodeLookup[VK_F9] = KEY_F9;
    KeyCodeLookup[VK_F10] = KEY_F10;
    KeyCodeLookup[VK_F11] = KEY_F11;
    KeyCodeLookup[VK_F12] = KEY_F12;

    KeyCodeLookup[VK_NUMPAD0] = KEY_NUMPAD_0;
    KeyCodeLookup[VK_NUMPAD1] = KEY_NUMPAD_1;
    KeyCodeLookup[VK_NUMPAD2] = KEY_NUMPAD_2;
    KeyCodeLookup[VK_NUMPAD3] = KEY_NUMPAD_3;
    KeyCodeLookup[VK_NUMPAD4] = KEY_NUMPAD_4;
    KeyCodeLookup[VK_NUMPAD5] = KEY_NUMPAD_5;
    KeyCodeLookup[VK_NUMPAD6] = KEY_NUMPAD_6;
    KeyCodeLookup[VK_NUMPAD7] = KEY_NUMPAD_7;
    KeyCodeLookup[VK_NUMPAD8] = KEY_NUMPAD_8;
    KeyCodeLookup[VK_NUMPAD9] = KEY_NUMPAD_9;
}

// EVENTUAL XINPUT STUFF MAYBE?
/*
*/
