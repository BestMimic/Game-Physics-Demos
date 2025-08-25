/// \file Polygon.cpp
/// \brief Code for the polygon class CPolygon

#include "Polygon.h"

/// \param p Pointer to a point shape at the polygon center.

CPolygon::CPolygon(CShape* p): 
  m_pCenterPoint(p)
{
  p->SetCanCollide(false);
} //constructor

/// Test whether a shape is part of this polygon.
/// This function is used during collision response when the
/// polygon is represented by a sprite that changes on collision.
/// \param pShape Pointer to a shape.
/// \return true if the shape is part of this polygon.

bool CPolygon::IsPartOfPolygon(CShape* pShape){
  for(auto p: m_stdShapes)
    if(p == pShape)return true;

  return false;
} //IsPartOfPolygon

/// Reader function for polygon center.
/// \return Pointer to polygon center.

CShape* CPolygon::GetCenterPoint(){
  return m_pCenterPoint;
} //GetCenterPoint
