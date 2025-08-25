/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

#include "Kraid.h"

/// Call renderer's Release function to do the required
/// Direct3D cleanup, then delete renderer and object manager.
/// Also delete Physics World, which MUST be deleted after
/// object manager because of way things are set up.

CGame::~CGame(){
  delete m_pObjectManager;
  delete m_pPhysicsWorld;
  delete m_pParticleEngine;

  delete m_pKraid;
} //destructor

/// Initialize renderer and object manager, load 
/// images and sounds, start the timer, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer; 
  m_pRenderer->Initialize(eSprite::Size);
  m_pRenderer->LoadImages(); //load images from xml file list

  //set up object manager and Physics World
  m_pObjectManager = new CObjectManager; //set up object manager
  m_pPhysicsWorld = new b2World(RW2PW(0, -2000)); //set up Physics World with gravity-2000
  m_pObjectManager->CreateWorldEdges(); //create world edges at edges of window
  m_pObjectManager->CreateWalls(); //create walls
  m_pPhysicsWorld->SetContactListener(&m_cContactListener); //load up my contact listener

  LoadSounds(); //load the sounds for this game
  
  m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

  //now start the game
  BeginGame();
} //Initialize

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);
  
  m_pAudio->Load(eSound::Whoosh, "whoosh");
  m_pAudio->Load(eSound::Yay, "yay");
  m_pAudio->Load(eSound::Bonk, "bonk");
  m_pAudio->Load(eSound::Buzz, "buzz");
  m_pAudio->Load(eSound::Restart, "restart");
  m_pAudio->Load(eSound::Bgm, "bgm");
  m_pAudio->Load(eSound::Boss, "boss");
  m_pAudio->Load(eSound::Attack, "attack");
  m_pAudio->Load(eSound::Kraid, "kraid");
  m_pAudio->Load(eSound::Boom, "boom");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Clear any old objects out of object manager and get ready
/// to start again.

void CGame::BeginGame(){  
  m_pParticleEngine->clear();
  m_pAudio->stop(); // this stops all sounds, I'll need it later
  m_pAudio->play(eSound::Restart);

  //Health = 3;

  m_pObjectManager->clear(); //clear old objects
  
  float w, h; //width and height of red button sprite

  m_pRenderer->GetSize(eSprite::RedButton, w, h); //get them
  CreateButton(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f); //create red button

  if (!level) {
      

      CreateTriangle(m_nWinWidth - 200, m_nWinHeight - 150, false);
      CreateMetroid(m_nWinWidth - 325, m_nWinHeight - 210);
      CreateTriangle(m_nWinWidth - 875, m_nWinHeight - 190, true);
      CreateTriangle(m_nWinWidth - 700, m_nWinHeight - 200, true);
      CreateTriangle(m_nWinWidth - 690, m_nWinHeight - 280, true);
      CreateChozo(m_nWinWidth - 460, m_nWinHeight - 460);//460 460
      CreateTriangle(m_nWinWidth - 350, m_nWinHeight - 250, false);
      CreateChozo(m_nWinWidth - 100, m_nWinHeight - 150);

      CreateTriangle(m_nWinWidth - 170, m_nWinHeight - 660, false);
      CreateChozo(m_nWinWidth - 550, m_nWinHeight - 736);
      CreateTriangle(m_nWinWidth - 500, m_nWinHeight - 485, true);
      CreateChozo(m_nWinWidth - 950, m_nWinHeight - 190);
      CreateTriangle(m_nWinWidth - 910, m_nWinHeight - 290, true);
      CreateBigLine(m_nWinWidth - 910, m_nWinHeight - 330);

      CreateMetroid(m_nWinWidth - 600, m_nWinHeight - 350);// 600 350
      CreateTriangle(m_nWinWidth - 800, m_nWinHeight - 450, false);
      CreateTriangle(m_nWinWidth - 910, m_nWinHeight - 550, true);
      CreateTriangle(m_nWinWidth - 800, m_nWinHeight - 650, false);
  }
  if (level) {
      SAFE_DELETE(m_pKraid);

      w = RW2PW(m_nWinWidth) / 2.0f;
      h = RW2PW(m_nWinHeight) / 2.0f;

      m_pKraid = new CKraid(w, RW2PW(540) / 2.0f);

      CreateBigLine(m_nWinWidth - 250, m_nWinHeight - 130);
      CreateMetroid(m_nWinWidth - 450, m_nWinHeight - 250);
      CreateTriangle(m_nWinWidth - 700, m_nWinHeight - 500, true);
      CreateChozo(m_nWinWidth - 150, m_nWinHeight - 736);
      CreateTriangle(m_nWinWidth - 100, m_nWinHeight - 450, true);
      CreateTriangle(m_nWinWidth - 350, m_nWinHeight - 400, false);
      CreateTriangle(m_nWinWidth - 400, m_nWinHeight - 425, false);
      CreateChozo(m_nWinWidth - 220, m_nWinHeight - 360);
      CreateBigLine(m_nWinWidth - 400, m_nWinHeight - 690);
      CreateMetroid(m_nWinWidth - 400, m_nWinHeight - 750);
      CreateTriangle(m_nWinWidth - 300, m_nWinHeight - 520, false);
  }
} //BeginGame

