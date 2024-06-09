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
    
    JSONToken *Token = {};
    JSONParser *Parser = {};
    const char *Buffer = {};
    
    JSONParseLevel(Buffer, int32(strlen(Buffer)), Parser, Token);
    
    
    
    
    
    
    
    
#if 0    
    for(int i = 0; i < WORLD_GRID.x;++i)
    {
        for(int j = 0; j < WORLD_GRID.y;++j)
        {
            CreateTileFromGrid({i, j}, GameInput, State);
            DrawTileSprite(GetTile(ivec2{i, j}, State), State);
        }
    }
#endif
}