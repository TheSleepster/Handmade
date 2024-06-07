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

struct OrthographicCamera 
{ 
    mat4 ProjectionMatrix;
    
    vec2 Position;
    vec2 Viewport;
    
    real32 Zoom;
};

struct RenderData 
{
    int TransformCount;
    Transform Transforms[MAX_TRANSFORMS];
    
    OrthographicCamera PlayerCamera;
    OrthographicCamera UICamera;
};

// API STUFF
enum SpriteID 
{
    SPRITE_DICE,
    SPRITE_TILE,
    SPRITE_COUNT
};

struct Sprite
{
    ivec2 AtlasOffset;
    ivec2 SpriteSize;
};

internal inline mat4
CreateOrthographicMatrix(real32 Left, real32 Right, real32 Top, real32 Bottom) 
{
    mat4 Result = {};
    
    Result.Elements[3][0] = -(Right + Left) / (Right - Left);
    Result.Elements[3][1] =  (Top + Bottom) / (Top - Bottom);
    Result.Elements[3][2] =  0.0f;
    
    Result.Elements[0][0] =  2.0f / (Right - Left);
    Result.Elements[1][1] =  2.0f / (Top - Bottom);
    Result.Elements[2][2] =  1.0f / (1.0f - 0.0f);
    Result.Elements[3][3] =  1.0f;
    
    return(Result);
}
