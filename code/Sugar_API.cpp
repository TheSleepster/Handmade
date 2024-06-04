#include "SugarAPI.h"
#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"

#include "Sugar_ECS.h"

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

// TODO : Allow users to pass in the type of Collider/Flags.
//        Perhaps make helper functions for generic Archetypes? (Dice, Player, Enemy, Wall, etc.)
internal bool 
CreateEntity(SpriteID SpriteID, vec2 Pos, vec2 Size, DynamicArray *Array, BumpAllocator *Memory) 
{
    Entity Entity = {};

    Entity.Sprite.SpriteID = SpriteID;
    Entity.Transform.Size = Size;
    Entity.EntityID = Array->CurrentSize;

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

    if(!ArrayAdd(Array, Memory, (void *)&Entity, Entity.EntityID)) 
    {
        ArrayGrow(Memory, Array);
        ArrayAdd(Array, Memory, (void *)&Entity, Entity.EntityID);
    }

    return(1);
}

internal void
KillEntity(GameState *State, uint32 Index) 
{
    ArrayRemove(&State->Entities, Index);
}

internal void 
DrawEntity(GameState *State, uint32 Index) 
{
    Entity *IndexEntity = (Entity *)ArrayGetElement(&State->Entities, Index);
    DrawSprite(IndexEntity->Sprite.SpriteID, IndexEntity->Transform.CurrentPosition, IndexEntity->Transform.Size, State->RenderData);
}
