/// \file ObjectManager.cpp
/// \brief Code for the object manager CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "Renderer.h"

CObjectManager::CObjectManager(){
} //constructor

/// The destructor clears the object list, which destructs
/// all of the objects in it.

CObjectManager::~CObjectManager(){
  clear();
} //destructor

/// Delete all of the entities managed by object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.

void CObjectManager::clear(){
  for(auto const& p: m_stdList) //for each object
    delete p; //delete object

  m_stdList.clear(); //clear the object list
} //clear

/// Draw the game objects using Painter's Algorithm.
/// The background is drawn first, then the game
/// objects are asked to draw themselves in the
/// order that they are in the object list. That is,
/// they are drawn from back to front.

void CObjectManager::draw(){  
  if(m_eDrawMode == eDrawMode::Sprites || m_eDrawMode == eDrawMode::Both)
    m_pRenderer->Draw(eSprite::Background, m_vWinCenter); //draw background

  for(auto const& p: m_stdList) //for each object
    p->draw(); //draw it in renderer
} //draw

void RemapPoints(b2Vec2 p[], UINT n, UINT w, UINT h) {
    for (UINT i = 0; i < n; i++) {
        p[i].x = RW2PW( p[i].x - w / 2.0f);
        p[i].y = RW2PW(-p[i].y + h / 2.0f);
    } //for
} //RemapPoints

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// bottom, right, and left edges of the screen in renderer. The left and
/// right edges continue upwards for a distance. There is no top to the world.
/*b2Body*/ void CObjectManager::CreateWalls() {
    float h = RW2PW(790.0f); //was 768.0f
    float w = RW2PW(1024.0f);
    b2BodyDef bd;
    bd.type = b2_staticBody;
    //bd.position.Set(RW2PW(m_nWinWidth)/2.0f, RW2PW(m_nWinHeight)/2.0f);
    
    b2Vec2 vertices[4];
    vertices[0].Set(RW2PW(0), h-RW2PW(0));
    vertices[1].Set(RW2PW(0), h-RW2PW(702));
    vertices[2].Set(RW2PW(40), h-RW2PW(702));
    vertices[3].Set(RW2PW(40), h-RW2PW(0));
    /*vertices[0] = b2Vec2(0, 0);
    vertices[1] = b2Vec2(0, 702);
    vertices[2] = b2Vec2(40, 702);
    vertices[3] = b2Vec2(40, 0);
    RemapPoints(vertices, 4, 768, 1024);*/
    
    b2PolygonShape LeftWall; // this is the wall of blocks on the left
    LeftWall.Set(vertices, 4);

    vertices[0].Set(RW2PW(980), h - RW2PW(85));
    vertices[1].Set(RW2PW(980), h - RW2PW(804));
    vertices[2].Set(RW2PW(1020), h - RW2PW(804));
    vertices[3].Set(RW2PW(1020), h - RW2PW(85));
    b2PolygonShape RightWall; // this is the wall of block on the right
    RightWall.Set(vertices, 4);

    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    pBody->CreateFixture(&LeftWall, 0.1f);
    pBody->CreateFixture(&RightWall, 0.1f);

    //return pBody;
}

void CObjectManager::CreateWorldEdges(){
  const float w = RW2PW(m_nWinWidth); //window width in Physics World units
  const float h = RW2PW(m_nWinHeight); //window height in Physics World units

  //corners of the window
  const b2Vec2 vBLeft  = b2Vec2(0, 0); //bottom left
  const b2Vec2 vBRight = b2Vec2(w, 0); //bottom right
  const b2Vec2 vTLeft  = b2Vec2(0, h); //top left
  const b2Vec2 vTRight = b2Vec2(w, h); //top right

  //Box2D ground
  b2BodyDef bd; //body definition
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd); //body
  b2EdgeShape shape; //shape

  //bottom of window
  shape.m_vertex1 = vBLeft;
  shape.m_vertex2 = vBRight;
  pBody->CreateFixture(&shape, 0);

  //left edge of window
  shape.m_vertex1 = vBLeft;
  shape.m_vertex2 = vTLeft;
  pBody->CreateFixture(&shape, 0);

  //right edge of window
  shape.m_vertex1 = vBRight;
  shape.m_vertex2 = vTRight;
  pBody->CreateFixture(&shape, 0); 
} //CreateWorldEdges

/// Create an object in object manager and link its Physics World
/// body to it.
/// \param t Sprite type.
/// \param p Pointer to Box2D body.

CObject* CObjectManager::create(eSprite t, b2Body* b) {
    CObject* p = new CObject(t, b);
    m_stdList.push_back(p);
    return p;
} //create

void CObjectManager::CreateObject(eSprite t, b2Body* p){
  CObject* pObj = new CObject(t, p);
  m_stdList.push_back(pObj);
  p->GetUserData().pointer = (uintptr_t)pObj;
} //CreateObject
