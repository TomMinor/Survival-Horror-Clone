#ifndef WORLD_H
#define WORLD_H
#include <SDL.h>
#include <vector>
#include "Actor.h"
#include "Room.h"

namespace Game {

class World
{
public:
  //BUG: Make private copy/assignment constructors

  // We only want 1 instance of World
  static World& instance()
  {
    static World m_instance;
    return m_instance;
  }

  bool init(const std::string& _assetpath = "assets/");
  void draw() const;
  void update();
  void changeRoom(const Room & _nextRoom) const;

  void toggleBBox() { m_debugBBoxDraw ^= 1; }

  inline void updateTime() { m_lastTime = SDL_GetTicks(); }
  inline double getLastTime() const { return m_lastTime; }
  inline double getCurrentTime() const { return SDL_GetTicks(); }

private:
  World() :
    m_player(Vec4(2,5,2), Vec4()), m_currentTime(0), m_lastTime(0), m_debugBBoxDraw(true)
  {;}

  void loadRooms();
  bool loadRoom(const std::string& _fileName);

  bool m_debugBBoxDraw;

  Actor m_player;
  std::vector<Room> m_rooms;
  double m_currentTime;
  double m_lastTime;

  std::string m_assetPath;
};

}
#endif // WORLD_H
