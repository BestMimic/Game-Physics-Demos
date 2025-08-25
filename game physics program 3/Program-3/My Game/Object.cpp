/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "LineSeg.h"
#include "DynamicCircle.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

////////////////////////////////////////////////////////////////////////////////////
// CObjDesc functions.

/// \param s0 Unlit sprite index.
/// \param s1 Lit sprite index.
/// \param snd Collision sound index.

CObjDesc::CObjDesc(eSprite s0, eSprite s1, eSound snd): 
  m_eUnlitSprite(s0), 
  m_eLitSprite(s1), 
  m_vSpriteOffset(Vector2(0.0f)),
  m_eSound(snd),
  m_nScore(0){
} //constructor

////////////////////////////////////////////////////////////////////////////////////
// CObject functions.

CObject::CObject(CShape* p, const CObjDesc& d):
  m_eUnlitSprite(d.m_eUnlitSprite), 
  m_eLitSprite(d.m_eLitSprite), 
  m_vSpriteOffset(d.m_vSpriteOffset),
  m_eSound(d.m_eSound),
  m_pShape(p),
  m_nScore(d.m_nScore){
} //constructor

/// Update object.

void CObject::Update(){
  if(m_pShape->GetMotionType() == eMotion::Dynamic){
    m_nSpriteIndex = (UINT)m_eUnlitSprite;
    m_vPos = m_pShape->GetPos();
  } //else if

  else{
    const float a = m_pShape->GetOrientation();
    const Vector2& v0 = m_vSpriteOffset; //shorthand
    const float x = v0.x*cosf(a) - v0.y*sinf(a);
    const float y = v0.x*sinf(a) + v0.y*cosf(a);
    
    m_nSpriteIndex = (UINT)(m_bRecentHit?m_eLitSprite: m_eUnlitSprite);
    m_vPos = m_pShape->GetPos() + Vector2(x, y);
    m_fRoll = a;
  } //else 

  if(m_pTimer->GetTime() - m_fLastHitTime > 0.1f)
    m_bRecentHit = false;
} //Update

/// Draw only the outline of the object's shape.
/// This is reasonably cheap for line segments
/// but hideously expensive for circles and arcs
/// because it laboriously constructs the curve
/// by drawing very short lines. Ideally
/// I should put it into the pixel shader but I can't
/// be bothered. This is good enough for now.

void CObject::DrawOutline(){
  const eSprite s = eSprite::BlackLine;

  switch(m_pShape->GetShapeType()){
    case eShape::LineSeg: {
      Vector2 p0, p1;
      ((CLineSeg*)m_pShape)->GetEndPts(p0, p1);
      m_pRenderer->DrawLine(s, p0, p1);
    } //case
    break;
      
    case eShape::Circle: {
      const float r = ((CCircle*)m_pShape)->GetRadius();
      const float w = m_pRenderer->GetWidth(s);
      const UINT count = (UINT)ceil(XM_2PI*r/w) + 1;

      for(UINT i=0; i<count; i++){
        const float a = XM_2PI*i/(float)count;
        const Vector2 v2 = m_pShape->GetPos() + r*Vector2(cosf(a), sinf(a));
        m_pRenderer->Draw(s, v2, XM_PI/2.0f + a);
      } //for
    } //case
    break;
      
    case eShape::Arc: {
      CArc* pArc = (CArc*)m_pShape;
      float r = pArc->GetRadius();
      UINT count = (UINT)ceil(XM_2PI*r) + 1;

      for(UINT i=0; i<count; i++){
        const float a = XM_2PI*i/(float)count;
        const Vector2 v2 = m_pShape->GetPos() + r*Vector2(cosf(a), sinf(a));
        if(pArc->PtInSector(v2))
          m_pRenderer->Draw(s, v2, XM_PI/2.0f + a);
      } //for
    } //case
  } //switch
} //DrawOutline

/// Reader function for the object's AABB.
/// It gets this by querying the oblect's shape's AABB.
/// \return The object's AABB.

const CAabb2D& CObject::GetAABB() const{
  return m_pShape->GetAABB();
} //GetAABB

/// Reader function for the object's shape.
/// \return A pointer to the object's shape.

CShape* CObject::GetShape() const{
  return m_pShape;
} //GetShape

/// Reader function for the object's motion type.
/// It gets this by querying the object's shape's motion type.
/// \return The object's motion type.

const eMotion CObject::GetMotionType() const{
  return m_pShape->GetMotionType();
} //GetMotionType
