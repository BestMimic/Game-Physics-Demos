/// \file Kraid.cpp
/// \brief Code for the windmill class CWindmill.

#include "Kraid.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// Create physics bodies for the windmill's blades and base,
/// and a rotational joint between them.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CKraid::CKraid(float x, float y) {
    pBlade = CreateBlade(x, y);
    pBase = CreateBase(x, y);
    pFoot = CreateFoot(x, y);

    /*revolute joint definition*/
    b2RevoluteJointDef wd;

    wd.Initialize(pBase, pBlade, pBlade->GetPosition());
    wd.motorSpeed = -20.0f;
    wd.maxMotorTorque = 4000.0f;
    wd.enableMotor = true;
    wd.collideConnected = false;

    /*create revolute joint*/ 
    m_pJoint = (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
} //constructor

/// Create a physics body for the windmill blades in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill blades.

b2Body* CKraid::CreateBlade(float x, float y) {
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(x + RW2PW(-370), y + RW2PW(-72));//60,200

    //create shape
    b2CircleShape s;
    s.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::KraidArm) / 2.0f);

    //create fixture definition
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 0.1f;

    //create physics body
    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    m_pObjectManager->create(eSprite::KraidArm, pBody);

    pBody->CreateFixture(&fd); //attach fixture

    return pBody;
} //CreateBlade

/// Create a physics body for the windmill base in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill base.

b2Body* CKraid::CreateBase(float x, float y) {
    float xmid = RW2PW(318) / 2.0f;

    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(x-42, y-11.5f);
    //bd.position.Set(RW2PW(4) - xmid, y - RW2PW(0));

    float w, h;
    m_pRenderer->GetSize(eSprite::Kraid, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 4.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.3f;

    b2Body* pBase = m_pPhysicsWorld->CreateBody(&bd);
    pBase->CreateFixture(&fd);
    m_pObjectManager->CreateObject(eSprite::Kraid, pBase);
    return pBase;
} //CreateBase

b2Body* CKraid::CreateFoot(float x, float y) {
    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(x - 33, y - 23);
    //bd.position.Set(RW2PW(4) - xmid, y - RW2PW(0));

    float w, h;
    m_pRenderer->GetSize(eSprite::KraidFoot, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.3f;

    b2Body* pBase = m_pPhysicsWorld->CreateBody(&bd);
    pBase->CreateFixture(&fd);
    m_pObjectManager->CreateObject(eSprite::KraidFoot, pBase);
    return pBase;
} //CreatFoot

void CKraid::KraidYeet() {
    m_pPhysicsWorld->DestroyBody(pBase);
    m_pPhysicsWorld->DestroyBody(pBlade);
    m_pPhysicsWorld->DestroyBody(pFoot);

    m_pAudio->play(eSound::Boom);
}

/// Reverse the windmill by setting the motor speed on the
/// revolute joint.

void CKraid::reverse() {
    m_pJoint->SetMotorSpeed(-m_pJoint->GetMotorSpeed());
} //reverse