/// \file ShapeMath.h
/// \brief Handy math functions for the collision module.

#ifndef __L4RC_PHYSICS_SHAPEMATH_H__
#define __L4RC_PHYSICS_SHAPEMATH_H__

#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

/// Fail by returning false if a given condition is true.
/// \param x Condition for failure.

#define FailIf(x) if(x)return false; 

/// \brief Handy squaring function.
/// Compute the square of any number.
/// \param x A number.
/// \return x squared.

#define sqr(x) ((x)*(x)) 

float NormalizeAngle(float); ///< Normalize angle to [0, 2*pi).
Vector2 Normalize(Vector2); ///< Normalize a vector, ie. make it unit length.
Vector2 perp(const Vector2&); ///< Perpendicular vector.
Vector2 AngleToVector(const float); ///< Normal vector from orientation.
Vector2 RotatePt(Vector2, const Vector2&, const float); ///< Rotate point.

Vector2 ParallelComponent(const Vector2&, const Vector2&); ///< Compute parallel component of vector.

#endif //__L4RC_PHYSICS_SHAPEMATH_H__