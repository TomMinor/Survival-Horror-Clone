#include "Camera.h"
#include "GLinc.h"
#include <iostream>

namespace Game {

Camera::Camera(const Vec4 &_pos, const Vec4 &_rotation, float _fov)
  : m_viewMatrix( Mat4() ), m_fov(_fov)
{
  setTransform(_pos, _rotation);
}

void Camera::setTransform(const Vec4 &_pos, const Vec4 &_rotation )
{
  m_viewMatrix.identity();

  // Rotation
  Mat4 tmpx(m_viewMatrix); tmpx.rotateX( _rotation.m_roll );
  Mat4 tmpy(m_viewMatrix); tmpy.rotateY( _rotation.m_pitch );
  Mat4 tmpz(m_viewMatrix); tmpz.rotateZ( _rotation.m_yaw );
  m_viewMatrix *= tmpx;
  m_viewMatrix *= tmpy;
  m_viewMatrix *= tmpz;

  // Translate
  m_viewMatrix.m_m[3][0] = _pos.m_x;
  m_viewMatrix.m_m[3][1] = _pos.m_y;
  m_viewMatrix.m_m[3][2] = _pos.m_z;

  setView();
}

void Camera::setView() const
{
    m_viewMatrix.loadModelView();
}

}
