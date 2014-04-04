#ifndef DOOR_H
#define DOOR_H

#include <iostream>
#include "Room.h"
#include "Vec4.h"

namespace Game {

class Room;

class Door
{
public:
  Door(const std::string& _nextRoom, const Vec4& _position, const Vec4& _triggerVolume = Vec4(1.0f, 1.5f, 1.0f)) :
    m_trigger(_position, _triggerVolume), m_time(0), m_nextRoom(_nextRoom)
  {;}

  void displayLoadingScreen();
  void drawVolume() const;
  bool canTrigger(const BBox& _bounds) const;
  inline std::string getNextRoom() const { return m_nextRoom; }

private:
  BBox m_trigger;

  int m_time;

  std::string m_nextRoom; // The name of the file to load when this door is triggered
};

}

#endif // DOOR_H
