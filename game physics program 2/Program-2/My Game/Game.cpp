/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "Helpers.h"

#include "shellapi.h"

/// The default constructor. 

CGame::CGame(){
} //constructor

/// Delete the renderer, the object manager, and the particle engine.

CGame::~CGame(){
  delete m_pRenderer;
  delete m_pObjectManager;
  delete m_pParticleEngine;
} //destructor

/// Initialize the renderer, the particle engine, the step timer, and the object
/// manager, load images and sounds, and begin the game.

void CGame::Initialize(){
  m_fGameStateTime = m_pTimer->GetTime(); //set game state timer
  
  m_pRenderer = new CRenderer(); 
  m_pRenderer->Initialize((int)eSprite::Size); 
  m_pRenderer->LoadImages(); //load images from xml file list

  m_pObjectManager = new CObjectManager;  //set up object manager 
  LoadSounds(); //load the sounds for this game

  m_pParticleEngine = new LParticleEngine2D((LSpriteRenderer*)m_pRenderer);
  
  m_pTimer->SetFixedTimeStep();
  m_pTimer->SetFrameTime(1/60.0f);

  //now start the game
  BeginGame();
} //Initialize

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);

  m_pAudio->Load(eSound::Cue, "cue");
  m_pAudio->Load(eSound::Click, "click");
  m_pAudio->Load(eSound::Thump, "thump");
  m_pAudio->Load(eSound::Pocket, "pocket");
  m_pAudio->Load(eSound::Win, "win");
  m_pAudio->Load(eSound::Lose , "lose");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  SAFE_DELETE(m_pRenderer); 
} //Release

/// Ask the object manager to create the game objects. This game has only two
/// objects, the 8-ball and the cue-ball.  This function creates them and sets
/// the impulse vector to point from the cue-ball to the 8-ball.

void CGame::CreateObjects(){
  const float mid = m_nWinHeight/2.0f; //half window height

  Vector2 v = Vector2(732.0f, mid); //initial 8-ball position
  m_pObjectManager->create(eSprite::Eightball, v); //create 8-ball

  v = Vector2(295.0f, mid); //initial cue-ball position
  m_pObjectManager->create(eSprite::Cueball, v); //create cue-ball

  m_pObjectManager->ResetImpulseVector(); //impulse vector from cue- to 8-ball
} //CreateObjects

/// Call this function to start a new game. This ought to be re-entrant so that
/// we can restart a new game without having to shut down and restart the
/// program, but it's moot here. All we really need to do is set the game state
/// to the initial state, clear any old objects out of the object manager, and
/// create some new ones.

void CGame::BeginGame(){  
  m_eGameState = eGameState::Initial; //initial state  
  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects 
} //BeginGame

/// Poll the keyboard state and respond to the key presses that happened in the
/// last frame. Responses to user keystrokes depend on the current game state.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 

  const float t = m_pTimer->GetFrameTime();

  const float MOVEDELTA = 120.0f*t; //small change in position
  const float ANGLEDELTA = 0.1f*t; //small change in angle
  const float ANGLEDELTA2 = t; //large change in angle
  
  if(m_pKeyboard->TriggerDown(VK_F1)) //help
    ShellExecute(0, 0, "https://larc.unt.edu/code/physics/pool/", 0, 0, SW_SHOW);
  
  if(m_pKeyboard->TriggerDown(VK_F2))
    m_bDrawFrameRate = !m_bDrawFrameRate;
  
  if(m_pKeyboard->TriggerDown(VK_F3)){ //toggle step mode
    m_bStepMode = !m_bStepMode; 
    m_bStep = false; //note: if not in step mode, this doesn't matter
    m_pParticleEngine->clear(); 
    if(m_bStepMode)m_bShowCollisions = false;
  } //if

  if(m_pKeyboard->TriggerDown(VK_F4)){ //toggle show collisions mode
    m_bShowCollisions = !m_bShowCollisions;
    if(m_bShowCollisions)m_bStepMode = false;
  } //if

  switch(m_eGameState){
    case eGameState::Initial:  //initial state, can move cue-ball on base line
      if(m_pKeyboard->Down(VK_UP)){  
        m_pObjectManager->AdjustCueBall(MOVEDELTA); 
        m_pObjectManager->ResetImpulseVector();
      } //if

      if(m_pKeyboard->Down(VK_DOWN)){      
        m_pObjectManager->AdjustCueBall(-MOVEDELTA); 
        m_pObjectManager->ResetImpulseVector();
      } //if

    //falls into next case with no break

    case eGameState::SetupShot: //initial state or setting up shot, can adjust cue direction
      if(m_pKeyboard->Down(VK_LEFT))
        m_pObjectManager->AdjustImpulseVector(ANGLEDELTA);

      if(m_pKeyboard->Down(VK_RIGHT))
        m_pObjectManager->AdjustImpulseVector(-ANGLEDELTA);

      if(m_pKeyboard->Down(VK_PRIOR))
        m_pObjectManager->AdjustImpulseVector(ANGLEDELTA2);

      if(m_pKeyboard->Down(VK_NEXT))
        m_pObjectManager->AdjustImpulseVector(-ANGLEDELTA2);

      if(m_pKeyboard->TriggerDown(VK_SPACE)){ //shoot!
        m_pObjectManager->Shoot(); //deliver impulse to ball
        m_eGameState = eGameState::InMotion; //change state
        m_bStep = true; //in case we are in Step Mode
      } //if
    break;

    case eGameState::Won:
    case eGameState::Lost: //if the user hits the space bar, they want out of this state
      if(m_pKeyboard->TriggerDown(VK_SPACE))
        BeginGame();
    break;

    case eGameState::InMotion: //we might be in Step Mode, so...
      if(m_pKeyboard->TriggerDown(VK_SPACE))
        m_bStep = true;
    break;
  } //switch
} //KeyboardHandler

