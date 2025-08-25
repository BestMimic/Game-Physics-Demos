/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

#include "shellapi.h"

CGame::~CGame(){
  delete m_pCurBallShape;
  delete m_pRenderer;
  delete m_pObjectManager;
} //destructor

/// Initialize the renderer, load the images for the background,
/// and start the game.

void CGame::Initialize(){
  m_fGravity = -200.0f;
  m_fFrequency = 60.0f*m_nMIterations;
  m_fTimeStep = 1.0f/m_fFrequency;
  m_eDrawMode = eDrawMode::Background; //default draw mode

  m_pRenderer = new CRenderer;
  m_pRenderer->SetBgColor(Colors::White);
  m_pRenderer->Initialize(eSprite::Size); 
  m_pRenderer->LoadImages(); //load images from xml file list
  m_pRenderer->SetBgColor(Colors::White);
  
  m_pObjectManager = new CObjectManager; //set up object manager 
  LoadSounds(); //load the sounds for this game
  
  m_cClipDesc0.m_nSpriteIndex = (UINT)eSprite::Clip;
  m_cClipDesc0.m_vPos = Vector2(42.0f, 860.0f - 170.0f);
  m_cClipDesc0.m_fRoll = -XM_PI/6.0f;

  m_cClipDesc1.m_nSpriteIndex = (UINT)eSprite::Clip;
  m_cClipDesc1.m_vPos = Vector2(388.0f, 860.0f - 170.0f);
  m_cClipDesc1.m_fRoll = -5.0f*XM_PI/6.0f;
  
  const Vector2 dw(m_pRenderer->GetWidth(eSprite::LED) + 2.0f, 0.0f);
  Vector2 pos(31.0f, 826.0f);

  for(UINT i=0; i<NUMSCOREDIGITS; i++){
    m_cScoreDesc[i].m_nSpriteIndex = (UINT)eSprite::LED;
    m_cScoreDesc[i].m_vPos = pos;
    pos += dw;
  } //for

  m_pTimer->SetFixedTimeStep();
  m_pTimer->SetFrameTime(1/60.0f);

  //now start the game
  BeginGame();
} //Initialize

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);

  m_pAudio->Load(eSound::Beep, "beep");
  m_pAudio->Load(eSound::Blaster, "blaster");
  m_pAudio->Load(eSound::Ballclick, "ballclick");
  m_pAudio->Load(eSound::Launch, "launch");
  m_pAudio->Load(eSound::Click, "click");
  m_pAudio->Load(eSound::Tink, "tink");
  m_pAudio->Load(eSound::Whiffle, "whiffle");
  m_pAudio->Load(eSound::FlipUp, "flipup");
  m_pAudio->Load(eSound::FlipDown, "flipdown");
  m_pAudio->Load(eSound::LostBall, "lostball");
  m_pAudio->Load(eSound::Load, "load"); 
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Create the edges of the world and some shapes.

void CGame::BeginGame(){   
  m_pObjectManager->MakeWorldEdges(); //make world edges
  m_pObjectManager->MakeShapes(); //make shapes

  m_nScore = 0;
} //BeginGame

/// If there is no ball, create one and place it in the chute ready for launch.
/// Otherwise, assuming that this has been done and the ball is ready to launch,
/// then apply a vertical impulse to it. Add a little bit of randomness to that
/// impulse so that it behaves slightly differently each time.

void CGame::Launch(){
  static bool bReadyForLaunch = false;

  const float r = m_pRenderer->GetWidth(eSprite::Ball)/2.0f;

  if(m_bBallInPlay){ //ball in play, ready to be launched
    const Vector2 pos = m_pCurBallShape->GetPos();

    if(pos.x > m_nWinWidth - 2.0f*r && pos.y <= r + 1.0f){
      const float speed = 1000.0f + 1000.0f*m_pRandom->randf(); 
      m_pCurBallShape->SetVel(Vector2(0.0f, speed));
      bReadyForLaunch = false;
      const float volume = std::max(0.1f, speed/4500.0f);
      m_pAudio->play(eSound::Launch, pos, volume); 
    } //if
  } //if

  else{ //ball is not in play
    const Vector2 pos = Vector2(m_nWinWidth - 1.5f*r, 48.0f);
    CDynamicCircleDesc d; 

    d.m_fElasticity = 0.9f;
    d.m_vPos = pos;
    d.m_fRadius = r;

    const CObjDesc od(eSprite::Ball, eSprite::Ball, eSound::Ballclick);
    delete m_pCurBallShape; //delete old ball
    m_pCurBallShape = (CDynamicCircle*)m_pObjectManager->AddShape(&d, od);

    bReadyForLaunch = true;
    m_bBallInPlay = true;
    m_pAudio->play(eSound::Load, pos); 
  } //else
} //Launch

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 
  
  if(m_pKeyboard->TriggerDown(VK_F1)) //help
    ShellExecute(0, 0, "https://larc.unt.edu/code/physics/pinball/", 0, 0, SW_SHOW);

  if(m_pKeyboard->TriggerDown(VK_F2)){ //change draw mode
    m_eDrawMode = eDrawMode((UINT)m_eDrawMode + 1);
    if(m_eDrawMode == eDrawMode::Size)m_eDrawMode = eDrawMode(0);
  } //if
  
  if(m_pKeyboard->TriggerDown(VK_SPACE)) //load and launch a ball
    Launch();
  
  if(m_pKeyboard->TriggerDown(VK_LSHIFT)) //left flipper up
    m_pObjectManager->LeftFlip(true);

  if(m_pKeyboard->TriggerUp(VK_LSHIFT)) //left flipper down
    m_pObjectManager->LeftFlip(false);
   
  if(m_pKeyboard->TriggerDown(VK_RSHIFT)) //right flipper up
    m_pObjectManager->RightFlip(true);
  
  if(m_pKeyboard->TriggerUp(VK_RSHIFT)) //right flipper down
    m_pObjectManager->RightFlip(false);
} //KeyboardHandler

/// Ask object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.
/// In addition, draw score and the CLIP_SPRITEs for the gates on top of
/// everything else.

void CGame::RenderFrame(){ 
  m_pRenderer->BeginFrame();
    if(m_eDrawMode == eDrawMode::Background || m_eDrawMode == eDrawMode::Both){ //draw sprites
      m_pRenderer->Draw(eSprite::Background, m_vWinCenter); //draw background

      //draw score
      int n = m_nScore; //current score

      for(int i=NUMSCOREDIGITS-1; i>=0; i--){
        m_cScoreDesc[i].m_nCurrentFrame = n%10; //get least significant digit
        m_pRenderer->Draw(&m_cScoreDesc[i]); //draw it
        n /= 10; //right shift one digit
      } //for

      m_pObjectManager->draw(); //draw objects
    } //if

    if(m_eDrawMode == eDrawMode::Background){ //draw clips over everything    
      m_pRenderer->Draw(&m_cClipDesc0);
      m_pRenderer->Draw(&m_cClipDesc1);
    } //if
  
    if(m_eDrawMode == eDrawMode::Both || m_eDrawMode == eDrawMode::Lines) //draw shape outlines
      m_pObjectManager->DrawOutlines();
  m_pRenderer->EndFrame();
} //RenderFrame

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify the 
/// audio player at the start of each frame so that it can 
/// prevent multiple copies of a sound from starting on the
/// same frame. Notify the timer of the start and end of the
/// frame so that it can calculate frame time. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun

  m_pTimer->Tick([&](){ 
    m_pObjectManager->move(); //move all objects
  });

  RenderFrame(); //render a frame of animation
} //ProcessFrame
