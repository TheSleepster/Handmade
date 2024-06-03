#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"

#include "Sugar_ECS.h"

// NOTE : Maybe look into a function for each specific type of entity rather than 1 massive switch-case

// TODO : ID manager?
global_variable uint8 EntityIndex = 0;

internal bool 
CreateDiceEntity(vec2 Pos, vec2 Scale, GameState *State) 
{
    Entity Entity = {};

    Entity.Sprite = SPRITE_DICE;
    Entity.PhysicsBody.Size = Scale;
    Entity.EntityID = EntityIndex;

    Entity.PhysicsBody.Position = Pos;
    Entity.PhysicsBody.Size = Scale;
    Entity.PhysicsBody.Velocity = {0, 0};

    Entity.Collider.Vertices[0] = 
        {Entity.PhysicsBody.Position.x, 
         Entity.PhysicsBody.Position.y};

    Entity.Collider.Vertices[1] = 
        {Entity.PhysicsBody.Position.x + Entity.PhysicsBody.Size.x, 
         Entity.PhysicsBody.Position.y};

    Entity.Collider.Vertices[2] = 
        {Entity.PhysicsBody.Position.x + Entity.PhysicsBody.Size.x, 
         Entity.PhysicsBody.Position.y + Entity.PhysicsBody.Size.y};

    Entity.Collider.Vertices[3] = 
        {Entity.PhysicsBody.Position.x, 
         Entity.PhysicsBody.Position.y + Entity.PhysicsBody.Size.y};
    Entity.Collider.VertexCount = 4;
    
    Entity.Flags |= (ACTIVE & 1 << 0);
    Entity.Flags |= (IS_STATIC & 1 << 2);
    Entity.Flags |= (TILE & 1 << 4);

    State->Entities[EntityIndex] = Entity;
    ++EntityIndex;
    return(1);
}

internal void
DrawEntity(Entity *Entity) 
{
    DrawSprite(Entity->Sprite, Entity->PhysicsBody.Position, Entity->PhysicsBody.Size);
}

// NOTE : This is a potential way of doing this system in the future. 
//        It seems a little naive but perhaps this will allow better organization for entities
//        in the long run
#if 0

struct Entity 
{
    uint64 *Flags;
    uint32 EntityID;

    RigidBody PhysicsBody;
    Collider CollisionData;
    SpriteID Sprite;
};

struct GameEntityData 
{ 
    union 
    {
        Entity Tiles [255];
        Entity Enemies[255];
        Entity Pots[255];
        Entity Doors[255];
    };
};

struct GameState 
{
    GameEntityData *GameEntityData;
    Entity Player;
};
#endif

