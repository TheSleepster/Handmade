#pragma once

#include <string.h>

#include "../Sugar_Intrinsics.h"
#include "Sugar_Memory.h"

// NOTE : This is a very naive implimentation, fix it later.

struct DynamicArray
{
    uint32 Size;
    uint32 Capacity;
    void **Entries;
};

inline bool32
ArrayInit(BumpAllocator *Memory, DynamicArray *Array, uint32 Capacity) 
{   
    *(uint32 *)&Array->Size = 0;
    *(uint32 *)&Array->Capacity = Capacity;
    
    Array->Entries = (void **)BumpAllocate(Memory, sizeof(void *)*Capacity);
    if(Array->Entries == 0) 
    {
        return(0);
    }
    return(1);
}

inline bool32
ArrayGrow(BumpAllocator *Memory, DynamicArray *Array)
{
    uint32 NewCapacity = Array->Capacity * 2;
    void **NewEntries = (void **)BumpAllocate(Memory, sizeof(void *)*NewCapacity);
    
    Array->Entries = NewEntries;
    *(uint32 *)&Array->Capacity = NewCapacity;
    BumpDeallocate(Memory, (void *)NewEntries);

    return(1);
}

inline void 
ArrayAdd(BumpAllocator *Memory, DynamicArray *Array, uint32 Index, void *Element) 
{ 
    // Move everything up one from the index
    for(uint32 ElementIndex = Array->Size;
        ElementIndex > Index; 
        --ElementIndex) 
    {
        Array->Entries[ElementIndex] = Array->Entries[ElementIndex - 1];
    }
    // Assign the index
    Array->Entries[Index] = Element;
    
    // Grow if needed
    if(++ *(uint32 *)&Array->Size == Array->Capacity) 
    {
        ArrayGrow(Memory, Array);
    }
}

inline const void *
ArrayGetElement(DynamicArray *Array, uint32 Index) 
{
    return(Array->Entries[Index]);
}

inline void const *
ArraySwap(DynamicArray *Array, uint32 Index, void *Element) 
{ 
    void const *Result = 0;
    if(Index >= Array->Size) 
    {
        return(0);
    }
    Result = Array->Entries[Index];
    Array->Entries[Index] = Element;

    return(Result);
}

inline const void * 
ArrayRemove(DynamicArray *Array, uint32 Index) 
{ 
    const void *Result = Array->Entries[Index];

    for(-- *(uint32 *)&Array->Size; 
        Index <= Array->Size; 
        ++Index) 
    {
        Array->Entries[Index] = Array->Entries[Index + 1];
    }

    return(Result);
}

inline void
ArrayDestroy(BumpAllocator *Memory, DynamicArray *Array) 
{ 
    BumpDeallocate(Memory, (void *)Array);
}
