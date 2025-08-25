/// \file Arc.h
/// \brief Interface for CArcDesc, CArc, and CKinematicArc.

#ifndef __L4RC_PHYSICS_ARC_H__
#define __L4RC_PHYSICS_ARC_H__

#include "Circle.h"

/// \brief Arc descriptor.
///
/// The arc descriptor describes an arc shape.

class CArcDesc: public CCircleDesc{
  private:
    Vector2 m_vPt0; ///< Point 0.
    Vector2 m_vPt1; ///< Point 1.
    
    float m_fAngle0 = 0.0f; ///< Angle from center to point 0.
    float m_fAngle1 = 0.0f; ///< Angle from center to point 1.
    
    Vector2 m_vTangent0; ///< Tangent at point 0.
    Vector2 m_vTangent1; ///< Tangent at point 1.

  public:
    CArcDesc(); ///< Constructor.
    CArcDesc(const Vector2&, float, float, float, float =1.0f); ///< Constructor.

    void SetAngles(float, float); ///< Set the angles.
    void GetEndPts(Vector2&, Vector2&); ///< Get end points.

    float GetAngle0(); ///< Get angle 0.
    float GetAngle1(); ///< Get angle 1.
}; //CArcDesc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Arc shape.
///
/// An arc represents a portion of the circumference of a circle bounded by a
/// pair of angles. If the first angle is smaller than the second angle,  then
/// the arc extends from the first angle to the second angle in a
/// counterclockwise direction. If the first angle is greater than the second
/// angle, then the arc extends from the first angle to the second angle in a
/// clockwise direction. If the first angle is the same as the second angle,
/// then you are an idiot.

class CArc: public CCircle{
  protected:
    Vector2 m_vPt0; ///< Point 0.
    Vector2 m_vPt1; ///< Point 1.

    float m_fAngle0; ///< Angle from center to point 0.
    float m_fAngle1; ///< Angle from center to point 1.

    Vector2 m_vTangent0; ///< Tangent at point 0.
    Vector2 m_vTangent1; ///< Tangent at point 1.
    
    void Update(); ///< Update from angles and radius.
    float PtToAngle(const Vector2&); ///< Point to angle.

  public:
    CArc(CArcDesc&); ///< Constructor.

    bool PreCollide(CContactDesc&); ///< Collision detection.

    bool PtInSector(const Vector2&); ///< Point in sector test.
    
    void GetEndPts(Vector2&, Vector2&); ///< Get end points.
    void GetTangents(Vector2&, Vector2&); ///< Get tangents.   
}; //CArc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Kinematic arc shape.
///
/// An arc shape whose motion type is `KINEMATIC`. 

class CKinematicArc: public CArc{
  private:
    Vector2 m_vOldPos; ///< Original position.
    float m_fOldAngle0; ///< Original angle 0.
    float m_fOldAngle1; ///< Original angle 1.

  public:
    CKinematicArc(CArcDesc&); ///< Constructor.
    
    void Rotate(const Vector2&, float); ///< Rotate.
    void Reset(); ///< Reset orientation.
}; //CKinematicArc

#endif //__L4RC_PHYSICS_ARC_H__
