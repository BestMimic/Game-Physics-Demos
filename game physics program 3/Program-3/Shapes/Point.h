/// \file Point.h
/// \brief Interface for CPointDesc, CPoint, and CKinematicPoint.

#ifndef __L4RC_PHYSICS_POINT_H__
#define __L4RC_PHYSICS_POINT_H__

#include "Shape.h"

/// \brief Point descriptor.
///
/// The point descriptor describes a point shape.

class CPointDesc: public CShapeDesc{
  public:
    CPointDesc(); ///< Default constructor.
    CPointDesc(const Vector2& p, float e=1.0f);  ///< Constructor.
}; //CPointDesc

/////////////////////////////////////////////////////////////////////////////

/// \brief Point shape.
///
/// A point has no dimensions, only position and elasticity.

class CPoint: public CShape{
  public:
    CPoint(const CPointDesc&); ///< Constructor.
    CPoint(const Vector2&); ///< Constructor.

    bool PreCollide(CContactDesc&); ///< Collision detection.
}; //CPoint

//////////////////////////////////////////////////////////////////////////

/// \brief Kinematic point shape.
///
/// A point whose motion type is KINEMATIC. 

class CKinematicPoint: public CPoint{
  private:
    Vector2 m_vOldPos; ///< Original position.

  public:
    CKinematicPoint(const CPointDesc&); ///< Constructor.
    
    void Rotate(const Vector2&, float); ///< Rotate.
    void Reset(); ///< Reset orientation.
}; //CKinematicPoint

#endif //__L4RC_PHYSICS_POINT_H__
