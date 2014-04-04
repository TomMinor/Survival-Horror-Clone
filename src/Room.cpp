#include "Room.h"
#include <iterator>

namespace Game {

bool Room::checkWallCollide(const BBox& _actor)
{
  for(std::vector<BBox>::iterator bound = m_collisionBoxes.begin();
      bound != m_collisionBoxes.end();
      bound++ )
  {
    if(bound->checkIntersection(_actor))
    {
      //m_collidingBBox = &(*bound);
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

  // Draw exit volumes
  for(std::vector<Door>::const_iterator exitDoor = m_exits.begin();
      exitDoor!= m_exits.end(); ++exitDoor)
  {
    glColor3f( 1.0f, 0.0f, 0.0f );
    exitDoor->drawVolume();
  }

  m_backgrounds[m_currentBG].drawBG();
}

void Room::drawForeground() const
{
  m_backgrounds[m_currentBG].drawFG();
}

void Room::updateCurrentBackground(const BBox& _actor)
{
  for(std::vector<Background>::iterator bg = m_backgrounds.begin();
      bg != m_backgrounds.end();
      bg++ )
  {
    //Don't bother updating the view if the player triggers the current background again
    if(bg->touchesTrigger(_actor) )
    {
      m_currentBG = (bg - m_backgrounds.begin());
      // Only reload the background texture when the view changes
      if(m_lastBG != m_currentBG)
      {
        m_lastBG = m_currentBG;
        m_backgrounds[m_currentBG].loadBackgroundTexture();
        m_backgrounds[m_currentBG].setCameraView();
      }
      return;
    }
  }
}

}
