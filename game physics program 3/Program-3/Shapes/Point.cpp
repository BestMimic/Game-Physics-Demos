/// \file Point.cpp
/// \brief Code for CPointDesc, CPoint, and CKinematicPoint.

#include "Shape.h"
#include "Point.h"
#include "DynamicCircle.h"
#include "Contact.h"

//////////////////////////////////////////////////////////////////////////////////
// CPointDesc functions.

/// The default contructor creates a point descriptor
/// from the default shape descriptor.

CPointDesc::CPointDesc(): CShapeDesc(eShape::Point){
} //constructor

/// This constructor creates a point descriptor given the point's
/// position and elasticity.
/// \param p Position.
/// \param e Elasticity.

CPointDesc::CPointDesc(const Vector2& p, float e):
  CShapeDesc(eShape::Point){
  m_vPos = p;
  m_fElasticity = e;
} //constructor

//////////////////////////////////////////////////////////////////////////////////
// CPoint functions.

/// Constructs a point described by a point descriptor.
/// \param r Point descriptor.

CPoint::CPoint(const CPointDesc& r): CShape(r){
  SetAABBPoint(Vector2(0.0f));
  SetPos(r.m_vPos);
} //constructor

/// Constructs a point at a given position.
/// \param p Position.

CPoint::CPoint(const Vector2& p):
  CPoint(CPointDesc(p)){
} //constructor

/// Collision detection with a dynamic circle.
/// \param c [in, out] Contact descriptor for this collision.
/// \return true is there was a collision.

bool CPoint::PreCollide(CContactDesc& c){
  if(!m_bCanCollide)return false; //bail and fail

  CDynamicCircle* pCirc = c.m_pCircle;
  
  const Vector2 p0 = GetPos();
  const Vector2 p1 = pCirc->GetPos();
  const Vector2 p = p1 - p0;
  const float r = pCirc->GetRadius();

  const float d = p.Length() - r; //setback distance

  FailIf(d >= 0.0f);
  
  c.m_vPOI = p0;
  c.m_fSetback = d;
  c.m_fSpeed = pCirc->GetVel().Length();
  c.m_vNorm = Normalize(p);

  return true;
} //PreCollide

///////////////////////////////////////////////////////////////////////////////////
// CKinematicPoint functions.

/// Constructs a kinematic point described by a point descriptor.
/// \param r Point descriptor.

CKinematicPoint::CKinematicPoint(const CPointDesc& r): 
  CPoint(r)
{
  m_eMotionType = eMotion::Kinematic;
  m_vOldPos = GetPos();
} //constructor

/// Rotate to a given orientation from original orientation.
/// \param p Center of rotation.
/// \param a Angle increment from original orientation.

void CKinematicPoint::Rotate(const Vector2& p, float a){
  SetPos(RotatePt(m_vOldPos, p, a));
} //Rotate

/// Reset to original orientation.

void CKinematicPoint::Reset(){
  SetPos(m_vOldPos);
} //Reset

