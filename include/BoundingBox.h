#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec4.h"

namespace Game {

class BBox
{
public:
  BBox( float _xmin=-2.0f, float _ymin=-2.0f, float _zmin=-2.0f,
        float _xmax=2.0f, float _ymax=2.0f, float _zmax=2.0f) :
    m_xmin(_xmin), m_ymin(_ymin), m_zmin(_zmin),
    m_xmax(_xmax), m_ymax(_ymax), m_zmax(_zmax), m_offset(Vec4())
  {;}

  bool checkCollision(const BBox& _b) const;
  void move(Vec4 _pos);
  void draw() const;
  Vec4 getCenter() const;
  Vec4 getCollisionNormal(const BBox& _b);

  friend std::ostream & operator<<(std::ostream &, const BBox &);
private:
  float m_xmin;
  float m_ymin;
  float m_zmin;

  float m_xmax;
  float m_ymax;
  float m_zmax;

  Vec4 m_offset;
};

std::ostream & operator<<(std::ostream &_output, const BBox &_b);

}
#endif // BOUNDINGBOX_H
