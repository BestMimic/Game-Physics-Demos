/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// This constructor assumes that a Physics World body
/// has already been created for this object. It
/// then has responsibility for deleting it in its destructor.
/// \param t Sprite type.
/// \param b Pointer to Physics World body.

CObject::CObject(eSprite t, b2Body* b){
  m_eSpriteType = t; 
  m_pBody = b;
} //constructor

/// This destructor assumes that Box2D hasn't been shut down yet.

CObject::~CObject(){
  if(m_pBody != nullptr)
    m_pPhysicsWorld->DestroyBody(m_pBody);
} //destructor

/// Draw in Render World.
/// Position and orientation must be gotten from Physics World.

void CObject::draw(){
  const float a = m_pBody->GetAngle(); //orientation
  const b2Vec2 v = m_pBody->GetPosition(); //position in Physics World units

  if(m_eSpriteType != eSprite::Size)
    m_pRenderer->Draw(m_eSpriteType, PW2RW(v), a); //draw in Render World
} //draw

///< Reader function for position.
///< \return Position in Render World units.

const Vector2 CObject::GetPos() const{
  return PW2RW(m_pBody->GetPosition());
} //GetPos

///< Reader function for the sprite type.
///< \return The sprite type.

const eSprite CObject::GetSpriteType() const{
  return m_eSpriteType;
} //GetSpriteType