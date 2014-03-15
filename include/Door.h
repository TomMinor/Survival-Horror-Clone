#ifndef DOOR_H
#define DOOR_H

#include "Room.h"

class Door
{
public:
  Door();
  void playAnimation() const;
  inline Room *getNextRoom() const;

private:
  Room *m_nextRoom;

};

#endif // DOOR_H
