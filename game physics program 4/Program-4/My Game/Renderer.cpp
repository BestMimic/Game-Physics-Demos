/// \file Renderer.cpp
/// \brief Code for the render world CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"

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

  Load(eSprite::WindmillBg, "windmillbg"); 
  Load(eSprite::WindmillBase, "windmillbase");
  Load(eSprite::WindmillBlade, "windmillblades");
  Load(eSprite::Crate, "crate");
  Load(eSprite::Tire, "tire");
  Load(eSprite::Ball, "ball");

  EndResourceUpload();
} //LoadImages
