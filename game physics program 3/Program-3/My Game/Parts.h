/// \file Parts.h
/// \brief Interface for the gate class CGate and the flipper class CFipper.

#ifndef __L4RC_GAME_PARTS_H__
#define __L4RC_GAME_PARTS_H__

#include "GameDefines.h"
#include "DynamicCircle.h"
#include "Contact.h"
#include "Component.h"
#include "Common.h"

#include "Compound.h"

/// \brief A gate.
///
/// A gate allows balls to cross in one direction only. It's essentially a line
/// segment with a couple of Boolean variables and some code that handles the
/// crossing-in-one-direction requirement. Care must be taken to do the right
/// thing when a ball gets nudged back while in the process of crossing.
///
/// A gate can be open or closed, latched or unlatched. When it is open, there
/// can be no collisions. When it is closed, balls collide with it if they are
/// travelling from one side but not the other. More specifically, if the vector
/// from the ball's center to the POI is in the opposite direction to  the line
/// segment's normal vector `CLineSeg::m_vNormal` then they can cross. Recall that
/// the direction of `CLineSeg::m_vNormal` depends on the order in which
/// `CLineSeg`'s end points were specified, so make sure you get it right.

class CGate: 
  public LComponent,
  public CCommon{

  private:
    CLineSeg* m_pLineSeg = nullptr; ///< Pointer to line segment representing gate.

    bool m_bOpen = false; ///< true if gate is open.
    bool m_bOccupied = false; ///< true if ball is holding gate open.

  public:
    CGate(CLineSeg* p); ///< Constructor.
    ~CGate(); ///< Destructor.

    void CloseGate(); ///< Check latch to see if gate should be closed.
    bool NarrowPhase(CDynamicCircle*); ///< Narrow phase collision detection and response.
}; //CGate

/// \brief A flipper.
///
/// From Wikipedia:
/// "The flippers are one or more small mechanically or electromechanically
/// controlled levers, roughly 3 to 7 cm in length, used for redirecting the
/// ball up the playfield. They are the main control that the player has over
/// the ball. Careful timing and positional control allows the player to
/// intentionally direct the ball in a range of directions with various levels
/// of velocity. With the flippers, the player attempts to move the ball to hit
/// various types of scoring targets, and to keep the ball from disappearing off
/// the bottom of the playfield." `CFlipper` implements one of those. It's made
/// up of a couple of circles and a couple of line segments that are tangents to
/// both of them.

class CFlipper: 
  public LComponent,
  public CCommon{

  private:    
    CCompoundShape* m_pFlipper = nullptr; ///< Pointers to flipper compound shapes.
    bool m_bFlipUp = false; ///< Flipper state.
    bool m_bCCW = false; ///< Whether it rotates counterclockwise for up.

  public:  
    CFlipper(CCompoundShape*, bool); ///< Constructor.
    ~CFlipper(); ///< Destructor.
    
    void Flip(bool); ///< Flip flipper.
    void EnforceBounds(); ///< Enforce bounds.
}; //CFlipper

#endif //__L4RC_GAME_PARTS_H__