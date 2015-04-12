#include "Door.h"


void Door::displayLoadingScreen()
{
  m_time++;
}

void Door::drawVolume() const
{
  m_trigger.draw();
}

bool Door::canTrigger(const BBox& _bounds) const
{
  return m_trigger.checkIntersection(_bounds);
}
