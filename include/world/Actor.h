#ifndef ACTOR_H
#define ACTOR_H
#include <vector>
#include "Vec4.h"
#include "BoundingBox.h"
#include "md2mesh.h"

class Actor
{
public:
  Actor(const Vec4 & _scale, const Vec4 & _pos );

  const BBox& getBoundingBox() const { return m_bbox; }
  void draw() const;
  void update();
  void damage(int _value);
  void move(float _offset, float _deg);

  void forceAnimation(Md2::Animation::Sequence _type) { m_meshBody.setAnimation(_type);
                                                        m_meshHead.setAnimation(_type); }

  void setPosition(const Vec4& _pos);
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

  enum
  {
    IDLE, WALK, DASH, PREPARE_ATTACK, ATTACK, PAIN, DYING, DEAD
  } m_state, m_previousState;

  BBox m_bbox;
};

class Controller
{
public:
  Controller();
private:

};

#endif // PLAYER_H
