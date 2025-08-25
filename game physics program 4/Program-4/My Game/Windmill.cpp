/// \file Windmill.cpp
/// \brief Code for the windmill class CWindmill.

#include "Windmill.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// Create physics bodies for the windmill's blades and base,
/// and a rotational joint between them.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CWindmill::CWindmill(float x, float y){
  b2Body* pBlade = CreateBlade(x, y);
  b2Body* pBase = CreateBase(x, y);

  //revolute joint definition
  b2RevoluteJointDef wd;

  wd.Initialize(pBase, pBlade, pBlade->GetPosition());
  wd.motorSpeed = -3.0f;
  wd.maxMotorTorque = 4000.0f;
  wd.enableMotor = true;
  wd.collideConnected = false;

  //create revolute joint 
  m_pJoint = (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
} //constructor

/// Create a physics body for the windmill blades in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill blades.

b2Body* CWindmill::CreateBlade(float x, float y){ 
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(x + RW2PW(60), y + RW2PW(200));//60,200

  //create shape
  b2CircleShape s;
  s.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::WindmillBlade)/2.0f);

  //create fixture definition
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.1f;

  //create physics body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::WindmillBlade, pBody);

  pBody->CreateFixture(&fd); //attach fixture

  return pBody;
} //CreateBlade

/// Create a physics body for the windmill base in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill base.

b2Body* CWindmill::CreateBase(float x, float y){ 
  b2BodyDef bd;
  bd.type = b2_staticBody;
  bd.position.Set(x, y);
  
  // this is the middle of the sprite
  float xmid = RW2PW(318) / 2.0f;

  b2Vec2 vertices[5];
  vertices[0].Set(RW2PW(4) - xmid, y - RW2PW(0));
  vertices[1].Set(RW2PW(0) - xmid, y - RW2PW(101));
  vertices[2].Set(RW2PW(117) - xmid, y - RW2PW(87));
  vertices[3].Set(RW2PW(130) - xmid, y - RW2PW(58));
  vertices[4].Set(RW2PW(118) - xmid, y - RW2PW(29));
  b2PolygonShape flag;
  flag.Set(vertices, 5);

  vertices[0].Set(RW2PW(235) - xmid, y - RW2PW(35));
  vertices[1].Set(RW2PW(174) - xmid, y - RW2PW(539));
  vertices[2].Set(RW2PW(297) - xmid, y - RW2PW(539));
  b2PolygonShape base;
  base.Set(vertices, 3);

  vertices[0].Set(RW2PW(162) - xmid, y - RW2PW(252));
  vertices[1].Set(RW2PW(199) - xmid, y - RW2PW(230));
  vertices[2].Set(RW2PW(314) - xmid, y - RW2PW(251));
  b2PolygonShape cros;
  cros.Set(vertices, 3);

  //create physics body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::WindmillBase, pBody);

  pBody->CreateFixture(&flag, 0.1f);
  pBody->CreateFixture(&base, 0.1f);
  pBody->CreateFixture(&cros, 0.1f);

  return pBody;
} //CreateBase

/// Reverse the windmill by setting the motor speed on the
/// revolute joint.

void CWindmill::reverse(){
  m_pJoint->SetMotorSpeed(-m_pJoint->GetMotorSpeed());
} //reverse