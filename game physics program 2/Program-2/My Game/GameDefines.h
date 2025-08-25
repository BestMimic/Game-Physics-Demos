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

enum class eSprite{
  Background, Cueball, Eightball, Arrow, Stepmode, Circle, Thickcircle, Line,
  Size //MUST BE LAST
}; //eSprite

/// \brief Game state.
///
/// State of game play, including whether the player has won or lost.

enum class eGameState{
  Initial, InMotion, SetupShot, Won, Lost
}; //eGameState

/// \brief Game sound enumerated type. 
///
/// These are the sounds used in gameplay. The sounds must be listed here in the
/// same order that they are in the sound settings XML file.

enum class eSound{ 
  Cue, Click, Thump, Pocket, Win, Lose, 
  Size //MUST BE LAST
}; //eSound

#endif //__L4RC_GAME_GAMEDEFINES_H__

