#pragma once
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Memory.h"
#include "Sugar_Input.h"
#include "SugarAPI.h"

struct GameMemory 
{
    BumpAllocator TransientStorage;
    BumpAllocator PermanentStorage;

    bool IsInitialized;
};

#define GAME_UPDATE_AND_RENDER(name) void name(GameMemory *GameMemory, RenderData *GameRenderDataIn, Input *GameInput)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
GAME_UPDATE_AND_RENDER(GameUpdateAndRenderStub) 
{
}
