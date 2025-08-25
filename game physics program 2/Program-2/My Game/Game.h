/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

/// \brief The game class.
///
/// The game class encapsulates the game.

class CGame: 
  public LComponent, //game components from the Engine
  public LSettings, //game settings from gamesettings.xml, via the Engine
  public CCommon //common parameters for this game
{
  private:  
    bool m_bDrawFrameRate = false; ///< Draw the frame rate.
    eGameState m_eGameState = eGameState::Initial; ///< Current game state.
    float m_fGameStateTime = 0; ///< Timer for game state changes.
    
    void LoadSounds(); ///< Load sounds.
    void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void RenderFrame(); ///< Render an animation frame.
    void DrawFrameRateText(); ///< Draw frame rate text to screen.
    void CreateObjects(); ///< Create game objects.

    void ProcessState(); ///< Process the game state.

  public:
    CGame(); ///< Constructor.
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
