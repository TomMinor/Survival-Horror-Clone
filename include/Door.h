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
  Door(const std::string& _nextRoom, const Vec4& _position, const Vec4& _triggerVolume = Vec4(2.0f, 2.0f, 2.0f)):
    m_position(_position), m_triggerSize(_triggerVolume), m_nextRoom(_nextRoom), m_time(0)
  {;}

  void displayLoadingScreen();
  inline std::string getNextRoom() const { return m_nextRoom; }

private:
  Vec4 m_position;
  Vec4 m_triggerSize;

  int m_time;

  std::string m_nextRoom; // The name of the file to load when this door is triggered
};

}

#endif // DOOR_H
