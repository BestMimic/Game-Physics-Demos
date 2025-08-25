/// \file Common.cpp
/// \brief Code for the class CCommon.
///
/// This file contains declarations and initial values
/// for CCommon's static member variables.

#include "Common.h"

CRenderer* CCommon::m_pRenderer = nullptr;
CObjectManager* CCommon::m_pObjectManager = nullptr;

UINT CCommon::m_nMIterations = 4; 
UINT CCommon::m_nCIterations = 1; 

float CCommon::m_fFrequency = 60.0f*m_nMIterations; 

eDrawMode CCommon::m_eDrawMode = eDrawMode::Background;

bool CCommon::m_bBallInPlay = false; 
UINT CCommon::m_nScore = 0; 
