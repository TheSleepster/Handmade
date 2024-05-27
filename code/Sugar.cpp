#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "Sugar_Input.h"

// TODO : Should the GameUpdateAndRender function ACTUALLY do the rendering? Should we be treating the OpenGL
// Renderer in the same way we are treating the platform? Where instead of the GAME Rendering the items. We call
// out to the renderer with information about what needs to be rendered.

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender) 
{
    GameRenderData = GameRenderDataIn; 

    if(IsGameKeyDown(ATTACK, GameInput))
    {
        DrawSprite(SPRITE_DICE, 
                 {(real32)GameInput->Keyboard.CurrentMouse.x - 50, 
                  (real32)GameInput->Keyboard.CurrentMouse.y - 50}, 
                  {100, 100});
    }
}
