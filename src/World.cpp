#include "GLinc.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

namespace Game {

static bool m_init = false;

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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // now draw etc

  // Draw
  for(std::vector<Room>::const_iterator room = m_rooms.begin();
      room!= m_rooms.end(); ++room)
  {
      room->debugDrawBounds();
  }

}

// Update actor states
void World::update()
{

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
