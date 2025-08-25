/// \file Contact.h
/// \brief Interface for the contact descriptor class CContactDesc.

#ifndef __L4RC_PHYSICS_CONTACT_H__
#define __L4RC_PHYSICS_CONTACT_H__

#include "DynamicCircle.h"

/// \brief Contact descriptor.
///
/// A record of contact between a shape 
/// (which may be static, kinematic, or dynamic)
/// and a dynamic circle. Collision detection functions
/// take as input a contact descriptor with the pointers
/// to the shapes, and it fills in the details of the contact
/// if there is one. This filled-out contact descriptor
/// is then passed to the collision response function.

class CContactDesc{
  public:
    CShape* m_pShape = nullptr; ///< Pointer to static or kinematic shape.
    CDynamicCircle* m_pCircle= nullptr; ///< Pointer to dynamic circle.
  
    Vector2 m_vPOI; ///< POI.
    Vector2 m_vNorm; ///< Normal to collision.

    float m_fSetback = 0.0f; ///< Setback distance.
    float m_fSpeed = 0.0f; ///< Collision speed.

    CContactDesc(CShape*, CDynamicCircle*); ///< Constructor.
    CContactDesc(); ///< Default constructor.
}; //CContactDesc

#endif //__L4RC_PHYSICS_CONTACT_H__
