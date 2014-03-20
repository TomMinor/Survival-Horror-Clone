#include "BoundingBox.h"

namespace Game {

BBox::BBox( float _xmin, float _ymin, float _zmin,
            float _xmax, float _ymax, float _zmax,
            const Vec4& _offset)
{
  m_xmin = _xmin + _offset.m_x;    m_xmax = _xmax + _offset.m_x;
  m_ymin = _ymin + _offset.m_y;    m_ymax = _ymax + _offset.m_y;
  m_zmin = _zmin + _offset.m_z;    m_zmax = _zmax + _offset.m_z;
}

bool BBox::checkCollision(const BBox & _b) const
{
  return ( _b.m_xmax );
}

void BBox::move(Vec4 _pos)
{

}

std::ostream & operator<<(std::ostream &_output, const BBox &_b)
{
  _output << "Min x: " << _b.m_xmin
        << "\nMin y: " << _b.m_ymin
        << "\nMin z: " << _b.m_zmin
        << "\nMax x: " << _b.m_xmax
        << "\nMax y: " << _b.m_ymax
        << "\nMax z: " << _b.m_zmax;

  return _output;
}

}

