#pragma once

#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"

struct Collider 
{
    vec2 Vertices[8];
    int VertexCount;
};

struct RigidBody 
{
    vec2 Position;
    vec2 Velocity;
    vec2 Size;

    // Restitution is an indicator of how Elastic/Inelastic the collision is.
    real32 Restitution;
    real32 Density;
    real32 Area;
    //real32 Rotation;
};

enum EntityType 
{ 
    DICE,
    ENTITYTYPECOUNT
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

struct Entity 
{
    uint32 EntityID;
    uint64 Flags;

    RigidBody PhysicsBody;
    Collider Collider;
    SpriteID Sprite;
};

struct GameState 
{
    Entity Entities[255];
};

