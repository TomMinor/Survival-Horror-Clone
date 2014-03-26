#include "Room.h"
#include <iterator>

namespace Game {

bool Room::checkWallCollide(const BBox& _actor)
{
  for(std::vector<BBox>::iterator bound = m_collisionBoxes.begin();
      bound != m_collisionBoxes.end();
      bound++ )
  {
    if(bound->checkCollision(_actor))
    {
      m_collidingBBox = &(*bound);
      return true;
    }
  }

  return false;
}

void Room::draw() const
{

}

void Room::debugDrawBounds() const
{
  for(std::vector<BBox>::const_iterator bbox = m_collisionBoxes.begin();
      bbox!= m_collisionBoxes.end(); ++bbox)
  {
      bbox->draw();
  }
}

void Room::updatePlayer()
{

}

}
