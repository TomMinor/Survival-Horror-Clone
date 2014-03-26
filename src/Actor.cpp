#include "Actor.h"
#include "GLFunctions.h"

#include <iostream>

namespace Game {

void Actor::draw() const
{
  m_bbox.draw();

  glPushMatrix();
    // Center within bbox
    glTranslatef(m_scale.m_x/2, m_scale.m_y/2, m_scale.m_z/2);
    m_pos.translateGL();
    glPushMatrix();
      glRotatef(m_yaw, 0, 1, 0);

      // Directional arrow
      glColor3f( 1.0f, 0.0f, 0.0f );
      GLFunctions::cone(m_scale.m_x/2, m_scale.m_y/2, 10, 10);

      // Cylinder body
      glRotatef(90, 1, 0, 0);
      glTranslatef(0, 0, m_scale.m_y/2);
      glColor3f( 0.8f, 0.8f, 0.25f );
      GLFunctions::cylinder(m_scale.m_x/2, m_scale.m_y*2, 10, 10);
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
