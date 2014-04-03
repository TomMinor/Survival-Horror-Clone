#ifndef Texture_H
#define Texture_H
#include <iostream>
#include "GLinc.h"
#include <SDL.h>
#include <SDL_image.h>


namespace Game {

class Texture
{
public:
  Texture(std::string _fileName = "error.png");
  ~Texture();

  void setCurrent() const;

  inline int height() const { return m_texHeight; }
  inline int width() const  { return m_texWidth; }
  inline bool usingFallback() const { return m_usingFallback; }
  inline bool hasAlphaMask() const { return m_hasAlpha; }

private:
  GLuint m_texID;

  bool m_usingFallback;
  bool m_hasAlpha;

  int m_texHeight;
  int m_texWidth;
};

}

#endif // Texture_H
