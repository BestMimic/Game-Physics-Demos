/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "Renderer.h"

/// This constructor assumes that a Physics World body
/// has already been created for this object. It
/// then has responsibility for deleting it in its destructor.
/// \param t Sprite type.
/// \param b Pointer to Physics World body.

CObject::CObject(eSprite t, b2Body* b):
  m_eSpriteType(t),
  m_pBody(b){
} //constructor

/// This destructor assumes that Box2D hasn't been shut down yet.

CObject::~CObject(){
  if(m_pBody != nullptr)
    m_pPhysicsWorld->DestroyBody(m_pBody);
} //destructor

/// Draw either as a sprite or using lines, or both, depending on `m_eDrawMode`.
/// Position and orientation must be gotten from Physics World.

void CObject::draw(){
  const float a = m_pBody->GetAngle(); //orientation
  const b2Vec2 v = m_pBody->GetPosition(); //position in Physics World units
  
  if(m_eDrawMode == eDrawMode::Sprites || m_eDrawMode == eDrawMode::Both)
    m_pRenderer->Draw(m_eSpriteType, PW2RW(v), a); //draw sprite

  if(m_eDrawMode == eDrawMode::Lines || m_eDrawMode == eDrawMode::Both)
    m_pRenderer->Drawb2Body(eSprite::Line, m_pBody); //draw outline
} //draw

/// Reader function for sprite type.
/// \return Sprite type.

eSprite CObject::GetSpriteType(){
  return m_eSpriteType;
} //GetSpriteType

/// Reader function for position in renderer.
/// \return Position in renderer coordinates.

Vector2 CObject::GetPos(){
  return PW2RW(m_pBody->GetPosition());
} //GetPosition

/// Reader function for speed in renderer.
/// \return Speed in renderer units.

float CObject::GetSpeed(){
  return PW2RW(m_pBody->GetLinearVelocity().Length());
} //GetSpeed