/// \file ShapeMath.cpp
/// \brief Handy math functions for the collision module.

#include "ShapeMath.h"

/// /brief Normalize angle to [0, 2PI).
///
/// \param a Angle in radians.
/// \return Normalized angle, that is, in the range [0, 2PI).

float NormalizeAngle(float a){
  a -= floorf(a/XM_2PI)*XM_2PI; //a is now in (-2PI, 2PI)
  return (a < 0.0f)? a + XM_2PI: a; //fix the negative angles
} //NormalizeAngle

/// The DirectXTK doesn't have a function that returns a
/// normalized vector, so here it is.
/// \param v A vector.
/// \return A unit vector that points in the direction of v.

Vector2 Normalize(Vector2 v){
  v.Normalize();
  return v;
} //Normalize

/// Compute the perpendicular to a given vector in a counterclockwise direction.
/// \param v A vector.
/// \return The vector perpendicular to v in a counterclockwise direction.

Vector2 perp(const Vector2& v){
  return Vector2(-v.y, v.x);
} //perp

/// Get a normalized vector at a given orientation.
/// \param a Orientation angle.
/// \return Unit vector at orientation a.

Vector2 AngleToVector(const float a){
  return Vector2(cosf(a), sinf(a));
} //AngleToVector

/// Rotate a point about an arbitrary center.
/// \param p Point to be rotated.
/// \param q Center of rotation.
/// \param a Angle of rotation in radians.
/// \return Rotated point.

Vector2 RotatePt(Vector2 p, const Vector2& q, const float a){
  const float s = sinf(a);
  const float c = cosf(a);

  p -= q;
  return q + Vector2(p.x*c - p.y*s, p.x*s + p.y*c);
} //RotatePt

/// Find the component of one vector parallel to another.
/// \param v0 Vector whose component we want.
/// \param v1 Vector that the component must be parallel to.
/// \return Component of v0 that is parallel to v1.

Vector2 ParallelComponent(const Vector2& v0, const Vector2& v1){
  const Vector2 v1hat = Normalize(v1);
  return v0.Dot(v1hat)*v1hat;
} //ParallelComponent