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
  Background( const BBox& _triggerVolume,
                          const std::string& _bgPath,
                          const std::string& _fgPath,
                          const Camera& _cameraView  )
    : m_cameraView(_cameraView), m_triggerVolume(_triggerVolume),
      m_bgTexture(Image(_bgPath)), m_fgTexture(Image(_fgPath)) {;}
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
