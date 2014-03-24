#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Vec4.h"
#include "BoundingBox.h"
#include "Texture.h"
#include "Camera.h"

namespace Game {

class Background
{
public:
  Background( const BBox& _triggerVolume,
                          const std::string& _bgPath,
                          const std::string& _fgPath,
                          const Camera& _cameraView  ) :
    m_cameraView(_cameraView), m_triggerVolume(_triggerVolume),
    m_bgTexture(Texture(_bgPath)), m_fgTexture(Texture(_fgPath)) {;}

  void drawBG() const;
  void drawFG() const;

private:
  Camera m_cameraView;
  BBox m_triggerVolume;
  Texture m_bgTexture; // Background Texture
  Texture m_fgTexture; // Foreground Texture
};

}

#endif // BACKGROUND_H