void CGame::CreateBigLine(float x, float y) {
    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(RW2PW(x), RW2PW(y));

    float w, h;
    m_pRenderer->GetSize(eSprite::BigLine, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.0f;

    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    pBody->CreateFixture(&fd);
    m_pObjectManager->CreateObject(eSprite::BigLine, pBody);
}

void CGame::CreateTriangle(float x, float y, bool flip) {
    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(RW2PW(x), RW2PW(y));

    float w, h;
    m_pRenderer->GetSize(eSprite::Triangle, w, h);
    b2Vec2 vertices[3];

    if (!flip) {
        vertices[0].Set(RW2PW(w / 2.0f), RW2PW(h / 2.0f));
        vertices[1].Set(RW2PW(w / -2.0f), RW2PW(h / -2.0f));
        vertices[2].Set(RW2PW(w / 2.0f), RW2PW(h / 2.0f - 1));
    }
    else {
        vertices[0].Set(RW2PW(w / -2.0f), RW2PW(h / 2.0f));
        vertices[1].Set(RW2PW(w / -2.0f), RW2PW(h / 2.0f - 1));
        vertices[2].Set(RW2PW(w / 2.0f), RW2PW(h / -2.0f));
    }

    b2PolygonShape s;
    s.Set(vertices, 3);
    //s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.3f;

    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    pBody->CreateFixture(&fd);

    if (!flip) {
        m_pObjectManager->CreateObject(eSprite::Triangle, pBody);
    }
    else {
        m_pObjectManager->CreateObject(eSprite::FlipTriangle, pBody);
    }
}

void CGame::CreateMetroid(float x, float y) {
    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(RW2PW(x), RW2PW(y));

    float w, h;
    m_pRenderer->GetSize(eSprite::Metroid, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.3f;

    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    pBody->CreateFixture(&fd);
    m_pObjectManager->CreateObject(eSprite::Metroid, pBody);
} //CreateCrate

void CGame::CreateChozo(float x, float y) {
    b2BodyDef bd;
    //bd.type = b2_dynamicBody;
    bd.type = b2_staticBody;
    bd.position.Set(RW2PW(x), RW2PW(y));

    float w, h;
    m_pRenderer->GetSize(eSprite::Chozo, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);

    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = 1.0f;
    fd.restitution = 0.3f;

    b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
    pBody->CreateFixture(&fd);
    m_pObjectManager->CreateObject(eSprite::Chozo, pBody);
} //CreateCrate


/// Create the button that signals the end of the Rube Goldberg machine.
/// \param x X coordinate of button in Physics World units.
/// \param y Y coordinate of button in Physics World units.

void CGame::CreateButton(float x, float y){
  //Physics World
  b2BodyDef bd; 
	bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //shape
  b2PolygonShape s;
  float w, h; //width and height of sprite
  m_pRenderer->GetSize(eSprite::RedButton, w, h);
  s.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  //fixture
  b2FixtureDef fd;
	fd.shape = &s;
	fd.density = 1.0f;
	fd.restitution = 0.2f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  p->CreateFixture(&fd);

  //object manager
  m_pObjectManager->CreateObject(eSprite::RedButton, p);
} //CreateButton

/// Place a ball in Physics World and object manager.
/// \param x Horizontal coordinate in Physics World units.
/// \param y Vertical coordinate in Physics World units.
/// \param xv Horizontal component of velocity.
/// \param yv Vectical component of velocity.

void CGame::CreateBall(float x, float y, float xv, float yv){ 
  //Physics World
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(x, y);
  bd.linearVelocity.Set(xv, yv);

  //shape
  b2CircleShape s;
	s.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::Ball))/2.0f;

  //fixture
	b2FixtureDef fd;
	fd.shape = &s;
	fd.density = 1.0f;
	fd.restitution = 0.7f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  p->CreateFixture(&fd);

  //object manager
  m_pObjectManager->CreateObject(eSprite::Ball, p);
} //CreateBall

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 

  if(m_pKeyboard->TriggerDown(VK_F2)){ //change draw mode
    m_eDrawMode = eDrawMode((UINT)m_eDrawMode + 1);
    if(m_eDrawMode == eDrawMode::Size)m_eDrawMode = eDrawMode(0);
  } //if

  if(m_pKeyboard->TriggerDown(VK_SPACE)){
    switch(m_eGameState){
      case eGameState::Initial:
        CreateBall(RW2PW(m_nWinWidth - 32), RW2PW(m_nWinHeight), -40.0f, 0.0f);
        m_eGameState = eGameState::Running;
        m_pAudio->play(eSound::Whoosh);

        if(!level)
            m_pAudio->play(eSound::Bgm, 0.5f);
        else
            m_pAudio->play(eSound::Boss);
        m_fStartTime = m_pTimer->GetTime();
      break;

      case eGameState::Finished:
          if (!level)
              level = true;
          else
              level = false;
        SAFE_DELETE(m_pKraid);
        BeginGame(); //begin again
        m_eGameState = eGameState::Initial;
        //m_pAudio->play(eSound::Restart); //must be after BeginGame(), which stops all sounds
      break;

      default: //m_pAudio->play(eSound::Buzz);
          const UINT w = (UINT)m_pRenderer->GetWidth(eSprite::Metroid);
          const float h = m_pRenderer->GetHeight(eSprite::Metroid);
          float x = (float)m_pRandom->randn(w, m_nWinWidth - w);
          const float y = m_nWinWidth + h; //above the top of the screen
          const float rand = m_pRandom->randf();
          //CreateCrate(x, 702);
          CreateBall(RW2PW(x), RW2PW(m_nWinHeight), 0.0f, 0.0f);
          x = (float)m_pRandom->randn(w, m_nWinWidth - w);
          CreateBall(RW2PW(x), RW2PW(m_nWinHeight), 0.0f, 0.0f);
          x = (float)m_pRandom->randn(w, m_nWinWidth - w);
          CreateBall(RW2PW(x), RW2PW(m_nWinHeight), 0.0f, 0.0f);
          x = (float)m_pRandom->randn(w, m_nWinWidth - w);
          CreateBall(RW2PW(x), RW2PW(m_nWinHeight), 0.0f, 0.0f);
          x = (float)m_pRandom->randn(w, m_nWinWidth - w);
          CreateBall(RW2PW(x), RW2PW(m_nWinHeight), 0.0f, 0.0f);

          //float temp = RW2PW(m_nWinWidth) / 2.0f;
          if (Health == 0)
              m_pKraid->KraidYeet();
    } //switch
  } //if
} //KeyboardHandler

