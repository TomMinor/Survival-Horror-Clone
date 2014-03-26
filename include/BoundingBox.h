#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec4.h"

namespace Game {

class BBox
{
public:
  BBox() {;}
  BBox( float _xmin, float _ymin, float _zmin,
        float _xmax, float _ymax, float _zmax) :
    m_xmin(_xmin), m_ymin(_ymin), m_zmin(_zmin),
    m_xmax(_xmax), m_ymax(_ymax), m_zmax(_zmax), m_offset(Vec4())
  {;}

  bool checkCollision(const BBox& _b) const;
  void move(Vec4 _pos);
  void draw() const;
  Vec4 intersectionAmount(const BBox& _b);

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
