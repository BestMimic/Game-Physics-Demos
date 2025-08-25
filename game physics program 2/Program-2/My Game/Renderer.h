/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#ifndef __L4RC_GAME_RENDERER_H__
#define __L4RC_GAME_RENDERER_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// CRenderer handles the game-specific rendering tasks, relying on the base
/// class CSpriteRenderer to do all of the actual API-specific rendering.

class CRenderer: public LSpriteRenderer{
  public:
    CRenderer(); ///< Constructor.

    void LoadImages(); ///< Load images.
    void DrawMessage(eGameState); ///< Draw win/lose text message to screen.
    
    void DrawLine(const Vector2& p0, const Vector2& p1, XMVECTORF32 c); ///< Draw colored line.
}; //CRenderer

#endif //__L4RC_GAME_RENDERER_H__
