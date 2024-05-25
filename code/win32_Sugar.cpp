

/*
    This is not a final layer. Additional Requirements: 

    - GameInput
    - Audio
    - DeltaTime
    - Asset Loading
    - Fullscreen
    - Multithreading
    - File Saving
    - Game Saving
    - Sleep/Inactivity Period
    - GetKeyboardLayout() (For non-standard QWERTY keyboards)
    - Raw Input (For multiple inputs)
    - WM_ACTIVATEAPP (For being the inactive window)
    - ClipCursor(Multi Monitor)
*/



#include "Sugar_Intrinsics.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

#define WIN32_LEAN_AND_MEAN
#define EXTRA_LEAN
#define NOMINMAX
#include <windows.h>
#include <wingdi.h>
#include <xinput.h>
#include "../data/deps/OpenGL/GLL.h"

global_variable Win32_WindowData WindowData;
#include "Sugar_OpenGLRenderer.cpp"
#include "Sugar_Input.cpp"

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
        Result.IsValid = (Result.UpdateAndRender);
    }
    if(!Result.IsValid) 
    {
        Result.UpdateAndRender= GameUpdateAndRenderStub;
    }
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
    Gamecode->UpdateAndRender = GameUpdateAndRenderStub;
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
        case WM_DESTROY: 
        {
            WindowData.GlobalRunning = false;
        }break;
        case WM_SIZE: 
        {
            RECT Rect = {};
            GetClientRect(hWnd, &Rect);
            WindowData.WindowWidth = Rect.right - Rect.left;
            WindowData.WindowHeight = Rect.bottom - Rect.top;
        }break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN: 
        case WM_KEYUP:
        {
            switch(wParam) 
            {
                case VK_ESCAPE: 
                {
                    WindowData.GlobalRunning = false;
                }break;

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
            WindowData.GlobalRunning = false;
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

    if(!Win32OpenGL->wglCreateContextAttribsARB||!Win32OpenGL->wglChoosePixelFormatARB) 
    {
        Assert(false, "Failed to manually load WGL functions!\n");
    }
    wglMakeCurrent(WindowDC, 0);
    wglDeleteContext(TempRC);
    DestroyWindow(WindowHandle);
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

    Window.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    Window.lpfnWndProc = Win32MainWindowCallback;
    Window.hInstance = hInstance;
    Window.lpszClassName = "Sugar";
    
    if(RegisterClass(&Window)) 
    {
        WindowData.X = 100;
        WindowData.Y = 100;
        WindowData.WindowWidth = 1280;
        WindowData.WindowHeight = 720;

        // NOTE : This Initializes OpenGL so we can make the next context
        Win32InitializeOpenGL(hInstance, Window, &Win32OpenGL);
        // NOTE : Then we create the real OpenGL Context
        HWND WindowHandle = 
            CreateWindow(Window.lpszClassName,
                    "Sugar",
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
            Assert(false, "Failed to choose the pixel format the second time\n");
        }
        PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
        DescribePixelFormat(WindowDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &DesiredPixelFormat);
        if(!SetPixelFormat(WindowDC, PixelFormat, &DesiredPixelFormat)) 
        {
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
            Assert(false, "Failed to create the RenderingContext\n");
        }

        if(!wglMakeCurrent(WindowDC, RenderingContext)) 
        {
            Assert(false, "Failed to make current wglMakeCurrent(WindowDc, RenderingContext)\n");
        }

        // NOTE : VSYNC
        Win32OpenGL.wglSwapIntervalEXT(1);
        // NOTE : VSYNC

        if(WindowHandle) 
        {
            GameMemory GameMemory = {};
            GameMemory.PermanentStorage = MakeBumpAllocator(Megabytes(100));
            GameMemory.TransientStorage = MakeBumpAllocator(Gigabytes(1));
            
            GameRenderData = (RenderData*)BumpAllocate(&GameMemory.PermanentStorage, sizeof(RenderData));
            Assert(GameRenderData, "Failed to allocate Permanent Memory for the GameRenderData Variable!\n");
            
            GameInput = (Input*)BumpAllocate(&GameMemory.PermanentStorage, sizeof(Input));
            Assert(GameInput, "Failed to allocate PermanentMemory for the Input Variable!\n");
            

            InitializeOpenGLRenderer(&GameMemory);
            Win32LoadKeyData();


            char *SourceDLLName = "GameCode.dll";
            Win32GameCode Game = Win32LoadGamecode(SourceDLLName);

            MSG Message;
            WindowData.WindowDC = GetDC(WindowHandle);
            WindowData.GlobalRunning = true;

            LARGE_INTEGER LastCounter;            
            QueryPerformanceCounter(&LastCounter);
            // TODO : Two functions are needed. Update(), and FixedUpdate(). 
            //        Update will operate under the Display Framerate, 
            //        whilst FixedUpdate has a fixed Update Time
            //        Update will be for things that are a part of the same update frequency as the renderer.
            //        FixedUpdate will be mainly for Physics.
            while(WindowData.GlobalRunning) 
            {
                FILETIME NewDLLWriteTime = Win32GetLastWriteTime(SourceDLLName);
                if(CompareFileTime(&NewDLLWriteTime, &Game.DLLLastWriteTime) != 0) 
                {
                    Win32UnloadGamecode(&Game);
                    Game = Win32LoadGamecode(SourceDLLName);
                }

                while(PeekMessageA(&Message, WindowHandle, 0, 0, PM_REMOVE)) 
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }

                Game.UpdateAndRender(&GameMemory, GameRenderData, GameInput);
                OpenGLRender(&GameMemory);
                GameMemory.TransientStorage.Used = 0;

                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);
                int64 DeltaCounter = EndCounter.QuadPart - LastCounter.QuadPart;    
                real32 MSPerFrame = (1000 * (real32)DeltaCounter) / (real32)PerfCountFrequency;
                int32 FPS = (int32)PerfCountFrequency / (int32)DeltaCounter;

                char Buffer[256];
                sprintf(Buffer, "%.02fms, FPS: %d\n", MSPerFrame, FPS);
                OutputDebugStringA(Buffer);
                LastCounter = EndCounter;
            }
        }
        else 
        {
            Assert(false, "Failed to create the WindowHandle!\n");
        }
    }
    else 
    {
        Assert(false, "Failed to create the WindowClass!\n");
    }
    return(0);
}
