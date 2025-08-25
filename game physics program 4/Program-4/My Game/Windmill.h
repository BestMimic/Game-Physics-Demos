/// \file Windmill.h
/// \brief Interface for the windmill class CWindmill.

#ifndef __L4RC_GAME_WINDMILL_H__
#define __L4RC_GAME_WINDMILL_H__

#include "Object.h"
#include "Box2D\Box2D.h"

#include "Component.h"
#include "Common.h"

/// \brief The windmill class.
/// 
/// A windmill with the vanes propelled by a revolute joint with a motor.
/// @image html screenshot0.png

class CWindmill: 
  public LComponent,
  public CCommon
{
  private:
    b2RevoluteJoint* m_pJoint = nullptr; ///< Pointer to revolute joint.

    b2Body* CreateBlade(float, float); ///< Create the blades.
    b2Body* CreateBase(float, float); ///< Create the base.

  public:
    CWindmill(float, float); ///< Constructor.
    void reverse(); ///< Reverse motor.
}; //CWindmill

#endif //__L4RC_GAME_WINDMILL_H__