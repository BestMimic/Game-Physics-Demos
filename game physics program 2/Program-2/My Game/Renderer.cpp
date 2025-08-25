/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"
#include "SpriteDesc.h"

#include "Abort.h"

CRenderer::CRenderer(): 
  LSpriteRenderer(eSpriteMode::Batched2D){
} //constructor

/// Load the specific images needed for this game.
/// This is where eSprite values from GameDefines.h get
/// tied to the names of sprite tags in gamesettings.xml. Those
/// sprite tags contain the name of the corresponding image file.
/// If the image tag or the image file are missing, then
/// the game should abort from deeper in the Engine code,
/// leaving you with a dialog box that tells you what
/// went wrong.

void CRenderer::LoadImages(){  
  BeginResourceUpload();

  Load(eSprite::Background, "background");  
  Load(eSprite::Arrow, "arrow"); 
  Load(eSprite::Cueball, "cueball"); 
  Load(eSprite::Eightball, "eightball"); 
  Load(eSprite::Stepmode, "stepmode"); 
  Load(eSprite::Circle, "circle");
  Load(eSprite::Thickcircle, "thickcircle");
  Load(eSprite::Line, "line");

  EndResourceUpload();
} //LoadImages
 
/// Draw a win/lose text message to the center of the window if the game is in
/// the appropriate state.
/// \param state Current game state.

void CRenderer::DrawMessage(eGameState state){
  if(state == eGameState::Won) 
    DrawCenteredText("You win!", Colors::White); 

  else if(state == eGameState::Lost) 
    DrawCenteredText("You lose!", Colors::Black);
} //DrawMessage


/// Draw a line by rendering multiple copies of a line sprite in
/// batched mode. The line sprite is assumed to be horizontal
/// and fairly small.
/// \param p0 Position of one end of the sprite in world space.
/// \param p1 Position of the other end of the sprite in world space.
/// \param c line color, such as Colors::Aqua.
  
void CRenderer::DrawLine(const Vector2& p0, const Vector2& p1, XMVECTORF32 c){ 
  LSpriteDesc2D d; //sprite descriptor for line sprite
  d.m_nSpriteIndex = (UINT)eSprite::Line; //line sprite index
  d.m_f4Tint = XMFLOAT4(c);
 
  const Vector2 dv = p0 - p1; //vector from p1 to p0
  d.m_fRoll = atan2f(dv.y, dv.x); //orientation angle

  const float w = GetWidth((UINT)eSprite::Line); //width of line sprite

  const int m = (unsigned)ceil(dv.Length()/w); //number of copies to render
  const Vector2 delta = dv/(float)m; //offset between copies
  Vector2 u = delta; //direction of line
  u.Normalize(); //make it a unit vector

  d.m_vPos = p1 + 0.5f*w*u; //offset the first copy by half a width

  for(int i=0; i<m; i++){ //draw copies
    Draw(&d); //draw copy
    d.m_vPos += delta; //advance along line
  } //for
} //DrawLine
