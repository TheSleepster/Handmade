#pragma once

#include "Sugar_Intrinsics.h"
#include "util/Sugar_Math.h"
#include "win32_Sugar.h"
#include "Sugar.h"
#include "SugarAPI.h"

struct RigidBody 
{
    vec2 Position;
    vec2 Velocity;
    vec2 Size;

    // Restitution is an indicator of how Elastic/Inelastic the collision is.
    real32 Restitution;
    real32 Density;
    real32 Area;
    //real32 Rotation;

    //bool IsStatic;
};
