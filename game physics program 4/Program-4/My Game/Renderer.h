/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#ifndef __L4RC_GAME_RENDERER_H__
#define __L4RC_GAME_RENDERER_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// The renderer handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.

class CRenderer: public LSpriteRenderer{
  public:
    CRenderer(); ///< Constructor.
    void LoadImages(); ///< Load images.
}; //CRenderer

#endif //__L4RC_GAME_RENDERER_H__
