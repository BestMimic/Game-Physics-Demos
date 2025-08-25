/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"
#include "Box2D\Box2D.h"

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite: UINT{
  WindmillBg, WindmillBase, WindmillBlade, Tire, Crate, Ball,
  Size //MUST be last
}; //eSprite

//Translate units between Render World and Physics World

const float fPRV = 10.0f; ///< Physics World to Render World rescale value.

/// \brief Physics World to Render World units for a float.
inline float PW2RW(float x){return x*fPRV;}; 

/// \brief Physics World to Render World units for a vector.
inline Vector2 PW2RW(const b2Vec2& v){return Vector2(v.x, v.y)*fPRV;}; 

/// \brief Render World to Physics World units for a float.
inline float RW2PW(float x){return x/fPRV;}; 

/// \brief Render World to Physics World units for an int.
inline float RW2PW(int x){return x/fPRV;}; 

/// \brief Render World to Physics World units for a vector provided as a pair of ints.
inline b2Vec2 RW2PW(int x, int y){return b2Vec2(x/fPRV, y/fPRV);};

/// \brief Render World to Physics World units for a vector.
inline b2Vec2 RW2PW(const Vector2& v){return b2Vec2(v.x/fPRV, v.y/fPRV);};

#endif
