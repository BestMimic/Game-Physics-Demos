/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "Component.h"
#include "Common.h"
#include "SpriteDesc.h"

/// \brief The game object.
///
/// Game objects are responsible for remembering information about themselves,
/// in particular, their representations in renderer and Physics World.

class CObject: 
  public LComponent,
  public LSpriteDesc2D,
  public CCommon
{ 
  private:
    eSprite m_eSpriteType = eSprite::Size; ///< Sprite type.
    b2Body* m_pBody; ///< Physics World body.

  public:
    CObject(eSprite, b2Body*); ///< Constructor.
    ~CObject(); ///< Destructor.

    void draw(); ///< Draw object.
    eSprite GetSpriteType(); ///< Get sprite type.
    Vector2 GetPos(); ///< Get position in renderer coordinates.
    float GetSpeed();  ///< Get speed in renderer units.
}; //CObject

#endif //__L4RC_GAME_OBJECT_H__
