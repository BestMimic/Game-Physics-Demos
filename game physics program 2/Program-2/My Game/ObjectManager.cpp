/// \file ObjectManager.cpp
/// \brief Code for the object manager class CObjectManager.

#include "ObjectManager.h"

#include "ComponentIncludes.h"
#include "ParticleEngine.h"

CObjectManager::CObjectManager(){
  m_cPDesc0.m_nSpriteIndex = (int)eSprite::Circle;
  m_cPDesc0.m_fLifeSpan = 15.0f;
  m_cPDesc0.m_fFadeOutFrac = 0.2f;

  m_cPDesc1.m_nSpriteIndex = (int)eSprite::Thickcircle;
  m_cPDesc1.m_f4Tint = (XMFLOAT4)Colors::Yellow;
  m_cPDesc1.m_fLifeSpan = 15.0f;
  m_cPDesc1.m_fFadeOutFrac = 0.2f;
  
  m_cPDesc2.m_nSpriteIndex = (int)eSprite::Thickcircle;
  m_cPDesc2.m_f4Tint = (XMFLOAT4)Colors::Yellow; 
  m_cPDesc2.m_fLifeSpan = 2.0f;
  m_cPDesc2.m_fScaleOutFrac = 0.1f;

  m_cPDesc3.m_nSpriteIndex = (int)eSprite::Circle;
  m_cPDesc3.m_fLifeSpan = 0.02f;
} //constructor

CObjectManager::~CObjectManager(){
  delete m_pCueBall; //delete the cue ball
  delete m_p8Ball; //delete the 8 ball
} //destructor

/// Create an object and put a pointer to it on the object list.
/// \param t Sprite type.
/// \param v Initial position.

void CObjectManager::create(eSprite t, const Vector2& v){
  const float r = m_pRenderer->GetWidth((int)t)/2.0f; //ball radius
  CObject* b = new CObject(t, v); //conjure a ball

  if(t == eSprite::Cueball) 
    m_pCueBall = b; //save cue-ball pointer

  else if(t == eSprite::Eightball)
    m_p8Ball = b; //save 8-ball pointer  
} //create

/// Delete all of the objects in the game. 

void CObjectManager::clear(){
  delete m_pCueBall; //delete the cue ball
  delete m_p8Ball; //delete the 8 ball
} //clear

/// Draw all of the objects in the game, two balls, the directional arrow, and
/// the step mode indicator if necessary.

void CObjectManager::Draw(){
  if(m_bStepMode) //draw step mode indicator
    m_pRenderer->Draw(eSprite::Stepmode, Vector2(120.0f, 120.0f));
  
  if(m_bShowCollisions){
    LSpriteDesc2D desc;
    desc.m_nSpriteIndex = (UINT)eSprite::Thickcircle;
    desc.m_f4Tint = (XMFLOAT4)Colors::Black;
    desc.m_vPos = Vector2(120.0f, 120.0f);
    m_pRenderer->Draw(&desc);
  } //if

  if(m_bDrawImpulseVector){ //draw arrow under cue ball
    const float d = m_pRenderer->GetWidth(eSprite::Arrow)/2; //distance to arrow center
    const Vector2 dv = d*Vector2(cosf(m_fCueAngle), sinf(m_fCueAngle)); //vector to arrow center
    m_pRenderer->Draw(eSprite::Arrow, m_pCueBall->m_vPos + dv, m_fCueAngle);
  } //if

  // this is probably where to put the new code
  if (m_bDrawCircle) {
      float vol = 0.0;
      MimicBallCollide(m_p8Ball, m_pCueBall, vol);
  }

  m_pCueBall->draw(); //draw cue-ball
  m_p8Ball->draw(); //draw 8-ball
} //Draw

/// Move all of the objects in the object list and perform broad phase 
/// collision detection and response. If in Step Mode, drop a particle.

void CObjectManager::move(){
  m_p8Ball->move(); //move 8-ball
  m_pCueBall->move(); //move cue-ball

  BroadPhase(); //broad phase collision detection and response
  
  if(m_bStepMode){
    m_cPDesc0.m_f4Tint = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    m_p8Ball->DropParticle(&m_cPDesc0);

    m_cPDesc0.m_f4Tint = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0);
    m_pCueBall->DropParticle(&m_cPDesc0);
  } //if
} //move

