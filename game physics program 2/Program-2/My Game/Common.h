/// \file Common.h
/// \brief Interface for the class CCommon.

#ifndef __L4RC_GAME_COMMON_H__
#define __L4RC_GAME_COMMON_H__

#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates things that are common to
/// different game components. Making it a singleton class means that we can
/// avoid passing its member variables around as parameters, which makes the
/// code minisculely faster and, more importantly, enhances readability by
/// reducing function clutter.

class CCommon{
  protected:      
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.
    static LParticleEngine2D* m_pParticleEngine; ///< Pointer to particle engine.
    
    static bool m_bShowCollisions; ///< Show ball positions at TOI.
    static bool m_bStepMode; ///< Is in step mode.
    static bool m_bStep; ///< Step flag.

    static float m_fXMargin; ///< Horizontal margin.
    static float m_fYMargin; ///< Vertical margin.
    static Vector2 m_vMargin; ///< Margins.

    static Vector2 m_vTopLPocket; ///< Position of top left pocket.
    static Vector2 m_vTopRPocket; ///< Position of top right pocket.
    static Vector2 m_vTopCPocket; ///< Position of top center pocket.

    static Vector2 m_vBotLPocket; ///< Position of bottom left pocket.
    static Vector2 m_vBotRPocket; ///< Position of bottom right pocket.
    static Vector2 m_vBotCPocket; ///< Position of bottom center pocket.
}; //CCommon

#endif //__L4RC_GAME_COMMON_H__
