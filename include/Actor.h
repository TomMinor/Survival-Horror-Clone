#ifndef ACTOR_H
#define ACTOR_H
#include "Vec4.h"
#include "BoundingBox.h"

namespace Game {

class Actor
{
public:
  Actor(const Vec4 & _scale, const Vec4 & _pos )
    : m_pos(_pos),
      m_scale(_scale),
      m_yaw(0),
      m_bbox( _pos.m_x, _pos.m_y, _pos.m_z,
              _pos.m_x + _scale.m_x,
              _pos.m_y + _scale.m_y,
              _pos.m_z + _scale.m_z) {;}

  inline BBox getBoundingBox() const { return m_bbox; }
  void draw() const;
  void move(float _offset, float _deg=0.0f);

private:
  Vec4 m_pos;
  Vec4 m_scale;
  float m_yaw;
  BBox m_bbox;
};

class Command
{
public:

};

}
#endif // PLAYER_H
