#pragma once 

#include "Sugar.h"
#include "SugarAPI.h"

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

inline FILETIME
Win32GetLastWriteTime(char *Filename) 
{
    FILETIME LastWriteTime = {};

    WIN32_FIND_DATA FindData;
    HANDLE FindHandle = FindFirstFileA(Filename, &FindData);
    if(FindHandle != INVALID_HANDLE_VALUE) 
    {
        LastWriteTime = FindData.ftLastWriteTime;
        FindClose(FindHandle);
    }

    return(LastWriteTime);
}
