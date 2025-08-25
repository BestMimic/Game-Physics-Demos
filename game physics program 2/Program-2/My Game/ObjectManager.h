/// \file ObjectManager.h
/// \brief Interface for the Object manager class CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include "GameDefines.h"

#include "BaseObjectManager.h"
#include "Object.h"
#include "Common.h"

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public LBaseObjectManager<CObject>,
  public CCommon
{
  private:
    LParticleDesc2D m_cPDesc0; ///< Particle descriptor for balls in step mode.
    LParticleDesc2D m_cPDesc1; ///< Particle descriptor for collisions in step mode.
    LParticleDesc2D m_cPDesc2; ///< Particle descriptor for collisions in real-time mode.
    LParticleDesc2D m_cPDesc3; ///< Particle descriptor for target ball

    CObject* m_pCueBall = nullptr; ///< Cue ball object pointer.
    CObject* m_p8Ball = nullptr; ///< 8 ball object pointer.

    float m_fCueAngle = 0; ///< Cue ball impulse angle.
    bool m_bDrawImpulseVector = true; ///< Whether to draw the impulse vector.

    bool m_bDrawCircle = true; // new addition
    
  private:
    void BroadPhase(); ///< Ball, rail, and pocket collision response for all balls.
    void NarrowPhase(CObject*, CObject*); ///< Ball collision response for two balls.

    bool BallCollide(CObject*, CObject*, float&); ///< Ball collision response for two balls.
    bool MimicBallCollide(CObject*, CObject*, float&); ///< for predicting a collision
    void RailCollide(CObject*); ///< Collision response for ball with rail.
    void PocketCollide(CObject*); ///< Collision response for ball with pocket.

  public:
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.

    void create(eSprite, const Vector2&); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    
    void Draw(); ///< Draw all objects.
    
    void ResetImpulseVector(); ///< Reset the Impulse Vector.
    void AdjustImpulseVector(float); ///< Adjust the Impulse Vector.
    void AdjustCueBall(float); ///< Move cue-ball up or down.
    void Shoot(); ///< Shoot the cue ball.

    bool BallDown(); ///< Is a ball down in a pocket?
    bool CueBallDown(); ///< Is the cue ball down in a pocket?
    bool AllStopped(); ///< Have all balls stopped moving?
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
