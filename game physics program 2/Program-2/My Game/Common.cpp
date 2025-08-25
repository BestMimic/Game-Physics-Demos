/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// Declarations and initial values for CCommon static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;

LParticleEngine2D* CCommon::m_pParticleEngine = nullptr;

bool CCommon::m_bShowCollisions = false; //testing stuff
bool CCommon::m_bStepMode = false;
bool CCommon::m_bStep = false;

float CCommon::m_fXMargin = 78.0f;
float CCommon::m_fYMargin = 64.0f;
Vector2 CCommon::m_vMargin = Vector2(m_fXMargin, m_fYMargin);

Vector2 CCommon::m_vTopLPocket = Vector2(71, 478);
Vector2 CCommon::m_vTopRPocket = Vector2(955, 478);
Vector2 CCommon::m_vTopCPocket = Vector2(514, 478);

Vector2 CCommon::m_vBotLPocket = Vector2(71, 53);
Vector2 CCommon::m_vBotRPocket = Vector2(955, 53);
Vector2 CCommon::m_vBotCPocket= Vector2(514, 48);
