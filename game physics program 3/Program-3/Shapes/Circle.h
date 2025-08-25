/// \file Circle.h
/// \brief Interface for CCircleDesc, CCircle, and CKinematicCircle.

#ifndef __L4RC_PHYSICS_CIRCLE_H__
#define __L4RC_PHYSICS_CIRCLE_H__

#include "Shape.h"

class CLineSegDesc;

/// \brief Circle descriptor.
///
/// The circle descriptor describes a circle shape.

class CCircleDesc: public CShapeDesc{
  public:
    float m_fRadius = 0.0f; ///< Radius.

    CCircleDesc(); ///< Constructor.
    CCircleDesc(const Vector2&, float, float =1.0f); ///< Constructor.
}; //CCircleDesc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Circle shape.
///
/// A circle is represented by the position of its center and its radius.

class CCircle: public CShape{
  protected:
    float m_fRadius = 0.0f; ///< Radius.
    float m_fRadiusSq = 0.0f; ///< Radius squared, used for faster distance calculations.

  public:
    CCircle(const CCircleDesc&); ///< Constructor.

    bool PreCollide(CContactDesc&); ///< Collision detection.
    
    bool PtInCircle(const Vector2&); ///< Point in circle test.
    Vector2 ClosestPt(const Vector2&); ///< Closest point on circle.

    bool Tangents(const Vector2&, Vector2&, Vector2&); ///< Tangents through external point.
    bool Tangents(CCircle*, CLineSegDesc&, CLineSegDesc&); ///< Common tangents.
    bool Tangent(CCircle*, CLineSegDesc&); ///< Common tangent.

    float GetRadius(); ///< Get radius.
}; //CCircle

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Kinematic circle shape.
///
/// A circle whose motion type is KINEMATIC. 

class CKinematicCircle: public CCircle{
  private:
    Vector2 m_vOldPos; ///< Original position.

  public:
    CKinematicCircle(const CCircleDesc&); ///< Constructor.
    
    void Rotate(const Vector2&, float); ///< Rotate.
    void Reset(); ///< Reset orientation.
}; //CKinematicCircle

#endif //__L4RC_PHYSICS_CIRCLE_H__
