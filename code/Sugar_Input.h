#pragma once

#include "Sugar_Intrinsics.h"
#include "SugarAPI.h"
#include <xinput.h>

enum KeyCodeID 
{ 
    KEY_MOUSE_LEFT,
    KEY_MOUSE_MIDDLE,
    KEY_MOUSE_RIGHT,

    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
    KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
    KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

    KEY_SPACE,
    KEY_TICK,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_FORWARD_SLASH,
    KEY_BACKWARD_SLASH,
    KEY_TAB,
    KEY_ESCAPE,
    KEY_PAUSE,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_BACKSPACE,
    KEY_RETURN,
    KEY_DELETE,
    KEY_INSERT,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_CAPS_LOCK,
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_MENU,
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,
    KEY_COMMAND,

    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, 
    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,

    KEY_NUMPAD_STAR,
    KEY_NUMPAD_PLUS,
    KEY_NUMPAD_MINUS,
    KEY_NUMPAD_DOT,
    KEY_NUMPAD_SLASH,

    KEY_COUNT = 255,
};

struct ControllerButton 
{
    uint8 HalfTransitionCount;
    bool IsDown;
};

struct ControllerInput 
{
    // STICKS 

    bool IsAnalog;
    vec2 Start;
    vec2 End;
    vec2 Min;
    vec2 Max;

    // BUTTONS 
    union 
    {
        ControllerButton Buttons[15];
        struct 
        {
            ControllerButton Y;
            ControllerButton A;
            ControllerButton X;
            ControllerButton B;
            ControllerButton LeftShoulder;
            ControllerButton RightShoulder;
            ControllerButton LeftTrigger;
            ControllerButton RightTrigger;
            ControllerButton DPadUp;
            ControllerButton DPadDown;
            ControllerButton DPadLeft;
            ControllerButton DPadRight;
            ControllerButton StartButton;
            ControllerButton Menu;
            ControllerButton Home;
        };
    };
};

struct Key 
{
    bool JustPressed;
    bool JustReleased;
    bool IsDown;
    uint8 HalfTransitionCount;
};

enum KeyBindings 
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,

    ATTACK,

    BINDING_COUNT,
};

struct Keymap 
{
    KeyCodeID Key;
};

struct KeyboardInput 
{
    ivec2 LastMouse;
    ivec2 CurrentMouse;
    ivec2 RelMouse;

    Key Keys[KEY_COUNT];
    Keymap Bindings[BINDING_COUNT];
};

struct Input 
{
    ControllerInput Controller;
    KeyboardInput Keyboard;
};

inline bool
IsKeyDown(KeyCodeID KeyCode, Input *GameInput) 
{
    Key Key = GameInput->Keyboard.Keys[KeyCode];
    if(Key.IsDown) 
    {
        return(true);
    }
    return(false);
}

inline bool
IsGameKeyDown(KeyBindings InputType, Input *GameInput) 
{
    KeyCodeID Keycode = GameInput->Keyboard.Bindings[InputType].Key;
    Key Key = GameInput->Keyboard.Keys[Keycode];
    if(Key.IsDown) 
    {
        return(true);
    }
    return(false);
}


global_variable KeyCodeID KeyCodeLookup[KEY_COUNT]; 

#define XINPUT_SET_STATE(name) DWORD name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef XINPUT_SET_STATE(xinput_set_state);
XINPUT_SET_STATE(XInputSetStateStub) 
{
    return(true);
}

#define XINPUT_GET_STATE(name) DWORD name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef XINPUT_GET_STATE(xinput_get_state);
XINPUT_GET_STATE(XInputGetStateStub) 
{
    return(true);
}
