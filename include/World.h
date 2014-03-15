#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "Actor.h"
#include "Room.h"

class World
{
public:
  World();

  bool init();
  void draw();
  void update();
  void updateTime();
  inline void getLastTime();
  void changeRoom(const Room & _nextRoom) const;

private:
  void loadRooms();

  bool m_isInit;
  Actor m_player;
  std::vector<Room> m_rooms;
  double m_currentTime;
  double m_lastTime;
};

#endif // WORLD_H
