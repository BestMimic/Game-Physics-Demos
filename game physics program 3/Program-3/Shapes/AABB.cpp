/// \file AABB.cpp
/// \brief Code for the AABB class CAabb2D.

#include "AABB.h"

int CAabb2D::m_nTestCount = 0;

//////////////////////////////////////////////////////////////////////////////////////
//Constructors.

/// This constructor creates an AABB given its top left and bottom
/// right corners. No effort is made to check whether the top
/// left corner is actually above and to the left of the bottom 
/// right corner, so beware.
/// \param tl Top left corner.
/// \param br Bottom right corner.

CAabb2D::CAabb2D(const Vector2& tl, const Vector2& br):
  m_vTopLeft(tl), m_vBottomRt(br){
} //constructor

/// The default constructor creates an AABB whose top left
/// and bottom right corners are the origin.

CAabb2D::CAabb2D(): 
  CAabb2D(Vector2(0.0f), Vector2(0.0f)){
} //default constructor

//////////////////////////////////////////////////////////////////////////////////////
//Member functions.

/// Overloaded && operator to determine whether an AABB overlaps another AABB.
/// \param a An AABB.
/// \param b An AABB.
/// \return true if a and b overlap

bool operator&&(const CAabb2D& a, const CAabb2D& b){
  ++a.m_nTestCount;

  return 
    a.m_vTopLeft.x  <= b.m_vBottomRt.x && //a's left side is to the left of b's right side
    a.m_vBottomRt.x >= b.m_vTopLeft.x  && //a's right side is to the right of b's left side
    a.m_vTopLeft.y  >= b.m_vBottomRt.y && //a's top is above b's bottom
    a.m_vBottomRt.y <= b.m_vTopLeft.y;    //a's bottom is below b's top
} //operator&&

/// Overloaded && operator to determine whether a point is inside an AABB.
/// \param a An AABB.
/// \param p A point.
/// \return true if p is inside a.

bool operator&&(const CAabb2D& a, const Vector2& p){
  return a.m_vTopLeft.x <= p.x && a.m_vBottomRt.x >= p.x &&
    a.m_vTopLeft.y >= p.y && a.m_vBottomRt.y <= p.y; 
} //operator&&

/// Overloaded assignment operator that sets an AABB to a single point.
/// \param p A point.
/// \return AABB for the point.

CAabb2D& CAabb2D::operator=(const Vector2& p){
  m_vTopLeft = m_vBottomRt = p;
  return *this;
} //operator=

/// Overloaded += operator that adds a point to an AABB, that is,
/// it extends the AABB to cover the new point in addition to its existing area.
/// \param p A point.
/// \return AABB consisting of old AABB extended to cover p.

CAabb2D& CAabb2D::operator+=(const Vector2& p){
  m_vTopLeft.x =  min(m_vTopLeft.x,   p.x);
  m_vBottomRt.x = max(m_vBottomRt.x,  p.x);

  m_vBottomRt.y = min(m_vBottomRt.y, p.y);
  m_vTopLeft.y =  max(m_vTopLeft.y,  p.y);
  
  return *this;
} //operator+=

/// Translate AABB to a new position. Adds the displacement
/// vector to the top left and bottom right corners of the AABB.
/// \param p Vector displacement.

void CAabb2D::Translate(const Vector2& p){
  m_vTopLeft += p;
  m_vBottomRt += p;
} //Translate

/// Reader function for the width.
/// \return AABB width.

float CAabb2D::GetWidth(){
  return m_vBottomRt.x - m_vTopLeft.x;
} //GetWidth

/// Reader function for the height.
/// \return AABB height.

float CAabb2D::GetHt(){
  return m_vTopLeft.y - m_vBottomRt.y;
} //GetHt

/// Reader function for the top left corner.
/// \return The top left corner.

const Vector2& CAabb2D::GetTopLeft(){
  return m_vTopLeft;
} //GetTopLeft

/// Reader function for the bottom right corner.
/// \return The bottom right corner.

const Vector2& CAabb2D::GetBottomRt(){
  return m_vBottomRt;
} //GetBottomRt

/// Get the number of AABB to AABB intersection tests made since 
/// the last time this function was called, and reset it to zero.
/// \return Number of tests since last call.

int CAabb2D::GetTestCount(){
  int result = m_nTestCount;
  m_nTestCount = 0;
  return result;
} //GetTestCount
