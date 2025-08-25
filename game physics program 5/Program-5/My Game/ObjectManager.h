/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include <vector>

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"

/// \brief object manager.
///
/// object manager is an abstract representation of all of
/// the objects in the game.

class CObjectManager: 
  public LComponent, 
  public LSettings,
  public CCommon 
{
  private:
    std::vector<CObject*> m_stdList; ///< Object list.

  public:
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.

    CObject* create(eSprite, b2Body*); ///< Create new object.

    void CreateObject(eSprite t, b2Body* p); ///< Create object.

    void clear(); ///< Reset to initial conditions.
    void draw(); ///< Draw all objects.

    /*b2Body*/void CreateWalls();
    void CreateWorldEdges(); ///< Create the edges of the world.
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__