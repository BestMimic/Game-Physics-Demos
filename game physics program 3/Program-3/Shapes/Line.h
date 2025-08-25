/// \file Line.h
/// \brief Interface for CLine.

#ifndef __L4RC_PHYSICS_LINE_H__
#define __L4RC_PHYSICS_LINE_H__

#include "Point.h"
#include "Circle.h"

/// \brief Line shape.
///
/// A line is infinite in both directions. It consists of
/// its gradient \f$m\f$ and \f$y\f$-incercept \f$c\f$, that is, it has the
/// equation \f$y = mx + c\f$. The gradient can be infinite, in
/// that case it has the equation \f$x = b\f$, where \f$b\f$ is its \f$x\f$-intercept.
/// Note that there is no line descriptor class CLineDesc. That is to
/// discourage the use of lines outside this project. If you
/// are thinking of using one, I recommend that you use a line segment instead.

class CLine: public CShape{
  protected:
    float m_fGradient = 0.0f; ///< Gradient.
    float m_fInverseGradient = 0.0f; ///< Inverse gradient.
    float m_fYIntercept = 0.0f; ///< Intercept with Y axis.
    float m_fXIntercept = 0.0f; ///< Intercept with X axis.

    Vector2 Intersect(const CLine&); ///< Get intersection point with line.
    Vector2 ClosestPt(const Vector2&); ///< Get closest point on line.

  public:
    CLine(const Vector2&, float); ///< Constructor.
}; //CLine

#endif //__L4RC_PHYSICS_LINE_H__
