#include "GLinc.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

namespace Game {

World::World(const std::string& _firstRoom) :
  m_currentRoom(NULL), m_lastTime(0),
  m_player( Vec4(1.0f, 2.0f, 1.0f), Vec4() ), m_playerOffset(0), m_playerYaw(0)
  {
    std::cout << "Loading assets :" <<  FileSystem().assetFolder() << "\n";

    if(!loadRoom(_firstRoom))
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
//  if(m_playerYaw > 1.0f)
//  {
//    m_player.move(0, -m_playerYaw);
//    m_player.move(Vec4(0, 0,m_playerOffset));
//  }

  m_player.update();

  m_currentRoom->updateCurrentBackground(m_player.getBoundingBox());

//  if(!m_currentRoom->checkWallCollide(tmp))
//  {
//    m_player.move(m_playerOffset, m_playerYaw);
//  }
//  else
//  {
//    m_player.move(-0.01, m_playerYaw);
//  }

  m_playerOffset = m_playerYaw = 0; // Reset movement
}

void World::playerWalk(float _offset)
{
    m_playerOffset = _offset;
}

/**
 * @brief World::loadRoom
 * Loads the specified '*.room' into the World's current active Room
 * and will attempt to handle common file handling exceptions if loading fails.
 * World's currently active Room will not be modified if the room could not be loaded
 * correctly.
 *
 * Exceptions will be automatically echoed to std::cerr
 * @param _fileName The file containing room data
 * @return True if the World's current active Room was updated, false if the file was not
 * loaded for some reason.
 */
bool World::loadRoom(const std::string& _fileName)
{
  Room* nextRoom = 0;

  try
  {
    nextRoom = RoomReader(_fileName).load();
  }
  catch(const std::invalid_argument& msg)
  {
    std::cerr << msg.what() << "\n";
    nextRoom = 0;
  }
  catch(const std::runtime_error& msg)
  {
    std::cerr << msg.what() << "\n";
    nextRoom = 0;
  }

  if(nextRoom)
  {
    delete m_currentRoom;
    m_currentRoom = nextRoom;
    m_player.setPosition(m_currentRoom->playerSpawn());
  }

  if(!m_currentRoom || !nextRoom)
  {
    std::cerr << "Malformed room file, not loading\n";
    return false;
  }

  return true;
}

}
