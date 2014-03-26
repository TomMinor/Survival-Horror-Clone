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
  return Vec4( m_xmax - _b.m_xmin,
               m_ymax - _b.m_ymin,
               m_zmax - _b.m_zmin );
}

void BBox::move(Vec4 _pos)
{
  m_offset = _pos;
}

void BBox::draw() const
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  float xmax = m_xmax + m_offset.m_x;   float xmin = m_xmin + m_offset.m_x;
  float ymax = m_ymax + m_offset.m_y;   float ymin = m_ymin + m_offset.m_y;
  float zmax = m_zmax + m_offset.m_z;   float zmin = m_zmin + m_offset.m_z;

  glColor3f( 1.0f, 0.25f, 0.0f );
  glPushMatrix();
    glTranslatef( (xmax+xmin)/2 , (ymax+ymin)/2, (zmax+zmin)/2);
    glPushMatrix();
      GLFunctions::cube(fabs(xmax - xmin),
                        fabs(ymax - ymin),
                        fabs(zmax - zmin));
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

