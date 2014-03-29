#include "Actor.h"
#include "GLFunctions.h"

#include "3dUtilities.h"
#include <iostream>

namespace Game {

void Actor::draw()
{
  m_bbox.draw();

  glPushMatrix();
    // Center within bbox
    glTranslatef(m_scale.m_x/2, 0.0f, m_scale.m_z/2);

    m_pos.translateGL();
    glPushMatrix();
      glRotatef(m_yaw, 0, 1, 0);

      body.drawMesh(m_time*0.05f);
      head.drawMesh(m_time*0.05f);

      util::drawWorldAxis(1.25);

      // Cylinder body
      //glRotatef(90, 1, 0, 0);
      //glColor3f( 0.8f, 0.8f, 0.25f );
      //glTranslatef(0, 0, m_scale.m_y/2);
      //GLFunctions::cylinder(m_scale.m_x/2, m_scale.m_y*2, 10, 10);
    glPopMatrix();
  glPopMatrix();
}

void Actor::move(float _offset, float _deg)
{
  m_yaw += _deg;

  // Correct the actor's translation to move along it's local (rotated) axis
  float offsetX = _offset * sin((PI*m_yaw)/180);
  float offsetZ = _offset * cos((PI*m_yaw)/180);

  m_pos.m_x += offsetX;
  m_pos.m_z += offsetZ;

  m_bbox.move(Vec4(offsetX, 0, offsetZ));
}

void Actor::update()
{
  m_time++;
}

}
