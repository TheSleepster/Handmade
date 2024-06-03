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

struct Entity 
{
    uint32 EntityID;
    uint64 *Flags;

    RigidBody PhysicsBody;
    Collider Collider;
    SpriteID Sprite;
};

struct GameState 
{
    Entity Entities[255];
};

