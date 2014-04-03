#include "Background.h"
#include <iostream>

#include "GLFunctions.h"


namespace Game {

bool Background::touchesTrigger(const BBox& _actor)
{
  //m_bgTexture.setCurrent();
  return m_triggerVolume.checkCollision(_actor);
}

void Background::loadBackgroundTexture()
{
  delete m_bgTexture;
  std::cout << "Current BG : " << FileSystem().roomPath(m_bgPath) << "\n";
  m_bgTexture = new Texture(FileSystem().roomPath(m_bgPath));
}

void Background::drawBG() const
{
  if(m_bgTexture) { m_bgTexture->setCurrent(); }
//  glColor3f(1,1,1);
//  glEnable(GL_TEXTURE_2D);


//  Game::Texture tmp("assets/backgrounds/BG_01_bg.tif");
//  tmp.setCurrent();

  //m_bgTexture->setCurrent();

  drawFacade(-499.0f);

//  glDisable(GL_TEXTURE_2D);
}

void Background::drawFG() const
{  
//  glDisable(GL_LIGHTING);
//  glEnable(GL_TEXTURE_2D);

  // Enable alpha blending & disable the depth test so the opaque pixels
  // (specified by the bg texture mask) are drawn on top of everything
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//  glColor3f(1.0f, 1.0f, 1.0f);

  if(m_bgTexture) { m_bgTexture->setCurrent(); }

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.1f);
  glDisable(GL_DEPTH_TEST);

  drawFacade(0.0f);

  glDisable(GL_ALPHA_TEST);
  glEnable(GL_DEPTH_TEST);

  glDisable( GL_BLEND );
//  glEnable(GL_LIGHTING);
}

void Background::drawFacade(float _zOffset) const
{
  if(m_bgTexture)   // Don't draw if no texture was loaded
  {
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
