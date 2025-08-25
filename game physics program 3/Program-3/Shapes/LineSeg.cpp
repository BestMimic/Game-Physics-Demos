/// \file LineSeg.cpp
/// \brief Code for CLineSegDesc, CLineSeg, and CKinematicLineSeg.

#include "LineSeg.h"
#include "Circle.h"
#include "Contact.h"

/////////////////////////////////////////////////////////////////////////////
// CLineSegDesc functions

/// The default contructor creates a line segment descriptor whose
/// end points are the origin. 

CLineSegDesc::CLineSegDesc(): 
  CLineSegDesc(Vector2(0.0f), Vector2(0.0f)){
} //constructor

/// This constructor creates a line segment descriptor given
/// the line segment's end points and elasticity.
/// \param p0 End point.
/// \param p1 End point.
/// \param e Elasticity, defaults to 1.0f.

CLineSegDesc::CLineSegDesc(const Vector2& p0, const Vector2& p1, float e): 
  CShapeDesc(eShape::LineSeg)
{
  SetEndPts(p0, p1);
  m_fElasticity = e;
} //constructor

/// Set the end points of this line segment descriptor, ensuring that
/// the first point is to the left of the second point so
/// that the gradient is computed correctly. Also computes the
/// normal vector (which will be counterclockwise from the
/// vector that points from the first and point to the second one
/// in the order in which they are given as parameters,.
/// \param p0 First end point.
/// \param p1 Second end point.

void CLineSegDesc::SetEndPts(const Vector2& p0, const Vector2& p1){
  m_vNormal = Normalize(perp(p0 - p1));

  m_vPt0 = p0; m_vPt1 = p1;
  if(p1.x < p0.x)std::swap(m_vPt0, m_vPt1); //ensure p0 is to the left of p1

  m_vPos = (p0 + p1)/2.0f;
  const Vector2 dp = m_vPt0 - m_vPt1;

  m_fGradient = dp.y/dp.x;
} //SetEndPts

/// Reader function for the positions of the end points.
/// \param p0 [out] Leftmost end point.
/// \param p1 [out] Other end point.

void CLineSegDesc::GetEndPts(Vector2& p0, Vector2& p1){
  p0 = m_vPt0; 
  p1 = m_vPt1;
} //GetEndPts

///< Reader function for end point 0.
/// \return End point 0.

const Vector2& CLineSegDesc::GetEndPt0(){
  return m_vPt0;
} //GetEndPt0

///< Reader function for end point 1.
/// \return End point 1.

const Vector2& CLineSegDesc::GetEndPt1(){
  return m_vPt1;
} //GetEndPt1

///< Reader function for normal.
/// \return Normal.

const Vector2& CLineSegDesc::GetNormal(){
  return m_vNormal;
} //GetNormal

/// Reader function for the gradient.
/// \return Gradient.

float CLineSegDesc::GetGradient(){
  return m_fGradient;
} //GetGradient

/////////////////////////////////////////////////////////////////////////////
// CLineSeg functions

/// Constructs a line segment described by a line segment descriptor.
/// \param r Line segment descriptor.

CLineSeg::CLineSeg(CLineSegDesc& r): 
  CLine(r.GetEndPt0(), r.GetGradient()),
  m_vPt0(r.GetEndPt0()),
  m_vPt1(r.GetEndPt1()),
  m_vNormal(r.GetNormal())
{
  m_eShapeType = eShape::LineSeg;
  m_fElasticity = r.m_fElasticity;
  SetPos(r.m_vPos);
  Update();
} //constructor

/// Update the line segment properties from its position and end points. 
/// The tangents and AABB are recomputed along with line
/// properties such as the gradient.

void CLineSeg::Update(){
  //recompute line properties
  const Vector2 dp = m_vPt0 - m_vPt1; //for gradient
  m_fGradient = dp.y/dp.x;
  m_fInverseGradient = 1.0f/m_fGradient;
  m_fYIntercept = m_vPt0.y - m_fGradient*m_vPt0.x;
  m_fXIntercept = m_vPt0.x;

  const Vector2 p = GetPos();

  m_vTangent0 = Normalize(m_vPt0 - p);
  m_vTangent1 = Normalize(m_vPt1 - p);

  SetAABBPoint(m_vPt0 - p);
  AddAABBPoint(m_vPt1 - p);
} //Update

/// Reader function for the end points of the line segment.
/// \param p0 [out] One end.
/// \param p1 [out] The other end.

void CLineSeg::GetEndPts(Vector2& p0, Vector2& p1){
  p0 = m_vPt0; 
  p1 = m_vPt1;
} //GetEndPts

/// Reader function for the tangents at the end points of the line segment.
/// These will be in the same order as the parameters of function GetEndPoints,
/// that is, v0 will be the tangent at p0 and v1 will be the tangent at p1.
/// \param v0 [out] One tangent.
/// \param v1 [out] The other tangent.

void CLineSeg::GetTangents(Vector2& v0, Vector2& v1){
  v0 = m_vTangent0; v1 = m_vTangent1;
} //GetTangents

/// Reader function for the normal.
/// \return The normal.

const Vector2& CLineSeg::GetNormal(){
  return m_vNormal;
} //GetNormal

/// Collision detection with a dynamic circle.
/// \param c [in, out] Contact  descriptor for this collision.
/// \return true is there was a collision.

bool CLineSeg::PreCollide(CContactDesc& c){
  const Vector2 p0 = m_vPt0;
  const Vector2 p1 = m_vPt1;
  const Vector2 p2 = c.m_pCircle->GetPos();

  FailIf(m_vTangent0.Dot(p0 - p2) < 0.0f);
  FailIf(m_vTangent1.Dot(p1 - p2) < 0.0f);
  
  CPoint poi(CPointDesc(ClosestPt(p2)));
  return poi.PreCollide(c);
} //PreCollide

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CKinematicLineSeg functions.

/// Constructs a kinematic line segment described by an 
/// line segment descriptor.
/// \param r Line segment descriptor.

CKinematicLineSeg::CKinematicLineSeg(CLineSegDesc& r):
  CLineSeg(r),
  m_vOldPt0(m_vPt0), m_vOldPt1(m_vPt1){
  m_eMotionType = eMotion::Kinematic;
} //constructor

/// Rotate to a given orientation from original orientation.
/// \param v Center of rotation.
/// \param a Angle increment from original orientation.

void CKinematicLineSeg::Rotate(const Vector2& v, float a){
  //rotate end points
  m_vPt0 = RotatePt(m_vOldPt0, v, a);
  m_vPt1 = RotatePt(m_vOldPt1, v, a);
  if(m_vPt1.x < m_vPt0.x)std::swap(m_vPt0, m_vPt1); //ensure p0 is to the left of p1

  SetPos((m_vPt0 + m_vPt1)/2.0f); //recompute center (may be different from center of rotation)
  
  Update(); 
} //Rotate

/// Reset to original orientation.

void CKinematicLineSeg::Reset(){
  m_vPt0 = m_vOldPt0;
  m_vPt1 = m_vOldPt1;
  if(m_vPt1.x < m_vPt0.x)std::swap(m_vPt0, m_vPt1); //ensure p0 is to the left of p1

  SetPos((m_vPt0 + m_vPt1)/2.0f); //recompute center (may be different from center of rotation)
  
  Update(); 
} //Reset
