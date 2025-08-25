/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"

/// Create the renderer and the sprite descriptor load images and sounds, and
/// begin the game.

void CGame::Initialize(){
  m_pRenderer = new LSpriteRenderer(eSpriteMode::Batched2D); 
  m_pRenderer->Initialize(eSprite::Size); 
  LoadImages(); //load images from xml file list
  LoadSounds(); //load the sounds for this game
} //Initialize

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages(){  
  m_pRenderer->BeginResourceUpload();

  m_pRenderer->Load(eSprite::Background, "background"); 
  m_pRenderer->Load(eSprite::Line, "line");  
  m_pRenderer->Load(eSprite::RedLine, "redline"); 
  m_pRenderer->Load(eSprite::Bezel, "bezel"); 

  m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);
  m_pAudio->Load(eSound::Tick, "tick");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 
  
  if(m_pKeyboard->TriggerDown(VK_F2)) //toggle frame rate 
    m_bDrawFrameRate = !m_bDrawFrameRate;
} //KeyboardHandler

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in gamesettings.xml.

void CGame::DrawFrameRateText(){
  const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw date and time string to a fixed position.
/// \param t The current time.

void CGame::RenderDateAndTime(time_t t){
  const UINT bufsize = 256;
  char buffer[bufsize];
  //const time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ctime_s(buffer, bufsize, &t);
  m_pRenderer->DrawScreenText(buffer, Vector2(150.0f, 580.0f));
} //RenderDateAndTime

/// Draw the clock's hands.
/// \param t The current time.

void CGame::RenderClockHands(time_t t){
  struct tm timeinfo;
  localtime_s(&timeinfo, &t);

  //parse time info into fHrs, fMins, fSecs.

  const float fSecs = (float)timeinfo.tm_sec;
  const float fMins = timeinfo.tm_min + fSecs/60;
  const float fHrs  = timeinfo.tm_hour%12 + fMins/60;

  //hour hand
  Vector2 vEnd = GetHandEnd(5*fHrs, 110.0f); //position of end of hand
  m_pRenderer->DrawLine(eSprite::Line, m_vWinCenter, vEnd); //draw hand
  RenderArrowhead(vEnd); //draw arrowhead on hand

  //minute hand
  vEnd = GetHandEnd(fMins, 140.0f); //position of end of hand
  m_pRenderer->DrawLine(eSprite::Line, m_vWinCenter, vEnd); //draw hand
  RenderArrowhead(vEnd); //draw arrowhead on hand

  //second hand
  vEnd = GetHandEnd(fSecs, 140.0f); //position of end of hand
  m_pRenderer->DrawLine(eSprite::RedLine, m_vWinCenter, vEnd); //draw hand
  
  //tick sound once per second
  if(fSecs != m_fPrevTicks)m_pAudio->play(eSound::Tick);
  m_fPrevTicks = fSecs;
} //RenderClockHands

/// Get the position of the end of a clock hand.
/// It is assumed that the center of the clock is m_vWinCenter.
/// \param t Time in units of 1/60 of a full rotation.
/// \param len Length of the hand.

Vector2 CGame::GetHandEnd(float t, float len) const{
  const float theta = XM_PIDIV2 - t*XM_2PI/60.0f; //rotation angle

  //**** All students: Modify the next line of code. 
  //**** Hint: theta contains the orientation of the hand.
  
  return m_vWinCenter + len*Vector2(cos(theta), sin(theta));
} //GetHandEnd

/// Render an arrowhead at the end of a hand, oriented perpendicular to the hand.
/// It is assumed that the center of the clock is m_vWinCenter.
/// \param vEnd Position of the end of the hand.

void CGame::RenderArrowhead(const Vector2& vEnd){
  //**** Students enrolled in CSCE 5255 - Your arrowhead code goes here
} //RenderArrowhead

/// Draw the clock. The renderer is notified of the start and end of the
/// frame so that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame(){
  m_pRenderer->BeginFrame(); //required before rendering
  
  m_pRenderer->Draw(eSprite::Background, m_vWinCenter); //draw background
  
  const time_t t = 
    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  RenderDateAndTime(t); //draw date and time string
  RenderClockHands(t); //render clock hands

  m_pRenderer->Draw(eSprite::Bezel, m_vWinCenter); //draw bezel

  if(m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required

  m_pRenderer->EndFrame(); //required after rendering
} //RenderFrame

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the  audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation.

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun

  m_pTimer->Tick([&](){ //all time-dependent function calls should go here
  });

  RenderFrame(); //render a frame of animation
} //ProcessFrame