/// Make the impulse vector point from the center of the cue-ball  to the center
/// of the 8-ball and set it as visible so it gets drawn (assuming that the only
/// reason to reset the impulse vector is because it needs to be drawn).

void CObjectManager::ResetImpulseVector(){
  m_bDrawImpulseVector = true;
  m_bDrawCircle = true;
  const Vector2 v = m_p8Ball->m_vPos - m_pCueBall->m_vPos; //difference in positions
  m_fCueAngle = atan2f(v.y, v.x);
} //ResetImpulseVector

/// Adjust the angle of the impulse vector. This should only be used in
/// eGameState::Initial or eGameState::SetupShot, but no check is made here.
/// \param a Amount to add to the angle of the impulse vector.

void CObjectManager::AdjustImpulseVector(float a){
  m_fCueAngle += a;
} //AdjustImpulseVector

/// Adjust the cue ball up or down on the base line, being careful stop it at
/// the horizontal rails. This should only be used in eGameState::Initial,
/// but no check is made here.
/// \param d Distance to move by.

void CObjectManager::AdjustCueBall(float d){
  if(m_pCueBall){ //safety
    const float r = m_pCueBall->m_fRadius; //ball radius 
    float& y = m_pCueBall->m_vPos.y; //shorthand
    y += d; //move it vertically  
    y = (std::max)((std::min)(y, m_nWinHeight - r), r); //clamp between top and bottom of the table
  } //if
} //AdjustCueBall

/// Shoot the cue-ball by giving it a fixed impulse. Disable the drawing of the
/// impulse vector and play a sound that is panned left or right depending on
/// where the cue-ball is on the table.

void CObjectManager::Shoot(){
  m_pCueBall->DeliverImpulse(m_fCueAngle, 30.0f); //deliver impulse to cue-ball
  m_pAudio->play(eSound::Cue, m_pCueBall->m_vPos); //play sound of cue hitting ball
  m_bDrawImpulseVector = false; //turn off the impulse vector arrow
  m_bDrawCircle = false; //turn off target circle
} //Shoot

/// Check whether the cue-ball or the 8-ball is in a pocket.
/// \return true If one of the balls is in a pocket.

bool CObjectManager::BallDown(){
  return m_pCueBall->m_bInPocket || m_p8Ball->m_bInPocket;
} //BallDown

/// Check whether the cue-ball is down a pocket.
/// \return true If the cue-ball is in a pocket.

bool CObjectManager::CueBallDown(){
  return m_pCueBall->m_bInPocket;
} //CueBallDown

/// Check whether both the cue-ball and the 8-ball have stopped moving. Notice
/// that we can set the velocity vector to the zero vector and expect it to
/// succeed because CObject::move zeros out the velocity of slow-moving objects.
/// \return true If both balls have stopped moving.

bool CObjectManager::AllStopped(){
  return m_p8Ball->m_vVel == Vector2::Zero &&
       m_pCueBall->m_vVel == Vector2::Zero;
} //AllStopped

/// Begin by computing velocities relative to b1. Calculate the relative
/// displacement c and the distance cdotvhat along the normal to common tangent 
/// vhat. Calculate d1 and d2, the  distances moved back by b1 and b2 
/// (respectively) to their positions at TOI. Calculate time elapsed since TOI,
/// `tdelta = d2/s2`. Move balls back to their positions at TOI and compute
/// their new velocities. Move both balls after impact using their new
/// velocities and tdelta.
/// \param p0 Pointer to first object.
/// \param p1 Pointer to first object.
/// \param s [OUT] Collision speed.
/// \return true if the two objects collide.

