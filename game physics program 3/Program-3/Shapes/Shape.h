/// \file Shape.h
/// \brief Interface for CShapeDesc and CShape.

#ifndef __L4RC_PHYSICS_SHAPE_H__
#define __L4RC_PHYSICS_SHAPE_H__

#include "AABB.h"
#include "ShapeMath.h"
#include "ShapeCommon.h"

/// \brief Shape type.

enum class eShape{
  Unknown, Point, Line, LineSeg, Circle, Arc
}; //eShape

/// \brief Shape motion type.
///
/// Shape motion can be either static, kinematic, or dynamic. Static shapes don't move,
/// they collide with only dynamic shapes, and they are unaffected by collisions.
/// Kinematic shapes move, they collide with only dynamic shapes, and they are 
/// unaffected by collisions. Dynamic shapes move, they collide with shapes of all
/// three motion types, and they are affected by collisions.

enum class eMotion{
  Static, Kinematic, Dynamic, Size
}; //eMotionType

class CContactDesc;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Shape Descriptor.
///
/// The shape descriptor is a simple structure that holds basic information
/// about a shape.

class CShapeDesc{
  public:
    eShape m_eShapeType = eShape::Unknown; ///< Type of shape.
    Vector2 m_vPos;  ///< Position.
    float m_fElasticity = 1.0f; ///< Elasticity, aka restitution, bounciness.
    eMotion m_eMotionType = eMotion::Static; ///< How shape moves.
    bool m_bIsSensor = false; ///< Sensor only, no rebound.

    CShapeDesc(eShape); ///< Constructor.
    CShapeDesc(); ///< Default constructor.
}; //CShapeDesc

///////////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Shape.
///
/// CShape is the base class for all shapes. It contains shape type information
/// and has a couple of handy constructors.

class CShape: public CShapeCommon{
  private:
    Vector2 m_vPos;  ///< Position, access ONLY through get and set functions.
    CAabb2D m_cObjSpaceAABB; ///< Axially aligned bounding box in Object Space.

  protected:
    eShape m_eShapeType = eShape::Unknown; ///< Type of shape.
    eMotion m_eMotionType = eMotion::Static; ///< How shape moves.
    float m_fElasticity = 1.0f; ///< Elasticity, aka restitution, bounciness.
    bool m_bIsSensor = false; ///< Sensor only, no rebound on collision.
    CAabb2D m_cAABB; ///< Axially aligned bounding box in World Space.
    bool m_bCanCollide = true; ///< Can collide with other shapes.

    float m_fOrientation = 0.0f; ///< Orientation angle.

    void* m_pUser; ///< Spare pointer for user in case they might need one.
    
    //for kinematic shapes
    Vector2 m_vRotCenter; ///< Center of rotation.
    float m_fRotSpeed = 0.0f; ///< Rotation speed.
    bool m_bRotating = false; ///< Whether rotating.

  public:  
    CShape(const CShapeDesc&); ///< Constructor.
    virtual ~CShape(); ///< Destructor.

    const eShape GetShapeType() const; ///< Get shape type.
    const eMotion GetMotionType() const; ///< Get motion type.
    const CAabb2D& GetAABB() const; ///< Get AABB.
    const bool GetSensor() const; ///< Is this shape a sensor?

    const Vector2& GetPos() const; ///< Get position.
    void SetPos(const Vector2&); ///< Set position.

    void SetAABBPoint(const Vector2&); ///< Set AABB point.
    void AddAABBPoint(const Vector2&); ///< Add AABB point.

    void SetCanCollide(bool =true); ///< Turn collisions on or off.
    const bool GetCanCollide() const; ///< Get whether shape can collide.

  public: //for kinematic shapes
    //virtual function stubs for kinematic shapes
    virtual void Rotate(const Vector2&, float); ///< Rotate.
    virtual void Reset(); ///< Reset orientation.
    virtual bool PreCollide(CContactDesc&); ///< Collision detection.
    virtual void move(); ///< Translate.

    const bool GetRotating() const; ///< Get whether rotating.
    void SetRotating(bool); ///< Start or stop rotating.

  public: //reader and writer functions
    const float GetOrientation() const; ///< Get orientation.
    const float GetRotSpeed() const; ///< Get rotation speed.
    const Vector2& GetRotCenter() const; ///< Get rotation speed.
    const float GetElasticity() const; ///< Get elasticity.
    
    void SetOrientation(float); ///< Set orientation.
    void SetRotSpeed(float); ///< Set rotation speed.
    void SetRotCenter(const Vector2&); ///< Set center of rotation.

    void* GetUserPtr() const; ///< Get user pointer.
    void SetUserPtr(void*); ///< Set user pointer.
}; //CShape

#endif //__L4RC_PHYSICS_SHAPE_H__
