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
  Background();
  void drawBG() const;
  void drawFG() const;

private:
  BBox m_triggerVolume;
  Image m_bgTexture; // Background image
  Image m_fgTexture; // Foreground image
  Camera m_cameraView;
};

}

#endif // BACKGROUND_H
