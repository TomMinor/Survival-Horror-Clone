#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"
#include "Vec4.h"

namespace Game {

class Camera
{
public:
  Camera() {;}
  Camera(const Vec4 &_pos, const Vec4 &_rotation, float _fov);
  void setTransform(const Vec4 &_pos, const Vec4 &_rotation );
  inline Mat4 getMatrix() const { return m_viewMatrix; }
  void setView() const;

private:
  Mat4 m_viewMatrix;
  float m_fov;
};

}

#endif // CAMERA_H
