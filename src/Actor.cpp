#include "Actor.h"

namespace Game {

void Actor::draw() const
{
  m_bbox.draw();
}

void Actor::move(Vec4 _offset)
{
  m_pos += _offset;
  m_bbox.move(_offset);
}

}
