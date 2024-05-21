#pragma once

#if SUGAR_SLOW

#define Assert(Expression, Message) if(!(Expression)) {OutputDebugStringA(Message); DebugBreak();}
#define Trace(Message) {OutputDebugStringA(Message);}
#define WIN32_LEAN_AND_MEAN
#define EXTRA_LEAN
#include <windows.h>

#else

#define Assert(Expression, Message)

#endif

#define Kilobytes(Value) ((uint64)(Value) * 1024)
#define Megabytes(Value) ((uint64)Kilobytes(Value) * 1024)
#define Gigabytes(Value) ((uint64)Megabytes(Value) * 1024)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#define global_variable static
#define local_persist static
#define internal static

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float real32;
typedef double real64;

struct GameMemory 
{
    uint64 PermanentStorageSize;
    void *PermanentStorage;

    uint64 TransientStorageSize;
    void *TransientStorage;

    bool IsInitialized;
};

struct BumpAllocator 
{
    size_t Capacity;
    size_t Used;
    char* Memory;
};

inline BumpAllocator 
MakeBumpAllocator(size_t Size) 
{
    BumpAllocator BumpAllocator = {};
    BumpAllocator.Memory = (char *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
    if(BumpAllocator.Memory) 
    {
        BumpAllocator.Capacity = Size;
        memset(BumpAllocator.Memory, 0, Size);
    }
    Assert(BumpAllocator.Memory, "Failed to create the BumpAllocator!\n");

    return(BumpAllocator);
}

inline char *
BumpAllocate(BumpAllocator *BumpAllocator, size_t Size) 
{
    char *Result = nullptr;
    size_t AllignedSize = (Size + 7) & ~ 7;
    if(BumpAllocator->Used + AllignedSize <= BumpAllocator->Capacity) 
    {
        Result = BumpAllocator->Memory + BumpAllocator->Used;
        BumpAllocator->Used += AllignedSize;
    }
    Assert(BumpAllocator->Used + AllignedSize <= BumpAllocator->Capacity, "BumpAllocation failed!\n");
    return(Result);
}

inline int64 
GetFileTimeStamp(const char *Filepath) 
{
    Assert(Filepath != 0, "File not found!\n");

    struct stat Filestat = {};
    stat(Filepath, &Filestat);
    return(Filestat.st_mtime);
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
