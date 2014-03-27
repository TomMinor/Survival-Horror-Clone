#include "GLinc.h"
#include "BoundingBox.h"
#include "GLFunctions.h"
#include <iostream>

namespace Game {

bool BBox::checkCollision(const BBox& _b) const
{
  return ( ((m_xmax > _b.m_xmin) && (m_xmin < _b.m_xmax)) &&
           ((m_ymax > _b.m_ymin) && (m_ymin < _b.m_ymax)) &&
           ((m_zmax > _b.m_zmin) && (m_zmin < _b.m_zmax)) );
}


Vec4 BBox::getCenter() const
{
  return Vec4( (m_xmax+m_xmin)/2,
               (m_ymax+m_ymin)/2,
               (m_zmax+m_zmin)/2 );
}

//Vec4 BBox::getCollisionNormal(const BBox& _b)
//{
//  (m_xmax > _b.m_xmin) && (m_xmin < _b.m_xmax)
//  (m_ymax > _b.m_ymin) && (m_ymin < _b.m_ymax)
//  (m_zmax > _b.m_zmin) && (m_zmin < _b.m_zmax)
//}

void BBox::move(Vec4 _pos)
{
  m_xmax += _pos.m_x;   m_xmin += _pos.m_x;
  m_ymax += _pos.m_y;   m_ymin += _pos.m_y;
  m_zmax += _pos.m_z;   m_zmin += _pos.m_z;
}

void BBox::draw() const
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  glColor3f( 1.0f, 0.25f, 0.0f );
  glPushMatrix();
    glTranslatef( (m_xmax+m_xmin)/2 , (m_ymax+m_ymin)/2, (m_zmax+m_zmin)/2);
    glPushMatrix();
      GLFunctions::cube(fabs(m_xmax - m_xmin),
                        fabs(m_ymax - m_ymin),
                        fabs(m_zmax - m_zmin));
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

