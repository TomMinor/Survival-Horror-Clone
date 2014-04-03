#include "Background.h"
#include <iostream>

#include "GLFunctions.h"

namespace Game {

bool Background::touchesTrigger(const BBox& _actor)
{
  return m_triggerVolume.checkCollision(_actor);
}

void Background::drawBG() const
{
  glColor3f(1,1,1);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  Game::Texture tmp("assets/backgrounds/BG_01_bg.tif");
  tmp.setCurrent();

  drawFacade(-499.0f);

  glDisable(GL_TEXTURE_2D);
}

void Background::drawFG() const
{
  // BUG Why am i loading this every frame oh christ fix me

  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor3f(1.0f, 1.0f, 1.0f);
  Game::Texture tmp("assets/backgrounds/BG_01_bg.tif");
  tmp.setCurrent();

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.1f);
  glDisable(GL_DEPTH_TEST);

  drawFacade(0.0f);

  glDisable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_TEXTURE_2D);
  glDisable( GL_BLEND );
  glEnable(GL_LIGHTING);
}

void Background::drawFacade(float _zOffset) const
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, 800, 0.0, 600, -500.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glLoadIdentity();

  // Draw a textured quad
  glTranslatef(0, 0, _zOffset);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(0, 1024, 0);
    glTexCoord2f(1, 0); glVertex3f(1024, 1024, 0);
    glTexCoord2f(1, 1); glVertex3f(1024, 0, 0);
  glEnd();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
}

void Background::drawTrigger() const
{
  m_triggerColour.colourGL();
  m_triggerVolume.draw();
}

void Background::setCameraView() const
{
  m_cameraView.setView();
}

}
