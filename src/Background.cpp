#include "Background.h"
#include <iostream>

namespace Game {

void Background::drawBG() const
{

}

void Background::drawFG() const
{

}

void Background::drawTrigger() const
{

  m_triggerColour.colourGL();
  m_triggerVolume.draw();
}

}