bool CObjectManager::BallCollide(CObject* p0, CObject* p1, float& s){
  Vector2& b0Pos = p0->m_vPos;
  Vector2& b0Vel = p0->m_vVel; 

  Vector2& b1Pos = p1->m_vPos;
  Vector2& b1Vel = p1->m_vVel;

  const float r = p0->m_fRadius + p1->m_fRadius + 1.0f;

  const Vector2 v = b1Vel - b0Vel; //relative velocity
  const float speed = v.Length(); //relative speed
  if(speed == 0.0f)return false; //bail

  //step 2
  Vector2 vhat; //normalized version of v
  v.Normalize(vhat); //normalize v into vhat

  //calculate relative displacement and distance along normal to common tangent

  //step 1
  const Vector2 c = b0Pos - b1Pos; //vector from b1 to b0
  const float cdotvhat = c.Dot(vhat); //relative distance along normal to tangent

  // draw circle at dot product of vhat every frame and see what happens (cdotvhat * vhat)
  //Vector2 temp = vhat.Dot(cdotvhat);
  //m_cPDesc0.m_vPos = c;
  //m_pParticleEngine->create(m_cPDesc0);

  //step 3
  float d; //distance moved back by b1 to position at TOI
  const float delta = cdotvhat*cdotvhat - c.LengthSquared() + r*r; //discriminant
  if(delta >= 0.0f) //guard against taking the square root of a negative number
    d = -cdotvhat + sqrtf(delta); //collision really did occur
  else return false; //fail, no collision, function should not have been called

  //step 4
  const float tdelta = d/speed; //time elapsed since time of impact

  //move balls back to position at TOI 

  b0Pos -= tdelta*b0Vel; 
  b1Pos -= tdelta*b1Vel; 

  //drop particles at TOI when in step mode

  if(m_bStepMode){
    m_cPDesc1.m_vPos = b0Pos; 
    m_pParticleEngine->create(m_cPDesc1); //first ball

    m_cPDesc1.m_vPos = b1Pos; 
    m_pParticleEngine->create(m_cPDesc1); //second ball
  } //if

  // this looks usefull
  else if(m_bShowCollisions){       
    m_cPDesc2.m_vPos = b0Pos; 
    m_pParticleEngine->create(m_cPDesc2); 

    m_cPDesc2.m_vPos = b1Pos; 
    m_pParticleEngine->create(m_cPDesc2); 
  } //if

  // FIX ME this is supposed to only draw the circle where the balls will collide, but instead it does it after they collide
  /*if (!m_bDrawCircle) {
      m_cPDesc0.m_vPos = b0Pos;
      m_pParticleEngine->create(m_cPDesc0);

      m_cPDesc0.m_vPos = b1Pos;
      m_pParticleEngine->create(m_cPDesc0);
  }*/

  //compute new velocities after impact

  Vector2 nhat; //normal to tangent
  Vector2 n = b0Pos - b1Pos; //vector joining centers at TOI
  n.Normalize(nhat); //normalize n into nhat

  s = v.Dot(nhat); //difference in speed
  const Vector2 vDiff = s*nhat; //difference in velocity
  b0Vel += vDiff; //what one ball gains
  b1Vel -= vDiff; //the other one loses

  //move by the correct amount after impact

  b0Pos += tdelta*b0Vel;
  b1Pos += tdelta*b1Vel;

  return true;
} //BallCollide

bool CObjectManager::MimicBallCollide(CObject* p0, CObject* p1, float& s) {
    Vector2& b0Pos = p0->m_vPos;
    Vector2& b0Vel = p0->m_vVel;

    Vector2& b1Pos = p1->m_vPos;
    Vector2& b1Vel = 30.0f * Vector2(cosf(m_fCueAngle), sinf(m_fCueAngle));

    const float r = p0->m_fRadius + p1->m_fRadius + 1.0f;

    const Vector2 v = b1Vel - b0Vel; //relative velocity
    const float speed = v.Length(); //relative speed
    if (speed == 0.0f)return false; //bail

    //step 2
    Vector2 vhat; //normalized version of v
    v.Normalize(vhat); //normalize v into vhat

    //calculate relative displacement and distance along normal to common tangent

    //step 1
    const Vector2 c = b0Pos - b1Pos; //vector from b1 to b0
    const float cdotvhat = c.Dot(vhat); //relative distance along normal to tangent

    // draw circle at dot product of vhat every frame and see what happens (cdotvhat * vhat)
    //Vector2 temp = c.Dot(vhat);
    //m_cPDesc0.m_vPos = (vhat * cdotvhat);
    //m_pParticleEngine->create(m_cPDesc0);

    /*m_cPDesc0.m_vPos = b1Pos;
    m_pParticleEngine->create(m_cPDesc0);*/

    //step 3
    float d; //distance moved back by b1 to position at TOI
    const float delta = cdotvhat * cdotvhat - c.LengthSquared() + r * r; //discriminant
    if (delta >= 0.0f) //guard against taking the square root of a negative number
        d = -cdotvhat + sqrtf(delta); //collision really did occur
    else return false; //fail, no collision, function should not have been called

    //step 4
    const float tdelta = d / speed; //time elapsed since time of impact

    //move balls back to position at TOI 
    Vector2 targetPos = b1Pos;

    /*b0Pos -= tdelta * b0Vel;
    b1Pos -= tdelta * b1Vel;*/
    targetPos -= tdelta * b1Vel;

    //drop particles at TOI when in step mode


    if (m_bDrawCircle) {
        /*m_cPDesc0.m_vPos = b0Pos;
        m_pParticleEngine->create(m_cPDesc0);*/

        m_cPDesc3.m_vPos = targetPos;
        m_pParticleEngine->create(m_cPDesc3);
    } //if

    return true;
} //MimicBallCollide

