#include "GLinc.h"
#include "BoundingBox.h"
#include "GLFunctions.h"
#include <iostream>

namespace Game {

bool BBox::checkCollision(const BBox & _b) const
{
  return ( _b.m_xmax );
}

void BBox::move(Vec4 _pos)
{

}

void BBox::draw() const
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  glColor3f( 1.0f, 0.25f, 0.0f );
  glPushMatrix();
    glTranslatef( (m_xmax+m_xmin)/2 , (m_ymax+m_ymin)/2, (m_zmax+m_zmin)/2);
    glPushMatrix();
      GLFunctions::cube(m_xmax - m_xmin,
                        m_ymax - m_ymin,
                        m_zmax - m_zmin);
    glPopMatrix();
  glPopMatrix();

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

std::ostream & operator<<(std::ostream &_output, const BBox &_b)
{
  _output << "Min x: " << _b.m_xmin
        << "\nMin y: " << _b.m_ymin
        << "\nMin z: " << _b.m_zmin
        << "\nMax x: " << _b.m_xmax
        << "\nMax y: " << _b.m_ymax
        << "\nMax z: " << _b.m_zmax;

  return _output;
}

}

