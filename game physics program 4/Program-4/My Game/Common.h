/// \file Common.h
/// \brief Interface for the class CCommon.

#ifndef __L4RC_GAME_COMMON_H__
#define __L4RC_GAME_COMMON_H__

#include "GameDefines.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class b2World;
class CRenderer;

class CWindmill; 

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

class CCommon{
  protected:   
    static b2World* m_pPhysicsWorld; ///< Pointer to Box2D Physics World.
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.
    
    static CWindmill* m_pWindmill; ///< Pointer to windmill.
}; //CCommon

#endif //__L4RC_GAME_COMMON_H__
