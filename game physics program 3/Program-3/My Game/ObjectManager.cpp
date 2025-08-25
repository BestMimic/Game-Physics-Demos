/// \file ObjectManager.cpp
/// \brief Code for the object manager class CObjectManager.

#include "ObjectManager.h"
#include "Parts.h"
#include "Renderer.h"
#include "Compound.h"
#include "ComponentIncludes.h"

const float TOP_MARGIN = 60.0f; ///< Height of top margin.

/// The destructor clears the shape lists, which destructs
/// all of the shapes in them.

CObjectManager::~CObjectManager(){
  for(eMotion m: {eMotion::Static, eMotion::Kinematic})
    for(auto const& p: m_stdShapes[(UINT)m])
      delete p; 

  for(auto const &p: m_stdObjects)
    delete p; 

  for(auto const &p: m_vBumperList)
    delete p; 

  //delete parts

  delete m_pLeftFlipper;
  delete m_pRightFlipper;

  delete m_pLeftGate;
  delete m_pRightGate;
} //destructor

/// Make the static shapes for the world boundaries. As with most physics code, this
/// function is long and tedious, but it's important to get the details right.

void CObjectManager::MakeWorldEdges(){ 
  const float w = (float)m_nWinWidth;
  const float h = (float)m_nWinHeight;
  
  m_cAABB = CAabb2D(Vector2(0.0f, h), Vector2(w, 0.0f));

  const Vector2 p0 = Vector2(0.0f, 0.0f);
  const Vector2 p1 = Vector2(w, 0.0f);
  
  const Vector2 p2 = Vector2(w, h - w/2.0f - TOP_MARGIN);
  const Vector2 p3 = Vector2(0.0f, h - w/2.0f - TOP_MARGIN);

  //right side vertical 
  
  CLineSegDesc lsDesc(p1, p2, 0.9f);
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, eSound::Size);
  AddShape(&lsDesc, nullObjDesc);

  //left side vertical

  lsDesc.SetEndPts(p3, p0);
  AddShape(&lsDesc, nullObjDesc);

  //arc at top 
  
  const Vector2 p4 = Vector2(w/2.0f, h - w/2.0f - TOP_MARGIN);
  
  CArcDesc arcDesc(p4, w/2.0f, 0.0f, 1.15f*XM_PI, 0.8f); 
  CArc* pArc = (CArc*)AddShape(&arcDesc, nullObjDesc);
  
  Vector2 u0, u1, u3;
  arcDesc.GetEndPts(u0, u1);
  Vector2 vTangent0, vTangent1;
  pArc->GetTangents(vTangent0, vTangent1);
  vTangent1.Normalize();
  u3 = u1 + 64.0f*vTangent1;

  lsDesc.SetEndPts(u1, u3);
  AddShape(&lsDesc, nullObjDesc);

  CPointDesc ptDesc;

  ptDesc.m_vPos = u3;
  AddShape(&ptDesc, nullObjDesc);
  
  vTangent1 = Vector2(vTangent1.y, -vTangent1.x);
  Vector2 u4 = u3 + 64.0f*vTangent1;
  lsDesc.SetEndPts(u3, u4);
  lsDesc.m_fElasticity = 1000.0f; 
  CObjDesc lsObjDesc(eSprite::UnlitSpecial, eSprite::LitSpecial, eSound::Beep);
  lsObjDesc.m_nScore = 1;
  const float sr = m_pRenderer->GetWidth(eSprite::UnlitSpecial);
  Vector2 v0, v1;
  lsDesc.GetEndPts(v0, v1);
  Vector2 n = v0 - v1;
  n.Normalize();
  n = Vector2(-n.y, n.x);
  lsObjDesc.m_vSpriteOffset = sr*n;
  AddShape(&lsDesc, lsObjDesc);

  //gates at top of table

  //the actual top arc was created above... this is an imaginary arc inside it
  const float r = w/2.0f - 1.5f*m_pRenderer->GetWidth(eSprite::Ball);
  arcDesc.m_fRadius = r;
  arcDesc.SetAngles(0.0f, XM_PI/6.0f);
  Vector2 q0, q1;
  arcDesc.GetEndPts(q0, q1);
  q1 += Vector2(-2.5f, 2.5f);
  lsDesc.SetEndPts(q1, pArc->ClosestPt(q1));
  lsDesc.m_fElasticity = 0.6f;
  CObjDesc lsObjDesc2(eSprite::None, eSprite::None, eSound::Click);
  
  CLineSeg* pShape = (CLineSeg*)MakeShape(&lsDesc, lsObjDesc2);
  m_pRightGate = new CGate(pShape);
  
  arcDesc.SetAngles(5.0f*XM_PI/6.0f, XM_PI);  
  arcDesc.GetEndPts(q0, q1);
  q0 += Vector2(2.5f, 2.5f);
  lsDesc.SetEndPts(pArc->ClosestPt(q0), q0);
  
  pShape = (CLineSeg*)MakeShape(&lsDesc, lsObjDesc2);
  m_pLeftGate = new CGate(pShape);

  //line segment to protect new ball

  const float lx = w - 1.5f*m_pRenderer->GetWidth(eSprite::Ball);
  const float ly = p4.y;
  
  const Vector2 p5 = Vector2(lx, 0.0f);
  const Vector2 p6 = Vector2(lx, ly);

  lsDesc.SetEndPts(p5, p6);
  AddShape(&lsDesc, nullObjDesc);
  
  //bottom of chute
  lsDesc.SetEndPts(p1, p5);
  lsDesc.m_fElasticity = 0.1f;
  AddShape(&lsDesc, lsObjDesc2);
  
  //LHS flipper base and associated shapes
  
  const float mid = 196.0f;
  
  const float r2 = 10.0f;
  const float dx = 1.2f*m_pRenderer->GetWidth(eSprite::Ball);

  const Vector2 p7 = Vector2(dx + r2, h - 754.0f);
  const Vector2 p8 = Vector2(112.0f, 67.0f);

  const Vector2 p9 = Vector2(0.0f, 80.0f);
  const Vector2 p10 = Vector2(mid - 75.0f, 16.0f);

  CCircleDesc circDesc(p7, r2, 0.4f);
  CCircle* pCirc0 = (CCircle*)AddShape(&circDesc, nullObjDesc);
  
  CCircleDesc circDesc2(p8, r2, 0.4f);
  CCircle* pCirc1 = (CCircle*)AddShape(&circDesc2, nullObjDesc);

  CLineSegDesc lsDesc0, lsDesc1;
  pCirc1->Tangents(pCirc0, lsDesc0, lsDesc1);
  lsDesc0.m_fElasticity = 1000.0f;
  lsDesc1.m_fElasticity = 0.4f;
  CObjDesc lsObjDesc3(eSprite::UnlitSpecial, eSprite::LitSpecial, eSound::Beep);
  lsObjDesc3.m_nScore = 1;
  lsDesc0.GetEndPts(v0, v1);
  n = v1 - v0;
  n.Normalize();
  n = Vector2(-n.y, n.x);
  lsObjDesc3.m_vSpriteOffset = sr*n;
  AddShape(&lsDesc0, lsObjDesc3);
  AddShape(&lsDesc1, nullObjDesc);

  const Vector2 p7a = p7 + Vector2(-r2, 0.0f);
  const Vector2 p7b = p7 + Vector2(-r2, 150.0f);

  //vertical line
  lsDesc0.SetEndPts(p7a, p7b);
  lsDesc0.m_fElasticity = 0.2f;
  AddShape(&lsDesc0, nullObjDesc);

  ptDesc.m_vPos = p7b;
  ptDesc.m_fElasticity = 0.2f;
  AddShape(&ptDesc, nullObjDesc);
  
  //lower diagonal
  lsDesc.SetEndPts(p9, p10);
  lsDesc.m_fElasticity = 0.1f;
  AddShape(&lsDesc, nullObjDesc);

  ptDesc.m_vPos = p10;
  AddShape(&ptDesc, nullObjDesc);
  
  lsDesc.SetEndPts(p10, Vector2(p10.x, 0.0f));
  AddShape(&lsDesc, nullObjDesc);
  
  //RHS flipper base and associated shapes

  const Vector2 p11 = Vector2(2.0f*mid - p7.x, p7.y);
  const Vector2 p12 = Vector2(2.0f*mid - p8.x, p8.y);
  const Vector2 p13 = Vector2(2.0f*mid - p9.x, p9.y);
  const Vector2 p14 = Vector2(2.0f*mid - p10.x, p10.y);
  
  CCircleDesc circDesc3(p11, r2, 0.4f);
  pCirc0 = (CCircle*)AddShape(&circDesc3, nullObjDesc);

  const Vector2 p11a = p11 + Vector2(r2, 0.0f);
  const Vector2 p11b = p11 + Vector2(r2, 150.0f);

  //vertical line
  lsDesc0.SetEndPts(p11a, p11b);
  lsDesc0.m_fElasticity = 0.2f;
  AddShape(&lsDesc0, nullObjDesc);

  ptDesc.m_vPos = p11b;
  ptDesc.m_fElasticity = 0.2f;
  AddShape(&ptDesc, nullObjDesc);
  
  CCircleDesc circDesc4(p12, r2, 0.4f);
  pCirc1 = (CCircle*)(AddShape(&circDesc4, nullObjDesc));

  pCirc1->Tangents(pCirc0, lsDesc0, lsDesc1);
  lsDesc0.m_fElasticity = 0.4f;
  lsDesc1.m_fElasticity = 2000.0f;
  lsDesc1.GetEndPts(v0, v1);
  n = perp(Normalize(v1 - v0));
  lsObjDesc3.m_vSpriteOffset = sr*n;
  AddShape(&lsDesc0, nullObjDesc);
  AddShape(&lsDesc1, lsObjDesc3);

  //lower diagonal
  lsDesc.SetEndPts(p13, p14);
  AddShape(&lsDesc, nullObjDesc);
  
  ptDesc.m_vPos = p14;
  AddShape(&ptDesc, nullObjDesc);
  
  lsDesc.SetEndPts(p14, Vector2(p14.x, 0.0f));
  AddShape(&lsDesc, nullObjDesc);
} //MakeWorldEdges

