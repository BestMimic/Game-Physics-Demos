/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include <vector>

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"

/// \brief The object manager.
///
/// The object manager is an abstract representation of all of
/// the objects in the game.

class CObjectManager:
  public LComponent, 
  public CCommon, 
  public LSettings{

  private:
    std::vector<CObject*> m_stdList; ///< Object list.

  public:
    ~CObjectManager(); ///< Destructor.

    CObject* create(eSprite, b2Body*); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void draw(); ///< Draw all objects.

    void CreateWorldEdges(); ///< Create world edges.
    bool DeleteObject(eSprite); ///< Delete the first object of a given sprite type.
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
