#include "GLinc.h"
#include "BoundingBox.h"
#include "GLFunctions.h"
#include <iostream>

namespace Game {

bool BBox::checkCollision(const BBox & _b) const
{
  return ( (m_xmax > _b.m_xmin) && (m_xmin < _b.m_xmax) &&
           (m_ymax > _b.m_ymin) && (m_ymin < _b.m_ymax) &&
           (m_zmax > _b.m_zmin) && (m_zmin < _b.m_zmax) );
}

Vec4 BBox::intersectionAmount(const BBox &_b)
{
  return Vec4( abs(m_xmax - _b.m_xmin),
               abs(m_ymax - _b.m_ymin),
               abs(m_zmax - _b.m_zmin) );
}



void BBox::move(Vec4 _pos)
{
  m_xmax += _pos.m_x;   m_xmin += _pos.m_x;
  m_ymax += _pos.m_x;   m_ymin += _pos.m_x;
  m_zmax += _pos.m_x;   m_zmin += _pos.m_x;
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