/// A bollard is one of the long skinny things at the top.
/// \param p Position.

void CObjectManager::MakeBollard(const Vector2& p){ 
  const float e = 0.4f;
  const float h = 54.0f;
  const float w = 10.0f;
  
  Vector2 p0 = p + Vector2(-w/2.0f, h/2.0f);
  Vector2 p1 = p + Vector2(-w/2.0f, -h/2.0f);
  Vector2 p2 = p + Vector2(w/2.0f, h/2.0f);
  Vector2 p3 = p + Vector2(w/2.0f, -h/2.0f);
  
  CCircleDesc circleDesc(p - Vector2(0.0f, h/2.0f), w/2.0f, e);
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, eSound::Size);
  CCircle* pCirc0 = (CCircle*)AddShape(&circleDesc, nullObjDesc);

  circleDesc.m_vPos = p + Vector2(0.0f, h/2.0f);
  CObjDesc bollardObjDesc(eSprite::None, eSprite::None, eSound::Size);
  bollardObjDesc.m_vSpriteOffset = Vector2(0.0f, -25.0f);
  CCircle* pCirc1 = (CCircle*)AddShape(&circleDesc, bollardObjDesc);

  CLineSegDesc lsDesc0, lsDesc1;
  pCirc0->Tangents(pCirc1, lsDesc0, lsDesc1);
  AddShape(&lsDesc0, nullObjDesc);
  AddShape(&lsDesc1, nullObjDesc);
} //MakeBollard

