/// \file Contact.cpp
/// \brief Code for the contact descriptor class CContactDesc.

#include "Contact.h"

/// This constructor constructs a contact descriptor
/// given a pointer to a shape and a pointer to a dynamic
/// circle. No attempt is made to determine whether
/// these shapes are indeed in contact, and the pointers
/// may be null.

CContactDesc::CContactDesc(CShape* p0, CDynamicCircle* p1): 
  m_pShape(p0), 
  m_pCircle(p1),
  m_fSetback(0.0f),
  m_fSpeed(0.0f),
  m_vPOI(Vector2(0.0f)), 
  m_vNorm(Vector2(1.0f, 0.0f)){
} //constructor

/// The default constructor constructs a contact descriptor
/// whose pointers are null. What could possibly go wrong?

CContactDesc::CContactDesc(): 
  CContactDesc(nullptr, nullptr){
} //default constructor
