/// \file Shape.cpp
/// \brief Code for CShapeDesc and CShape.

#include "Shape.h"

////////////////////////////////////////////////////////////////////////////////////////
//CShapeDesc functions

/// This constructor creates a shape descriptor given
/// a shape type.
/// \param s Shape type.

CShapeDesc::CShapeDesc(eShape s): 
  m_eShapeType(s){
} //constructor

/// The default contructor creates a
/// shape descriptor of unknown type.

CShapeDesc::CShapeDesc(): CShapeDesc(eShape::Unknown){
} //constructor

///////////////////////////////////////////////////////////////////////////////////////
//CShape functions

/// Constructs a shape described by a shape descriptor.
/// It is assumed that the shape is collidable.
/// \param r Shape descriptor.

CShape::CShape(const CShapeDesc& r){
  m_eShapeType = r.m_eShapeType; 
  m_eMotionType = r.m_eMotionType; 
  m_fElasticity = r.m_fElasticity;
  m_bIsSensor = r.m_bIsSensor; 

  SetPos(m_vPos); //move AABB
} //constructor

/// This is needed because virtual functions.
/// Otherwise you risk memory leaks. Apparently.

CShape::~CShape(){
} //destructor

/////////////////////////////////////////////////////////////////////
// CShape reader functions

/// Reader function for the shape type.
/// \return Shape type.

const eShape CShape::GetShapeType() const{
  return m_eShapeType;
} //GetShape

/// Reader function for the motion type.
/// \return Motion type.

const eMotion CShape::GetMotionType() const{
  return m_eMotionType;
} //GetMotion

/// Reader function for the position.
/// \return Position.

const Vector2& CShape::GetPos() const{
  return m_vPos;
} //GetPos

/// Reader function for the axially aligned bounding box.
/// \return Axially aligned bounding box.

const CAabb2D& CShape::GetAABB() const{
  return m_cAABB;
} //GetAABB

/// Reader function for the sensor setting.
/// \return true if this shape is a sensor.

const bool CShape::GetSensor() const{
  return m_bIsSensor;
} //GetSensor

/// Writer function for the position. This both changes the
/// shape's position and translates its AABB.
/// \param p New position.

void CShape::SetPos(const Vector2& p){
  m_vPos = p;

  //translate the AABB to the new position.
  m_cAABB = m_cObjSpaceAABB;
  m_cAABB.Translate(p); 
} //SetPos

/// Set m_cObjSpaceAABB and m_cAABB to a single point.
/// \param p A point in Object Space.

void CShape::SetAABBPoint(const Vector2& p){
  m_cObjSpaceAABB = p; 
  m_cAABB = m_cObjSpaceAABB;
  m_cAABB.Translate(m_vPos); 
} //SetAABBPoint

/// Extend m_cObjSpaceAABB and m_cAABB to cover a new point.
/// \param p A point in Object Space.

void CShape::AddAABBPoint(const Vector2& p){
  m_cObjSpaceAABB += p;
  m_cAABB = m_cObjSpaceAABB;
  m_cAABB.Translate(m_vPos); 
} //AddAABBPoint

/// This function should be used to enable or disable collisions
/// at run-time.
/// \param b true if collisions are enabled, false if disabled.

void CShape::SetCanCollide(bool b){
  m_bCanCollide = b;
} //SetCanCollide

/// Reader function for the collidable flag.
/// \return true if shape can collide.

const bool CShape::GetCanCollide() const{
  return m_bCanCollide;
} //GetCanCollide

/// Reader function for the user pointer.
/// \return User pointer.

void* CShape::GetUserPtr() const{
  return m_pUser;
} //GetUserPtr

/// Writer function for the user pointer.
/// \param p User pointer.

void CShape::SetUserPtr(void* p){
  m_pUser = p;
} //SetUserPtr

//////////////////////////////////
// CShape virtual function stubs for kinematic shapes.

/// Rotate to a given orientation  This virtual function
/// is a stub only. It will be overridden by the appropriate functions
/// that perform a rotation for various specific kinematic shapes.
/// \param v Center of rotation.
/// \param a Angle increment from original orientation.

void CShape::Rotate(const Vector2& v, float a){
} //Rotate

/// Reset to original orientation. This virtual function
/// is a stub only. It will be overridden by the appropriate functions
/// that perform a reset for various specific kinematic shapes.

void CShape::Reset(){
} //Reset

/// Collision detection with a dynamic circle. This virtual function
/// is a stub that will be overridden by the appropriate functions
/// that do collision detection for various specific shapes.
/// \param c [in, out] Contact  descriptor for this collision.
/// \return true is there was a collision.

bool CShape::PreCollide(CContactDesc& c){
  return false;
} //PreCollide

/// Virtual move function. This is for shapes that move, obviously not
/// static ones. Kinematic shapes are handled here. Dynamic shapes
/// get handled by a virtual function in CDynamicCircle.

void CShape::move(){
  if(m_eMotionType == eMotion::Kinematic){
    m_fOrientation += XM_2PI*m_fRotSpeed*m_fTimeStep; //add change in orientation
    m_fOrientation = NormalizeAngle(m_fOrientation); //normalize it for safety
    Rotate(m_vRotCenter, m_fOrientation); //this call to a virtual function will be promoted up to a kinematic shape when possible
  } //if
} //move

//////////////////////////////////////////////////////////////////
//More CShape functions

/// Reader function for orientation.
/// \return Orientation.

const float CShape::GetOrientation() const{
  return m_fOrientation;
} //GetOrientation

/// Reader function for rotation speed.
/// \return Rotation speed.

const float CShape::GetRotSpeed() const{
  return m_fRotSpeed;
} //GetRotSpeed

/// Reader function for the center of rotation.
/// \return Center of rotation.

const Vector2& CShape::GetRotCenter() const{
  return m_vRotCenter;
} //GetRotCenter

/// Reader function for the elasticity.
/// \return Elasticity.

const float CShape::GetElasticity() const{
  return m_fElasticity;
} //GetElasticity

/// \param s Rotation speed.

void CShape::SetRotSpeed(float s){
  m_fRotSpeed = s;
  m_bRotating = s != 0.0f;
} //SetRotSpeed

/// Reader function for rotation flag.
/// \return true if rotating.

 const bool CShape::GetRotating() const{
   return m_bRotating;
 } //GetRotating

/// \param b true to start rotating, false to stop rotating.

void CShape::SetRotating(bool b){
  m_bRotating = b;
} //SetRotating

/// \param p Center of rotation.

void CShape::SetRotCenter(const Vector2& p){
  m_vRotCenter = p;  
} //SetRotCenter

/// \param a Angle.

void CShape::SetOrientation(float a){
  m_fOrientation = a;
} //SetOrientation
