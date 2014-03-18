#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "Actor.h"
#include "Room.h"

namespace Game {

class World
{
public:
  World();

  // We only want 1 instance of World
  static World* init()
  {
    if(!m_isInit) m_instance = new World;
    return m_instance;
  }
  void draw();
  void update();
  void updateTime();
  inline void getLastTime();
  void changeRoom(const Room & _nextRoom) const;

private:
  void loadRooms();

  static World* m_instance;
  static bool m_isInit;
  Actor m_player;
  std::vector<Room> m_rooms;
  double m_currentTime;
  double m_lastTime;
};

}
#endif // WORLD_H
