#include "GLinc.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

namespace Game {

World::World() :
  m_init(false),
  m_currentRoom(NULL), m_lastTime(0), m_playerYaw(0), m_playerOffset(0),
  m_player(Vec4(1,2,1), Vec4(0.0f, 0.0f, 0.0f))
  {
    std::cout << "Loading assets :" <<  FileSystem().assetFolder() << "\n";

    if(!loadRoom("ROOM_01.room"))
    {
      throw std::runtime_error("Error loading assets");
    }

    updateTime();
  }


// Draw actors and room
void World::draw() const
{
  m_currentRoom->draw();
  drawActors();
  m_currentRoom->drawForeground();
}

void World::drawActors() const
{
  m_player.draw();

  // Draw enemies here too
}

// Update actor states
void World::update()
{
  updateTime();

  m_player.move(m_playerOffset, m_playerYaw);

  m_player.update();

  m_currentRoom->updateCurrentBackground(m_player.getBoundingBox());

//  if(!m_currentRoom->checkWallCollide(tmp))
//  {
//    m_player.move(m_playerOffset, m_playerYaw);
//  }
//  else
//  {
//  m_player.move(-0.01, m_playerYaw);
//  }

  m_playerOffset = m_playerYaw = 0; // Reset movement
}

void World::playerWalk(float _offset)
{
    m_playerOffset = _offset;
}

bool World::loadRoom(const std::string& _fileName)
{
  Room* nextRoom = 0;

  try
  {
    nextRoom = RoomReader(_fileName).load();
  }
  catch(std::ios_base::failure &msg)
  {
    std::cerr << msg.what() << "\n";
    nextRoom = 0;
  }
  catch(std::runtime_error &msg)
  {
    std::cerr << msg.what() << "\n";
    nextRoom = 0;
  }
  catch(...)
  {
    nextRoom = 0;
  }

  if(nextRoom)
  {
    delete m_currentRoom;
    m_currentRoom = nextRoom;
  }

  if(!m_currentRoom || !nextRoom)
  {
    std::cout << "room load failed\n";
    return false;
  }

  std::cout << "room load success\n";
  return true;
}

}