/// Ask object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame(){ 
  m_pRenderer->BeginFrame();
    m_pObjectManager->draw(); //draw the objects
    m_pParticleEngine->Draw(); //draw particles
    DrawClock(); //draw the timer
    if(m_eGameState == eGameState::Initial)
      m_pRenderer->DrawCenteredText("Hit space to begin.");
    else if(m_eGameState == eGameState::Finished)
      m_pRenderer->DrawCenteredText("Hit space to reset.");
  m_pRenderer->EndFrame();
} //RenderFrame

/// Draw a digital clock at the top right of the window, even if the
/// camera is panned left or right.

void CGame::DrawClock(){ 
  const float dx = 68.0f; //distance from right of screen
  const float dy = 48.0f; //distance from top of screen
  const float x = m_pRenderer->GetCameraPos().x; //ensure we are in screen space

  const Vector2 pos = Vector2(x + m_nWinWidth/2.0f - dx, m_nWinHeight - dy);
  m_pRenderer->Draw(eSprite::ClockFace, pos); //clock background

  float t = 0.0f; //for the time

  switch(m_eGameState){ //set t depending on game state
    case eGameState::Initial:  t = 0.0f; break; //clock reads zero
    case eGameState::Running:  t = m_pTimer->GetTime() - m_fStartTime; break;
    case eGameState::Finished: t = m_fTotalTime; break; //clock is stopped
    default: t = 0.0f;
  } //switch
  
  const UINT min = (UINT)floorf(t/60.0f); //minutes
  const UINT sec = (UINT)floorf(t - 60.0f*min); //seconds

  const std::string str = std::to_string(min) + ":" + //string to be drawn
    std::to_string(sec/10) + std::to_string(sec%10);

  const Vector2 pos2 = Vector2(m_nWinWidth - 112.0f, 18.0f); //text position
  m_pRenderer->DrawScreenText(str.c_str(), pos2, Colors::White); //draw in white
} //DrawClock

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify 
/// the timer of the start and end of the
/// frame so that it can calculate frame time. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  m_pAudio->BeginFrame(); //notify sound manager that frame has begun

  m_pTimer->Tick([&](){ 
    m_pPhysicsWorld->Step(m_pTimer->GetFrameTime(), 6, 2); //move all objects 
    m_pParticleEngine->step(); //move particles in particle effects
  });

  if (m_eGameState == eGameState::Running)
      if (Health == 0) {
          m_pKraid->KraidYeet();
          Health--;
      }

  RenderFrame(); //render a frame of animation 
} //ProcessFrame
