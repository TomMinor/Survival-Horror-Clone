#include "Room.h"
#include <iterator>

namespace Game {

bool Room::checkWallCollide(BBox _player) const
{

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
