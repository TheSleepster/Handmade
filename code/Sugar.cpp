#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "Sugar_Input.h"

// ENGINE UTILS
#include "util/Sugar_Array.h"

// GAME UTILS
#include "Sugar_GJK.cpp"
#include "Sugar_API.cpp"

#define WORLD_WIDTH 320
#define WORLD_HEIGHT 180

extern "C" 
INIT_GAME_DATA(InitGameData)
{
}

extern "C" 
GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    State->RenderData->PlayerCamera.Viewport = {WORLD_WIDTH, WORLD_HEIGHT};
    State->RenderData->PlayerCamera.Position = {0, 0};
    CreateEntity(SPRITE_DICE, {0, 0}, {1.0f, 1.0f}, 0, State->Entities);

    DrawEntity(State, 0);
}
