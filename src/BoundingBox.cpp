#include "GLinc.h"
#include "BoundingBox.h"
#include "GLFunctions.h"
#include <iostream>

namespace Game {

bool BBox::m_debugDraw = true;

bool BBox::checkCollision(const BBox& _b) const
{
  return ( ((m_max.m_x > _b.m_min.m_x) && (m_min.m_x < _b.m_max.m_x)) &&
           ((m_max.m_y > _b.m_min.m_y) && (m_min.m_y < _b.m_max.m_y)) &&
           ((m_max.m_z > _b.m_min.m_z) && (m_min.m_z < _b.m_max.m_z)) );
}


Vec4 BBox::getCenter() const
{
  return Vec4( (m_max.m_x+m_min.m_x)/2,
               (m_max.m_y+m_min.m_y)/2,
               (m_max.m_z+m_min.m_z)/2 );
}

void BBox::move(Vec4 _pos)
{
//  m_max.m_x += _pos.m_x;   m_min.m_x += _pos.m_x;
//  m_max.m_y += _pos.m_y;   m_min.m_y += _pos.m_y;
//  m_max.m_z += _pos.m_z;   m_min.m_z += _pos.m_z;

  m_max += _pos;
  m_min += _pos;
}

void BBox::draw() const
{
  if(m_debugDraw)
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glColor3f( 1.0f, 0.25f, 0.0f );
    glPushMatrix();
      glTranslatef( (m_max.m_x+m_min.m_x)/2 ,
                    (m_max.m_y+m_min.m_y)/2,
                    (m_max.m_z+m_min.m_z)/2);
      glPushMatrix();
        GLFunctions::cube(fabs(m_max.m_x - m_min.m_x),
                          fabs(m_max.m_y - m_min.m_y),
                          fabs(m_max.m_z - m_min.m_z));
      glPopMatrix();
    glPopMatrix();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
}

std::ostream & operator<<(std::ostream &_output, const BBox &_b)
{
  _output << "Min x: " << _b.m_min.m_x
        << "\nMin y: " << _b.m_min.m_y
        << "\nMin z: " << _b.m_min.m_z
        << "\nMax x: " << _b.m_max.m_x
        << "\nMax y: " << _b.m_max.m_y
        << "\nMax z: " << _b.m_max.m_z;

  return _output;
}

}

