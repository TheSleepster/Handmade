#include "Sugar.h"
#include "win32_Sugar.h"

internal void 
GameUpdateAndRender(Win32_WindowData WindowData, BumpAllocator *TransientStorage) 
{
    glViewport(0, 0, WindowData.WindowWidth, WindowData.WindowHeight);
    glClearColor(1.0f, 0.4f, 1.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    SwapBuffers(WindowData.WindowDC);
}
