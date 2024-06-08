#include "SugarAPI.h"
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_ECS.h"
#include "Sugar_Input.h"


extern "C"
{
#include "../data/deps/JSON/cJSON.h"
#include "../data/deps/JSON/cJSON_Utils.h"
}

#define s_Size(Array, Type) (sizeof(Array) / sizeof(Type))

// NOTE :
// Perhaps a better idea for dynamic arrays is too not have them at all,
// and Instead when we want to add an entity, simply check if it's index is nullptr then overwrite it.
// then have an iterator that's checking to see if the entity is alive. If it's dead, set it to nullptr.

internal void
CreateEntity(SpriteID SpriteID, vec2 Pos, vec2 Size, uint16 Index, Entity *Array) 
{ 
    Entity Entity = {};
    
    Entity.Sprite.SpriteID = SpriteID;
    
    Entity.Transform.CurrentPosition = Pos;
    Entity.Transform.Size = Size;
    Entity.Transform.CurrentVelocity = {0, 0};
    
    Entity.Collider.Vertices[0] = 
    {Entity.Transform.CurrentPosition.x, 
        Entity.Transform.CurrentPosition.y};
    
    Entity.Collider.Vertices[1] = 
    {Entity.Transform.CurrentPosition.x + Entity.Transform.Size.x, 
        Entity.Transform.CurrentPosition.y};
    
    Entity.Collider.Vertices[2] = 
    {Entity.Transform.CurrentPosition.x + Entity.Transform.Size.x, 
        Entity.Transform.CurrentPosition.y + Entity.Transform.Size.y};
    
    Entity.Collider.Vertices[3] = 
    {Entity.Transform.CurrentPosition.x, 
        Entity.Transform.CurrentPosition.y + Entity.Transform.Size.y};
    Entity.Collider.VertexCount = 4;
    
    Entity.Flags |= (ACTIVE & 1 << 0);
    Entity.Flags |= (IS_STATIC & 1 << 2);
    
    Array[Index] = Entity;
}

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

internal void 
DrawSprite(SpriteID SpriteID, vec2 Pos, vec2 Scale, RenderData *RenderData) 
{
    Sprite Sprite = GetSprite(SpriteID);
    Transform transform = {};
    transform.Position = Pos - ((v2Convert(Sprite.SpriteSize) / 2) * Scale);
    transform.Size = v2Convert(Sprite.SpriteSize) * Scale;
    transform.AtlasOffset = Sprite.AtlasOffset;
    transform.SpriteSize = Sprite.SpriteSize;
    
    RenderData->Transforms[RenderData->TransformCount++] = transform;
}

internal void 
DrawEntity(GameState *State, uint32 Index) 
{
    DrawSprite(State->HighEntities[Index].Sprite.SpriteID, 
               State->HighEntities[Index].Transform.CurrentPosition, 
               State->HighEntities[Index].Transform.Size, 
               State->RenderData);
}


internal void 
CreateTileFromGrid(ivec2 GridPosition, Input *Input, GameState *State)
{
    Entity Tile = {0};
    Tile.Sprite.SpriteID = SPRITE_FLOOR;
    Tile.Flags = ACTIVE | IS_STATIC | TILE | RENDERABLE | IS_VISIBLE;
    Tile.Transform.CurrentPosition = v2Convert(GridPosition) * TILE_SIZE;
    
    State->Level->Tilemap[GridPosition.x][GridPosition.y] = Tile;
}


internal void
DrawTileSprite(Entity *Tile, GameState *State)
{
    Sprite Sprite = GetSprite(Tile->Sprite.SpriteID);
    Transform Transform = {0};
    Transform.Position = (Tile->Transform.CurrentPosition) - v2Cast(Sprite.SpriteSize / 2);
    Transform.Size = v2Convert(Sprite.SpriteSize);
    Transform.AtlasOffset = (Sprite.AtlasOffset);
    Transform.SpriteSize = Sprite.SpriteSize;
    
    State->RenderData->Transforms[State->RenderData->TransformCount++] = Transform;
}

// TODO(Sleepster): Rename to SugarGlider
internal Entity *
GetTile(ivec2 GridPos, GameState *State)
{
    Entity *Tile = {};
    Tile = &State->Level->Tilemap[GridPos.x][GridPos.y];
    return(Tile);
}

internal void 
TestFunction(void)
{
}

#if 0

internal Entity *
GetTile(vec2 WorldPos, GameState *State)
{
    ivec2 GridPos = iv2Cast(WorldPos) / TILE_SIZE;
    return(GetTile(GridPos, State));
}

internal ivec2 
CursorToWorldPosition(Input *Input)
{
    ivec2 Result = Input->Keyboard.CurrentMouse / TILE_SIZE;
    return(Result);
}

internal void 
CreateTile(vec2 WorldPosition, Input *Input, GameState *State)
{
    Entity Tile = {0};
    Tile.Sprite.SpriteID = SPRITE_FLOOR;
    Tile.Flags = ACTIVE | IS_STATIC | TILE | RENDERABLE | IS_VISIBLE;
    Tile.Transform.CurrentPosition = WorldPosition;
    
    ivec2 GridPosition = iv2Cast(WorldPosition) / TILE_SIZE;
    State->Level->Tilemap[GridPosition.x][GridPosition.y] = Tile;
}


internal void
DestroyEntity(GameState *State, EntityType Order, uint32 Index) 
{
    if(Order == HIGH_ORDER)
    {
        State->HighEntities[Index] = {0};
    }
    else
    {
        State->LowEntities[Index] = {0};
    }
}
#endif
