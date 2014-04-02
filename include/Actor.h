#ifndef ACTOR_H
#define ACTOR_H
#include <vector>
#include "Vec4.h"
#include "BoundingBox.h"
#include "md2mesh.h"

namespace Game {

class Actor
{
public:
  Actor(const Vec4 & _scale, const Vec4 & _pos )
    : m_time(0), m_pos(_pos), m_scale(_scale), m_yaw(0), m_health(100),
      m_meshBody("assets/actor/mach-body.md2", "assets/actor/soldier.jpg", 0.05f),
      m_meshHead("assets/actor/mach-head.md2", "assets/actor/mach-head.jpg", 0.05f),
      m_previousState(IDLE), m_state(IDLE),
      m_bbox( _pos.m_x, _pos.m_y, _pos.m_z,
              _pos.m_x + _scale.m_x,
              _pos.m_y + _scale.m_y,
              _pos.m_z + _scale.m_z)
  {
    m_meshBody.setAnimation(Md2::Animation::STAND);
    m_meshHead.setAnimation(Md2::Animation::STAND);
  }

  const BBox& getBoundingBox() const { return m_bbox; }
  void draw() const;
  void update();
  void damage(int _value);
  void move(float _offset, float _deg=0.0f);

  void forceAnimation(Md2::Animation::Sequence _type) { m_meshBody.setAnimation(_type);
                                                        m_meshHead.setAnimation(_type); }

private:
  void onAttack();
  void onDamage();
  void onDeath();

  int m_time;
  Vec4 m_pos;
  Vec4 m_scale;
  float m_yaw;
  int m_health;

  Md2::Mesh m_meshBody;
  Md2::Mesh m_meshHead;

  BBox m_bbox;

  enum
  {
    IDLE, WALK, DASH, PREPARE_ATTACK, ATTACK, PAIN, DYING, DEAD
  } m_state, m_previousState;
};

class Controller
{
public:
  Controller();
private:

};

}
#endif // PLAYER_H
