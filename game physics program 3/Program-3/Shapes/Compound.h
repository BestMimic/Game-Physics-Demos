/// \file Compound.h
/// \brief Interface for the compound shape class CCompoundShape.

#ifndef __L4RC_PHYSICS_COMPOUND_H__
#define __L4RC_PHYSICS_COMPOUND_H__

#include <vector>

#include "Point.h"
#include "Contact.h"
#include "DynamicCircle.h"

/// \brief Compound shape.
///
/// A compound shape consists of a collection of shapes
/// that ought to be grouped together for convenience.

class CCompoundShape{
  protected:
    std::vector<CShape*> m_stdShapes; ///< List of shapes.

  public:
    void AddShape(CShape* p); ///< Add a shape.
    
    void SetOrientation(float); ///< Set orientation.
    void SetRotSpeed(float); ///< Set rotation speed.
    void SetRotCenter(const Vector2&); ///< Set center of rotation.
    
    float GetOrientation(); ///< Get orientation.
    float GetRotSpeed(); ///< Get rotation speed.
    Vector2 GetRotCenter(); ///< Get center of rotation.
}; //CCompoundShape

#endif //__L4RC_PHYSICS_COMPOUND_H__