/// Make a polygonal bumper out of lines and points.
/// \param n Number of sides.
/// \param p Position of polygon center.
/// \param e Elasticity.
/// \param unlit Unlit sprite index.
/// \param lit Lit sprite index.
/// \param snd Collision sound index.
/// \param nScore Score for hitting bumper.

void CObjectManager::MakeBumper(UINT n, const Vector2& p, float e, 
  eSprite unlit, eSprite lit, eSound snd, UINT nScore)
{   
  if(n < 3)ABORT("Too few edges in polygon");

  //multimedia descriptors
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, snd);
  CObjDesc bumperObjDesc(unlit, lit, snd);
  bumperObjDesc.m_nScore = nScore;

  CObjDesc lineDesc(eSprite::BlackLine, eSprite::BlackLine, snd);
  CPointDesc pointDesc(p);

  CShape* pCenterPoint = AddShape(&pointDesc, bumperObjDesc);
  CPolygon* pBumper = new CPolygon(pCenterPoint); 
   
  //****ALL STUDENTS: YOUR CODE STARTS HERE
  if (unlit == eSprite::UnlitTriangle) {
      Vector2 p0, p1, p2;
      float w = m_pRenderer->GetWidth(unlit);
      p0.x = p.x;
      p0.y = p.y + (sqrt(3) / 3) * w;

      p1.x = p.x - w / 2;
      p1.y = p.y - (sqrt(3) / 6) * w;

      p2.x = p.x + w / 2;
      p2.y = p.y - (sqrt(3) / 6) * w;

      CLineSegDesc lsDesc(p1, p0, 0.9f);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p2, p0);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p2, p1);
      AddShape(&lsDesc, lineDesc);
  }
  else if (unlit == eSprite::UnlitDiamond) {
      Vector2 p0, p1, p2, p3;
      float w = (m_pRenderer->GetWidth(unlit))/2;
      float s = sqrt(w*w + w*w);

      p0.x = p.x - (s / 2) * (sin(0.78f) + cos(0.78f));
      p0.y = p.y - (s / 2) * (sin(0.78f) - cos(0.78f));

      p1.x = p.x + (s / 2) * (sin(0.78f) - cos(0.78f));
      p1.y = p.y - (s / 2) * (sin(0.78f) + cos(0.78f));

      p2.x = p.x + (s / 2) * (sin(0.78f) + cos(0.78f));
      p2.y = p.y + (s / 2) * (sin(0.78f) - cos(0.78f));

      p3.x = p.x - (s / 2) * (sin(0.78f) - cos(0.78f));
      p3.y = p.y + (s / 2) * (sin(0.78f) + cos(0.78f));

      // I am the unstoppable force. I am power itself.

      CLineSegDesc lsDesc(p1, p0, 0.9f);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p2, p1);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p3, p2);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p0, p3);
      AddShape(&lsDesc, lineDesc);
  }
  else if (unlit == eSprite::UnlitPentagon) {
      Vector2 p0, p1, p2, p3, p4;
      float r = (m_pRenderer->GetHeight(unlit))/2;

      // when math don't work, use brute force lmao

      p0.x = p.x;
      p0.y = p.y + r;

      p1.x = p0.x + sin((2 * 3.14) / 5) + r;
      p1.y = p0.y - cos((2 * 3.14) / 5) - r + 10;

      p2.x = p0.x + sin((4 * 3.14) / 5) + 20;
      p2.y = p0.y - cos(3.14 / 5) - r - 30;

      p3.x = p0.x + sin((4 * 3.14) / 5) - 22;
      p3.y = p0.y - cos(3.14 / 5) - r - 30;

      p4.x = p0.x + sin((2 * 3.14) / 5) - r - 3;
      p4.y = p0.y - cos((2 * 3.14) / 5) - r + 10;

      CLineSegDesc lsDesc(p1, p0, 0.9f);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p2, p1);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p3, p2);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p4, p3);
      AddShape(&lsDesc, lineDesc);

      lsDesc.SetEndPts(p0, p4);
      AddShape(&lsDesc, lineDesc);
  }
  //****ALL STUDENTS: YOUR CODE ENDS HERE

  //REMINDER: CSCE 5255 students must also
  //add code to CObjectManager::NarrowPhase().

  m_vBumperList.push_back(pBumper);
} //MakeBumper


