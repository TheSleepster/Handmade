#include "SugarAPI.h"
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

#define ArrayCount(Array, Type) (sizeof(Array) / sizeof(Type))

extern "C"
{
#include "../data/deps/JSON/cJSON.h"
#include "../data/deps/JSON/cJSON.c"
}

// NOTE(Sleepster): Entities will be store in High/Low ranges sorted by how close/far they are to the player. High Range Entities will update
// more Frequently than that of their Low Range counterparts. 

internal Sprite 
GetSprite(SpriteID SpriteID) 
{
    Sprite Sprite = {};
    switch(SpriteID) 
    {
        case SPRITE_DICE: 
        {
            Sprite.AtlasOffset = {0, 0};
            Sprite.SpriteSize = {16, 16};
        }break;
        case SPRITE_FLOOR:
        {
            Sprite.AtlasOffset = {16, 0};
            Sprite.SpriteSize = {16, 16};
        }break;
    }
    return(Sprite);
}

// TODO(Sleepster): Make sure this works properly, it should "promote" and "demote" entities properly, but I'm not toooo sure

internal void
PromoteEntityToHighRange(int32 LowIndex, GameState *State)
{
    LowEntity *EntityLow = &State->LowEntities[LowIndex];
    
    if(!(State->HighEntityCount < ArrayCount(State->HighEntities, HighEntity)))
    {
        uint32 HighIndex = EntityLow->HighEntityIndex;
        HighEntity *EntityHigh = &State->HighEntities[HighIndex];
        
        EntityHigh->Sprite.SpriteID = EntityLow->Sprite.SpriteID;
        EntityHigh->Transform.Size = EntityLow->Transform.Size;
        EntityHigh->Transform.CurrentPosition = EntityLow->Transform.CurrentPosition;
        
        EntityLow->HighEntityIndex = HighIndex;
    }
    else
    {
        Trace("Invalid Code Path\n");
    }
}

internal void
ClearEntity(GameState *State, uint32 EntityIndex)
{
    State->LowEntities[EntityIndex] = {0};
    State->HighEntities[EntityIndex] = {0};
    State->LowEntities[EntityIndex].HighEntityIndex = EntityIndex;
    State->HighEntities[EntityIndex].LowEntityIndex = EntityIndex;
}

// TODO(Sleepster): Free List perhaps? This would store "Freed" Elements, they would be the first to be overwritten with new data
// Similar in idea to the Memory Arena's freelist
internal void
DemoteEntityToLowRange(int32 HighIndex, GameState *State)
{
    HighEntity *EntityHigh = &State->HighEntities[HighIndex];
    
    if(!(State->LowEntityCount < ArrayCount(State->LowEntities, LowEntity)))
    {
        uint32 LowIndex = State->LowEntityCount++;
        LowEntity *EntityLow = &State->LowEntities[LowIndex];
        
        EntityHigh->LowEntityIndex = LowIndex;
    }
}

internal Entity
GetEntity(uint32 EntityIndex, GameState *State)
{
    Entity Result = {0};
    
    Result.Low = &State->LowEntities[EntityIndex];
    Result.High = &State->HighEntities[EntityIndex];
    
    return(Result);
}

internal void
CreateDiceEntity(vec2 WorldPosition, int32 LowIndex, GameState *State)
{
    Entity Dice = {0};
    
    ClearEntity(State, LowIndex);
    Dice = GetEntity(LowIndex, State);
    
    Dice.Low->Sprite.SpriteID = GetSprite(SPRITE_DICE);
    Dice.Low->Transform.Size = v2Cast(Dice.Low->Sprite.SpriteID.SpriteSize);
    Dice.Low->Transform.CurrentPosition = WorldPosition - (Dice.Low->Transform.Size / 2);
}

internal void
DrawStaticEntitySprite(Entity *Entity, int8 Scale, RenderData *RenderData)
{
    Transform transform = {0};
    
    transform.Position = Entity->High->Transform.CurrentPosition;
    transform.Size = v2Cast(Entity->High->Sprite.SpriteID.SpriteSize) * Scale;
    transform.AtlasOffset = Entity->High->Sprite.SpriteID.AtlasOffset;
    transform.SpriteSize = Entity->High->Sprite.SpriteID.SpriteSize;
    
    RenderData->Transforms[RenderData->TransformCount++] = transform;
}

internal inline void
DrawEntity(uint32 EntityIndex, int8 Scale, GameState *State)
{
    Entity Result = {0};
    Result = GetEntity(EntityIndex, State);
    DrawStaticEntitySprite(&Result, Scale, State->RenderData);
}

internal inline void
CreateEntity(EntityType EntityArchetype, vec2 WorldPosition, int32 Index, GameState *State)
{
    switch(EntityArchetype)
    {
        case Entity_Dice:
        {
            CreateDiceEntity(WorldPosition, Index, State);
        }break;
        case Entity_Tile:
        {
        }break;
    }
}