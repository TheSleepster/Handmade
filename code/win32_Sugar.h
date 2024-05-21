#pragma once 

#include "Sugar.h"

struct Win32_WindowData 
{
    int X;
    int Y;
    int WindowWidth;
    int WindowHeight;
    HDC WindowDC;

    bool GlobalRunning;
};

struct Win32_WindowClient 
{
    int Width;
    int Height;
};
