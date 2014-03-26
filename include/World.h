#ifndef WORLD_H
#define WORLD_H
#include <SDL.h>
#include <vector>
#include "Actor.h"
#include "Room.h"

namespace Game {

class World
{
private:
  bool m_init;
  bool m_debugBBoxDraw;

  Actor m_player;
  std::vector<Room> m_rooms;
  int m_currentRoom;
  double m_currentTime;
  double m_lastTime;

  std::string m_assetPath;

public:
  World(const Vec4& _playerSpawn = Vec4()) :
    m_init(false), m_debugBBoxDraw(true), m_player(Vec4(2,2,2), Vec4(_playerSpawn)),
    m_currentTime(0), m_lastTime(0), m_currentRoom(0)
  {;}
  bool init(const std::string& _assetpath = "assets/");
  void draw() const;
  void update();
  void changeRoom(const Room & _nextRoom) const;

  inline void toggleBBox() { m_debugBBoxDraw ^= 1; }

  inline void updateTime() { m_lastTime = SDL_GetTicks(); }
  inline double getLastTime() const { return m_lastTime; }
  inline double getCurrentTime() const { return SDL_GetTicks(); }
  inline double getElapsedTime() const { return SDL_GetTicks() - m_lastTime; }

private:
  void loadRooms();
  bool loadRoom(const std::string& _fileName);


};

}
#endif // WORLD_H
