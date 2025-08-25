/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite: UINT{
  None, Background, BlackLine, UnlitSpecial, LitSpecial,
  UnlitTriangle, LitTriangle, UnlitDiamond, LitDiamond, 
  UnlitPentagon, LitPentagon, 
  UnlitSlot, LitSlot, Flipper, Clip, Ball, LED,
  Size //MUST BE LAST
}; //eSprite

/// \brief Draw mode enumerated type.
///
/// An enumerated type for the drawing mode. `Size` must be last.

enum class eDrawMode: UINT{
  Background, Both, Lines,
  Size //MUST be last
}; //eDrawMode

/// \brief Sound enumerated type.
///
/// An enumerated type for the sounds, which will be cast to an unsigned
/// integer and used for the index of the corresponding sample. `Size` must 
/// be last.

enum class eSound: UINT{ 
  Beep, Blaster, Ballclick, Launch, Whiffle, FlipUp, FlipDown, LostBall, Load,
  Click, Tink,
  Size //MUST be last
}; //eSound

#endif //__L4RC_GAME_GAMEDEFINES_H__
