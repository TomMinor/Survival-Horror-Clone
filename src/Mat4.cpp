#include "GLinc.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

#include "Mat4.h"

Mat4::Mat4(float _s)
{
  memset(&m_m, 0, sizeof(m_m)); // initialise matrix to be 0's

  // Set diagonals
  m_00 = _s;
  m_11 = _s;
  m_22 = _s;
  m_33 = _s;
}

Mat4::Mat4(const Mat4 &_r)
{
  //memcpy(m_m, &_r.m_m, sizeof(m_m));
  *this = _r;
}

void Mat4::identity()
{
  memset(&m_m, 0, sizeof(m_m));

  m_00 = 1.0f;
  m_11 = 1.0f;
  m_22 = 1.0f;
  m_33 = 1.0f;
}

void Mat4::transpose()
{
  Mat4 copy(*this);

  for(int x=0; x<4; x++)
  {
    for(int y=0; y<4; y++)
    {
      m_m[x][y] = copy.m_m[y][x];
    }
  }
}

void Mat4::rotate(Vec4 _rotation)
{

}

void Mat4::rotateX(float _deg)
{
  float beta = (_deg/180.0f) * M_PI;
  float sr = sin(beta);
  float cr = cos(beta);
  identity();

  m_11 = cr; m_21 = -sr;
  m_12 = sr; m_22 = cr;
}

void Mat4::rotateY(float _deg)
{
  float beta = (_deg/180.0f) * M_PI;
  float sr = sin(beta);
  float cr = cos(beta);
  identity();

  m_00 = cr;  m_20 = sr;
  m_02 = -sr; m_22 = cr;
}

void Mat4::rotateZ(float _deg)
{
  float beta = (_deg/180.0f) * M_PI;
  float sr = sin(beta);
  float cr = cos(beta);
  identity();

  m_00 = cr; m_10 = -sr;
  m_01 = sr; m_11 = cr;
}

void Mat4::loadModelView() const
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMultMatrixf(&m_openGL[0]);
}

void Mat4::loadProjection() const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMultMatrixf(&m_openGL[0]);
}


Mat4 Mat4::operator *(Vec4 _rhs) const
{
  Mat4 tmp(*this);
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
    {
      tmp.m_m[i][j] *= _rhs.m_openGL[i];
    }
  }
  return tmp;
}

void Mat4::operator *=(Vec4 _rhs)
{
  for(int i=0; i<4; ++i)
  {
    for(int j=0; j<4; ++j)
    {
      m_m[j][i] *= _rhs.m_openGL[j];
    }
  }
}

Mat4 Mat4::operator *(Mat4 _rhs) const
{

}

void Mat4::operator *=(Mat4 _rhs)
{

}

Mat4 Mat4::operator -(const Mat4 &_rhs) const
{
  Mat4 tmp(*this);
  for(int i=0; i<16; ++i)
  {
    tmp.m_openGL[i] -= _rhs.m_openGL[i];
  }
  return tmp;
}

void Mat4::operator -=(const Mat4 &_rhs)
{
  for(int i=0; i<16; ++i)
  {
    m_openGL[i] -= _rhs.m_openGL[i];
  }
}

Mat4 Mat4::operator+(const Mat4 &_rhs) const
{
  Mat4 tmp(*this);
  for(int i=0; i<16; ++i)
  {
    tmp.m_openGL[i] += _rhs.m_openGL[i];
  }
  return tmp;
}

void Mat4::operator+=(const Mat4 &_rhs)
{
  for(int i=0; i<16; ++i)
  {
    m_openGL[i] += _rhs.m_openGL[i];
  }
}

bool Mat4::operator==(const Mat4 &_rhs) const
{
  for(int i=0; i<16; ++i)
  {
    if(m_openGL[i] != _rhs.m_openGL[i]) { return false; }
  }
  return true;
}

void Mat4::operator=(const Mat4 &_rhs) const
{
  // Baaaaaaaaaaaaaaaaaaaad, I think?
  memcpy(const_cast<float*>(m_openGL),
         const_cast<float*>(_rhs.m_openGL), sizeof(m_openGL));
}


std::ostream & operator<<(std::ostream &_output, const Mat4 &_v)
{
  std::cout.setf(std::ios::fixed |
                 std::ios::adjustfield |
                 std::ios::showpos
                 );

  std::cout.precision(std::numeric_limits<double>::digits10+1);

  return _output
      <<"["<<_v.m_00<<","<<_v.m_01<<","<<_v.m_02<<","<<_v.m_03<<"]\n"
      <<"["<<_v.m_10<<","<<_v.m_11<<","<<_v.m_12<<","<<_v.m_13<<"]\n"
      <<"["<<_v.m_20<<","<<_v.m_21<<","<<_v.m_22<<","<<_v.m_23<<"]\n"
      <<"["<<_v.m_30<<","<<_v.m_31<<","<<_v.m_32<<","<<_v.m_33<<"]\n";

}
