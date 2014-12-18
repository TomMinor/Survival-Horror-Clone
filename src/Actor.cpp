#include "Actor.h"
#include "GLFunctions.h"

#include "3dUtilities.h"
#include <iostream>

namespace Game {

Actor::Actor(const Vec4 & _scale, const Vec4 & _pos )
  : m_time(0), m_pos(_pos), m_scale(_scale), m_yaw(0), m_health(100),
    m_meshBody("assets/actor/mach-body.md2", "assets/actor/soldier.jpg", 0.05f),
    m_meshHead("assets/actor/mach-head.md2", "assets/actor/mach-head.jpg", 0.05f),
    m_state(IDLE), m_previousState(IDLE),
    m_bbox(_pos, _scale)
{
  m_meshBody.setAnimation(Md2::Animation::STAND);
  m_meshHead.setAnimation(Md2::Animation::STAND);
}

void Actor::draw() const
{
  glPushMatrix();
    m_pos.translateGL();
    glPushMatrix();
      glRotatef(m_yaw, 0, 1, 0);
      m_meshBody.drawMesh();
      m_meshHead.drawMesh();
      util::drawWorldAxis(1.25);
    glPopMatrix();
  glPopMatrix();

  glColor3f( 0.0f, 0.25f, 1.0f );
  m_bbox.draw();

  glColor3f( 1.0f, 1.0f, 1.0f );
}

void Actor::move(float _offset, float _deg)
{
  if(m_state != PAIN && m_state != DYING && m_state != DEAD)
  {
    // Only set the walk animation when the player moves
    if(_offset != 0 || _deg != 0) { m_state = WALK; }
    else                          { m_state = IDLE; }

    m_yaw += _deg;

    //std::cout << m_yaw << "\n";
    // Z
//    if(m_yaw < 0) { m_yaw = 0; }
//    if(m_yaw > 180) { m_yaw = 180; }

    // -Z
//    if(m_yaw > 0) { m_yaw = 0; }
//    if(m_yaw < -180) { m_yaw = -180; }

    // X
//    if(m_yaw < 90) { m_yaw = 90; }
//    if(m_yaw > 270) { m_yaw = 270; }

    // -X
//    if(m_yaw > 90) { m_yaw = 90; }
//    if(m_yaw < 270) { m_yaw = 270; }


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

void Actor::setPosition(const Vec4& _pos)
{
  m_bbox.move(_pos-m_pos);
  m_pos = _pos;
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

  m_meshBody.updateAnimation(m_time*0.05f);
  m_meshHead.updateAnimation(m_time*0.05f);

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
