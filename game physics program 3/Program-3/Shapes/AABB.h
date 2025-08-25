/// \file AABB.h
/// \brief Interface for the AABB class CAabb2D.

#ifndef __L4RC_PHYSICS_AABB_H__
#define __L4RC_PHYSICS_AABB_H__

#include <windows.h>
#include <windowsx.h>

#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

/// \brief 2D Axially Aligned Bounding Box.
///
/// An AABB, or Axially Aligned Bounding Box, is a rectangle with 
/// axes aligned with the cardinal axes. 

class CAabb2D{
  private:
    Vector2 m_vTopLeft; ///< Top left point.
    Vector2 m_vBottomRt; ///< Bottom right point.

    static int m_nTestCount; ///< Number of AABB to AABB intersection tests.

  public:
    CAabb2D(const Vector2&, const Vector2& ); ///< Constructor.
    CAabb2D(); ///< Default constructor.

    void Translate(const Vector2&); ///< Translate AABB.

    CAabb2D& operator=(const Vector2&); ///< Set AABB to point.
    CAabb2D& operator+=(const Vector2&); ///< Add point to AABB.

    friend bool operator&&(const CAabb2D&, const CAabb2D&); ///< AABB intersection test.
    friend bool operator&&(const CAabb2D&, const Vector2&); ///< AABB intersection test.

    float GetWidth(); ///< Get width of AABB.
    float GetHt(); ///< Get height of AABB.
    const Vector2& GetTopLeft(); ///< Get top left corner.
    const Vector2& GetBottomRt(); ///< Get bottom right corner.

    int GetTestCount(); ///< Get number of AABB to AABB intersection tests.
}; //CAabb2D

#endif //__L4RC_PHYSICS_AABB_H__