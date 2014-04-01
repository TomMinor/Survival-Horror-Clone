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
  damage(1);
  if(m_state == IDLE)
  {
    // Only set the walk animation when the player moves
    if(_offset != 0 || _deg != 0) { m_state = WALK; }
    else                          { m_state = IDLE; }

    m_yaw += _deg;

    // Correct the actor's translation to move along it's local (rotated) axis
    float offsetX = _offset * sin((PI*m_yaw)/180);
    float offsetZ = _offset * cos((PI*m_yaw)/180);

    m_pos.m_x += offsetX;
    m_pos.m_z += offsetZ;

    m_bbox.move(Vec4(offsetX, 0, offsetZ));
  }
}

void Actor::update()
{
  m_time++;

  // Update animations
  if(m_state != m_previousState)
  {
    switch(m_state)
    {
      case WALK:          { forceAnimation(Md2::Animation::WALK);       break; }
      case DASH:          { forceAnimation(Md2::Animation::RUN);        break; }
      case PREPARE_ATTACK:{ forceAnimation(Md2::Animation::PREP_ATTACK);break; }
      case ATTACK:        { forceAnimation(Md2::Animation::ATTACK);     break; }
      case PAIN:          { forceAnimation(Md2::Animation::PAIN);       break; }
      case DYING:         { forceAnimation(Md2::Animation::DEATH);      break; }
      case DEAD:          { forceAnimation(Md2::Animation::DEATH_IDLE); break; }
      default:
      {
        forceAnimation(Md2::Animation::STAND);  break;
      }
    }
  }

  m_previousState = m_state;
}

void Actor::damage(int _value)
{
  m_health -= _value;
  if(m_health <= 0) { m_state = DYING; }
  else              { m_state = PAIN;  }
}

void Actor::die()
{

}

}
