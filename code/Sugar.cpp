#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

// GAME UTILS
#include "Sugar_GJK.cpp"
#include "Sugar_ECS.cpp"

extern "C" INIT_GAME_DATA(InitGameData)
{
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
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
    Dice2Box.Vertices[0] = {Dice2Pos.x, Dice2Pos.y};
    Dice2Box.Vertices[1] = {Dice2Pos.x + Dice2Size.x, Dice2Pos.y};
    Dice2Box.Vertices[2] = {Dice2Pos.x + Dice2Size.x, Dice2Pos.y + Dice2Size.y};
    Dice2Box.Vertices[3] = {Dice2Pos.x, Dice2Pos.y + Dice2Size.y};
    Dice2Box.VertexCount = 4;

    DrawSprite(SPRITE_DICE, Dice1Pos, Dice1Size);
    DrawSprite(SPRITE_DICE, Dice2Pos, Dice2Size);

    int IsColliding = GJK(&Dice1Box, &Dice2Box);
    char Buffer[256] = {};
    if(IsColliding) 
    {
        sprintf(Buffer, "Collision!\n");
        Trace(Buffer);
    }
}
