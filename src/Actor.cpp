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

      m_meshBody.drawMesh(m_time*0.05f);
      m_meshHead.drawMesh(m_time*0.05f);


      util::drawWorldAxis(1.25);
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

  m_state = WALK;
}

void Actor::update()
{
//  if(m_state != m_previousState)
//  {
//    switch(m_state)
//    {
//      case WALK:          { m_meshBody.setAnimation(MeshMd2::WALK);
//                            m_meshHead.setAnimation(MeshMd2::WALK);   break; }
//      case DASH:          { m_meshBody.setAnimation(MeshMd2::RUN);
//                            m_meshHead.setAnimation(MeshMd2::RUN);    break; }
//      case PREPARE_ATTACK:{ m_meshBody.setAnimation(MeshMd2::PREPARE);
//                            m_meshBody.setAnimation(MeshMd2::PREPARE);break; }
//      case ATTACK:        { m_meshBody.setAnimation(MeshMd2::ATTACK);
//                            m_meshBody.setAnimation(MeshMd2::ATTACK);  break; }
//      case PAIN:          { m_meshBody.setAnimation(MeshMd2::PAIN_A);
//                            m_meshBody.setAnimation(MeshMd2::PAIN_A);  break;}
//      default:
//      {
//        m_meshBody.setAnimation(MeshMd2::STAND);
//        m_meshHead.setAnimation(MeshMd2::STAND);  break;
//      }
//    }
//  }

  //m_previousState = m_state;

  m_time++;
}

}
