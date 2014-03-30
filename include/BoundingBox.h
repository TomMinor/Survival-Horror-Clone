#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec4.h"

namespace Game {

class BBox
{
public:
  BBox( float _xmin=-1.0f, float _ymin=-1.0f, float _zmin=-1.0f,
        float _xmax= 1.0f, float _ymax= 1.0f, float _zmax= 1.0f) :
    m_min(_xmin, _ymin, _zmin ), m_max(_xmax, _ymax, _zmax )
  {;}

  void move(Vec4 _pos);
  void draw() const;
  Vec4 getCenter() const;
  Vec4 getCollisionNormal(const BBox& _b);
  bool checkCollision(const BBox& _b) const;

  friend std::ostream & operator<<(std::ostream &, const BBox &);
private:
  Vec4 m_min;
  Vec4 m_max;
};

std::ostream & operator<<(std::ostream &_output, const BBox &_b);

}
#endif // BOUNDINGBOX_H
