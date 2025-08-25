/// \file Circle.cpp
/// \brief Code for CCircleDesc, CCircle, and CKinematicCircle.

#include "Circle.h"
#include "LineSeg.h"
#include "Line.h"
#include "AABB.h"
#include "Contact.h"

///////////////////////////////////////////////////////////////////
// CCircleDesc functions.

/// The default contructor creates a circle descriptor
/// of zero radius.

CCircleDesc::CCircleDesc(): 
  CShapeDesc(eShape::Circle){
} //constructor

/// This constructor creates a circle descriptor given the circle's
/// center, radius, and elasticity.
/// \param p Center point.
/// \param r Radius.
/// \param e Elasticity.

CCircleDesc::CCircleDesc(const Vector2& p, float r, float e):
  CShapeDesc(eShape::Circle), 
  m_fRadius(r)
{
  m_vPos = p;
  m_fElasticity = e;
} //constructor

///////////////////////////////////////////////////////////////////
// CCircle functions.

/// Constructs a circle described by a circle descriptor.
/// \param r Circle descriptor.

CCircle::CCircle(const CCircleDesc& r): 
  CShape(r), 
  m_fRadius(r.m_fRadius), 
  m_fRadiusSq(r.m_fRadius*r.m_fRadius)
{
  m_eShapeType = eShape::Circle;

  SetAABBPoint(Vector2(m_fRadius, 0.0f));
  AddAABBPoint(Vector2(-m_fRadius, 0.0f));
  AddAABBPoint(Vector2(0.0f, m_fRadius));
  AddAABBPoint(Vector2(0.0f, -m_fRadius));

  SetPos(r.m_vPos);
} //constructor

/// Determine whether a given point is strictly inside this circle,
/// that is, if the point's distance to the center of this circle
/// is strictly less that its radius.
/// \param p A point.
/// \return true if p is in this circle.

bool CCircle::PtInCircle(const Vector2& p){
  return (GetPos() - p).LengthSquared() < m_fRadiusSq;
} //PtInCircle

/// Find the point on the perimeter of this circle that is
/// closest to a given point.
/// \param p A point.
/// \return The closest point to p that is on the perimeter.

Vector2 CCircle::ClosestPt(const Vector2& p){
  const Vector2 p0 = GetPos();
  const Vector2 nhat = Normalize(p - p0);
  return p0 + m_fRadius*nhat;
} //ClosestPt

/// Collision detection with a dynamic circle.
/// \param c [in, out] Contact descriptor for this collision.
/// \return true is there was a collision.

bool CCircle::PreCollide(CContactDesc& c){
  CPoint poi = ClosestPt(c.m_pCircle->GetPos()); //point of impact
  return CPoint(poi).PreCollide(c);
} //PreCollide

/// Compute the points of intersection of tangents passing through a point.
/// Note that there are two possible tangents to a circle that pass through
/// a given point outside the circle. If the point is inside the circle,
/// then the tangents don't exist.
/// \param p Point that must lie on the tangents.
/// \param [out] p0 Intersection point with first tangent.
/// \param [out] p1 Intersection point with second tangent.
/// \return true if the tangents exist.

bool CCircle::Tangents(const Vector2& p, Vector2& p0, Vector2& p1){
  FailIf(PtInCircle(p)); //no tangents

  const Vector2 v = GetPos() - p; //vector from p to center of circle
  const float d = v.Length(); //distance from p to center of circle

  const float delta = sqrtf(d*d - m_fRadiusSq); //distance from p along line to center of circle
  const float phi = atan2f(v.y, v.x); //orientation of v
  const float theta = asinf(m_fRadius/d); //angle from v to tangent (plus or minus)

  const float ccw = phi + theta; //orientation of counterclockwise tangent
  const float cw = phi - theta; //orientation of clockwise tangent

  p0 = p + delta*Vector2(cosf(ccw), sinf(ccw)); //distance delta along tangent from p
  p1 = p + delta*Vector2(cosf(cw), sinf(cw)); //distance delta along tangent from p

  return true;
} //Tangents

/// A line tangent to circles at centers \f$p_0\f$ and \f$p_1\f$
/// of radii \f$r_0\f$ and \f$r_1 < r_0\f$ is constructed by constructing the tangent
/// to a single circle of radius \f$r_0 - r_1\f$ centered at \f$p_0\f$ and through \f$p_1\f$,
/// then translating this line segment along the radius through \f$p_0\f$ a distance \f$r_1\f$.
/// See J. Casey, "A sequel to the First Six Books of the Elements of Euclid", pp. 31-32, 1888.
/// \param pCirc Pointer to another circle.
/// \param [out] t0 Line segment descriptor for first common tangent.
/// \param [out] t1 Line segment descriptor for second common tangent.
/// \return true if tangents exist.

bool CCircle::Tangents(CCircle* pCirc, CLineSegDesc& t0, CLineSegDesc& t1){
  Vector2 p0 = GetPos();
  Vector2 p1 = pCirc->GetPos();
  
  float r0 = m_fRadius;
  float r1 = pCirc->m_fRadius;

  //make sure that the first circle is the biggest one
  if(r0 < r1) //it's not
    return pCirc->Tangents(this, t1, t0); //flip the circles so that the largest one is first

  Vector2 p2, p3; //points where tangents touch this circle

  if(r0 == r1) //same radii
    p2 = p3 = p0; //easy case, tangents are parallel with line joining centers

  else{ //general case, use Casey's method
    CCircle circ2(CCircleDesc(p0, r0 - r1));
    FailIf(!circ2.Tangents(p1, p2, p3));
  } //else

  //compute tangent offsets
  const Vector2 dv0 = r1*Normalize(perp(p2 - p1));
  const Vector2 dv1 = -r1*Normalize(perp(p3 - p1));

  //move intersection points by the appropriate offset
  t0.SetEndPts(p2 + dv0, p1 + dv0);
  t1.SetEndPts(p3 + dv1, p1 + dv1);

  return true;
} //Tangents

/// Compute the common tangent with another circle that is counterclockwise
/// from the vector pointing from this circle to that one.
/// \param pCirc Pointer to another circle.
/// \param [out] t Line segment descriptor for common tangent.
/// \return true if tangent exists.

bool CCircle::Tangent(CCircle* pCirc, CLineSegDesc& t){
  CLineSegDesc t0; //for the tangent we don't want
  FailIf(!Tangents(pCirc, t0, t)); //we want the second one
  return true;
} //Tangent

/// Reader function for the radius.
/// \return The radius.

float CCircle::GetRadius(){
  return m_fRadius;
} //GetRadius

///////////////////////////////////////////////////////////////////////////////////
// CKinematicCircle functions.

/// Constructs a kinematic circle described by a circle descriptor.
/// \param r Circle descriptor.

CKinematicCircle::CKinematicCircle(const CCircleDesc& r):
  CCircle(r)
{
  m_vOldPos = GetPos();
  m_eMotionType = eMotion::Kinematic;
} //constructor

/// Rotate to a given orientation from original orientation.
/// \param v Center of rotation.
/// \param a Angle increment from original orientation.

void CKinematicCircle::Rotate(const Vector2& v, float a){  
  SetPos(RotatePt(m_vOldPos, v, a));
} //Rotate

/// Reset to original orientation.

void CKinematicCircle::Reset(){
  SetPos(m_vOldPos);
} //Reset

