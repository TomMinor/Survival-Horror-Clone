#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"

class Camera
{
public:
  Camera();
  void setTransform(const Vec4 &_pos, float _pitch, float _yaw, float _roll );
  inline Mat4 getMatrix() const;

private:
  float m_viewMatrix[16];

};

#endif // CAMERA_H
