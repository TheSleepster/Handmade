#pragma once
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Memory.h"
#include "Sugar_Input.h"
#include "SugarAPI.h"
#include "../data/deps/OpenGL/GLL.h"

#define WORLD_WIDTH 160
#define WORLD_HEIGHT 90
#define MAX_VERTICES 24

constexpr int32 TILE_SIZE = 16;
constexpr ivec2 WORLD_GRID = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};

struct GameMemory 
{
    BumpAllocator TransientStorage;
    BumpAllocator PermanentStorage;
    
    bool IsInitialized;
};

struct glContext 
{
    GLuint ProgramID; 
    GLuint TextureID;
    GLuint TransformSBOID;
    GLuint ScreenSizeID;
    GLuint ProjectionMatrixID;
    
    const char *VertexShaderFilepath;
    const char *FragmentShaderFilepath;
    const char *TextureDataFilepath;
    
    FILETIME TextureTimestamp;
    FILETIME ShaderTimestamp;
};

enum ShapeType
{ 
    CIRCLE     = 0x0,
    BOX        = 0x1,
    PENTAGON   = 0x2,
    HEXAGON    = 0x3,
};

enum EntityFlags 
{
    ACTIVE       = 0x0,
    PLAYER       = 0x1,
    IS_STATIC    = 0x2,
    AI           = 0x3,
    TILE         = 0x4,
    HAS_PHYSICS  = 0x5,
    RENDERABLE   = 0x6,
    IS_DEAD      = 0x7,
    IS_VISIBLE   = 0x8,
};

enum SpriteID 
{
    SPRITE_DICE,
    SPRITE_WALL,
    SPRITE_FLOOR,
    SPRITE_COUNT
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
    vec2 PreviousVelocity;
    vec2 CurrentVelocity;
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
    Sprite SpriteID;
};

struct NeighborComponent 
{ 
    BYTE NeighborCount;
    BYTE TileType;
};

enum EntityType
{
    Entity_Dice,
    Entity_Tile,
};

// TODO(Sleepster): Decide what EXACTLY seperates high/low entities, and what is different between the two
struct LowEntity
{
    uint32 Flags;
    
    PhysicsComponent Physics;
    TransformComponent Transform;
    ColliderComponent Collider;
    SpriteComponent Sprite;
    NeighborComponent NeighborBit;
    
    uint32 HighEntityIndex;
};

struct HighEntity
{
    uint32 Flags;
    
    PhysicsComponent Physics;
    TransformComponent Transform;
    ColliderComponent Collider;
    SpriteComponent Sprite;
    NeighborComponent NeighborByte;
    
    uint32 LowEntityIndex;
};

struct Entity 
{
    HighEntity *High;
    LowEntity *Low;
};

struct World
{
    bool Active;
    Entity Tilemap[WORLD_GRID.x][WORLD_GRID.y];
};

struct GameState 
{
    // MEMORY
    GameMemory GameMemory;
    bool IsInitialized;
    
    // FUNCTIONAL COMPONENTS
    KeyCodeID KeyCodeLookup[KEY_COUNT];
    glContext glContext;
    RenderData *RenderData;
    
    // ENTITY DATA
    int32 HighEntityCount;
    HighEntity *HighEntities;
    
    int32 LowEntityCount;
    LowEntity *LowEntities;
    
    Entity Player;
    
    // MAP DATA
    World *World;
    
    // CAMERA MODE
    bool PlayerCamera;
    bool UICamera;
    bool EditorCamera;
};

#define GAME_UPDATE_AND_RENDER(name) void name(GameState *State, Input *GameInput)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
GAME_UPDATE_AND_RENDER(GameUpdateAndRenderStub) 
{
}

#define INIT_GAME_DATA(name) void name(GameState *State)
typedef INIT_GAME_DATA(init_game_data);
INIT_GAME_DATA(InitGameDataStub) 
{ 
}

#ifndef SUGAR_SLOW
static void GameUpdateAndRender(GameState *State, Input *GameInput);
#endif
