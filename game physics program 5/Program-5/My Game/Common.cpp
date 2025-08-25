/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr; 
b2World* CCommon::m_pPhysicsWorld = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;
LParticleEngine2D* CCommon::m_pParticleEngine = nullptr;

float CCommon::m_fStartTime = 0;
float CCommon::m_fTotalTime = 0;
int CCommon::Health = 3;

eGameState CCommon::m_eGameState = eGameState::Initial;
eDrawMode CCommon::m_eDrawMode = eDrawMode::Sprites;

CKraid* CCommon::m_pKraid = nullptr;