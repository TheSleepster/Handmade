#pragma once
#include "../Sugar.h"
#include "../Win32_Sugar.h"
#include "../Sugar_Intrinsics.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define EXTRA_LEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

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
    Assert(BumpAllocator->Used + AllignedSize <= BumpAllocator->Capacity, "Not Enough Memory for Allocation!\n");
    return(Result);
}
