#include "Background.h"
#include <iostream>

#include "GLFunctions.h"


namespace Game {

bool Background::touchesTrigger(const BBox& _actor)
{
  return m_triggerVolume.checkIntersectOrTouch(_actor);
}

void Background::loadBackgroundTexture()
{
  delete m_bgTexture;
  m_bgTexture = new Texture(FileSystem().roomPath(m_bgPath));
}

void Background::drawBG() const
{
  // Draw facade very close to the farZ plane
  // This should be far enough back from the origin for the player to never reach it
  drawFacade(-499.0f);
}

void Background::drawFG() const
{
  // Enable alpha blending & draw the opaque pixels on a plane very close to the nearZ plane
  // This gives the effect of being infront of the scene
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.1f);

  if(m_bgTexture && m_bgTexture->hasAlphaMask())
  {
    drawFacade(0.0f);
  }

  glDisable(GL_ALPHA_TEST);
  glDisable( GL_BLEND );
}

void Background::drawFacade(float _zOffset) const
{
  // Don't draw if no texture was loaded or the fallback texture was used
  if(m_bgTexture && !m_bgTexture->usingFallback())
  {
    m_bgTexture->setCurrent();

    // Draw in orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      glOrtho(0.0, 800, 0.0, 600, -500.0, 500.0);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        // Draw the facade so that it fills the screen
        glTranslatef(0, 0, _zOffset);
        glBegin(GL_QUADS);
          glTexCoord2f(0, 1); glVertex3f(0,                    0,                     0);
          glTexCoord2f(0, 0); glVertex3f(0,                    m_bgTexture->height(), 0);
          glTexCoord2f(1, 0); glVertex3f(m_bgTexture->width(), m_bgTexture->height(), 0);
          glTexCoord2f(1, 1); glVertex3f(m_bgTexture->width(), 0,                     0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

        glPopMatrix();
      glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
  }
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
