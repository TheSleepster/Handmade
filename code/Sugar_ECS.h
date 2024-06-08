#pragma once

#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "util/Sugar_Array.h"

#define MAX_VERTICES 24

enum ShapeType 
{ 
    CIRCLE     = 1 << 0,
    BOX        = 1 << 1,
    PENTAGON   = 1 << 2,
    HEXAGON    = 1 << 3,
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
    IS_VISIBLE   = 1 << 8,
};

enum TileType 
{
    FLOOR1 = 1 << 0,
    FLOOR2 = 1 << 1,
    FLOOR3 = 1 << 2,
    FLOOR4 = 1 << 3, 
    WALL1  = 1 << 4,
    WALL2  = 1 << 5,
    WALL3  = 1 << 6,
    WALL4  = 1 << 7,
    
    TYPECOUNT
};

enum SpriteID 
{
    SPRITE_DICE,
    SPRITE_WALL,
    SPRITE_FLOOR,
    SPRITE_COUNT
};

enum EntityType
{
    HIGH_ORDER,
    LOW_ORDER,
};

struct Sprite
{
    ivec2 AtlasOffset;
    ivec2 SpriteSize;
};

struct TransformComponent 
{
    vec2 PreviousPosition;
    vec2 CurrentPosition;
    vec2 PreviousVelocity;
    vec2 CurrentVelocity;
    vec2 Size;
};

struct PhysicsShape 
{ 
    ShapeType ShapeType;
    
    vec2 VertexPositions[MAX_VERTICES];
    vec2 VertexNormals[MAX_VERTICES];
    int32 VertexCount;
    
    real32 Radius;
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
    BYTE TileType;
};

// MEGA STRUCT WILL HOLD EVERYTHING FOR NOW
struct Entity 
{
    EntityType Order;
    uint64 Flags;
    
    PhysicsComponent Physics;
    TransformComponent Transform;
    ColliderComponent Collider;
    SpriteComponent Sprite;
    NeighborComponent NeighborCount; 
};

// Things like animation components, storages ect.
#if 0
struct Player : Entity 
{ 
    bool FillerSoTheCompilerIsntAngry;
};

struct Tile : Entity 
{
    TileType Type;
    NeighborComponent NeighborCount; 
};

struct Enemy : Entity
{
};

struct Boss : Entity 
{
};

struct Animation 
{ 
};

struct PlayerIdle : Animation 
{ 
};

struct PlayerWalking : Animation 
{ 
};

struct PlayerHurt : Animation 
{ 
};

struct PlayerDeath : Animation 
{ 
};
#endif 