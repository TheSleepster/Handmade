/*
    This is not a final layer. Additional Requirements: 

    COMPLETE
    - GameInput (Keyboard)
    - Keymapping?
    - DeltaTime
    - GameInput(XInput)


    TODO 
    - Audio (DirectSound?, SokolAudio?, Or XAudio?)
    - Audio Formats (.WAV exclusively?)
    - Asset Loading (Maybe defer this to the renderer? After all it is the renderer that uses them.)
    - Fullscreen
    - Multithreading
    - File Saving
    - Game Saving
    - Sleep/Inactivity Period
    - GetKeyboardLayout() (For non-standard QWERTY keyboards)
    - Raw Input (For multiple inputs)
    - WM_ACTIVATEAPP (For being the inactive window)
    - ClipCursor(Multi Monitor)
    - Upload with WebAssembly
*/


#include "../data/deps/OpenGL/GLL.h"

#include "Sugar_Intrinsics.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"
#include "Sugar_ECS.h"

#define NOMINMAX
#include <windows.h>
#include <xinput.h>

#include "Sugar_OpenGLRenderer.cpp"
#include "Sugar_Input.cpp"
#include "Sugar_Physics.cpp"


// For release builds Sugar.cpp will be a part of the main file. We need to make sure that the functions are
// called correctly from the CPP file instead of from pointers in this configuration.
#ifndef SUGAR_SLOW
#include "Sugar.cpp"
#endif

global_variable bool GlobalRunning = {};
global_variable int ClientWidth = {};
global_variable int ClientHeight = {}; 


#define SIMRATE ((1.0f/60.0f) * 1000.0f)


#ifdef SUGAR_SLOW
internal Win32GameCode
Win32LoadGamecode(char *SourceDLLName) 
{ 
    Win32GameCode Result = {};

    char *TempDLLName = "GamecodeTemp.dll";
    Result.DLLLastWriteTime = Win32GetLastWriteTime(SourceDLLName);

    Result.IsLoaded = false;
    while(!Result.IsLoaded) 
    {
        CopyFile(SourceDLLName, TempDLLName, FALSE);
        Result.IsLoaded = true;
    }
    Result.GameCodeDLL = LoadLibraryA(TempDLLName);

    if(Result.GameCodeDLL) 
    {
        Result.UpdateAndRender = 
            (game_update_and_render *)GetProcAddress(Result.GameCodeDLL, "GameUpdateAndRender"); 
        Result.InitData = 
            (init_game_data *)GetProcAddress(Result.GameCodeDLL, "InitGameData");

        Result.IsValid = (Result.UpdateAndRender && Result.InitData);
    }
    if(!Result.IsValid) 
    {
        Result.UpdateAndRender = GameUpdateAndRenderStub;
        Result.InitData = InitGameDataStub;
    }
    Sleep(100);
    return(Result);
}

internal void 
Win32UnloadGamecode(Win32GameCode *Gamecode) 
{
    if(Gamecode->GameCodeDLL) 
    {
        FreeLibrary(Gamecode->GameCodeDLL);
        Gamecode->GameCodeDLL = 0;
    }
    Gamecode->IsValid = false;
    Gamecode->IsLoaded = false;
    Gamecode->UpdateAndRender = GameUpdateAndRenderStub;
    Gamecode->InitData = InitGameDataStub;
}

#endif

internal XInputPointers 
Win32LoadXInput(char *Filename) 
{
    XInputPointers Result = {};
    Result.XInputDLL = LoadLibraryA(Filename);

    if(Result.XInputDLL) 
    {
        Result.GetState = 
            (xinput_get_state *)GetProcAddress(Result.XInputDLL, "XInputGetState");
        Result.SetState = 
            (xinput_set_state *)GetProcAddress(Result.XInputDLL, "XInputSetState");
    }
    else 
    {
        Result.GetState = XInputGetStateStub;
        Result.SetState = XInputSetStateStub;
    }
    return(Result);
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND    hWnd,
                        UINT    Message,
                        WPARAM  wParam,
                        LPARAM  lParam) 
{
    LRESULT Result = 0;
    switch(Message) 
    {
        case WM_CLOSE: 
        {
            GlobalRunning = false;
        }break;
        case WM_DESTROY: 
        {
            DestroyWindow(hWnd);
        }break;
        case WM_SIZE: 
        {
            RECT Rect = {};
            GetClientRect(hWnd, &Rect);
            ClientWidth = Rect.right - Rect.left;
            ClientHeight = Rect.bottom - Rect.top;
        }break;
        default: 
        { 
            Result = DefWindowProc(hWnd, Message, wParam, lParam);
        }break;
    }
    return(Result);
}

