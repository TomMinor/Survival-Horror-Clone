#include "World.h"
#include "SDL2/SDL.h"

namespace Game {

bool World::m_isInit = false;

World::World()
  : m_player(Vec4(5,5,5), Vec4()),
    m_currentTime(0), m_lastTime(0)
{
  // Read assets
  //    Store in m_rooms
  ;
}

void World::draw()
{

}


void World::update()
{

}

void World::updateTime()
{
  m_currentTime = SDL_GetTicks();
}

}
