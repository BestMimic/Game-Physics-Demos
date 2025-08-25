/// \file Renderer.h 
/// \brief Interface for the renderer class CRenderer.

#ifndef __L4RC_GAME_RENDERER_H__
#define __L4RC_GAME_RENDERER_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"

/// \brief The renderer.
///
/// The renderer world handles the game-specific rendering tasks, relying on
/// the base class to do all of the actual API-specific rendering.

class CRenderer: public LSpriteRenderer{
  private:
    void Drawb2Shape(eSprite, b2Shape*, const b2Vec2, float); ///< Draw Box2D shape.

    void Drawb2Polygon(eSprite, b2PolygonShape*, const b2Vec2, float); ///< Draw Box2D polygon shape.
    void Drawb2Edge(eSprite, b2EdgeShape*, const b2Vec2); ///< Draw Box2D shape.
    void Drawb2Circle(eSprite, b2CircleShape*, const b2Vec2); ///< Draw Box2D edge shape.
    void Drawb2Chain(eSprite, b2ChainShape*, const b2Vec2); ///< Draw Box2D chain shape.

  public:
    CRenderer(); ///< Constructor.

    void LoadImages(); ///< Load images.
    void Drawb2Body(eSprite, b2Body*); ///< Draw Box2D body.
}; //CRenderer

#endif //__L4RC_GAME_RENDERER_H__
