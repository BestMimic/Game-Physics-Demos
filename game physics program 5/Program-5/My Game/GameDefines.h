/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"
#include "Box2D\Box2D.h"

/// \brief Sprite enumerated type.

enum class eSprite{  
  Background, Line, RedButton, ClockFace, Ball, Metroid, Kraid, KraidArm, KraidFoot, KraidDead, Chozo, Triangle, FlipTriangle, BigLine,
  Size //MUST BE LAST
}; //eSprite

/// \brief Game state enumerated type.
///
/// State of game play, including whether the player has won or lost.

enum class eGameState{
  Initial, Running, Finished
}; //eGameState

/// \brief Draw mode enumerated type.
///
/// An enumerated type for the drawing mode. `Size` must be last.

enum class eDrawMode: UINT{
  Sprites, Both, Lines,
  Size //MUST be last
}; //eDrawMode

/// Game sound enumerated type. 
///
/// These are the sounds used in actual gameplay. 

enum eSound{ 
  Whoosh, Yay, Bonk, Buzz, Restart, Bgm, Boss, Attack, Kraid, Boom,
  Size //MUST BE LAST
}; //eSound

//Translate units between renderer and Physics World

const float fPRV = 10.0f; ///< Physics World to renderer rescale value.

/// \brief Physics World to renderer units for a float.
inline float PW2RW(float x){return x*fPRV;}; 

/// \brief Physics World to renderer units for a vector.
inline Vector2 PW2RW(const b2Vec2& v){return Vector2(v.x, v.y)*fPRV;}; 

/// \brief renderer to Physics World units for a float.
inline float RW2PW(float x){return x/fPRV;}; 

/// \brief renderer to Physics World units for an int.
inline float RW2PW(int x){return x/fPRV;}; 

/// \brief renderer to Physics World units for an unsigned int.
inline float RW2PW(unsigned x){return x/fPRV;}; 

/// \brief renderer to Physics World units for a vector provided as a pair of ints.
inline b2Vec2 RW2PW(int x, int y){return b2Vec2(x/fPRV, y/fPRV);};

/// \brief renderer to Physics World units for a vector.
inline b2Vec2 RW2PW(const Vector2& v){return b2Vec2(v.x/fPRV, v.y/fPRV);};

#endif //__L4RC_GAME_GAMEDEFINES_H__