internal void
Win32InitializeOpenGL(HINSTANCE hInstance, WNDCLASS Window, Win32OpenGLFunctions *Win32OpenGL) 
{
    HWND WindowHandle = 
        CreateWindow(Window.lpszClassName,
                "Sugar",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                hInstance,
                0);
    HDC WindowDC = GetDC(WindowHandle);

    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
    DesiredPixelFormat.nVersion = 1;
    DesiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.cColorBits = 32;
    DesiredPixelFormat.cAlphaBits = 8;
    DesiredPixelFormat.cDepthBits = 24;
    
    int PixelFormat = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
    DescribePixelFormat(WindowDC, PixelFormat, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
    SetPixelFormat(WindowDC, PixelFormat, &SuggestedPixelFormat);

    HGLRC TempRC = wglCreateContext(WindowDC);
    wglMakeCurrent(WindowDC, TempRC);

// NOTE : Loading wgl specific OpenGL functions
    Win32OpenGL->wglChoosePixelFormatARB = 
        (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    Win32OpenGL->wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    Win32OpenGL->wglSwapIntervalEXT =
        (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    if(!Win32OpenGL->wglCreateContextAttribsARB||
       !Win32OpenGL->wglChoosePixelFormatARB||
       !Win32OpenGL->wglSwapIntervalEXT) 
    {
        MessageBoxA(0, "Failed to load WGL Functions!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
    } 

    if(!Win32OpenGL->wglCreateContextAttribsARB||
       !Win32OpenGL->wglChoosePixelFormatARB) 
    {
        MessageBoxA(0, "Failed to manually load WGL functions!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE);
    }
    wglMakeCurrent(WindowDC, 0);
    wglDeleteContext(TempRC);
    DestroyWindow(WindowHandle);
}

internal void 
Win32ProcessInputMessages(MSG Message, 
                          HWND WindowHandle, 
                          Input *GameInput, 
                          GameState *State) 
{
    while(PeekMessageA(&Message, WindowHandle, 0, 0, PM_REMOVE)) 
    {
        if(Message.message == WM_QUIT) 
        {
            GlobalRunning = false;
        }

        switch(Message.message) 
        {
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN: 
            case WM_KEYUP:
            {
                uint32 VKCode = (uint32)Message.wParam;
                bool WasDown = ((Message.lParam & (1 << 30)) != 0);
                bool IsDown = ((Message.lParam & (1 << 31)) == 0);
                
                KeyCodeID KeyCode = State->KeyCodeLookup[Message.wParam];
                Key *Key = &GameInput->Keyboard.Keys[KeyCode];
                Key->JustPressed = !Key->JustPressed && !Key->IsDown && IsDown;
                Key->JustReleased = !Key->JustReleased && Key->IsDown && !IsDown;
                Key->IsDown = IsDown;


                bool AltKeyIsDown = ((Message.lParam & (1 << 29)) !=0);
                if(VKCode == VK_F4 && AltKeyIsDown) 
                {
                    GlobalRunning = false;
                }
            }break;
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP: 
            case WM_XBUTTONUP:
            {
                uint32 VKCode = (uint32)Message.wParam;
                bool IsDown = (GetKeyState(VKCode) & (1 << 15));

                KeyCodeID KeyCode = State->KeyCodeLookup[Message.wParam];
                Key *Key = &GameInput->Keyboard.Keys[KeyCode];
                Key->JustPressed = !Key->JustPressed && !Key->IsDown && IsDown;
                Key->JustReleased = !Key->JustReleased && Key->IsDown && !IsDown;
                Key->IsDown = IsDown;
            }break;
            default: 
            {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }break;
        }
    }
}

int APIENTRY
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd) 
{
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    int64 PerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    WNDCLASS Window = {};
    Win32OpenGLFunctions Win32OpenGL = {};
    Win32_WindowData WindowData = {};

    Window.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    Window.lpfnWndProc = Win32MainWindowCallback;
    Window.hInstance = hInstance;
    Window.lpszClassName = "Sugar";
    
    if(RegisterClass(&Window)) 
    {
        WindowData.X = 100;
        WindowData.Y = 100;
        WindowData.WindowWidth = 1280;
        WindowData.WindowHeight = 640;

        // NOTE : This Initializes OpenGL so we can make the next context
        Win32InitializeOpenGL(hInstance, Window, &Win32OpenGL);
        // NOTE : Then we create the real OpenGL Context
        HWND WindowHandle = 
            CreateWindow(Window.lpszClassName,
                    "Sugar Framework", 
                    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                    WindowData.X,
                    WindowData.Y,
                    WindowData.WindowWidth,
                    WindowData.WindowHeight,
                    0,
                    0,
                    hInstance,
                    0);
        HDC WindowDC = GetDC(WindowHandle);
        const int PixelAttributes[] = 
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SWAP_METHOD_ARB,    WGL_SWAP_COPY_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            0
        };
        UINT NumPixelFormats;
        int PixelFormat = 0;
        if(!Win32OpenGL.wglChoosePixelFormatARB(WindowDC, PixelAttributes, 0, 1, &PixelFormat, &NumPixelFormats)) 
        {
            MessageBoxA(0, "Failed to load the pixel format!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            Assert(false, "Failed to choose the pixel format the second time\n");
        }
        PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
        DescribePixelFormat(WindowDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &DesiredPixelFormat);
        if(!SetPixelFormat(WindowDC, PixelFormat, &DesiredPixelFormat)) 
        {
            MessageBoxA(0, "Failed to set the pixel format!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            Assert(false, "Failed to set the main PixelFormat\n");
        }

        const int ContextAttributes[] = 
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            0
        };

        HGLRC RenderingContext = Win32OpenGL.wglCreateContextAttribsARB(WindowDC, 0, ContextAttributes);
        if(!RenderingContext) 
        {
            MessageBoxA(0, "Failed to set the Rendering Context!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            Assert(false, "Failed to create the RenderingContext\n");
        }

        if(!wglMakeCurrent(WindowDC, RenderingContext)) 
        {
            MessageBoxA(0, "Failed to set the Make the current the Context!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            Assert(false, "Failed to make current wglMakeCurrent(WindowDc, RenderingContext)\n");
        }

        // NOTE : VSYNC
        Win32OpenGL.wglSwapIntervalEXT(0);
        // NOTE : VSYNC

        if(WindowHandle) 
        {
            GameState GameState = {};
            GameState.GameMemory.PermanentStorage = MakeBumpAllocator(Megabytes(100));
            GameState.GameMemory.TransientStorage = MakeBumpAllocator(Megabytes(1000));
            GameState.KeyCodeLookup[KEY_COUNT] = {};
            GameState.IsInitialized = 0;
            
            GameState.RenderData = (RenderData *)BumpAllocate(&GameState.GameMemory.PermanentStorage, sizeof(RenderData));
            if(!GameState.RenderData) 
            {
                MessageBoxA(WindowHandle, "Failed to set the Allocate memory!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            }
            
            GameState.Entities = (Entity *)BumpAllocate(&GameState.GameMemory.PermanentStorage, sizeof(Entity) * MAX_ENTITIES);
            if(!GameState.Entities) 
            {
                MessageBoxA(WindowHandle, "Failed to allocate memory for the entities!\n", "Lol programmer sux", MB_ABORTRETRYIGNORE);
            }

            // INITIALIZE
            char *SourceDLLName = "GameCode.dll";
            InitializeOpenGLRenderer(&GameState);
            Win32LoadKeyData(&GameState); 
            Win32GameCode Game = Win32LoadGamecode(SourceDLLName);
            GlobalRunning = true;

            WindowData.WindowDC = GetDC(WindowHandle);

            LARGE_INTEGER LastCounter;            
            QueryPerformanceCounter(&LastCounter);
            // TODO : Two functions are needed. Update(), and FixedUpdate(). 
            //        Update will operate under the Display Framerate, 
            //        whilst FixedUpdate has a fixed Update Time
            //        Update will be for things that are a part of the same update frequency as the renderer.
            //        FixedUpdate will be mainly for Physics.
            Input GameInput = {};
            // KEYBOARD BINDINGS
            GameInput.Keyboard.Bindings[MOVE_UP].Key = KEY_W;
            GameInput.Keyboard.Bindings[MOVE_DOWN].Key = KEY_S;
            GameInput.Keyboard.Bindings[MOVE_LEFT].Key = KEY_A;
            GameInput.Keyboard.Bindings[MOVE_RIGHT].Key = KEY_D;
            GameInput.Keyboard.Bindings[ATTACK].Key = KEY_MOUSE_LEFT;

            XInputPointers XInput = Win32LoadXInput("XInput1_4.dll");
            if(!XInput.SetState,
               !XInput.GetState) 
            {
                MessageBoxA(0, "Failed to load WGL Functions!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
                MessageBoxA(0, "Failed to set the XInput Pointers!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            }

            real32 SimulationDelta = {};
            while(GlobalRunning) 
            {
#ifdef SUGAR_SLOW
                // HOTRELOADING
                FILETIME NewDLLWriteTime = Win32GetLastWriteTime(SourceDLLName);
                if(CompareFileTime(&NewDLLWriteTime, &Game.DLLLastWriteTime) != 0) 
                {
                    Win32UnloadGamecode(&Game);
                    Game = Win32LoadGamecode(SourceDLLName);
                    GameState.IsInitialized = 0;
                }
#endif
                if(!GameState.IsInitialized) 
                {
                    Game.InitData(&GameState);
                }

                MSG Message = {};
                WindowData.WindowWidth = ClientWidth;
                WindowData.WindowHeight = ClientHeight;

                // TODO : Determine whether or not it's a mistake to use "ms" for the unit
                if(SimulationDelta >= SIMRATE) 
                {
                    if(SimulationDelta >= (SIMRATE*2)) 
                    {
                        SimulationDelta = SIMRATE;
                    }

                    // MESSAGES
                    Win32ProcessInputMessages(Message, WindowHandle, &GameInput, &GameState);
                    
                    // MOUSE 
                    POINT MousePos;
                    GameInput.Keyboard.LastMouse.x = GameInput.Keyboard.CurrentMouse.x;
                    GameInput.Keyboard.LastMouse.y = GameInput.Keyboard.CurrentMouse.y;
                    GetCursorPos(&MousePos);
                    ScreenToClient(WindowHandle, &MousePos);
                    GameInput.Keyboard.CurrentMouse.x = MousePos.x;
                    GameInput.Keyboard.CurrentMouse.y = MousePos.y;
                    GameInput.Keyboard.RelMouse = GameInput.Keyboard.CurrentMouse - GameInput.Keyboard.LastMouse;
                    
                    real32 InterpolateDelta = SimulationDelta / SIMRATE; 
                    iVLerp(GameInput.Keyboard.CurrentMouse, GameInput.Keyboard.LastMouse, InterpolateDelta);

                    for(DWORD ControllerIndex = 0; 
                        ControllerIndex < 1; 
                        ++ControllerIndex) 
                    {
                        XINPUT_STATE ControllerState;
                        if(XInput.GetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS) 
                        {   
                            XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

                            bool DPadUp = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                            bool DPadDown = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                            bool DPadLeft = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                            bool DPadRight = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                            bool StartButton = (Pad->wButtons & XINPUT_GAMEPAD_START);
                            bool BackButton = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
                            bool LeftThumbDown = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
                            bool RightThumbDown = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
                            bool LeftShoulderDown = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                            bool RightShoulderDown = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                            bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
                            bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
                            bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
                            bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

                            uint8 LeftTrigger = Pad->bLeftTrigger;
                            uint8 RightTrigger = Pad->bRightTrigger;
                            int16 LeftStickX = Pad->sThumbLX;
                            int16 LeftStickY = Pad->sThumbLY;
                            int16 RightStickX = Pad->sThumbRX;
                            int16 RightStickY = Pad->sThumbRY;

                            XINPUT_VIBRATION Vibration = {};    
                            Vibration.wLeftMotorSpeed = 6500;
                            Vibration.wRightMotorSpeed = 6500;
                            XInput.SetState(ControllerIndex, &Vibration);
                        }
                        else 
                        {
                            break; // NOTE : No controller avaliable
                        }
                    }
                }

#ifdef SUGAR_SLOW
                // TODO : This will eventually go inside of out "Update()" loop
                Game.UpdateAndRender(&GameState, &GameInput);
#else
                GameUpdateAndRender(&GameState, &GameInput);
#endif
                OpenGLRender(&GameState, &WindowData);

                // RESET MEMORY
                BumpReset(&GameState.GameMemory.TransientStorage);

                // DELTA TIME
                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);
                int64 DeltaCounter = EndCounter.QuadPart - LastCounter.QuadPart;    
                real32 MSPerFrame = (1000 * (real32)DeltaCounter) / (real32)PerfCountFrequency;
                int32 FPS = (int32)PerfCountFrequency / (int32)DeltaCounter;

                SimulationDelta += MSPerFrame;       // DELTA TIME
                LastCounter = EndCounter;

                // PERFORMANCE PROFILING
            }
        }
        else 
        {
            MessageBoxA(0, "Failed to create the window handle!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
            Assert(false, "Failed to create the WindowHandle!\n");
        }
    }
    else 
    {
        MessageBoxA(0, "Failed to create the window Class!", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
        Assert(false, "Failed to create the WindowClass!\n");
    }
    return(0);
}
