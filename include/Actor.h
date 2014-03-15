#ifndef ACTOR_H
#define ACTOR_H
#include "Vec4.h"
#include "BoundingBox.h"

class Actor
{
public:
  Actor( Vec4 _size, Vec4 _pos ) : m_pos(_pos)
  {
    m_yaw = 0;
    m_bbox = new BBox( _pos.m_x, _pos.m_y, _pos.m_z,
                       _pos.m_x + _size.m_x,
                       _pos.m_y + _size.m_y,
                       _pos.m_z + _size.m_z);
  }
  BBox getBoundingBox() const;
  void draw() const;
  void move(Vec4 _offset);

private:
  Vec4 m_pos;
  float m_yaw;
  BBox m_bbox;
};

#endif // PLAYER_H
