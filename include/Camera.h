#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"
#include "Vec4.h"

class Camera
{
public:
  Camera(const Vec4 &_pos = Vec4(),
         const Vec4 &_rotation = Vec4(),
         float _fov = 90.0f);
  void setTransform(const Vec4 &_pos, const Vec4 &_rotation );
  inline Mat4 getMatrix() const   { return m_viewMatrix; }
  inline Vec4 getRotation() const { return m_rotation; }
  inline Vec4 getPosition() const { return m_position; }
  void setView() const;
  void draw() const;

private:
  Vec4 m_position;
  Vec4 m_rotation;
  Mat4 m_viewMatrix;
  float m_fov;
};

#endif // CAMERA_H
