#ifndef ACTOR_H
#define ACTOR_H
#include <vector>
#include "Vec4.h"
#include "BoundingBox.h"
#include "md2.h"

namespace Game {

class Actor
{
public:
  Actor(const Vec4 & _scale, const Vec4 & _pos )
    : m_time(0), m_pos(_pos), m_scale(_scale), m_yaw(0), m_state(IDLE),
      m_meshBody("assets/actor/mach-body.md2", "assets/actor/soldier.jpg", 0.05f),
      m_meshHead("assets/actor/mach-head.md2", "assets/actor/mach-head.jpg", 0.05f),
      m_bbox( _pos.m_x, _pos.m_y, _pos.m_z,
              _pos.m_x + _scale.m_x,
              _pos.m_y + _scale.m_y,
              _pos.m_z + _scale.m_z)
  {
    m_meshBody.setAnimation(MeshMd2::WALK);
    m_meshHead.setAnimation(MeshMd2::WALK);
  }

  const BBox& getBoundingBox() const { return m_bbox; }
  void draw();
  void update();
  void move(float _offset, float _deg=0.0f);

private:
  //std::vector<action> m_state;
  int m_time;
  Vec4 m_pos;
  Vec4 m_scale;
  float m_yaw;

  MeshMd2 m_meshBody;
  MeshMd2 m_meshHead;

  BBox m_bbox;

  enum {
    IDLE,
    WALK,
    DASH,
    PREPARE_ATTACK,
    ATTACK,
    PAIN
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
