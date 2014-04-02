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
              const Camera& _cameraView  ) :
    m_cameraView(_cameraView), m_triggerVolume(_triggerVolume),
    m_bgTexture(Texture(_bgPath)),
    // Choose a random trigger volume colour (to differentiate different triggers)
    m_triggerColour( (float)rand()/(float)(RAND_MAX/1.0),
                     (float)rand()/(float)(RAND_MAX/1.0),
                     (float)rand()/(float)(RAND_MAX/1.0) )
    {;}

  void drawBG() const;
  void drawFG() const;
  void drawTrigger() const;

private:
  Camera m_cameraView;
  BBox m_triggerVolume;
  // This stores an RGBA background image, where the alpha channel is used
  // as a mask for the foreground (actors can walk behind it etc)
  Texture m_bgTexture;

  Vec4 m_triggerColour;
};

}

#endif // BACKGROUND_H
