#include "3dUtilities.h"
#include "GLinc.h"

namespace util {

void drawWorldAxis(float _scale)
{
  glScalef(_scale, _scale, _scale);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
    glColor3f(1.0,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
  glEnd();
  glEnable(GL_LIGHTING);
  glScalef(1/_scale, 1/_scale, 1/_scale);
}

}
