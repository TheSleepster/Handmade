#include "SugarAPI.h"
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

extern "C"
{
#include "../data/deps/JSON/cJSON.h"
#include "../data/deps/JSON/cJSON.c"
}

#define ArrayCount(Array, Type) (sizeof(Array) / sizeof(Type))

// NOTE :
// Perhaps a better idea for dynamic arrays is too not have them at all,
// and Instead when we want to add an entity, simply check if it's index is nullptr then overwrite it.
// then have an iterator that's checking to see if the entity is alive. If it's dead, set it to nullptr.

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
        uint32 HighIndex = State->HighEntityCount++;
        HighEntity *EntityHigh = &State->HighEntities[HighIndex];
        
        EntityLow->HighEntityIndex = HighIndex;
    }
    else
    {
        Trace("Invalid Code Path\n");
    }
}

// TODO(Sleepster): Free List perhaps? This would store "Freed" Elements, they would be the first to be overwritten with new data
internal void
DemoteEntityToLowRange(int32 HighIndex, GameState *State)
{
    HighEntity *EntityHigh = &State->HighEntities[HighIndex];
    
    // TODO(Sleepster): Finish this function
    if(!(State->LowEntityCount < ArrayCount(State->LowEntities, LowEntity)))
    {
        uint32 LowIndex = State->LowEntityCount++;
        LowEntity *EntityLow = &State->LowEntities[LowIndex];
        
        EntityHigh->LowEntityIndex = LowIndex;
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
    
    transform.Position = Entity->Low->Transform.CurrentPosition;
    transform.Size = v2Cast(Entity->Low->Sprite.SpriteID.SpriteSize) * Scale;
    transform.AtlasOffset = Entity->Low->Sprite.SpriteID.AtlasOffset;
    transform.SpriteSize = Entity->Low->Sprite.SpriteID.SpriteSize;
    
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