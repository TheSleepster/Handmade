#pragma once

#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "util/Sugar_Array.h"

struct TransformComponent 
{
    vec2 PreviousPosition;
    vec2 CurrentPosition;
    vec2 PreviousVelocity;
    vec2 CurrentVelocity;
    vec2 Size;
};

struct PhysicsComponent 
{
    // Restitution is an indicator of how Elastic/Inelastic the collision is.
    real32 Restitution;
    real32 Density;
    real32 Area;
    //real32 Rotation;
};

struct ColliderComponent 
{
    vec2 Vertices[8];
    int VertexCount;
};

struct SpriteComponent 
{
    SpriteID SpriteID;
};

struct NeighborComponent 
{ 
    BYTE NeighborCount;
};

enum EntityFlags 
{
    ACTIVE       = 1 << 0,
    PLAYER       = 1 << 1,
    IS_STATIC    = 1 << 2,
    AI           = 1 << 3,
    TILE         = 1 << 4,
    HAS_PHYSICS  = 1 << 5,
    RENDERABLE   = 1 << 6,
    IS_DEAD      = 1 << 7,
};

// MEGA STRUCT WILL HOLD EVERYTHING FOR NOW
struct Entity 
{
    uint64 Flags;

    PhysicsComponent Physics;
    TransformComponent Transform;
    ColliderComponent Collider;
    SpriteComponent Sprite;
};

#if 0
// Things like animation components, storages ect.

struct Tiles : Entity 
{
    NeighborComponent NeighborCount; 
};

struct Player : Entity 
{ 
};

struct Enemy : Entity
{
};

struct Boss : Entity 
{
};
#endif 
