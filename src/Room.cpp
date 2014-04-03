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
  // ----- Debug BBox drawing ----------------

  // Draw room bounds
  for(std::vector<BBox>::const_iterator bbox = m_collisionBoxes.begin();
      bbox!= m_collisionBoxes.end(); ++bbox)
  {
    glColor3f( 0.0f, 0.25f, 0.0f );
    bbox->draw();
  }

  // Draw background triggers
  for(std::vector<Background>::const_iterator background = m_backgrounds.begin();
      background!= m_backgrounds.end(); ++background)
  {
    background->drawTrigger();
  }

  m_backgrounds[0].drawBG();
}

void Room::drawFG() const
{
  m_backgrounds[0].drawFG();
}

void Room::updatePlayer()
{

}

}
