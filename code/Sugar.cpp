#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
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
    ArrayInit(&State->GameMemory.PermanentStorage, &State->Entities, 10);

    // NOTE : Add renderData and the game's memory to gamestate
    CreateEntity(SPRITE_DICE, {150, 200}, {300, 300}, &State->Entities, &State->GameMemory.PermanentStorage);
    DrawEntity(State, 0);
    KillEntity(State, 0);
} 
