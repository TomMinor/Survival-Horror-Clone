#include "Actor.h"
#include "GLFunctions.h"

#include "3dUtilities.h"
#include <iostream>

namespace Game {

void Actor::draw()
{
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

  glColor3f(0.5, 0.5, 0.5f);
  m_bbox.draw();
}

void Actor::move(float _offset, float _deg)
{
  std::cout << m_pos << "\n";
  if(m_state != PAIN && m_state != DYING && m_state != DEAD)
  {
    // Only set the walk animation when the player moves
    if(_offset != 0 || _deg != 0) { m_state = WALK; }
    else                          { m_state = IDLE; }

    m_yaw += _deg;

    // Only allow rotation while preparing to attack
    if(m_state != PREPARE_ATTACK)
    {
      // Correct the actor's translation to move along it's local (rotated) axis
      float offsetX = _offset * sin((PI*m_yaw)/180);
      float offsetZ = _offset * cos((PI*m_yaw)/180);

      m_pos.m_x += offsetX;
      m_pos.m_z += offsetZ;

      m_bbox.move(Vec4(offsetX, 0, offsetZ));
    }
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

  //std::cout << m_meshBody.timesLooped() << "\n";

  std::cout << m_state << "\n";

  m_previousState = m_state;
}

void Actor::onAttack()
{
  if(m_state == ATTACK  && m_meshBody.timesLooped() >= 1) { m_state = IDLE; }
}

void Actor::onDamage()
{
  if(m_state == PAIN  && m_meshBody.timesLooped() >= 1) { m_state = IDLE; }
}

void Actor::onDeath()
{
  if(m_state == DYING && m_meshBody.timesLooped() >= 1)
  {
    m_state = DEAD;
  }
  else
  {
    forceAnimation(Md2::Animation::DEATH);
  }
}

void Actor::damage(int _value)
{
  std::cout << m_health << "\n";

  if(m_health > 0)
  {
    m_health -= _value;
    m_state = PAIN;
  }
  else
  {
    m_state = DYING;
  }
}


}
