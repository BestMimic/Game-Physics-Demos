/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "BaseObject.h"

/// \brief The game object. 
///
/// The abstract representation of an object. `CObjectManager` is a friend of
/// this class so that it can access any private members as needed to manage
/// the objects without the need for reader and set functions for each private
/// or protected member variable. This class must contain public member
/// functions `move()` and `draw()` to move and draw the object, respectively.

class CObject: 
  public CCommon,
  public LBaseObject
{
  friend class CObjectManager; ///< Object manager needs access so it can manage.

  private:
    Vector2 m_vOldPos; ///< Previous position. Only needed for Step Mode.
    Vector2 m_vVel; ///< Current velocity.

    float m_fRadius = 0.0f; ///< Radius.
    bool m_bInPocket = false; ///< Whether currently in a pocket.

  public:
    CObject(eSprite t, const Vector2& p); ///< Constructor.
    
    void move(); ///< Move object.
    void draw(); ///< Draw object.

    void DropParticle(LParticleDesc2D*); ///< Drop a particle.
    void DeliverImpulse(float, float); ///< Deliver an impulse.
}; //CObject

#endif //__L4RC_GAME_OBJECT_H__
