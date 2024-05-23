#pragma once
#include "Sugar_Intrinsics.h"
#include "win32_Sugar.h"
#include "SugarAPI.h"

#define OPENGL_RENDER(name) void name(Win32_WindowData *rWindowData, GameMemory *GameMemory)
typedef OPENGL_RENDER(opengl_render);
OPENGL_RENDER(OpenGLRenderStub)
{
}

#define INITIALIZE_OPENGL_RENDERER(name) void name(GameMemory *GameMemory, RenderData *GameRenderDataIn, Win32_WindowData *WindowDataIn)
typedef INITIALIZE_OPENGL_RENDERER(initialize_opengl_renderer);
INITIALIZE_OPENGL_RENDERER(InitializeOpenGLRendererStub) 
{
}
