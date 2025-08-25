/// \file Common.h
/// \brief Interface for the class CCommon.

#ifndef __L4RC_GAME_COMMON_H__
#define __L4RC_GAME_COMMON_H__

#include "GameDefines.h"
#include "ShapeCommon.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates 
/// things that are common to different game components,
/// including game state variables.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.

class CCommon: public CShapeCommon{
  protected:  
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.

    static UINT m_nMIterations; ///< Number of motion iterations.
    static UINT m_nCIterations; ///< Number of collision iterations.

    static float m_fFrequency; ///< Frequency, number of physics iterations per second.
    
    static eDrawMode m_eDrawMode;  ///< Draw mode.
    static bool m_bBallInPlay; ///< Is there a ball currently in play?
    static UINT m_nScore; ///< Current score.
}; //CCommon

#endif //__L4RC_GAME_COMMON_H__
