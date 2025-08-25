/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H_

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

/// \brief The game class.

class CGame:   
  public LComponent, //game components from the Engine
  public LSettings, //game settings from gamesettings.xml, via the Engine
  public CCommon
{ 
  private:  
    void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void RenderFrame(); ///< Render an animation frame.
    void DrawBackground(); ///< Draw the background.
    void CreateObjects(); ///< Create game objects.
    void CreateCrate(float, float); ///< Create a crate.
    void CreateRoundThing(eSprite, float, float); ///< Create a round thing.

  public:
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
