#include "Camera.h"

namespace Game {

Camera::Camera(const Vec4 &_pos, const Vec4 &_rotation, float _fov)
  : m_viewMatrix( Mat4() ), m_fov(_fov)
{
  setTransform(_pos, _rotation);
}

void Camera::setTransform(const Vec4 &_pos, const Vec4 &_rotation )
{
  Mat4 tmp1(m_viewMatrix);
  Mat4 tmp2(m_viewMatrix);
  Mat4 tmp3(m_viewMatrix);

  tmp1.rotateZ( _rotation.m_yaw );
  tmp2.rotateY( _rotation.m_pitch );
  tmp3.rotateX( _rotation.m_roll );

  m_viewMatrix *= tmp1;


//  m_viewMatrix.rotateZ( _rotation.m_yaw );
//  m_viewMatrix.rotateY( _rotation.m_pitch );
//  m_viewMatrix.rotateX( _rotation.m_roll );

  // Translate
  m_viewMatrix.m_30 = _pos.m_x;
  m_viewMatrix.m_31 = _pos.m_y;
  m_viewMatrix.m_32 = _pos.m_z;

  setView();
}

void Camera::setView() const
{
  m_viewMatrix.loadModelView();
}

}
