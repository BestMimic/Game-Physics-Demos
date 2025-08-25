/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Settings.h"
#include "SpriteDesc.h"
#include "SpriteRenderer.h"
#include "EventTimer.h"

/// \brief The game class.
///
/// The game class is the object-oriented implementation of the game. This class
/// must contain the following public member functions. `Initialize()` does
/// initialization and will be run exactly once at the start of the game.
/// `ProcessFrame()` will be called once per frame to create and render the
/// next animation frame. `Release()` will be called at game exit but before
/// any destructors are run.

class CGame: 
  public LComponent, 
  public LSettings{ 

  private:
    bool m_bDrawFrameRate = false; ///< Draw the frame rate.
    float m_fPrevTicks = 0.0f; ///< Time of previous tick.
    LSpriteRenderer* m_pRenderer = nullptr; ///< Pointer to renderer.
    
    void LoadImages(); ///< Load images.
    void LoadSounds(); ///< Load sounds.
    void KeyboardHandler(); ///< The keyboard handler.
    void RenderFrame(); ///< Render an animation frame.
    void DrawFrameRateText(); ///< Draw frame rate text to screen.
    void RenderDateAndTime(time_t); ///< Render date and time.
    void RenderClockHands(time_t); ///< Render clock hands.
    void RenderArrowhead(const Vector2&); ///< Render arrowhead on a hand.
    Vector2 GetHandEnd(float, float) const; ///< Get position of end of hand.

  public:
    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
