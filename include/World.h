#ifndef WORLD_H
#define WORLD_H
#include <SDL.h>
#include <vector>
#include "Actor.h"
#include "Room.h"
#include "FileSystem.h"


class World
{
private:
  //----World---------
  Room* m_currentRoom;
  double m_lastTime;

  //----Actors---------
  Actor m_player;
  float m_playerOffset;
  float m_playerYaw;

public:
  World(const std::string& _firstRoom = "ROOM_01.room");

  bool init();

  void draw() const;
  void drawActors() const;
  void update();
  void changeRoom(const std::string& _nextRoom);

  // Input - bit of a crappy interface but i need to sort everything else out
  void playerWalk(float _offset);
  void playerTurn(float _deg)     { m_playerYaw = _deg;   }
  void playerDash()               { if(m_playerOffset > 0) { m_playerOffset *= 2; }  }
  void damagePlayer(int _val)     { m_player.damage(_val); }

  //------Time Functions-------------
  inline void updateTime() { m_lastTime = SDL_GetTicks(); }
  inline double getLastTime() const { return m_lastTime; }
  inline double getCurrentTime() const { return SDL_GetTicks(); }
  inline double getElapsedTime() const { return SDL_GetTicks() - m_lastTime; }

  bool loadRoom(const std::string& _fileName);

private:
  //bool loadRoom(const std::string& _fileName);
};

#endif // WORLD_H
