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

// NOTE(Sleepster): The plan now is to just create our OWN EDITOR that we will eventually be able to serialize the data for somehow.

extern "C" 
GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    State->RenderData->PlayerCamera.Viewport = {WORLD_WIDTH, WORLD_HEIGHT};
    State->RenderData->PlayerCamera.Position = {160, -90};
    
    CreateEntity(Entity_Dice, {160, 90}, 0, State);
    DrawEntity(0, 1, State);
}