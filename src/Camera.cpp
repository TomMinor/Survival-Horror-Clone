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

  m_viewMatrix.m_30 = _pos.m_x;
  m_viewMatrix.m_31 = _pos.m_y;
  m_viewMatrix.m_32 = _pos.m_z;

  m_viewMatrix.loadModelView();

  // BUG: Implement matrix operators again
  //m_viewMatrix = m_viewMatrix * _pos;
}

}
