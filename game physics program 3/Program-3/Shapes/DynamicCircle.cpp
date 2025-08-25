/// \file DynamicCircle.cpp
/// \brief Code for the dynamic circle class CDynamicCircle and the dynamic circle descriptor class CDynamicCircleDesc.

#include "DynamicCircle.h"
#include "Contact.h"
#include "ShapeMath.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// CDynamicCircleDesc functions.

/// The default contructor creates a dynamic circle descriptor 
/// consisting of the default circle descriptor and
/// velocity zero.

CDynamicCircleDesc::CDynamicCircleDesc(): CCircleDesc(){
  m_eMotionType = eMotion::Dynamic;
} //constructor

////////////////////////////////////////////////////////////////////////////////////////////////////
// CDynamicCircle functions.

/// Constructs a dynamic circle described by a dynamic circle descriptor.
/// \param r Dynamic circle descriptor.

CDynamicCircle::CDynamicCircle(const CDynamicCircleDesc& r): 
  CCircle(r), 
  m_vVel(r.m_vVel),
  m_fMass(XM_PI*r.m_fRadius*r.m_fRadius*r.m_fRadius)
{
  SetAABBPoint(Vector2(m_fRadius, 0.0f));
  AddAABBPoint(Vector2(-m_fRadius, 0.0f));
  AddAABBPoint(Vector2(0.0f, m_fRadius));
  AddAABBPoint(Vector2(0.0f, -m_fRadius));
} //constructor

/// Does the AABB for this dynamic circle overlap the AABB for another dynamic circle?
/// \param pCirc Pointer to a dynamic circle.
/// \return true if their AABBs overlap.

bool CDynamicCircle::AABBCollide(CDynamicCircle* pCirc){
  return m_cAABB && pCirc->m_cAABB;
} //AABBCollide

/// Collision response for a dynamic circle colliding with a static shape. 
/// \param cd Contact descriptor which has been filled in by collision detection.

void CDynamicCircle::PostCollideStatic(const CContactDesc& cd){
  const Vector2& nhat = cd.m_vNorm; //shorthand
  SetPos(GetPos() - cd.m_fSetback*nhat); //set back to POI

  if(m_vVel.Dot(nhat) < 0.0f){ //heading towards POI
    const Vector2 dv = ParallelComponent(m_vVel, nhat); 
    const float e = m_fElasticity*cd.m_pShape->GetElasticity();
    m_vVel -= dv + e*(e <= 1.0f? dv: -nhat);
  } //if
} //PostCollideStatic

/// Collision response for a dynamic circle colliding with a kinematic
/// shape. This circle's velocity is affected by the kinematic shape's rotation.
/// This code assumes that all elasticities are less than unity.
/// \param cd Contact descriptor that has been filled in by collision detection.

void CDynamicCircle::PostCollideKinematic(const CContactDesc& cd){ 
  PostCollideStatic(cd); //start by reflecting off as if static

  if(cd.m_pShape->GetRotating()){ //if actually rotating
    Vector2& v0 = m_vVel; //shorthand for the dynamic circle's velocity
    CShape* p = cd.m_pShape; //pointer to the kinematic shape being collided with

    const Vector2 v1 = perp(cd.m_vPOI - p->GetRotCenter()); //tangent times POI's radius about kinematic shape's center of rotation
    const Vector2 v2 = p->GetRotSpeed()*XM_2PI*v1 - v0; //POI's velocity relative to this dynamic circle
    const Vector2 v3 = ParallelComponent(v2, GetPos() - cd.m_vPOI); //component of that through center of this dynamic circle

    if(v2.Dot(cd.m_vNorm) >= 0.0f) //if bouncing off the front of the kinematic shape (need >= not > in case the dynamic circle is stationary)
      v0 += m_fElasticity*cd.m_pShape->GetElasticity()*v3; //add to velocity of this dynamic circle
  } //if
} //PostCollideKinematic

/// Collision response for a dynamic circle colliding with a dynamic shape. 
/// \param cd Contact descriptor which has been filled in by collision detection.

void CDynamicCircle::PostCollideDynamic(const CContactDesc& cd){
  CDynamicCircle* pCirc = (CDynamicCircle*)(cd.m_pShape); //the other dynamic circle
  const Vector2 nhat = cd.m_vNorm; //collision normal
  
  const float m0 = m_fMass; //mass of this dynamic circle
  const float m1 = pCirc->m_fMass; //mass of the other dynamic circle
  const float msum = m0 + m1; //sum of the masses
  const float mdiff = m0 - m1; //difference of the masses

  //setback distances are inversely proportional to masses,
  //that is, the heavier one gets set back the least.

  const Vector2 delta0 = m1*cd.m_fSetback*nhat/msum;
  const Vector2 delta1 = m0*cd.m_fSetback*nhat/msum;

  SetPos(GetPos() - 2.0f*delta0);
  pCirc->SetPos(pCirc->GetPos() + delta1);

  //now for the velocities
  
  Vector2& u = m_vVel; //shorthand
  Vector2& v = pCirc->m_vVel; //shorthand
  const Vector2 uperp = u.Dot(nhat)*nhat;
  const Vector2 vperp = v.Dot(nhat)*nhat;

  //kill the components along the normals
  u -= uperp;
  v -= vperp;

  //Now apportion rebound velocities according to masses
  //You need both the law of conservation of momentum and
  //the law of conservation of energy to figure this one out.

  const float e = m_fElasticity*pCirc->m_fElasticity; //collision elasticity

  u += e*(2.0f*m1*vperp + mdiff*uperp)/msum; 
  v += e*(2.0f*m0*uperp - mdiff*vperp)/msum; 
} //PostCollideDynamic

/// Collision response for a dynamic circle colliding with a shape. 
/// \param cd Contact descriptor which has been filled in by collision detection.

void CDynamicCircle::PostCollide(const CContactDesc& cd){
  switch(cd.m_pShape->GetMotionType()){ 
    case eMotion::Static:    PostCollideStatic(cd); break;
    case eMotion::Kinematic: PostCollideKinematic(cd); break;
    case eMotion::Dynamic:   PostCollideDynamic(cd); break;
  } //switch
} //PostCollide

/// Move the shape using Euler integration, depending on the
/// physics time step and the gravity constant.

void CDynamicCircle::move(){ 
  SetPos(GetPos() + m_fTimeStep*m_vVel); //move
  m_vVel.y += m_fTimeStep*m_fGravity; //acceleration due to gravity
} //move

/// Reader function for the velocity.
/// \return The velocity.

Vector2 CDynamicCircle::GetVel(){
  return m_vVel;
} //GetVel

/// Set the velocity to a new value.
/// \param v New velocity.

void CDynamicCircle::SetVel(const Vector2& v){
  m_vVel = v;
} //SetVel


