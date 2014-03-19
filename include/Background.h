#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Vec4.h"
#include "BoundingBox.h"
#include "Image.h"
#include "Camera.h"

namespace Game {

class Background
{
public:
  Background(
              const BBox& _triggerVolume,
              const std::string& _bgPath,
              const std::string& _fgPath,
              const Camera& _cameraView )
    : m_cameraView(_cameraView)
  {;}
  void drawBG() const;
  void drawFG() const;

private:
  const BBox m_triggerVolume;
  const Image m_bgTexture; // Background image
  const Image m_fgTexture; // Foreground image
  const Camera m_cameraView;
};

}

#endif // BACKGROUND_H
