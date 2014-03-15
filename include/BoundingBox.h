#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vec4.h"

class BBox
{
public:
  BBox( float _xmin, float _ymin, float _zmin,
        float _xmax, float _ymax, float _zmax, ) :
    m_xmin(_xmin), m_xmin(_ymin), m_xmin(_zmin),
    m_xmax(_xmax), m_ymax(_ymax), m_zmax(_zmax) {;};

  bool checkCollision(const BBox & _b) const;
  void move(Vec4 _pos);

private:
  union
  {
    struct {
      float m_xmin;
      float m_ymin;
      float m_zmin;

      float m_xmax;
      float m_ymax;
      float m_zmax;
    };

    float m_corners[6];
  };
};

#endif // BOUNDINGBOX_H
