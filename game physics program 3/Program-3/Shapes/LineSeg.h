/// \file LineSeg.h
/// \brief Interface for CLineSegDesc, CLineSeg, and CKinematicLineSeg.

#ifndef __L4RC_PHYSICS_LINESEG_H__
#define __L4RC_PHYSICS_LINESEG_H__

#include "Line.h"

/// \brief Line segment descriptor.
///
/// The line segment descriptor describes a line segment.

class CLineSegDesc: public CShapeDesc{
  protected:
    Vector2 m_vPt0; ///< Point 0.
    Vector2 m_vPt1; ///< Point 1.

    Vector2 m_vNormal; ///< Normal.
    float m_fGradient = 0.0f; ///< Gradient.

  public:
    CLineSegDesc(); ///< Constructor.
    CLineSegDesc(const Vector2&, const Vector2&, float =1.0f); ///< Constructor.

    void SetEndPts(const Vector2&, const Vector2&); /// Set end points.
    void GetEndPts(Vector2&, Vector2&); ///< Get end points.

    const Vector2& GetEndPt0(); ///< Get end point 0.
    const Vector2& GetEndPt1(); ///< Get end point 1.
    const Vector2& GetNormal(); ///< Get normal.
    float GetGradient(); ///< Get gradient.
}; //CLineSegDesc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Line segment shape.
///
/// A line segment is the portion of a line drawn from one
/// point to another, that is, it is finite in both directions.

class CLineSeg: public CLine{
  protected:
    Vector2 m_vPt0; ///< Point at end of line segment.
    Vector2 m_vPt1; ///< Point at other end of line segment.
    
    Vector2 m_vTangent0; ///< Tangent at point 0.
    Vector2 m_vTangent1; ///< Tangent at point 1.

    Vector2 m_vNormal; ///< Normal to line segment.
    
    void Update(); ///< Update other properties from the end points.

  public:
    CLineSeg(CLineSegDesc&); ///< Constructor.  

    bool PreCollide(CContactDesc&); ///< Collision detection.

    void GetEndPts(Vector2&, Vector2&); ///< Get end points.
    void GetTangents(Vector2&, Vector2&); ///< Get tangents. 
    const Vector2& GetNormal(); ///< Get normal. 
}; //CLineSeg

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Kinematic line segment shape.
///
/// A line segment whose motion type is KINEMATIC. 

class CKinematicLineSeg: public CLineSeg{
  private:
    Vector2 m_vOldPt0; ///< Old point 0.
    Vector2 m_vOldPt1; ///< Old point 1.

  public:
    CKinematicLineSeg(CLineSegDesc&); ///< Constructor.
    
    void Rotate(const Vector2&, float); ///< Rotate.
    void Reset(); ///< Reset orientation.
}; //CKinematicLineSeg

#endif //__L4RC_PHYSICS_LINESEG_H__
