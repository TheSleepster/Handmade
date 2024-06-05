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
    CreateEntity(SPRITE_DICE, {200, 300}, {300, 300}, 0, State->Entities);
    CreateEntity(SPRITE_DICE, {700, 400}, {100, 100}, 1, State->Entities);
    CreateEntity(SPRITE_DICE, {600, 100}, {50, 50}, 2, State->Entities);

    DrawEntity(State, 0);
    DrawEntity(State, 1);
    DrawEntity(State, 2);
}