/// Draw the current frame rate to a hard-coded position in the window.

void CGame::DrawFrameRateText(){
  const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps";
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f);
  m_pRenderer->DrawScreenText(s.c_str(), pos, Colors::White);
} //DrawFrateRateText

/// Render the current animation frame. The renderer is notified of the start
/// and end of the frame so that it can let Direct3D do its pipelining. Draw 
/// the background, the particles, the game objects, a win or lose text if
/// appropriate, and the frame rate if required.

void CGame::RenderFrame(){ 
  m_pRenderer->BeginFrame();

  m_pRenderer->Draw(eSprite::Background, m_vWinCenter); //draw background
  m_pParticleEngine->Draw();
  m_pObjectManager->Draw(); 
  m_pRenderer->DrawMessage(m_eGameState);
  if(m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required

  m_pRenderer->EndFrame();
} //RenderFrame

/// Check whether the game state needs to be changed, and take the appropriate
/// action if it does.

void CGame::ProcessState(){
  switch(m_eGameState){
    case eGameState::InMotion: //balls are supposed to be moving
      if(m_pObjectManager->CueBallDown()){ //cue ball is down
        m_eGameState = eGameState::Lost; //player has lost
        m_fGameStateTime = m_pTimer->GetTime(); //set state timer
        m_pAudio->play(eSound::Lose); //boo
        m_pParticleEngine->clear(1.0f); 
      } //if

      else if(m_pObjectManager->AllStopped()){ //all balls have stopped
        m_pParticleEngine->clear(1.0f); 

        if(m_pObjectManager->BallDown()){ //if a ball went down, it must be the 8-ball 
          m_eGameState = eGameState::Won; //player has won
          m_fGameStateTime = m_pTimer->GetTime(); //set state timer
          m_pAudio->play(eSound::Win); //applause
        } //if

        else{       
          m_eGameState = eGameState::SetupShot; //player is ready for the next shot
          m_pObjectManager->ResetImpulseVector(); //compute new impulse vector
        } //else if
      } //else if
    break;

    case eGameState::Won:
    case eGameState::Lost: { 
      if(m_pTimer->GetTime() >= m_fGameStateTime + 3) //after 3 seconds 
        BeginGame(); //restart 
    } //case
    break;
  } //switch
} //ProcessState

/// Process the current frame. This involves responding to keyboard input,
/// moving the game objects and rendering them in their new positions and
/// orientations. Also notify the audio player at the start of each frame so
/// that it can prevent multiple copies of a sound from starting on the same
/// frame, notify the step timer of the start and end of the frame so that
/// it can calculate frame time, and take any actions required by the current
/// game state.

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input

  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  m_pTimer->Tick([&](){ 
    m_pObjectManager->move(); //move all objects
    m_pParticleEngine->step(); //fade out old particles   
    m_bStep = false; 
  });

  RenderFrame(); //render a frame of animation  
  ProcessState(); //state-based actions
} //ProcessFrame
