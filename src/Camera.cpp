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

void Camera::setView() const
{
  m_viewMatrix.loadModelView();
}

void Camera::draw() const
{
    std::cout << m_viewMatrix << "\n";

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