/// Flippers are compound shapes made up of 2 circles and 2 line segments.
/// \param p Position of center of rotation.
/// \param d Offset from position to center of sprite.
/// \param a Initial orientation.

CCompoundShape* CObjectManager::MakeFlipper(const Vector2& p, const Vector2& d, float a){
  CCompoundShape* pFlipper = new CCompoundShape(); //result

  //multimedia descriptors
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, eSound::Size),
    flipObjDesc(eSprite::Flipper, eSprite::Flipper, eSound::Size);
  flipObjDesc.m_vSpriteOffset = -d; 

  //add large circle centered at p
  const float rp = 10.0f;
  CCircleDesc circDesc(p, rp, 0.2f);
  circDesc.m_eMotionType = eMotion::Kinematic;
  CCircle* pCirc0 = (CCircle*)AddShape(&circDesc, flipObjDesc);
  pFlipper->AddShape(pCirc0);

  //add small circle
  const Vector2 q = p + Vector2(58.0f, 0.0f);
  const float rq = 6.5f;
  circDesc.m_vPos = q;
  circDesc.m_fRadius = rq;
  CCircle* pCirc1 = (CCircle*)AddShape(&circDesc, nullObjDesc);
  pFlipper->AddShape(pCirc1);

  //line segment descriptor for tangents
  CLineSegDesc lsDesc0;
  lsDesc0.m_fElasticity = 0.1f;
  lsDesc0.m_eMotionType = eMotion::Kinematic;
  CLineSegDesc lsDesc1(lsDesc0);

  //add tangent line segments
  pCirc1->Tangents(pCirc0, lsDesc0, lsDesc1);
  CShape* pLineSeg0 = AddShape(&lsDesc0, nullObjDesc);
  pFlipper->AddShape(pLineSeg0);
  CShape* pLineSeg1 = AddShape(&lsDesc1, nullObjDesc);
  pFlipper->AddShape(pLineSeg1);

  //do these last to make sure thay get applied to all the shapes
  pFlipper->SetRotCenter(p);
  pFlipper->SetOrientation(a);

  return pFlipper;
} //MakeFlipper

