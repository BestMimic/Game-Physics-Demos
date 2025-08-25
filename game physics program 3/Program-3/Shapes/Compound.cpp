/// \file Compound.cpp
/// \brief Code for the compound shape class CCompoundShape.

#include "Compound.h"
#include "DynamicCircle.h"

/// Append a given shape to the shape list. There is no attempt to ensure that it's
/// not already in there, so beware.

void CCompoundShape::AddShape(CShape* p){
  m_stdShapes.push_back(p);
} //AddShape

/// Set the rotation speed of all of the shapes in the shape list.
/// Make sure you call this after all shapes have been added,
/// since this rotation speed won't be applied to
/// shapes added after this function is called.
/// \param s Rotation speed in radians per millisecond.

void CCompoundShape::SetRotSpeed(float s){
  for(auto const &p: m_stdShapes)
    p->SetRotSpeed(s);
} //SetRotSpeed

/// Set the center of rotation of all of the shapes in the shape list.
/// Make sure you call this after all shapes have been added,
/// since this rotation center won't be applied to
/// shapes added after this function is called.
/// \param p Center of rotation.

void CCompoundShape::SetRotCenter(const Vector2& p){ 
  for(auto const &q: m_stdShapes)
    q->SetRotCenter(p);
} //SetRotCenter

/// Reader function for the center of rotation. 
/// This function assumes that all of the shapes
/// in the shape list have the same rotation speed, so
/// it just returns the center of rotation of the first shape
/// (or the zero vector if there are no shapes yet).
/// \return The position of the center of rotation.

Vector2 CCompoundShape::GetRotCenter(){
  if(m_stdShapes.size() > 0)
    return m_stdShapes[0]->GetRotCenter();
  else return Vector2(0.0f);
} //GetRotCenter

/// Set the orientation of all of the shapes in the shape list.
/// Make sure you call this after all shapes have been added,
/// since this orientation won't be applied to
/// shapes added after this function is called.
/// \param a Angle.

void CCompoundShape::SetOrientation(float a){
  for(auto const &p: m_stdShapes)
    p->SetOrientation(a);
} //SetOrientation

/// Reader function for the current orientation.
/// This function assumes that all of the shapes
/// in the shape list have the same orientation, so
/// it just returns the orientation of the first shape
/// (or zero if there are no shapes yet).
/// \return Orientation.

float CCompoundShape::GetOrientation(){
  if(m_stdShapes.size() > 0)
    return m_stdShapes[0]->GetOrientation();
  else return 0.0f;
} //GetOrientation

/// Reader function for the current rotation speed.
/// This function assumes that all of the shapes
/// in the shape list have the same rotation speed, so
/// it just returns the rotation speed of the first shape
/// (or zero if there are no shapes yet).
/// \return Rotation speed.

float CCompoundShape::GetRotSpeed(){
  if(m_stdShapes.size() > 0)
    return m_stdShapes[0]->GetRotSpeed();
  else return 0.0f;
} //GetRotSpeed


