/// \file Line.cpp
/// \brief Code for CLine.

#include "Line.h"
#include "Circle.h"
#include <limits>

/// Given a point and a gradient, construct the unique line
/// through that point with that gradient.
/// \param p Point.
/// \param m Gradient.

CLine::CLine(const Vector2& p, float m): 
  CShape(eShape::Line), 
  m_fGradient(m), m_fInverseGradient(1.0f/m), 
  m_fYIntercept(p.y - m*p.x), m_fXIntercept(p.x - p.y/m){
} //constructor

/// Given another line, find the unique point that is on both lines if they
/// are not parallel, otherwise fail. We have to careful whem
/// one or both of the lines are vertical because vertical
/// lines have infinite gradient.
/// \param Line A line to intersect with.
/// \return Point of intersection of this line with that one, if there is one.

Vector2 CLine::Intersect(const CLine& Line){
  //some handy shorthands to make this more readable
  const float m0 = m_fGradient;
  const float c0 = m_fYIntercept;
  const float d0 = m_fXIntercept;

  const float m1 = Line.m_fGradient;
  const float c1 = Line.m_fYIntercept;
  const float d1 = Line.m_fXIntercept;

  if(m0 == m1) //parallel lines meet at infinity
    return Vector2(INFINITY, INFINITY);

  else if(isfinite(m0) && isfinite(m1)){ //neither line vertical
    const float px = (c1 - c0)/(m0 - m1);
    return Vector2(px, m0*px + c0);
  } //else if

  else if(isinf(m0) && isfinite(m1)) //only this line is vertical
    return Vector2(d0, m1*d0 + c1);

  else //only the other line is vertical
    return Vector2(d1, m0*d1 + c0);
} //Intersect

/// Given a point, find the point on this line that is closest to it.
/// This is done by intersecting this line with a line through p perpendicular to it.
/// \param p A point.
/// \return The point on this line that is closest to it.

Vector2 CLine::ClosestPt(const Vector2& p){
  return Intersect(CLine(p, -m_fInverseGradient));  
} //ClosestPt

