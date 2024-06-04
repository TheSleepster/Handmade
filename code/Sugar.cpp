#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

// ENGINE UTILS
#include "util/Sugar_Array.h"

// GAME UTILS
#include "Sugar_GJK.cpp"
#include "Sugar_ECS.cpp"

extern "C" 
INIT_GAME_DATA(InitGameData)
{
}

extern "C" 
GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    GameRenderData = GameRenderDataIn; 
    DynamicArray Entities;
    ArrayInit(&GameMemory->PermanentStorage, &Entities, 10);

    CreateDiceEntity({400, 200}, {250, 250}, &Entities, &GameMemory->PermanentStorage);
 
    Entity *Result = (Entity *)ArrayGetElement(&Entities, 0); 
    DrawEntity(Result);
} 
