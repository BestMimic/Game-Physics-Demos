/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr; 
b2World* CCommon::m_pPhysicsWorld = nullptr; 
CObjectManager* CCommon::m_pObjectManager = nullptr;

CWindmill* CCommon::m_pWindmill      = nullptr; 
