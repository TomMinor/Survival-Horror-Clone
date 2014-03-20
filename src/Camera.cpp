#include "Camera.h"

namespace Game {

Camera::Camera(const Vec4 &_pos, float _pitch, float _yaw, float _roll, float _fov)
  : m_fov(_fov)
{
  setTransform(_pos, _pitch, _yaw, _roll);
}

void Camera::setTransform(const Vec4 &_pos, float _pitch, float _yaw, float _roll )
{

}

inline Mat4 Camera::getMatrix() const
{

}

}
