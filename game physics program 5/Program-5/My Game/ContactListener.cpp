/// \file ContactListener.cpp
/// \brief Code for my Box2D contact listener.

#include "ContactListener.h"

#include "Common.h"
#include "GameDefines.h"
#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "Kraid.h"

//static int health = 0;
/// Count the number of bodies out of *m_pBodyA and *m_pBodyB that have objects
/// have a given sprite type. Returns 0, 1, or 2.
/// \param t Sprite Type.
/// \return Number of *m_pBodyA and *m_pBodyB that have type t.

UINT CMyListener::Count(eSprite t){
  if(m_pBodyA == nullptr || m_pBodyB == nullptr)return 0; //safety

  CObject* objA = (CObject*)m_pBodyA->GetUserData().pointer; //pointer to object A
  CObject* objB = (CObject*)m_pBodyB->GetUserData().pointer; //pointer to object B

  UINT count = 0; //return value

  if(objA && objA->GetSpriteType() == t)count++; //object A is one
  if(objB && objB->GetSpriteType() == t)count++; //object B is one

  return count;
} //Count

/// Collision speed is proportional to the magnitude of the relative velocity.
/// \param wp World point.
/// \return Collision speed in Physics World units.

float CMyListener::GetSpeed(const b2Vec2& p){
  const b2Vec2 vA = m_pBodyA->GetLinearVelocityFromWorldPoint(p); //velocity of body A
  const b2Vec2 vB = m_pBodyB->GetLinearVelocityFromWorldPoint(p); //velocity of body B
  return (vA - vB).Length(); //speed is magnitude of the velocity of one body relative to the other
} //GetSpeed

/// Presolve function. Renders a colored star at each contact point and plays
/// the appropriate sound, depending on what type of objects are contacting.
/// \param c Pointer to the contact.
/// \param m Pointer to the old contact manifold as it was before this contact.

void CMyListener::PreSolve(b2Contact* c, const b2Manifold* m){
  b2WorldManifold wm;
  c->GetWorldManifold(&wm);
  b2PointState state1[2], state2[2];
  b2GetPointStates(state1, state2, m, c->GetManifold());

  for(int i=0; i<2; i++)
    if(state2[i] == b2_addState){
      m_pBodyA = c->GetFixtureA()->GetBody(); //pointer to body A
      m_pBodyB = c->GetFixtureB()->GetBody(); //pointer to body B

      //contact response
      
      const b2Vec2 wp = wm.points[0]; //world point
      const float speed = GetSpeed(wp); //collision speed

      if(speed > 8.0f){ //objects moving fast enough
        const Vector2 pos = PW2RW(wp); //position in renderer

        const int nBalls = Count(eSprite::Ball); //number of balls
        const int nButtons = Count(eSprite::RedButton); //number of buttons
        const int nMetroids = Count(eSprite::Metroid); //number of metroids
        const int nKraid = Count(eSprite::Kraid); //number of Kraid
        const int nKraidArm = Count(eSprite::KraidArm); //number of Kraid arms
        const int nKraidLeg = Count(eSprite::KraidFoot); //number of Kraid feet
        const int nChozo = Count(eSprite::Chozo); //number of Chozo

        const float vol = (speed - 8.0f)/32.0f; //sound volume

        if (nBalls > 0) { //there's a ball involved
            if (nButtons > 0 && m_eGameState != eGameState::Finished) { //ball to button, once only
                m_pAudio->play(eSound::Yay);
                m_eGameState = eGameState::Finished;
                m_fTotalTime = m_pTimer->GetTime() - m_fStartTime;
            } //if
            else if (nMetroids > 0 && m_eGameState != eGameState::Finished) {
                m_pAudio->play(eSound::Attack, PW2RW(wp), vol);
                b2Vec2 test;
                test.Set(100.0f, 100.0f);
                b2Vec2 v;
                v.Set(wp.x * -10, wp.y * -10);
                //v.Set(wp.x*1000000, -wp.y);

                m_pBodyB->ApplyLinearImpulseToCenter(v, true);
            }
            else if (nKraid > 0 && m_eGameState != eGameState::Finished) {
                if (Health > 0) {
                    m_pAudio->play(eSound::Kraid, PW2RW(wp), vol);
                    Health--;
                }
                else {
                    Health--;
                    //m_pKraid->KraidYeet();
                }
            }
            else if (nKraidArm > 0 && m_eGameState != eGameState::Finished) {
                m_pAudio->play(eSound::Attack, PW2RW(wp), vol);
                b2Vec2 v;
                v.Set(400000.0f, 1000.0f);

                m_pBodyB->ApplyLinearImpulseToCenter(v, true);
            }
            else if (nKraidLeg > 0 && m_eGameState != eGameState::Finished) {
                m_pAudio->play(eSound::Kraid, PW2RW(wp), vol);
                m_pAudio->play(eSound::Attack, PW2RW(wp), vol);
                b2Vec2 v;
                v.Set(400000.0f, 1000.0f);

                Health--;

                m_pBodyB->ApplyLinearImpulseToCenter(v, true);
            }
            else if (nChozo > 0 && m_eGameState != eGameState::Finished) {// revers gravity if it touches a chozo
                m_pAudio->play(eSound::Attack, PW2RW(wp), vol);
                b2Vec2 grav;
                grav = m_pPhysicsWorld->GetGravity();
                grav.Set(grav.x, -grav.y);
                /*if ( grav.y > 0) {
                    grav.Set(grav.x, -grav.y);
                }
                else {
                    grav.Set(grav.x, -grav.y);
                }*/
                m_pPhysicsWorld->SetGravity(grav);
            }
            else m_pAudio->play(eSound::Bonk, PW2RW(wp), vol); //everything else
        }
      } //if
    } //if
} //PreSolve