#include "GLinc.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include "Vec4.h"


Vec4 Vec4::operator *(float _rhs) const
{
  return Vec4(m_x*_rhs, m_y*_rhs, m_z*_rhs, m_w);
}

void Vec4::operator *=(float _rhs)
{
  m_x *= _rhs;
  m_y *= _rhs;
  m_z *= _rhs;
}

Vec4 Vec4::operator +(const Vec4 &_rhs) const
{
  return Vec4(m_x + _rhs.m_x, m_y + _rhs.m_y, m_z + _rhs.m_z, 0.0f);
}

void Vec4::operator +=(const Vec4 &_rhs)
{
  m_x += _rhs.m_x;
  m_y += _rhs.m_y;
  m_z += _rhs.m_z;
}

Vec4 Vec4::operator -(const Vec4 &_rhs) const
{
  return Vec4(m_x - _rhs.m_x, m_y - _rhs.m_y, m_z - _rhs.m_z, 0.0f);
}

Vec4 Vec4::operator -() const
{
  return Vec4(-m_x, -m_y, -m_z);
}

void Vec4::operator -=(const Vec4 &_rhs)
{
  m_x -= _rhs.m_x;
  m_y -= _rhs.m_y;
  m_z -= _rhs.m_z;
}

bool Vec4::operator==(const Vec4 &_rhs) const
{
  const static float delta = 0.0001f;

  return ( ((m_x-delta) < (_rhs.m_x)) && ((m_x+delta) > (_rhs.m_x)) ) &&
         ( ((m_y-delta) < (_rhs.m_y)) && ((m_y+delta) > (_rhs.m_y)) ) &&
         ( ((m_z-delta) < (_rhs.m_z)) && ((m_z+delta) > (_rhs.m_z)) );
}

std::ostream & operator<<(std::ostream &_output, const Vec4 &_v)
{
  return _output << "[" << _v.m_x << "," << _v.m_y << "," << _v.m_z << "," << _v.m_w << "]\n";
}



void Vec4::print() const
{
  std::cout << "[" << m_x << "," << m_y << "," << m_z << "," << m_w << "]\n";
}

float Vec4::length() const
{
  return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

float Vec4::lengthSquared() const
{
  return (m_x*m_x + m_y*m_y + m_z*m_z);
}

void Vec4::normalize()
{
  float l = length();
  assert(l != 0.0f);

  m_x /= l;
  m_y /= l;
  m_z /= l;
}

float Vec4::dot(const Vec4 &_b) const
{
  return (m_x * _b.m_x) + (m_y * _b.m_y) + (m_z * _b.m_z);
}

float Vec4::dot(const Vec4 &_a, const Vec4 &_b) const
{
  return (_a.m_x * _b.m_x) + (_a.m_y * _b.m_y) + (_a.m_z * _b.m_z);
}

Vec4 Vec4::cross(const Vec4 &_b) const
{
  return Vec4(
               m_y*_b.m_z - m_z*_b.m_y,
               m_z*_b.m_x - m_x*_b.m_z,
               m_x*_b.m_y - m_y*_b.m_x,
               0.0f
             );
}

Vec4 Vec4::cross(const Vec4 &_a, const Vec4 &_b) const
{
  return Vec4(
               _a.m_y*_b.m_z - _a.m_z*_b.m_y,
               _a.m_z*_b.m_x - _a.m_x*_b.m_z,
               _a.m_x*_b.m_y - _a.m_y*_b.m_x,
               0.0f
               );
}

void Vec4::set(float _x, float _y, float _z, float _w)
{
  m_x = _x;
  m_y = _y;
  m_z = _z;
  m_w = _w;
}
