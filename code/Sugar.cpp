#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "Sugar_Input.h"

// ENGINE UTILS
#include "util/Sugar_Array.h"

// GAME UTILS
#include "Sugar_GJK.cpp"
#include "Sugar_API.cpp"

extern "C" 
INIT_GAME_DATA(InitGameData)
{
}

extern "C" 
GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    State->RenderData->PlayerCamera.Viewport = {WORLD_WIDTH, WORLD_HEIGHT};
    State->RenderData->PlayerCamera.Position = {160, -90};
    
    CreateEntity(SPRITE_DICE, {160, 90}, {1.0f, 1.0f}, 0, State->HighEntities);
    DrawEntity(State, 0);
}
