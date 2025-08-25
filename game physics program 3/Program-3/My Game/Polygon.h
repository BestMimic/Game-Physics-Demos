/// \file Polygon.h
/// \brief Interface for the polygon class CPolygon.

#ifndef __L4RC_GAME_POLYGON_H__
#define __L4RC_GAME_POLYGON_H__

#include "Compound.h"

/// \brief A polygon made up of shapes.
///
/// A compound shape for a regular polygon.

class CPolygon: public CCompoundShape{
  private:
    CShape* m_pCenterPoint = nullptr; ///< Point at polygon center.

  public:
    CPolygon(CShape*); ///< Create a polygon.

    bool IsPartOfPolygon(CShape*); ///< Test whether shape is a part of this polygon.
    CShape* GetCenterPoint(); ///< Get pointer to polygon center.
}; //CPolygon

#endif
