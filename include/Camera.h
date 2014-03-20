#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"
#include "Vec4.h"

namespace Game {

class Camera
{
public:
  Camera() {;}
  Camera(const Vec4 &_pos, float _pitch, float _yaw, float _roll, float _fov);
  void setTransform(const Vec4 &_pos, float _pitch, float _yaw, float _roll );
  inline Mat4 getMatrix() const;

private:
  float m_viewMatrix[16];
  float m_fov;
};

}

#endif // CAMERA_H
