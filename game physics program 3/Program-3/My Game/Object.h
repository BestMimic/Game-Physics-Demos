/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "Component.h"
#include "Common.h"
#include "Shape.h"
#include "SpriteDesc.h"

/// \brief Object descriptor.
///
/// Stores information about an object.

class CObjDesc{
  public:
    eSprite m_eUnlitSprite; ///< Unlit sprite.
    eSprite m_eLitSprite; ///< Lit sprite.
    Vector2 m_vSpriteOffset; ///< Sprite offset in local coordinates.
    eSound m_eSound; ///< Collision sound.

    UINT m_nScore; ///< Score for collision.
    
    CObjDesc(eSprite s0, eSprite s1, eSound snd); ///< Constructor.
}; //CObjDesc

/// \brief The game object. 
//
/// CObject is the abstract representation of an object.

class CObject: 
  public LSpriteDesc2D,
  public LComponent,
  public CCommon
{
  friend class CObjectManager;

  private:
    eSprite m_eUnlitSprite = eSprite::Size; ///< Unlit sprite.
    eSprite m_eLitSprite = eSprite::Size; ///< Unlit sprite.

    Vector2 m_vSpriteOffset; ///< Sprite offset in local coordinates.

    CShape* m_pShape = nullptr; ///< Pointer to shape.  
    
    bool m_bRecentHit = false; ///< Was hit recently.
    float m_fLastHitTime = 0; ///< Time of last hit.

    UINT m_nScore = 0; ///< Score for collision.
    eSound m_eSound = eSound::Size; ///< Collision sound.

  public:
    CObject(CShape*, const CObjDesc&); ///< Constructor.

    void Update(); ///< Update object.
    void DrawOutline(); ///< Draw outline.

    const CAabb2D& GetAABB() const; ///< Get AABB.
    CShape* GetShape() const; ///< Get pointer to shape.
    const eMotion GetMotionType() const; ///< Get motion type.
}; //CObject

#endif //__L4RC_GAME_OBJECT_H__