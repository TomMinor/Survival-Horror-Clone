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
    m_name(_name), m_playerSpawn(_spawnPos),
    m_collisionBoxes(_bboxes), m_backgrounds(_backgrounds), m_exits(_exits), m_currentBG(0)
  {;}

  bool checkWallCollide(const BBox& _actor);
  Vec4 closestCenter(const Vec4& _b);

  void updateCurrentBackground(const BBox& _actor);

  void draw() const;
  void drawForeground() const;
  void debugDrawBounds() const;

private:
  int m_currentBG;  // The currently active background
  //int m_lastBG;     // Prevent the player from
  std::string m_name;
  Vec4 m_playerSpawn;
  //BBox* m_collidingBBox;  // Points to the last room bound to collide
  std::vector<BBox> m_collisionBoxes;

  std::vector<Background> m_backgrounds;
  std::vector<Door> m_exits;
};


}

#endif // ROOM_H
