/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Helpers.h"

#include "Windmill.h"

#include "shellapi.h"

/// Call Render World's Release function to do the required
/// Direct3D cleanup, then delete the renderer and the object manager.
/// Also delete Physics World, which MUST be deleted after
/// the object manager because of way things are set up.
/// Lastly delete the toy's components. All but one of
/// those pointers will be NULL, but who cares?

CGame::~CGame(){
  delete m_pRenderer;
  delete m_pObjectManager;
  delete m_pPhysicsWorld;
  
  delete m_pWindmill;
} //destructor

/// Initialize the renderer and the object manager, load 
/// images and sounds, start the timer, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer; //here's Render World
  m_pRenderer->Initialize(eSprite::Size);
  m_pRenderer->LoadImages(); //load images from xml file list

  m_pPhysicsWorld = new b2World(RW2PW(0, -400)); //set up Physics World with gravity

  m_pObjectManager = new CObjectManager; //set up the object manager
  m_pObjectManager->CreateWorldEdges(); //create world edges at edges of window

  BeginGame();
} //Initialize

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create the game objects
/// for the current level.

void CGame::CreateObjects(){
  //Delete current component. All but one will be NULL, but it gets the job done
  SAFE_DELETE(m_pWindmill);

  const float w = RW2PW(m_nWinWidth)/2.0f;
  const float h = RW2PW(m_nWinHeight)/2.0f;

  m_pWindmill = new CWindmill(w, RW2PW(540)/2.0f);
} //CreateObjects

void CGame::CreateCrate(float x, float y){ 
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  float w, h; 
  m_pRenderer->GetSize(eSprite::Crate, w, h);
  b2PolygonShape s;
  s.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 1.0f;
  fd.restitution = 0.3f;

  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd); 
  pBody->CreateFixture(&fd);
  m_pObjectManager->create(eSprite::Crate, pBody);
} //CreateCrate

void CGame::CreateRoundThing(eSprite t, float x, float y){ 
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(RW2PW(x), RW2PW(y));

  b2CircleShape s;
	s.m_radius = RW2PW(m_pRenderer->GetWidth(t))/2.0f;

	b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.8f;
  fd.restitution = 0.8f;

  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  pBody->CreateFixture(&fd);
  m_pObjectManager->create(t, pBody);
} //CreateRoundThing

/// Clear any old objects out of the object manager and create
/// the edges of the game world, which correspond to the 
/// edges of the window.

void CGame::BeginGame(){  
  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create the first object
} //BeginGame

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 

  static int nCrateCount = 0; //number of crates generated

  if(m_pKeyboard->TriggerDown(VK_F1)) //help
    ShellExecute(0, 0, "https://larc.unt.edu/code/physics/joint/", 0, 0, SW_SHOW);

  if(m_pKeyboard->TriggerDown(VK_RETURN))
    m_pWindmill->reverse(); 
  
  if(m_pKeyboard->TriggerDown(VK_SPACE)){
    const UINT w = (UINT)m_pRenderer->GetWidth(eSprite::Crate);
    const float h = m_pRenderer->GetHeight(eSprite::Crate);
    const float x = (float)m_pRandom->randn(w, m_nWinWidth - w);
    const float y = m_nWinWidth + h; //above the top of the screen
    const float rand = m_pRandom->randf();

    if(m_pRandom->randf() < 0.2f)
      CreateCrate(x, y); 
    else if(m_pRandom->randf() < 0.4f)
      CreateRoundThing(eSprite::Ball, x, y);
    else CreateRoundThing(eSprite::Tire, x, y);
  } //if
  
  if(m_pKeyboard->TriggerDown(VK_BACK))
    BeginGame();
} //KeyboardHandler

/// Draw the appropriate background for the current level.

void CGame::DrawBackground(){
  m_pRenderer->Draw(eSprite::WindmillBg, m_vWinCenter); //draw background
} //DrawBackground

/// Ask object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame(){
  m_pRenderer->BeginFrame();
    DrawBackground(); //draw the background
    m_pObjectManager->draw(); //draw the objects
  m_pRenderer->EndFrame();
} //RenderFrame

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations.  
/// There's also some extra processing to be done
/// outside Box2D in order to move the nautilus gears,
/// the car, the pulleys, and the rack-and-pinion.

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  
  m_pTimer->Tick([&](){ 
    m_pPhysicsWorld->Step(m_pTimer->GetFrameTime(), 4, 6); //move all objects
  });

  RenderFrame(); //render a frame of animation 
} //ProcessFrame

