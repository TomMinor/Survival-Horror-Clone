#include "GLinc.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

namespace Game {

bool World::init(const std::string& _assetpath)
{
  if(!m_init)
  {
    // Read assets
    //    Store in m_rooms
    m_assetPath = _assetpath;
    std::cout << "Loading assets :" <<  m_assetPath << "\n";

    try
    {
      loadRooms();
    }
    catch(std::ios_base::failure &msg)
    {
      std::cerr << msg.what() << "\n";
      return false;
    }

    updateTime();
    m_init = true;
  }
  else
  {
    std::cerr << "World already loaded, not reinitialising\n";
  }

  return true;
}

// Draw actors and room
void World::draw() const
{
  // Draw room bounds for collision testing
  if(m_debugBBoxDraw)
  {
    m_rooms[m_currentRoom].debugDrawBounds();

//    for(std::vector<Room>::const_iterator room = m_rooms.begin();
//        room!= m_rooms.end(); ++room)
//    {
//        room->debugDrawBounds();
//    }
  }

  m_player.draw();
}

// Update actor states
void World::update()
{
  updateTime();

  BBox tmp(m_player.getBoundingBox());
  tmp.move(m_playerOffset*0.001);
  if(!m_rooms[m_currentRoom].checkWallCollide(tmp))
  {
    m_player.move(m_playerOffset, m_playerYaw);
  }
  else
  {
    m_player.move(0, m_playerYaw);
  }



  m_playerOffset = m_playerYaw = 0; // Reset movement
}

void World::playerWalk(float _offset)
{

    m_playerOffset = _offset;

}

void World::loadRooms()
{
  std::ifstream manifestFile;
  std::string path = m_assetPath + "manifest.bg";
  std::string line;

  manifestFile.open(path.c_str(), std::ios::in);
  if( manifestFile.is_open() )
  {
    while(std::getline(manifestFile, line))
    {
      try
      {
        RoomReader(line, m_rooms, m_assetPath).load();
      }
      catch(std::invalid_argument &msg)
      {
        std::cerr << msg.what() << "\n";
      }
      catch(std::runtime_error &msg)
      {
        std::cerr << "Failed loading file " << line << "\n" <<
                     "(Parse error) " << msg.what() << "\n";
      }
    }
    manifestFile.close();
  }
  else
  {
    // Throw an error
    throw std::ios_base::failure("Could not load manifest file\n");
  }


}

}
