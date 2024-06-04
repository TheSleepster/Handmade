#pragma once

#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "util/Sugar_Math.h"

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

// API STUFF
enum SpriteID 
{
    SPRITE_DICE,
    SPRITE_BLUE,
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
        case SPRITE_BLUE: 
        {
            Sprite.AtlasOffset = {16, 0};
            Sprite.SpriteSize = {16, 16};
        }
    }
    return(Sprite);
}
