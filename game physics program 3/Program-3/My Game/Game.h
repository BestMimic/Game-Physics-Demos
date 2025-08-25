/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

/// \brief The game class.

class CGame: 
  public LComponent, 
  public LSettings, 
  public CCommon{ 

  private:  
    static const UINT NUMSCOREDIGITS = 6; ///< Number of digits in score.
    
    CObjectManager* m_pObjectManager = nullptr; ///< Pointer to object manager.
    
    LSpriteDesc2D m_cClipDesc0; ///< Sprite descriptor for clip 0.
    LSpriteDesc2D m_cClipDesc1; ///< Sprite descriptor for clip 0.
    LSpriteDesc2D m_cScoreDesc[NUMSCOREDIGITS]; ///< Sprite descriptors for score digits.
    
    CDynamicCircle* m_pCurBallShape = nullptr; ///< Pointer to current ball shape.
    
    void LoadSounds(); ///< Load sounds.
    void BeginGame(); ///< Begin playing the game.
    void KeyboardHandler(); ///< The keyboard handler.
    void RenderFrame(); ///< Render an animation frame.

    void Launch(); ///< Launch a ball.

  public:
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
