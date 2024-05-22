#pragma once

#include "Sugar.h"
#include "util/Sugar_Math.h"
#include "Sugar_Intrinsics.h"

// RENDER DATA
#define MAX_TRANSFORMS 1000

struct Transform
{
    ivec2 AtlasOffset;
    ivec2 SpriteSize;
    vec2 Position;
    vec2 Size;
};

struct RenderData 
{
    int TransformCount;
    Transform Transforms[MAX_TRANSFORMS];
};

// TODO : Change this from global when hot reloading is a thing
static RenderData *GameRenderData;

// API STUFF
enum SpriteID 
{
    SPRITE_DICE,
    SPRITE_COUNT
};

struct Sprite
{
    ivec2 AtlasOffset;
    ivec2 SpriteSize;
};

inline Sprite GetSprite(SpriteID SpriteID) 
{
    Sprite Sprite = {};
    switch(SpriteID) 
    {
        case SPRITE_DICE: 
        {
            Sprite.AtlasOffset = {0, 0};
            Sprite.SpriteSize = {16, 16};
        }break;
    }
    return(Sprite);
}

inline void DrawSprite(SpriteID SpriteID, vec2 Pos, vec2 Size) 
{
    Sprite Sprite = GetSprite(SpriteID);
    Transform transform = {};
    transform.Position = Pos;
    transform.Size = Size;
    transform.AtlasOffset = Sprite.AtlasOffset;
    transform.SpriteSize = Sprite.SpriteSize;

    GameRenderData->Transforms[GameRenderData->TransformCount++] = transform;
}
