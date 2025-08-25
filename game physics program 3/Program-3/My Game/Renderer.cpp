/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"

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
  Load(eSprite::BlackLine, "blackline");
  Load(eSprite::Clip, "clip");
  
  Load(eSprite::UnlitTriangle, "triangle0");
  Load(eSprite::LitTriangle, "triangle1");
  Load(eSprite::UnlitDiamond, "diamond0");
  Load(eSprite::LitDiamond, "diamond1");
  Load(eSprite::UnlitPentagon, "pentagon0");
  Load(eSprite::LitPentagon, "pentagon1");

  Load(eSprite::Flipper, "flipper");
  
  Load(eSprite::UnlitSpecial, "special0");
  Load(eSprite::LitSpecial, "special1");

  Load(eSprite::UnlitSlot, "slot0");
  Load(eSprite::LitSlot, "slot1");
  
  Load(eSprite::Ball, "ball"); 
  Load(eSprite::LED, "LED");

  EndResourceUpload();
} //LoadImages
