/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

/// Create an object, given its sprite type and initial position.
/// \param t Type of ball.
/// \param pos Initial position.

CObject::CObject(eSprite t, const Vector2& pos):
  LBaseObject(t, pos),
  m_vOldPos(pos),
  m_fRadius(m_pRenderer->GetWidth(m_nSpriteIndex)/2)
{
  m_fRadius = m_pRenderer->GetWidth(m_nSpriteIndex)/2.0f;
} //constructor

/// Move using Euler Integration, applying a constant amount of friction. Stop
/// if the velocity is sufficiently small. Exactly what constitutes
/// "sufficiently small" is hard-coded.

void CObject::move(){ 
  if(m_bInPocket){ //in pocket, so draw smaller and darker
    m_fXScale = m_fYScale = 0.9f; //smaller
    m_f4Tint = XMFLOAT4(Colors::Gray); //darker
  } //if

  else{ //in play on table
    const float t = m_bStepMode? (m_bStep? 1/30.0f: 0): m_pTimer->GetFrameTime();
    const float SCALE = 50.0f; //to scale up motion
    const float FRICTION = 0.6f; //coefficient of friction

    m_vOldPos = m_vPos; //current position is now the old one
    m_vPos += m_vVel*t*SCALE; //new current position
    m_vVel *= 1.0f - t*FRICTION; //apply friction

    const float MINSPEEDSQ = 0.5f; //minimum speed squared

    if(m_vVel.LengthSquared() < MINSPEEDSQ) //if moving too slowly
      m_vVel = Vector2::Zero; //stop
  } //else
} //move

/// Ask the renderer to draw the sprite described in the sprite descriptor.
/// Note that `CObject` is derived from `LBaseObject` which is inherited from
/// `LSpriteDesc2D`. Therefore `LSpriteRenderer::Draw(const LSpriteDesc2D*)`
/// will accept `this` as a parameter, automatically down-casting it from
/// `CObject*` to `LSpriteDesc2D*`, effectively drawing the object from its
/// sprite descriptor.

void CObject::draw(){ 
  m_pRenderer->Draw(this);
} //draw

/// Drop a particle using the back particle engine provided the object has
/// actually moved in the last frame. This is intended to be used in Step Mode.

void CObject::DropParticle(LParticleDesc2D* p){
  if(m_vPos != m_vOldPos){ //don't drop two particles in the same place
    p->m_vPos = m_vOldPos; 
    m_pParticleEngine->create(*p);
    m_vOldPos = m_vPos; //this ball is forbidden from drawing another particle here
  } //if
} //DropParticle

/// React to an impulse of a given angle and magnitude. The velocity vector is
/// instantaneously changed; no need to faff about with forces applied over time.
/// \param a Impulse angle.
/// \param m Impulse magnitude.

void CObject::DeliverImpulse(float a, float m){ 
  m_vVel = m*Vector2(cosf(a), sinf(a));
} //DeliverImpulse
