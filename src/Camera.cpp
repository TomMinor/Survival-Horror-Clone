#include "Camera.h"
#include "GLinc.h"
#include <iostream>

#include "3dUtilities.h"

#include <cstring>

namespace Game {

Camera::Camera(const Vec4 &_pos, const Vec4 &_rotation, float _fov)
  : m_viewMatrix( Mat4() ), m_fov(_fov)
  {
    setTransform(_pos, _rotation);
  }

void Camera::setTransform(const Vec4 &_pos, const Vec4 &_rotation )
{
  m_position = _pos;
  m_rotation = _rotation;

  m_viewMatrix.identity();

//  // Rotation
  m_viewMatrix.rotate(_rotation);

  // Translate
  Mat4 tmp;
  tmp.identity();
  tmp.m_m[3][0] = _pos.m_x;
  tmp.m_m[3][1] = _pos.m_y;
  tmp.m_m[3][2] = _pos.m_z;

//  tmp.m_m[0][3] *= -1;
//  tmp.m_m[3][0] *= -1;

//  tmp.m_m[1][3] *= -1;
//  tmp.m_m[3][1] *= -1;

  tmp *= m_viewMatrix;
  m_viewMatrix = tmp;

  setView();
}

Mat4 Camera::billboard(Vec4 _position, Vec4 _up) const
{
  Vec4 look = m_position - _position;
  Vec4 right = _up.cross(look);
  Vec4 up = look.cross(right);

  Mat4 rot;
  rot.m_m[0][0] = right.m_x;  rot.m_m[0][1] = up.m_x;  rot.m_m[0][1] = look.m_x;  rot.m_m[0][3] = _position.m_x;
  rot.m_m[1][0] = right.m_y;  rot.m_m[1][1] = up.m_y;  rot.m_m[1][1] = look.m_y;  rot.m_m[1][3] = _position.m_y;
  rot.m_m[2][0] = right.m_z;  rot.m_m[2][1] = up.m_z;  rot.m_m[2][1] = look.m_z;  rot.m_m[2][3] = _position.m_z;
  rot.m_m[3][0] = 0.0f;       rot.m_m[3][1] = 0.0f;    rot.m_m[3][1] = 0.0f;      rot.m_m[3][3] = 1.0f;

  return rot;
}

void Camera::setView() const
{
  m_viewMatrix.loadModelView();
}

void Camera::draw() const
{
//    std::cout << m_viewMatrix << "\n";

  glPushMatrix();
    // Center within bbox
//    Vec4 position( m_viewMatrix.m_m[3][0],
//                   m_viewMatrix.m_m[3][1],
//                   m_viewMatrix.m_m[3][2] );

//    position.translateGL();
    glMultMatrixf(m_viewMatrix.m_openGL);

    glPushMatrix();
      util::drawWorldAxis(1.25);
    glPopMatrix();
  glPopMatrix();
}

}
