#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

// GAME UTILS
#include "Sugar_GJK.cpp"
#include "Sugar_ECS.cpp"

global_variable GameState State;

extern "C" 
INIT_GAME_DATA(InitGameData)
{
    CreateDiceEntity({400, 500}, {100.0f, 100.0f}, &State);
}

extern "C" 
GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    GameRenderData = GameRenderDataIn; 
    vec2 Dice1Pos = {(real32)GameInput->Keyboard.CurrentMouse.x, 
        (real32)GameInput->Keyboard.CurrentMouse.y}; 
    
    vec2 Dice2Pos = {200, 300};
    
    vec2 Dice1Size = {100, 100};
    vec2 Dice2Size = {100, 100};
    
    Collider Dice1Box = {};
    Collider Dice2Box = {};
    
    // Define vertices for Dice1
    Dice1Box.Vertices[0] = {Dice1Pos.x, Dice1Pos.y};
    Dice1Box.Vertices[1] = {Dice1Pos.x + Dice1Size.x, Dice1Pos.y};
    Dice1Box.Vertices[2] = {Dice1Pos.x + Dice1Size.x, Dice1Pos.y + Dice1Size.y};
    Dice1Box.Vertices[3] = {Dice1Pos.x, Dice1Pos.y + Dice1Size.y};
    Dice1Box.VertexCount = 4;
    
    // Define vertices for Dice2
    Dice2Box.VertexCount = 4;
    Dice2Box.Vertices[0] = {Dice2Pos.x, Dice2Pos.y};
    Dice2Box.Vertices[1] = {Dice2Pos.x + Dice2Size.x, Dice2Pos.y};
    Dice2Box.Vertices[2] = {Dice2Pos.x + Dice2Size.x, Dice2Pos.y + Dice2Size.y};
    Dice2Box.Vertices[3] = {Dice2Pos.x, Dice2Pos.y + Dice2Size.y};
    
    DrawSprite(SPRITE_DICE, Dice1Pos, Dice1Size);
    DrawSprite(SPRITE_DICE, Dice2Pos, Dice2Size);

    for(int Entity = 0; Entity < EntityIndex; ++Entity) 
    {
        DrawEntity(&State.Entities[Entity]);

        char Buffer[256];
        int IsColliding = GJK(&Dice1Box, &State.Entities[Entity].Collider);
        if(IsColliding) 
        {
            sprintf(Buffer, "Collision On Dice Entity!\n");
            OutputDebugStringA(Buffer);
        }
        if(State.Entities[Entity].Flags & ACTIVE) 
        {
            sprintf(Buffer, "ACTIVE\n");
            OutputDebugStringA(Buffer);
        }
        if(State.Entities[Entity].Flags & IS_STATIC) 
        {
            sprintf(Buffer, "STATIC\n");
            OutputDebugStringA(Buffer);
        }
        if(State.Entities[Entity].Flags & TILE) 
        {
            sprintf(Buffer, "TILES\n");
            OutputDebugStringA(Buffer);
        }
    }
    
    char Buffer[256];
    int IsColliding = GJK(&Dice1Box, &Dice2Box);
    if(IsColliding) 
    {
        sprintf(Buffer, "Collision!\n");
        OutputDebugStringA(Buffer);
    }
} 
