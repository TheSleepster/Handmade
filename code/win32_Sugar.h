#pragma once 

#include "Sugar.h"
#include "SugarAPI.h"
#include "util/Sugar_Memory.h"
#include "Sugar_Intrinsics.h"
#include <stdio.h>
#include <stdint.h>

typedef int32_t int32;
typedef int64_t int64;

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

inline int32
GetFileSizeInBytes(const char *Filepath) 
{
    int32 FileSize = 0;
    FILE *File = fopen(Filepath, "rb");
    Assert(File != nullptr, "File not found\n");
    
    fseek(File, 0, SEEK_END);
    FileSize = ftell(File);
    fseek(File, 0, SEEK_SET);
    fclose(File);

    return(FileSize);
}

inline char *
ReadEntireFile(const char *Filepath, int *Size, char *Buffer) 
{
    Assert(Filepath != nullptr, "Cannot find the file designated!\n");
    Assert(Buffer != nullptr, "Provide a valid buffer!\n");
    Assert(Size >= 0, "Size is less than 0!\n");

    *Size = 0;
    FILE *File = fopen(Filepath, "rb");

    fseek(File, 0, SEEK_END);
    *Size = ftell(File);
    fseek(File, 0, SEEK_SET);

    memset(Buffer, 0, *Size + 1);
    fread(Buffer, sizeof(char), *Size, File);

    fclose(File);
    return(Buffer);
}

inline char *
ReadEntireFileBA(const char *Filepath, int *FileSize, BumpAllocator *BumpAllocator) 
{
    char *File = nullptr; 
    int32 FileSize2 = GetFileSizeInBytes(Filepath);
    Assert(FileSize2 >= 0, "FileSize is less than 0!\n");

    char *Buffer = BumpAllocate(BumpAllocator, size_t(FileSize2 + 1));
    File = ReadEntireFile(Filepath, FileSize, Buffer);

    return(File);
}
