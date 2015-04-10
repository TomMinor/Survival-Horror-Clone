#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec4.h"

class BBox
{
public:
  enum Face
  {
    LEFT,  BACK,  BOTTOM, // Negative xyz axis
    RIGHT, FRONT, TOP,    // Positive xyz axis
    NONE
  };

  // Explicitly pass min/max
  BBox( float _xmin=-1.0f, float _ymin=-1.0f, float _zmin=-1.0f,
        float _xmax= 1.0f, float _ymax= 1.0f, float _zmax= 1.0f) :
    m_min(_xmin, _ymin, _zmin ), m_max(_xmax, _ymax, _zmax )
  {;}

  // Centre the bbox around the position, and set it's height to _size
  BBox(Vec4 _position, Vec4 _size) :
    m_min(_position.m_x - _size.m_x/2, _position.m_y,             _position.m_z - _size.m_z/2),
    m_max(_position.m_x + _size.m_x/2, _position.m_y + _size.m_y, _position.m_z + _size.m_z/2)
  {;}

  void move(Vec4 _pos);
  void draw() const;
  Vec4 getCenter() const;

  Face getTouchingFace(const BBox& _b) const;
  bool checkIntersection(const BBox& _b) const;
  bool checkIntersectOrTouch(const BBox& _b) const;
  bool checkTouch(const BBox& _b) const;
  Vec4 getIntersectOffset(const BBox& _b) const;

  static void toggleDebugDraw() { m_debugDraw ^= 1;}
  friend std::ostream & operator<<(std::ostream &, const BBox &);
private:
  // A flag to toggle drawing of all bounding boxes for debugging purposes
  static bool m_debugDraw;
  Vec4 m_min;
  Vec4 m_max;
};

std::ostream & operator<<(std::ostream &_output, const BBox &_b);

#endif // BOUNDINGBOX_H