/// I don't know what to call it. One of the things on the left and right
/// of the bollards at the top of the play area. This is the left one.

void CObjectManager::MakeThingL(){ 
  const float w = (float)m_nWinWidth;
  const float h = (float)m_nWinHeight;
  const float e = 0.4f; //elasticity
  
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, eSound::Size);
  CObjDesc thingObjDesc(eSprite::None, eSprite::None, eSound::Size);
  thingObjDesc.m_vSpriteOffset = Vector2(-12.0f, -46.0f);
  
  const Vector2 p0 = Vector2(w/2.0f, h - w/2.0f - TOP_MARGIN - 4.0f);
  const float r = w/2.0f - 1.5f*m_pRenderer->GetWidth(eSprite::Ball);
  CArcDesc arcDesc(p0, r, 5.0f*XM_PI/6.0f, XM_PI, e); 

  AddShape(&arcDesc, nullObjDesc);
  
  Vector2 p1, p2;
  arcDesc.GetEndPts(p1, p2);

  const float w0 = 10.0f;
  const float w1 = 16.0f;

  CCircleDesc circleDesc(p2 + Vector2(w1/2.0f, 0.0f), w1/2.0f, e);
  CCircle* pCirc0 = (CCircle*)AddShape(&circleDesc, nullObjDesc);

  const Vector2 p3 = p2 + Vector2(w1, 0.0f);
  const Vector2 p4 = p2 + Vector2(25.0f, 31.0f);

  circleDesc.m_vPos = p4;
  CCircle* pCirc1 = (CCircle*)AddShape(&circleDesc, nullObjDesc);
  
  circleDesc.m_fRadius = w0/2.0f;
  circleDesc.m_vPos = p1 + Vector2(w0/2.0f, 0.0f);
  CCircle* pCirc2 = (CCircle*)AddShape(&circleDesc, thingObjDesc);
  
  CLineSegDesc lsDesc;
  lsDesc.m_fElasticity = e;

  pCirc1->Tangent(pCirc0, lsDesc);
  AddShape(&lsDesc, nullObjDesc);
  
  pCirc2->Tangent(pCirc1, lsDesc);
  AddShape(&lsDesc, nullObjDesc);
} //MakeThingL

/// I don't know what to call it. One of the things on the left and right
/// of the bollards at the top of the play area. This is the right one.

