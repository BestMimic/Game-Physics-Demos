/// \file ObjectManager.h
/// \brief Interface for the object manager class CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include <vector>

#include "DynamicCircle.h"
#include "Parts.h"

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"
#include "SpriteDesc.h"
#include "Polygon.h"

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public CCommon, 
  public LComponent,
  public LSettings{

  private:  
    std::vector<CShape*> m_stdShapes[(UINT)eMotion::Size]; ///< Array of lists of shapes.
    std::vector<CObject*> m_stdObjects; ///< Object list.
    
    CGate* m_pLeftGate = nullptr; ///< Pointer to left gate.
    CGate* m_pRightGate = nullptr; ///< Pointer to right gate.
    
    std::vector<CPolygon*> m_vBumperList; ///< Bumper list.

    CAabb2D m_cAABB; ///< AABB for the whole window.

    CFlipper* m_pLeftFlipper = nullptr; ///< Pointer to left flipper.
    CFlipper* m_pRightFlipper = nullptr; ///< Pointer to right flipper.
    
    CShape* MakeShape(CShapeDesc*, const CObjDesc&); ///< Make a shape.

    void BroadPhase(); ///< Broad phase collision detection and response.
    bool NarrowPhase(CShape*, CDynamicCircle*); ///< Narrow phase collision detection and response. 
     
    void MakeBumper(UINT, const Vector2&, float, eSprite, eSprite, eSound , UINT); ///< Make a polygonal bumper.

    void MakeBollard(const Vector2&); ///< Make a bollard.
    CCompoundShape* MakeFlipper(const Vector2&, const Vector2&, float); ///< Make a flipper.
    void MakeThingL(); ///< Make a thing (left).
    void MakeThingR(); ///< Make a thing (right).

  public:
    ~CObjectManager(); ///< Destructor.
    
    CShape* AddShape(CShapeDesc*, const CObjDesc&); ///< Add shape.

    void move(); ///< Move all objects.  
    void draw(); ///< Draw all objects.
    void DrawOutlines(); ///< Draw outlines of all objects.

    void MakeWorldEdges(); ///< Create shapes for world edges.
    void MakeShapes(); ///< Create shapes.
    
    void LeftFlip(bool); ///< Flip left flipper.
    void RightFlip(bool); ///< Flip right flipper.
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
