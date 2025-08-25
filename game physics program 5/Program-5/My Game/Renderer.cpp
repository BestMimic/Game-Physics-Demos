/// \file Renderer.cpp
/// \brief Code for the renderer class CRenderer.

#include "Renderer.h"
#include "ComponentIncludes.h"

CRenderer::CRenderer():
  LSpriteRenderer(eSpriteMode::Batched2D){
} //constructor

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CRenderer::LoadImages(){  
  BeginResourceUpload();

  Load(eSprite::Background, "background"); 
  Load(eSprite::Ball, "ball"); 
  Load(eSprite::RedButton, "button"); 
  Load(eSprite::ClockFace, "clockface"); 
  Load(eSprite::Line, "line");
  Load(eSprite::Metroid, "metroid");
  Load(eSprite::KraidArm, "kraidArm");
  Load(eSprite::KraidFoot, "kraidFoot");
  Load(eSprite::Kraid, "kraid");
  Load(eSprite::KraidDead, "kraidDead");
  Load(eSprite::Chozo, "chozo");
  Load(eSprite::Triangle, "triangle");
  Load(eSprite::FlipTriangle, "flipTriangle");
  Load(eSprite::BigLine, "bigLine");

  EndResourceUpload();
} //LoadImages

/// Draw a Box2D polygon shape using lines. Iterate through the polygon points,
/// drawing a line from each one to the one before it. The line from the first
/// point to the last one is a special case.
/// \param t Line sprite type.
/// \param p Pointer to a Box2D polygon shape.
/// \param pos Position in Physics World.
/// \param theta Orientation.

void CRenderer::Drawb2Polygon(eSprite t, b2PolygonShape* p,
  const b2Vec2 pos, float theta)
{
  const Vector2 p0 = PW2RW(pos + b2Mul(b2Rot(theta), p->m_vertices[0]));
  Vector2 p1(p0); //first point

  for(int32 i=1; i<p->m_count; i++){ //for each vertex after the first
    const Vector2 p2 = PW2RW(pos + b2Mul(b2Rot(theta), p->m_vertices[i])); //second point
    DrawLine(t, p1, p2); //draw from first point to second point
    p1 = p2; //move on to next point
  } //for
              
  DrawLine(eSprite::Line, p0, p1); //draw from first point to last point
} //Drawb2Polygon

/// Draw a Box2D edge shape as a line.
/// \param t Line sprite type.
/// \param p Pointer to a Box2D edge shape.
/// \param pos Position in Physics World.

void CRenderer::Drawb2Edge(eSprite t, b2EdgeShape* p, const b2Vec2 pos){
  DrawLine(t, PW2RW(pos + p->m_vertex1),
    PW2RW(pos + p->m_vertex2));
} //Drawb2Edge

/// Draw a Box2D circle shape by breaking it up into lots of little lines.
/// \param t Line sprite type.
/// \param p Pointer to a Box2D circle shape.
/// \param pos Position in Physics World.

void CRenderer::Drawb2Circle(eSprite t, b2CircleShape* p, const b2Vec2 pos){
  const float r = PW2RW(p->m_radius); //radius
  const UINT n = (UINT)ceilf(XM_2PI*r/GetWidth(eSprite::Line)) + 1; //number of lines

  for(UINT i=0; i<n; i++){ //for each tiny line (almost a point really)
    const float theta = XM_2PI*i/(float)n; //rotation angle
    const Vector2 v2 = PW2RW(pos) + r*Vector2(cosf(theta), sinf(theta));
    Draw(t, v2, XM_PI/2.0f + theta); //draw line tangent to circle
  } //for
} //Drawb2Circle

/// Draw a Box2D chain shape using lines.
/// This function is a stub that has not yet been implemented.
/// \param t Line sprite type.
/// \param p Pointer to a Box2D chain shape.
/// \param pos Position in Physics World.

void CRenderer::Drawb2Chain(eSprite t, b2ChainShape* p, const b2Vec2 pos){
  //not implemented
} //Drawb2Chain

/// Draw a Box2D shape using lines. This is just a `switch` statement that
/// calls one of Drawb2Circle(), Drawb2Edge(), Drawb2Polygon(), or Drawb2Chain()
/// depending on the shape's type.
/// \param t Line sprite type.
/// \param p Pointer to a Box2D shape.
/// \param pos Position in Physics World.
/// \param theta Orientation.

void CRenderer::Drawb2Shape(eSprite t, b2Shape* p, const b2Vec2 pos, float theta){
  switch(p->GetType()){
    case b2Shape::e_circle: 
      Drawb2Circle(t, (b2CircleShape*)p, pos); 
      break;

		case b2Shape::e_edge:
      Drawb2Edge(t, (b2EdgeShape*)p, pos);
      break;

		case b2Shape::e_polygon:
      Drawb2Polygon(t, (b2PolygonShape*)p, pos, theta);
      break;

		case b2Shape::e_chain:
      Drawb2Chain(t, (b2ChainShape*)p, pos);
      break;
  } //switch
} //Drawb2Shape

/// Draw a Box2D body using lines. Iterate through the fixtures attached to the
/// body, calling Drawb2Shape() to draw the shape of each fixture.
/// \param t Line sprite type.
/// \param pBody Pointer to a Box2D body.

void CRenderer::Drawb2Body(eSprite t, b2Body* pBody){ 
  for(b2Fixture* p = pBody->GetFixtureList(); p; p = p->GetNext())
    Drawb2Shape(t, p->GetShape(), pBody->GetPosition(), pBody->GetAngle());
} //Drawb2Body