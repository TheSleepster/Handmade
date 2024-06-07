#pragma once
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Memory.h"
#include "Sugar_Input.h"
#include "SugarAPI.h"
#include "Sugar_ECS.h"
#include "../data/deps/OpenGL/GLL.h"

#define WORLD_WIDTH 160
#define WORLD_HEIGHT 90

constexpr int TILE_SIZE = 16;
constexpr ivec2 WORLD_GRID = {WORLD_WIDTH, WORLD_HEIGHT};

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

struct SimulationRegion 
{
    ivec2 SimulationSize;
    struct SimulationRegion *NextRegion;
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
    Entity *HighEntities;     
    Entity *LowEntities;
    Entity Player;
    
    // MAP DATA
    Entity *Tilemap;
    SimulationRegion *SimRegion;
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
