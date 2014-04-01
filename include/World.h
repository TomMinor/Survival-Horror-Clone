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
  float m_playerOffset;
  float m_playerYaw;
  std::vector<Room> m_rooms;
  int m_currentRoom;
  double m_lastTime;

  std::string m_assetPath;

public:
  World(const Vec4& _playerSpawn = Vec4()) :
    m_init(false), m_debugBBoxDraw(true), m_player(Vec4(1,2,1), Vec4(_playerSpawn)),
    m_lastTime(0), m_currentRoom(0)
  {;}
  bool init(const std::string& _assetpath = "assets/");
  void draw();
  void update();
  void changeRoom(const Room & _nextRoom) const;

  // Input - bit of a crappy interface but i need to sort everything else out (BUG)
  void playerWalk(float _offset);
  void playerTurn(float _deg)     { m_playerYaw = _deg;   }
  void playerDash()               { if(m_playerOffset > 0) { m_playerOffset *= 2; }  }

  inline void toggleBBox() { m_debugBBoxDraw ^= 1; }

  inline void updateTime() { m_lastTime = SDL_GetTicks(); }
  inline double getLastTime() const { return m_lastTime; }
  inline double getCurrentTime() const { return SDL_GetTicks(); }
  inline double getElapsedTime() const { return SDL_GetTicks() - m_lastTime; }

  void nextRoom() { if(m_currentRoom < m_rooms.size()) m_currentRoom=1; }
  void prevRoom() { if(m_currentRoom > 0) --m_currentRoom; }

  void damagePlayer(int _val) { m_player.damage(_val); }

private:
  void loadRooms();
  bool loadRoom(const std::string& _fileName);


};

}
#endif // WORLD_H
