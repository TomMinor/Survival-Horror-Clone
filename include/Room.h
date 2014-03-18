#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include "Background.h"
#include "BoundingBox.h"
#include "Door.h"

namespace Game {

// Forward declaration of Door
class Door;

class Room
{
public:
  Room();
  bool checkWallCollide(BBox _player) const;
  void draw() const;
  void updatePlayer();

private:
  Vec4 m_playerSpawn;
  std::vector<BBox> m_collisionBoxes;
  std::vector<Background> m_backgrounds;
  std::vector<Door> m_exits;
};


}

#endif // ROOM_H
