/// \file ObjectManager.cpp
/// \brief Code for the object manager CObjectManager.

#include "ObjectManager.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Helpers.h"

/// The destructor clears the object list, which destructs
/// all of the objects in it.

CObjectManager::~CObjectManager(){
  clear();
} //destructor

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// bottom, right, and left edges of the screen in Render World. The left and
/// right edges continue upwards for a distance. There is no top to the world.

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

/// Create an object in the object manager.
/// \param t Sprite type.
/// \param b Pointer to Physics World body.
/// \return Pointer to the object created.

CObject* CObjectManager::create(eSprite t, b2Body* b){ 
  CObject* p = new CObject(t, b);
  m_stdList.push_back(p);
  return p;
} //create

/// Delete all of the entities managed by object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.
/// Ditto for the line list.

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
/// The lines are drawn behind the objects.

void CObjectManager::draw(){  
  for(auto const& p: m_stdList) //for each object
    if(p != nullptr) //safety
      p->draw(); //draw it in Render World
} //draw

/// Delete the first object of a given sprite type.
/// \param t Sprite type.
/// \return true if one was actually deleted.

bool CObjectManager::DeleteObject(eSprite t){
  bool bDeleted = false; //return result

  for(auto& p: m_stdList) //for each object
    if(p != nullptr && p->GetSpriteType() == t){ //right type
      SAFE_DELETE(p); //delete it and set to null
      bDeleted = true; //we deleted something
      break; //only the first one
    } //if

  return bDeleted;
} //DeleteObject


