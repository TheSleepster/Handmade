#include "SugarAPI.h"
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_ECS.h"

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
    Entity.Transform.Size = Size;

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
    Entity.Flags |= (TILE & 1 << 4);

    Array[Index] = Entity;
}

internal void
CreateTile() 
{
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
        case SPRITE_BLUE: 
        {
            Sprite.AtlasOffset = {16, 0};
            Sprite.SpriteSize = {16, 16};
        }
    }
    return(Sprite);
}

internal void 
DrawSprite(SpriteID SpriteID, vec2 Pos, vec2 Size, RenderData *RenderData) 
{
    Sprite Sprite = GetSprite(SpriteID);
    Transform transform = {};
    transform.Position = Pos;
    transform.Size = Size;
    transform.AtlasOffset = Sprite.AtlasOffset;
    transform.SpriteSize = Sprite.SpriteSize;

    RenderData->Transforms[RenderData->TransformCount++] = transform;
}

internal void 
DrawEntity(GameState *State, uint32 Index) 
{
    DrawSprite(State->Entities[Index].Sprite.SpriteID, 
               State->Entities[Index].Transform.CurrentPosition, 
               State->Entities[Index].Transform.Size, 
               State->RenderData);
}

internal void
DestroyEntity(GameState *State, uint32 Index) 
{
    State->Entities[Index] = {};
}
