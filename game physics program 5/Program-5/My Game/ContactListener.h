/// \file ContactListener.h
/// \brief Interface for my Box2D contact listeners.

#ifndef __L4RC_GAME_CONTACTLISTENER_H__
#define __L4RC_GAME_CONTACTLISTENER_H__

#include "Component.h"
#include "Common.h"

#include "Box2D\Box2D.h"

/// \brief My contact listener.

class CMyListener: 
  public b2ContactListener,
  public LComponent,
  public CCommon{

  private:
    b2Body* m_pBodyA; ///< Pointer to body A.
    b2Body* m_pBodyB; ///< Pointer to body B.

    UINT Count(eSprite t); ///< Count number of bodies that have sprite type t.
    float GetSpeed(const b2Vec2& p); ///< Get the collision speed.

  public:
    void PreSolve(b2Contact* c, const b2Manifold* m); ///< Presolve function.
}; //CMyListener

#endif //__L4RC_GAME_CONTACTLISTENER_H__
