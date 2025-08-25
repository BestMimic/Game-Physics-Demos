/// \file DynamicCircle.h
/// \brief Interface for CDynamicCircleDesc and CDynamicCircle.

#ifndef __L4RC_PHYSICS_DYNAMICCIRCLE_H__
#define __L4RC_PHYSICS_DYNAMICCIRCLE_H__

#include "LineSeg.h"
#include "Arc.h"

/// \brief Dynamic circle descriptor.
///
/// The dynamic circle descriptor describes a dynamic circle.

class CDynamicCircleDesc: public CCircleDesc{
  public:
    Vector2 m_vVel; ///< Velocity. Speed is measured in pixels per second.

    CDynamicCircleDesc(); ///< Constructor.
}; //CDynamicCircleDesc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Dynamic circle.
///
/// A dynamic circle is a circle shape that moves and can collide with 
/// static, kinematic, and dynamic shapes.

class CDynamicCircle: public CCircle{
  private:
    Vector2 m_vVel; ///< Velocity. Speed is measured in pixels per second.
    float m_fMass = 0.0f; ///< Mass.
    
    void PostCollideStatic(const CContactDesc&); ///< Collision response for static shape.
    void PostCollideKinematic(const CContactDesc&); ///< Collision response for kinematic shape.
    void PostCollideDynamic(const CContactDesc&); ///< Collision response for dynamic shape.

  public:
    CDynamicCircle(const CDynamicCircleDesc&); ///< Constructor.
    void move(); ///< Move using Euler integration.
    
    bool AABBCollide(CDynamicCircle*);  ///< Collide detection using AABBs.
    void PostCollide(const CContactDesc&);  ///< Collision response 

    Vector2 GetVel(); ///< Get velocity.  
    void SetVel(const Vector2&); ///< Set velocity.
}; //CDynamicCircle

#endif //__L4RC_PHYSICS_DYNAMICCIRCLE_H__