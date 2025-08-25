/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "Common.h"
#include "Component.h"

/// \brief The game object.
///
/// Game objects are responsible for remembering information about themselves,
/// in particular, their representations in Render World and Physics World.

class CObject: 
  public CCommon,
  public LComponent
{ 
  private:
    eSprite m_eSpriteType = eSprite::Size; ///< Sprite type.
    b2Body* m_pBody = nullptr; ///< Physics World body.

  public:
    CObject(eSprite, b2Body*); ///< Constructor.
    ~CObject(); ///< Destructor.

    void draw(); ///< Draw object in Render World.
    const Vector2 GetPos() const; ///< Get position in Render World.
    const eSprite GetSpriteType() const; ///< Get sprite type.
}; //CObject

#endif //__L4RC_GAME_OBJECT_H__
