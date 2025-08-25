/// \file Parts.cpp
/// \brief Code for the gate class CGate and the flipper class CFipper.

#include "Parts.h"
#include "Compound.h"
#include "GameDefines.h"
#include "Sound.h"

////////////////////////////////////////////////////////////////////////////////////
// CGate functions.

/// Construct a closed and unlatched gate from a line segment.
/// \param p Pointer to a line segment.

CGate::CGate(CLineSeg* p):
  m_pLineSeg(p){
} //constructor

CGate::~CGate(){
  delete m_pLineSeg;
} //destructor

/// If a dynamic circle collides with a gate and it is moving in the
/// correct direction, then the gate opens and the dynamic circle is
/// allowed through. Otherwise the dynamic circle bounces off the
/// gate as usual.
/// \param p Pointer to a dynamic circle.
/// \return true if the dynamic circle bounces off the gate.

bool CGate::NarrowPhase(CDynamicCircle* p){
  bool bHit = false; //return result
  
  CContactDesc cd(m_pLineSeg, p); //contact descriptor
  
  if(m_pLineSeg->PreCollide(cd)){ //there's a collision 
    m_bOccupied = true; //ball is in gate, holding it open
    bHit = true; //it's a hit

    if(!m_bOpen){ //gate is closed
      const Vector2& nhat = m_pLineSeg->GetNormal(); //normal to line segment
      const Vector2 v = cd.m_vPOI - p->GetPos(); //vector from center of *p to POI

      if(nhat.Dot(v) <= 0.0f){ //right way
        //m_pLineSeg->CanCollide(false); //disable collision
        m_bOpen = true; //mark open
        
        if(cd.m_fSpeed > 100.0f) 
          m_pAudio->play(eSound::Tink, cd.m_vPOI);
      } //if

      else{ //wrong way, bounce off 
        p->PostCollide(cd); //bounce off closed gate

        if(cd.m_fSpeed > 100.0f) 
          m_pAudio->play(eSound::Click, cd.m_vPOI, cd.m_fSpeed/1000.0f);
      } //else
    } //if
  } //if

  return bHit;
} //NarrowPhase

/// Close gate if open and there is no ball currently holding
/// it open. Unset the occupied flag ready for use the the next frame.
/// We assume that this is called at the end of the frame.

void CGate::CloseGate(){
  if(m_bOpen && !m_bOccupied) //open but no collision in this frame
    m_bOpen = false; //close the gate

  m_bOccupied = false; //assume no ball is holding the gate open in the next frame
} //CloseGate

////////////////////////////////////////////////////////////////////////////////////
// CFlipper functions.

/// Flippers are compound shapes made up of 2 circles and 2 line segments.
/// \param p Pointer to compound shape for flipper.
/// \param bCCW true if counterclockwise is up.

CFlipper::CFlipper(CCompoundShape* p, bool bCCW): 
  m_pFlipper(p), 
  m_bCCW(bCCW){
} //constructor

CFlipper::~CFlipper(){
  delete m_pFlipper;
} //destructor

const float ROTSPEED = 4.0f; ///< Flipper rotational speed in revs per second.

/// If flipper isn't moving up, set its rotational
/// velocity to ROTSPEED (with the correct sign indicating direction),.
/// \param bUp true for up flip, false for down flip.

void CFlipper::Flip(bool bUp){  
  if(bUp && m_bFlipUp)return; //already flipping up
  if(!bUp && !m_bFlipUp)return; //already not flipping up

  m_bFlipUp = !m_bFlipUp; //flip

  const float speed = (bUp^m_bCCW? -1.0f: 1.0f)*ROTSPEED;
  m_pFlipper->SetRotSpeed(speed); //start rotating in the right direction
} //Flip

/// Enforce bounds on the orientation of flipper, assuming that
/// if it rotates counterclockwise then it is a right flipper,
/// otherwise it is a left flipper.

void CFlipper::EnforceBounds(){
  const float down = (m_bCCW? 11.0f: 7.0f)*XM_PI/6.0f; //angle when fully down
  const float up = (m_bCCW? 1.0f: 3.0f)*XM_PI/4.0f; //angle when fully up

  const float a = m_pFlipper->GetOrientation(); //current angle
  const Vector2 pos = m_pFlipper->GetRotCenter(); //center of rotation

  if(m_bCCW){ //rotating counterclockwise
    if(a < XM_PI && a > up){ //gone past up angle
      m_pFlipper->SetOrientation(up); //reset to up angle
      m_pFlipper->SetRotSpeed(0.0f); //stop
      m_pAudio->play(eSound::FlipUp, pos);
    } //if

    else if(a > XM_PI && a < down){ //gone past down angle
      m_pFlipper->SetOrientation(down); //reset to down angle
      m_pFlipper->SetRotSpeed(0.0f); //stop
      m_pAudio->play(eSound::FlipDown, pos);
    } //else if
  } //if

  else{ //rotating clockwise
    if(a < up){ //gone past up angle
      m_pFlipper->SetOrientation(up); //reset to up angle
      m_pFlipper->SetRotSpeed(0.0f); //stop
      m_pAudio->play(eSound::FlipUp, pos);
    } //if
  
    else if(a > down){ //gone past down angle
      m_pFlipper->SetOrientation(down); //reset to down angle
      m_pFlipper->SetRotSpeed(0.0f); //stop
      m_pAudio->play(eSound::FlipDown, pos);
    } //if
  } //else
} //EnforceBounds