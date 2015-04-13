#include "GLinc.h"

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

#include "Item.h"
#include "ItemFactory.h"
#include "ItemParser.h"

World::World(const std::vector<ItemName> &_startingItems, const std::string& _firstRoom, bool _realTimeHud) :
  m_currentRoom(NULL), m_lastTime(0),
  m_player( new Actor(Vec4(1.0f, 2.0f, 1.0f), Vec4()) ), m_playerOffset(0), m_playerYaw(0),
  m_hud( new HUD() ), m_bRealTimeOverlayHud(_realTimeHud), m_bDrawHud(false)
  {
    std::cout << "Loading assets :" <<  FileSystem().assetFolder() << "\n";

    if(!loadRoom(_firstRoom))
    {
      throw std::runtime_error("Error loading first room");
    }

    ItemArray items;

    try
    {
        parseItemManifest("assets/items/items_manifest.txt", items);
    }
    catch(std::exception& e)
    {
        std::cerr << "Item Manifest Error : " << e.what() << std::endl;
    }

    for(auto item : _startingItems)
    {
      m_hud->m_inventory->addItem( createItem(item) );
    }

    updateTime();
  }


// Draw actors and room
void World::draw() const
{
  if(!m_bDrawHud || (m_bDrawHud && m_bRealTimeOverlayHud))
  {
    m_currentRoom->draw();
    drawActors();
    m_currentRoom->drawForeground();
  }

  if(m_bDrawHud)
  {
    if(!m_bRealTimeOverlayHud)
    {
      glClearColor(0.078, 0.137, 0.353,0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    m_hud->draw();
  }

}

void World::drawActors() const
{
  m_player->draw();

  // Draw enemies here too
}

// Update actor states
void World::update()
{
  m_time += 0.05f;
  updateTime();

  if(!m_bDrawHud || (m_bDrawHud && m_bRealTimeOverlayHud))
  {
    m_player->move(m_playerOffset, m_playerYaw);
  //  if(m_playerYaw > 1.0f)
  //  {
  //    m_player.move(0, -m_playerYaw);
  //    m_player.move(Vec4(0, 0,m_playerOffset));
  //  }

    m_player->update(m_time);

    m_currentRoom->updateCurrentBackground(m_player->getBoundingBox());

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

  if(m_bDrawHud)
  {
    m_hud->update(m_time);
  }
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
    m_player->setPosition(m_currentRoom->playerSpawn());
  }

  if(!m_currentRoom || !nextRoom)
  {
    std::cerr << "Malformed room file, not loading\n";
    return false;
  }

  return true;
}
