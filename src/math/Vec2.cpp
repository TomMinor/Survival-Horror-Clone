#include "Vec2.h"

#include <cmath>
#include <cassert>

Vec2 Vec2::operator *(float _rhs) const
{
  return Vec2(m_x*_rhs, m_y*_rhs);
}

void Vec2::operator *=(float _rhs)
{
  m_x *= _rhs;
  m_y *= _rhs;
}

Vec2 Vec2::operator +(const Vec2 &_rhs) const
{
  return Vec2(m_x + _rhs.m_x, m_y + _rhs.m_y);
}

void Vec2::operator +=(const Vec2 &_rhs)
{
  m_x += _rhs.m_x;
  m_y += _rhs.m_y;
}

Vec2 Vec2::operator -(const Vec2 &_rhs) const
{
  return Vec2(m_x - _rhs.m_x, m_y - _rhs.m_y);
}

Vec2 Vec2::operator -() const
{
  return Vec2(-m_x, -m_y);
}

void Vec2::operator -=(const Vec2 &_rhs)
{
  m_x -= _rhs.m_x;
  m_y -= _rhs.m_y;
}

bool Vec2::operator==(const Vec2 &_rhs) const
{
  const static float delta = 0.0001f;

  return ( ((m_x-delta) < (_rhs.m_x)) && ((m_x+delta) > (_rhs.m_x)) ) &&
         ( ((m_y-delta) < (_rhs.m_y)) && ((m_y+delta) > (_rhs.m_y)) );
}

std::ostream & operator<<(std::ostream &_output, const Vec2 &_v)
{
  return _output << "[" << _v.m_x << "," << _v.m_y << "]\n";
}

void Vec2::print() const
{
  std::cout << *this;
}

float Vec2::length() const
{
  return sqrt(m_x*m_x + m_y*m_y);
}

float Vec2::lengthSquared() const
{
  return (m_x*m_x + m_y*m_y);
}

void Vec2::normalize()
{
  float l = length();
  assert(l != 0.0f);

  m_x /= l;
  m_y /= l;
}
void Vec2::set(float _x, float _y)
{
  m_x = _x;
  m_y = _y;
}

