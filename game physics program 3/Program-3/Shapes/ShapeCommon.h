/// \file ShapeCommon.h
/// \brief Interface for the class CShapeCommon.

#ifndef __L4RC_PHYSICS_SHAPECOMMON_H__
#define __L4RC_PHYSICS_SHAPECOMMON_H__

/// \brief The shape common variables class.
///
/// CShapeCommon is a singleton class that encapsulates 
/// things that are common to different shapes.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.

class CShapeCommon{
  protected:  
    static float m_fGravity; ///< Gravitational constant.
    static float m_fTimeStep; ///< Time step per animation frame (fictional).
}; //CShapeCommon

#endif //__L4RC_PHYSICS_SHAPECOMMON_H__
