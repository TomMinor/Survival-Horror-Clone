#include "World.h"
#include "SDL2/SDL.h"

World::World()
{
  m_isInit = false;
}

bool World::init()
{

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