/// Collision detection and response for ball hitting any rail. Check for a
/// collision and do the necessary housework for reflecting the ball if it hits
/// a rail. If there is a collision, a sound is played at a volume proportional
/// to the speed of collision and panned to the left or right according to
/// where the collision occurred.
/// \param b Pointer to a ball object to collide with rails.

void CObjectManager::RailCollide(CObject* b){ 
  if(b->m_bInPocket)return;

  const float r = b->m_fRadius; //ball radius
  
  //ball center at rail collision for each of the 4 rails
  const float TOP = m_nWinHeight - m_fYMargin - r;
  const float BOTTOM = m_fYMargin + r;
  const float LEFT = m_fXMargin + r;
  const float RIGHT = m_nWinWidth - m_fXMargin - r;

  Vector2& p = b->m_vPos; //ball position
  Vector2& v = b->m_vVel; //ball velocity

  const bool bHitY = p.y > TOP || p.y < BOTTOM; //hit a horizontal rail
  const bool bHitX = p.x < LEFT || p.x > RIGHT; //hit a vertical rail
  const bool bHit = bHitY || bHitX; //hit a rail

  Vector2 POI = Vector2::Zero; //point of impact

  //correct ball position

  if(p.x < LEFT){ //left rail
    m_cPDesc1.m_vPos = Vector2(LEFT, p.y - v.y*(p.x - LEFT)/v.x); //position at time of impact
    POI = m_cPDesc1.m_vPos - Vector2(r, 0.0f); //point of impact
    p.x = 2.0f*LEFT - p.x; //position after bounce
  } //if

  else if(p.x > RIGHT){ //right rail
    m_cPDesc1.m_vPos = Vector2(RIGHT, p.y - v.y*(p.x - RIGHT)/v.x); //position at TOI  
    POI = m_cPDesc1.m_vPos + Vector2(r, 0.0f); //point of impact
    p.x = 2.0f*RIGHT - p.x; //position after bounce
  } //else if

  else if(p.y > TOP){ //top rail
    m_cPDesc1.m_vPos = Vector2(p.x - v.x*(p.y - TOP)/v.y, TOP); //position at TOI
    POI = m_cPDesc1.m_vPos + Vector2(0.0f, r); //point of impact
    p.y = 2.0f*TOP - p.y; //position after bounce
  } //else if
 
  else if(p.y < BOTTOM){ //bottom rail
    m_cPDesc1.m_vPos = Vector2(p.x - v.x*(p.y - BOTTOM)/v.y, BOTTOM); //position at TOI
    POI = m_cPDesc1.m_vPos - Vector2(0.0f, r); //point of impact
    p.y = 2.0f*BOTTOM - p.y; //position after bounce
  } //else if

  //flip ball velocity and slow down
  const float RESTITUTION = 0.8f; //how bouncy the rails are

  if(bHitY) //horizontal rail
    v.y = -RESTITUTION*v.y;

  if(bHitX) //vertical rail
    v.x = -RESTITUTION*v.x;

  if(bHit){ //hit processing
    const float vol = std::min(v.Length()/10.0f, 1.0f); //volume
    m_pAudio->play(eSound::Thump, p, vol);
    
    if(m_bStepMode)
      m_pParticleEngine->create(m_cPDesc1); //thick circle at position at TOI 

    else if(m_bShowCollisions){       
      m_cPDesc2.m_vPos = m_cPDesc1.m_vPos; 
      m_pParticleEngine->create(m_cPDesc2); 
    } //if
  } //if
} //RailCollide