void CObjectManager::MakeThingR(){ 
  const float w = (float)m_nWinWidth;
  const float h = (float)m_nWinHeight;
  const float e = 0.4f; //elasticity
  
  CObjDesc nullObjDesc(eSprite::None, eSprite::None, eSound::Size);
  CObjDesc thingObjDesc(eSprite::None, eSprite::None, eSound::Size);
  thingObjDesc.m_vSpriteOffset = Vector2(12.0f, -46.0f);
  
  const Vector2 p0 = Vector2(w/2.0f, h - w/2.0f - TOP_MARGIN - 4.0f);
  const float r = w/2.0f - 1.5f*m_pRenderer->GetWidth(eSprite::Ball);
  CArcDesc arcDesc(p0, r, 0.0f, XM_PI/6.0f, e); 

  AddShape(&arcDesc, nullObjDesc);
  
  Vector2 p1, p2;
  arcDesc.GetEndPts(p2, p1);

  const float w0 = 10.0f;
  const float w1 = 16.0f;

  CCircleDesc circleDesc(p2 - Vector2(w1/2.0f, 0.0f), w1/2.0f, e);
  CCircle* pCirc0 = (CCircle*)AddShape(&circleDesc, nullObjDesc);

  const Vector2 p3 = p2 + Vector2(-w1, 0.0f);
  const Vector2 p4 = p2 + Vector2(-25.0f, 31.0f);

  circleDesc.m_vPos = p4;
  CCircle* pCirc1 = (CCircle*)AddShape(&circleDesc, nullObjDesc);
  
  circleDesc.m_fRadius = w0/2.0f;
  circleDesc.m_vPos = p1 - Vector2(w0/2.0f, 0.0f);
  CCircle* pCirc2 = (CCircle*)AddShape(&circleDesc, thingObjDesc);
  
  CLineSegDesc lsDesc;
  lsDesc.m_fElasticity = e;

  pCirc0->Tangent(pCirc1, lsDesc);
  AddShape(&lsDesc, nullObjDesc);
  
  pCirc1->Tangent(pCirc2, lsDesc);
  AddShape(&lsDesc, nullObjDesc);
} //MakeThingR

/// Make the static shapes for the things in the world. As with most physics code, this
/// function is long and tedious, but it's important to get the details right.

void CObjectManager::MakeShapes(){ 
  const float w = (float)m_nWinWidth;
  const float h = (float)m_nWinHeight;

  const float mid = 196.0f; //center x-coordinate of play area
  const float e = 500.0f; //elasticity of bumpers

  const eSound snd = eSound::Blaster;
  const float y = 580.0f - TOP_MARGIN; //y coordinate of bumpers
  const float dx = 100.0f; //distance between bumpers
  
  //this is usefull
  MakeBumper(3, Vector2(mid + 14.0f - dx, y), e, 
    eSprite::UnlitTriangle, eSprite::LitTriangle, snd, 10);
  MakeBumper(4, Vector2(mid + 14.0f,      y), e, 
    eSprite::UnlitDiamond, eSprite::LitDiamond, snd, 100);
  MakeBumper(5, Vector2(mid + 14.0f + dx, y), e, 
    eSprite::UnlitPentagon, eSprite::LitPentagon, snd, 100);
  
  const float dx2 = 84.0f; //half distance between flippers
  const float y2 = 67.0f; //y coordinate of flippers

  const Vector2 vLeft = Vector2(mid - dx2, y2);
  const float aLeft = 11.0f*XM_PI/6.0f;
  const Vector2 d = Vector2(-28.0f, 0.0f);
  CCompoundShape* pFlipper = MakeFlipper(vLeft, d, aLeft);
  m_pLeftFlipper = new CFlipper(pFlipper, true);
  
  const Vector2 vRight = Vector2(mid + dx2, y2);
  const float aRight = 7.0f*XM_PI/6.0f;
  pFlipper = MakeFlipper(vRight, d, aRight);
  m_pRightFlipper = new CFlipper(pFlipper, false);

  //make bollards

  CPointDesc ptDesc(Vector2(0.0f), 0.0f);
  ptDesc.m_bIsSensor = true;
  CObjDesc ptObjDesc(eSprite::UnlitSlot, eSprite::LitSlot, eSound::Whiffle);
  ptObjDesc.m_nScore = 5;

  const float dx3 = 1.2f*m_pRenderer->GetWidth(eSprite::Ball) + 12.5f;
  float y3 = h - 156.0f - TOP_MARGIN;
  const UINT numbollards = 6;

  for(UINT i=0; i<numbollards; i++){
    float x3 = w/2.0f + (i - numbollards/2.0f)*dx3 + dx3/2.0f;
    MakeBollard(Vector2(x3, y3));
    if(i < numbollards - 1){
      ptDesc.m_vPos = Vector2(x3 + dx3/2.0f, y3);
      AddShape(&ptDesc, ptObjDesc);
    } //if
  } //for

  //thing at top left
  MakeThingL(); //left thing
  MakeThingR(); //right thing
} //MakeShapes

