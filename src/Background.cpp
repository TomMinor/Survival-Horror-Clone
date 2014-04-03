#include "Background.h"
#include <iostream>

#include "GLFunctions.h"

namespace Game {

void Background::drawBG() const
{
//  glPushMatrix();
//    Texture tmp("assets/backgrounds/BG_01_bg.tif");

//    glDisable(GL_LIGHTING);
//    glEnable(GL_TEXTURE_2D);

//    glColor3f(1.0f, 1.0f, 1.0f);
//    tmp.setCurrent();

//    //Mat4 tmpMat(m_cameraView.billboard(Vec4(), Vec4(0,1,0)));

//    glPushMatrix();
//      //glMultMatrixf(tmpMat.m_openGL);
//      glTranslatef(0, 2.5, 0);
//      drawBillboard();
//    glPopMatrix();

//    glDisable(GL_TEXTURE_2D);
//    glEnable(GL_LIGHTING);
//  glPopMatrix();

//  m_cameraView.setView();
}

void Background::drawFG() const
{
  static float	rotate_x = 30;
  static float	rotate_y = 15;
  static float	rotate_z = -5;

  // BUG Why am i loading this every frame oh christ fix me


  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor3f(1.0f, 1.0f, 1.0f);


//  glLoadIdentity ();
//  glTranslatef (0, 0, -10.);

//  glRotatef (rotate_z, 0, 0, 1);
//  glRotatef (rotate_y, 1, 0, 0);
//  glRotatef (rotate_x, 0, 1, 0);

//  glPushMatrix ();
//  glRotatef (rotate_x, 0, -1, 0);
//  glRotatef (rotate_y, -1, 0, 0);
//  glRotatef (rotate_z, 0, 0, -1);
//  drawBillboard ();
//  glPopMatrix ();

  Mat4 tmpMat;

//  glTranslatef(0.01, 0.1,0);
//  glPushMatrix();

  drawBillboard();

//    Vec4 tmpView(m_cameraView.getRotation());
//    //tmpView *= -1;
//    tmpMat.rotate(tmpView);
//    glMultMatrixf(tmpMat.m_openGL);
//    glTranslatef(0, 0, 0.1);

//  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
  glDisable( GL_BLEND );
  glEnable(GL_LIGHTING);
}

void Background::drawBillboard() const
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, 800, 0.0, 600, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();


  glLoadIdentity();
  glDisable(GL_LIGHTING);


  glColor3f(1,1,1);
  glEnable(GL_TEXTURE_2D);
  //glBindTexture(GL_TEXTURE_2D, mark_textures[0].id);

  Texture tmp("assets/backgrounds/BG_01_bg.tif");
  tmp.setCurrent();

  // Draw a textured quad
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
  glTexCoord2f(0, 1); glVertex3f(0, 100, 0);
  glTexCoord2f(1, 1); glVertex3f(100, 100, 0);
  glTexCoord2f(1, 0); glVertex3f(100, 0, 0);
  glEnd();


  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);


//  glPushMatrix();
//    glRotatef(180, 1, 0, 0);
//    glTranslatef(0,0,0);
//    glBegin(GL_TRIANGLE_FAN);
//      glTexCoord2f(1.0f, 0.0f);
//      glVertex3f(2, -2, 0);
//      glTexCoord2f(1.0f, 1.0f);
//      glVertex3f(2, 2, 0);
//      glTexCoord2f(0.0f, 1.0f);
//      glVertex3f(-2, 2, 0);
//      glTexCoord2f(0.0f, 0.0f);
//      glVertex3f(-2, -2, 0);
//    glEnd();
//  glPopMatrix();
}

/*
** Screen aligned billboard (text)
*/
void		screen_billboard (void)
{

}

void Background::drawTrigger() const
{

  m_triggerColour.colourGL();
  m_triggerVolume.draw();
}

}
