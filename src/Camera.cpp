#include "Camera.h"

namespace Game {

Camera::Camera(const Vec4 &_pos, float _pitch, float _yaw, float _roll, float _fov)
  : m_viewMatrix( Mat4() ), m_fov(_fov)
{
  setTransform(_pos, _pitch, _yaw, _roll);
}

void Camera::setTransform(const Vec4 &_pos, float _pitch, float _yaw, float _roll )
{
  m_viewMatrix.rotateX( _roll );
  m_viewMatrix.rotateX( _pitch );
  m_viewMatrix.rotateZ( _yaw );

  m_viewMatrix = m_viewMatrix * _pos;
}

}
