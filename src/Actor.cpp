#include "Actor.h"
#include "GLFunctions.h"

#include <iostream>

namespace Game {

void Actor::draw() const
{
  m_bbox.draw();

  glColor3f( 0.25f, 1.0f, 0.0f );
  glPushMatrix();
    // Center the cone within the bbox
    glTranslatef(m_scale.m_x/2, m_scale.m_y/2, m_scale.m_z/2);
    glTranslatef(m_pos.m_x, m_pos.m_y, m_pos.m_z);
    glPushMatrix();
      glRotatef(m_yaw, 0, 1, 0);
      GLFunctions::cone(m_scale.m_x, m_scale.m_x, 10, 10);
    glPopMatrix();
  glPopMatrix();
}

void Actor::move(float _offset, float _deg)
{
  m_yaw += _deg;
  m_pos.m_x += _offset * sin((PI*m_yaw)/180);
  m_pos.m_z += _offset * cos((PI*m_yaw)/180);

  m_bbox.move(m_pos);
}

}