/// Create a new shape and a contact descriptor for that shape.
/// \param sd Pointer to a shape descriptor.
/// \param od An object descriptor.
/// \return Pointer to a new contact descriptor.

CShape* CObjectManager::MakeShape(CShapeDesc* sd, const CObjDesc& od){
  CShape* p = nullptr;

  switch(sd->m_eMotionType){
    case eMotion::Kinematic:
      switch(sd->m_eShapeType){
        case eShape::Point:   p = new CKinematicPoint(  *(CPointDesc*)  sd); break;
        case eShape::LineSeg: p = new CKinematicLineSeg(*(CLineSegDesc*)sd); break;
        case eShape::Circle:  p = new CKinematicCircle( *(CCircleDesc*) sd); break;
        case eShape::Arc:     p = new CKinematicArc(    *(CArcDesc*)    sd); break;
      } //switch 
      break;
      
    case eMotion::Static:
      switch(sd->m_eShapeType){
        case eShape::Point:   p = new CPoint(  *(CPointDesc*)  sd); break;
        case eShape::LineSeg: p = new CLineSeg(*(CLineSegDesc*)sd); break;
        case eShape::Circle:  p = new CCircle( *(CCircleDesc*) sd); break;
        case eShape::Arc:     p = new CArc(    *(CArcDesc*)    sd); break;
      } //switch
      break;
    
    case eMotion::Dynamic:
      p = new CDynamicCircle(*(CDynamicCircleDesc*)sd);
      break;
  } //switch

  CObject* pObject = new CObject(p, od);
  m_stdObjects.push_back(pObject);
  p->SetUserPtr(pObject);

  return p;
} //MakeShape

/// Creates a new shape and pushes a contact descriptor for that
/// shape into the shape list.
/// \param sd Pointer to a shape descriptor.
/// \param od Object descriptor.
/// \return Pointer to created shape.

CShape* CObjectManager::AddShape(CShapeDesc* sd, const CObjDesc& od){
  CShape* p = MakeShape(sd, od); 
  m_stdShapes[(UINT)p->GetMotionType()].push_back(p);
  return p;
} //AddShape

/// Draw the sprites for all objects.

void CObjectManager::draw(){
  for(auto const& p: m_stdObjects) //for each object
    if(p->m_nSpriteIndex != (UINT)eSprite::None) //if it has a sprite
      m_pRenderer->Draw((LSpriteDesc2D*)p); //draw it
} //draw

/// Draw the outlines of the shapes in all objects.

void CObjectManager::DrawOutlines(){ 
  for(auto const& p: m_stdObjects) //for each object
    p->DrawOutline(); //ask it to draw its outline
} //draw

/// Move all of the shapes in the dynamic and kinematic shape lists and perform collision response.

void CObjectManager::move(){ 
  for(UINT j=0; j<m_nMIterations; j++){
    for(auto const &p: m_stdShapes[(UINT)eMotion::Kinematic])
      p->move();

    auto i=m_stdShapes[(UINT)eMotion::Dynamic].begin();
    while(i!=m_stdShapes[(UINT)eMotion::Dynamic].end()){
      (*i)->move(); //move it

      //delete lost ball

      if(!(m_cAABB && (*i)->GetAABB())){
        CObject* pObj = (CObject*)((*i)->GetUserPtr()); //get object pointer from shape

        for(auto j=m_stdObjects.begin(); j!=m_stdObjects.end(); j++)
          if(*j == pObj){ //if it's the object corr. to the shape
            delete *j; //delete the object
            m_stdObjects.erase(j); //remove object pointer from object list
            break;
          } //if

        i = m_stdShapes[(UINT)eMotion::Dynamic].erase(i); //remove shape pointer from shape list
        m_pAudio->play(eSound::LostBall);
        m_bBallInPlay = false;
      } //if

      else ++i;
    } //while

    for(UINT i=0; i<m_nCIterations; i++)
      BroadPhase(); //broadphase collision detection and response
  } //for
  
  m_pLeftFlipper->EnforceBounds();
  m_pRightFlipper->EnforceBounds();

  m_pLeftGate->CloseGate();
  m_pRightGate->CloseGate();

  for(auto const& p: m_stdObjects)
    p->Update();
} //move

