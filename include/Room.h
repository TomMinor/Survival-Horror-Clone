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
  Room( const std::string& _name,
        const Vec4& _spawnPos,
        const std::vector<BBox>& _bboxes,
        const std::vector<Background>& _backgrounds,
        const std::vector<Door>& _exits ) :
    m_name(_name), m_playerSpawn(_spawnPos), m_collisionBoxes(_bboxes),
    m_backgrounds(_backgrounds), m_exits(_exits)
  {
    ;
  }

  bool checkWallCollide(BBox _player) const;
  void draw() const;
  void debugDrawBounds() const;
  void updatePlayer();

private:
  std::string m_name;
  Vec4 m_playerSpawn;
  std::vector<BBox> m_collisionBoxes;

  std::vector<Background> m_backgrounds;
  std::vector<Door> m_exits;
};


}

#endif // ROOM_H
