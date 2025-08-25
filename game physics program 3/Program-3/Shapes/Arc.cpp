/// \file Arc.cpp
/// \brief Code for CArcDesc, CArc, and CKinematicArc.

#include "Arc.h"

#include "Contact.h"
#include "Circle.h"
#include "LineSeg.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CArcDesc functions.

/// The default contructor creates an arc descriptor whose angles are zero.

CArcDesc::CArcDesc(): 
  CCircleDesc(){
  m_eShapeType = eShape::Arc;
} //constructor

/// This constructor creates an arc descriptor given the arc's
/// center, radius, angles, and elasticity.
/// \param p0 Position of center.
/// \param r Radius.
/// \param a0 First angle.
/// \param a1 Second angle.
/// \param e Elasticity.

CArcDesc::CArcDesc(const Vector2& p0, float r, float a0, float a1, float e): 
  CCircleDesc(), m_fAngle0(a0), m_fAngle1(a1)
{
  m_eShapeType = eShape::Arc;
  m_vPos = p0;
  m_fRadius = r;
  m_fElasticity = e;
} //constructor

/// Set the arc descriptor's angles, making sure to normalzie them first.
/// \param a0 The first angle.
/// \param a1 The second angle.

void CArcDesc::SetAngles(float a0, float a1){
  m_fAngle0 = NormalizeAngle(a0);
  m_fAngle1 = NormalizeAngle(a1);
} //SetAngles

/// Calculate the end points from the arc center, angles, and radius.
/// Assumes that these values have already been set.
/// \param p0 [out] An end point.
/// \param p1 [out] Another end point.

void CArcDesc::GetEndPts(Vector2& p0, Vector2& p1){
  p0 = m_vPos + m_fRadius*Vector2(cosf(m_fAngle0), sinf(m_fAngle0));
  p1 = m_vPos + m_fRadius*Vector2(cosf(m_fAngle1), sinf(m_fAngle1));
} //GetEndPts

/// Reader function for angle 0.
/// \return Angle 0.

float CArcDesc::GetAngle0(){
  return m_fAngle0;
} //GetAngle0

/// Reader function for angle 1.
/// \return Angle 1.

float CArcDesc::GetAngle1(){
  return m_fAngle1;
} //GetAngle1

///////////////////////////////////////////////////////////////////////////////////////
// CArc functions.

/// Constructs an arc described by an arc descriptor.
/// \param r Arc descriptor.

CArc::CArc(CArcDesc& r): CCircle(r), 
  m_fAngle0(r.GetAngle0()), 
  m_fAngle1(r.GetAngle1())
{
  m_eShapeType = eShape::Arc;
  Update();
} //constructor

/// Update the arc properties from its center, radius, and angles. 
/// The end points, tangents, and AABB are recomputed.

void CArc::Update(){
  const float r = m_fRadius;
  const Vector2 p = GetPos();
  const Vector2 p0 = AngleToVector(m_fAngle0);
  const Vector2 p1 = AngleToVector(m_fAngle1);

  //update end points
  m_vPt0 = p + r*p0;
  m_vPt1 = p + r*p1;

  //update tangents
  m_vTangent0 = -perp(p0); 
  m_vTangent1 =  perp(p1);  
  
  //update AABB
  SetAABBPoint(r*p0); //set to first end point
  AddAABBPoint(r*p1); //add second end point
  
  //add farthest extents of bounding circle if they are on the arc

  Vector2 q(r, 0.0f); //farthest extent to the right

  for(int i=0; i<4; i++){ //four quadrants of the bounding circle
    if(PtInSector(p + q)) //q is on the arc
      AddAABBPoint(q); //add to AABB
    q = perp(q); //next extent
  } //for
} //Update

/// Draw imaginary lines from the center of this arc (meaning the center of the
/// circle containing it) to its end points and continue them on infinitely.
/// A point is said to be inside the sector if it is between those two lines.
/// \param p A point.
/// \return true if p is inside the sector defined by this arc.

bool CArc::PtInSector(const Vector2& p){ 
  const float a = PtToAngle(p); 

  if(m_fAngle0 < m_fAngle1)
    return a >= m_fAngle0 && a <= m_fAngle1;
  else return a >= m_fAngle0 || a <= m_fAngle1;
} //PtInSector

float CArc::PtToAngle(const Vector2& p){ 
  const Vector2 v = p - GetPos();
  return NormalizeAngle(atan2f(v.y, v.x)); //atan2 computes angle in range -PI <=a <= 2PI
} //PtToAngle

/// Collision detection with a dynamic circle.
/// \param c [in, out] Contact descriptor for this collision.
/// \return true is there was a collision.

bool CArc::PreCollide(CContactDesc& c){
  const Vector2 p0 = m_vPt0;
  const Vector2 p1 = m_vPt1;
  const Vector2 p2 = c.m_pCircle->GetPos();

  FailIf(!PtInSector(p2)); //fail if center is outside sector

  if(!PtInCircle(p2)){ //outside collision
    FailIf(m_vTangent0.Dot(p0 - p2) <= 0.0f); //coming from outside
    FailIf(m_vTangent1.Dot(p1 - p2) <= 0.0f); //coming from outside
  } //if
  
  CPoint poi = CPoint(ClosestPt(p2));
  return poi.PreCollide(c);
} //PreCollide

/// Reader function for the end points.
/// \param p0 [out] First end point.
/// \param p1 [out] Second end point.

void CArc::GetEndPts(Vector2& p0, Vector2& p1){
  p0 = m_vPt0; p1 = m_vPt1;
} //GetEndPts

/// Reader function for the tangents. Tangents are returned
/// in the same order as GetEndPoints().
/// \param v0 [out] Tangent at fist end point.
/// \param v1 [out] Tangent at second end point.

void CArc::GetTangents(Vector2& v0, Vector2& v1){
  v0 = m_vTangent0; v1 = m_vTangent1;
} //GetTangents

///////////////////////////////////////////////////////////////////////////////////
// CKinematicArc functions.

/// Constructs a kinematic arc described by an arc descriptor.
/// \param r Arc descriptor.

CKinematicArc::CKinematicArc(CArcDesc& r): CArc(r),
  m_fOldAngle0(m_fAngle0), m_fOldAngle1(m_fAngle1)
{
  m_eMotionType = eMotion::Kinematic;
  m_vOldPos = GetPos();
} //constructor

/// Rotate to a given orientation from original orientation.
/// \param v Center of rotation.
/// \param a Angle increment from original orientation.

void CKinematicArc::Rotate(const Vector2& v, float a){
  m_fAngle0 = NormalizeAngle(m_fOldAngle0 + a);
  m_fAngle1 = NormalizeAngle(m_fOldAngle1 + a);

  SetPos(RotatePt(m_vOldPos, v, a));
  Update();
} //Rotate

/// Reset to original orientation.

void CKinematicArc::Reset(){
  m_fAngle0 = m_fOldAngle0;
  m_fAngle1 = m_fOldAngle1;

  Update();
} //Reset



