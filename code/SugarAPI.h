#pragma once

#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "util/Sugar_Math.h"

// RENDER DATA
#define MAX_TRANSFORMS 1000
#define MAX_ENTITIES 1000

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