/// Collision and response for ball-in-pocket. Check for a collision and
/// does the necessary housework for disabling a ball that is in a pocket.
/// If there is a collision, a sound is played at a volume proportional to the
/// speed of collision and panned to the left or right according to where the 
/// pocket is on the table. 
/// \param b Pointer to a ball object to collide with pockets.

void CObjectManager::PocketCollide(CObject* b){ 
  if(b->m_bInPocket)return; //ignore balls already in pockets

  const float pw = 3.0f*b->m_fRadius; //pocket width 
  const float hpw = pw/2.0f; //half of that

  //ball center at rail collision for each of the 4 rails
  const float TOP = m_nWinHeight - m_fYMargin - hpw;
  const float BOTTOM = m_fYMargin + hpw;
  const float LEFT = m_fXMargin + hpw;
  const float RIGHT = m_nWinWidth - m_fXMargin - hpw;

  Vector2& vVel = b->m_vVel;
  bool& bInPocket = b->m_bInPocket;

  //pocket collision calculation

  const Vector2 pos = b->m_vPos; //save current position
  float& x = b->m_vPos.x; //shorthand
  float& y = b->m_vPos.y; //shorthand

  //top of table

  if(y > TOP){
    if(x < LEFT){ //top left corner
      bInPocket = true;
      b->m_vPos = m_vTopLPocket;
    } //if 

    else if(fabs(x - m_nWinWidth/2) < hpw/2.0f){ //top center corner
      bInPocket = true;
      b->m_vPos = m_vTopCPocket;
    } //else if

    else if(x > RIGHT){ //top right corner
      bInPocket = true;
      b->m_vPos = m_vTopRPocket;
    } //else if
  } //if

  //bottom of table

  else if(y < BOTTOM){
    if(x < LEFT){ //bottom left corner
      bInPocket = true;
      b->m_vPos = m_vBotLPocket;
    } //if 

    else if(fabs(x - m_nWinWidth/2) < hpw/2.0f){ //bottom center corner
      bInPocket = true;
      b->m_vPos = m_vBotCPocket;
    } //else if

    else if(x > RIGHT){ //bottom right corner
      bInPocket = true;
      b->m_vPos = m_vBotRPocket;
    } //else if
  } //else if

  //respond to a collision with the pocket: 
  //stop the ball, play a sound, drop a collision indicator particle

  if(bInPocket){ 
    const float vol = std::min(std::max(0.2f, vVel.Length()/20.0f), 1.0f); //volume
    m_pAudio->play(eSound::Pocket, b->m_vPos, vol);
    vVel = Vector2::Zero; //stop the ball

    if(m_bStepMode){
      m_cPDesc1.m_vPos = pos; //draw at saved position
      m_pParticleEngine->create(m_cPDesc1); 
    } //if
      
    else if(m_bShowCollisions){       
      m_cPDesc2.m_vPos = pos; //draw at saved position
      m_pParticleEngine->create(m_cPDesc2); 
    } //else if
  } //if
} //PocketCollide

/// Collision response for all balls against each other and the rails and the
/// pockets. We do pocket collision for all balls first to remove them from the
/// subsequent calculations.

void CObjectManager::BroadPhase(){
  //ball to pocket collision
  PocketCollide(m_p8Ball); 
  PocketCollide(m_pCueBall); 

  //ball to rail collision
  RailCollide(m_p8Ball);
  RailCollide(m_pCueBall); 

  //ball to ball collision for just 2 balls
  if(!m_p8Ball->m_bInPocket && !m_pCueBall->m_bInPocket){
    Vector2 v = m_p8Ball->m_vPos - m_pCueBall->m_vPos; //position difference
    const float d = m_p8Ball->m_fRadius + m_pCueBall->m_fRadius; //separation dist ance
    if(v.LengthSquared() < d*d) //if close enough, then they collide
      NarrowPhase(m_p8Ball, m_pCueBall);
  } //if
} //BroadPhase

/// Perform collision detection and response for a pair of objects.
/// Play impact sound if they collide.
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.

void CObjectManager::NarrowPhase(CObject* p0, CObject* p1){
  float vol; //impact sound volume
  const bool hit = BallCollide(p0, p1, vol);
  vol = std::min(vol/50.0f, 1.0f);

  if(hit) //if there was a collision
    m_pAudio->play(eSound::Click, p0->m_vPos, vol); 
} //NarrowPhase