/// Do collision detection for all dynamic shapes against all
/// static and kinematic shapes, and against all dynamic shapes
/// that appear after it in the dynamic shape list. 

void CObjectManager::BroadPhase(){
  const auto begin = m_stdShapes[(UINT)eMotion::Dynamic].begin();
  const auto end = m_stdShapes[(UINT)eMotion::Dynamic].end();

  for(UINT k=0; k<4; k++)
    for(auto i=begin; i!=end; i++){
      const auto pCirc = (CDynamicCircle*)*i; //pointer to current dynamic shape

      m_pLeftGate->NarrowPhase(pCirc); //left gate   
      m_pRightGate->NarrowPhase(pCirc);  //right gate

      for(auto const& pShape: m_stdShapes[(UINT)eMotion::Static]) //static shapes
        NarrowPhase(pShape, pCirc);
    
      for(auto const& pShape: m_stdShapes[(UINT)eMotion::Kinematic]) //kinematic shapes
        NarrowPhase(pShape, pCirc);
     
      for(auto j=next(i); j!=end; j++) //dynamic shapes, later numbered to avoid doubling up
        NarrowPhase(*j, pCirc);
    } //for
} //BroadPhase

/// Check whether a pair of shapes collides and make appropriate response.
/// \param pShape Pointer to a static or kinematic shape.
/// \param pCirc Pointer to moving circle.
/// \return true if there was a collision.

bool CObjectManager::NarrowPhase(CShape* pShape, CDynamicCircle* pCirc){
  bool bHit = false;

  CContactDesc cd(pShape, pCirc);
  
  if(pShape->PreCollide(cd)){ //there's a collision
    if(!pShape->GetSensor())
      pCirc->PostCollide(cd);

    bHit = true;
    CObject* pObj0 = (CObject*)(pCirc->GetUserPtr());

    if(pShape->GetMotionType() == eMotion::Dynamic){ //dynamic shape
      if(pObj0 != nullptr)
        m_pAudio->play(pObj0->m_eSound, cd.m_vPOI, cd.m_fSpeed/1000.0f);
    } //if

    else{ //static or kinematic shape
      CObject* pObj1 = (CObject*)(pShape->GetUserPtr());

      if(cd.m_fSpeed > 10.0f){
        m_pAudio->play(pObj1->m_eSound, cd.m_vPOI);  

        if(!pObj1->m_bRecentHit)
          m_nScore += pObj1->m_nScore;
      } //if

      pObj1->m_bRecentHit = true;
      pObj1->m_fLastHitTime = m_pTimer->GetTime();
    } //else
  } //if
  
  //****CSCE 5255 STUDENTS: YOUR CODE STARTS HERE

  //****CSCE 5255 STUDENTS: YOUR CODE ENDS HERE

  return bHit;
} //NarrowPhase

////////////////////////////////////////////////////////////////////////////////////////
// Code for flippers

/// If left flipper isn't moving up, set its rotational
/// velocity to ROTSPEED (with the correct sign indicating direction),
/// and play a sound.
/// \param bUp true for up flip, false for down flip.

void CObjectManager::LeftFlip(bool bUp){  
  m_pLeftFlipper->Flip(bUp);
} //LeftFlip

/// If right flipper isn't moving up, set its rotational
/// velocity to ROTSPEED (with the correct sign indicating direction),
/// and play a sound.
/// \param bUp true for up flip, false for down flip.

void CObjectManager::RightFlip(bool bUp){  
  m_pRightFlipper->Flip(bUp);
} //RightFlip
