#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Physics.h"

enum EntityType 
{ 
    CIRCLE, 
    SQUARE,
    ENTITYTYPECOUNT
};

struct Entity 
{
    RigidBody PhysicsBody;
    Collider CollisionData;
    SpriteID Sprite;
};

struct GameEntityData 
{ 
    Entity Players[255];
    Entity Tiles [255];
    Entity Enemies[255];
    Entity Pots[255];
    Entity Doors[255];
};

struct GameState 
{
    GameEntityData *GameEntityData;
};
