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

  int textureHeight() const { return texHeight; }
  int textureWidth() const  { return texWidth; }

private:
  GLuint m_texID;
  int texHeight;
  int texWidth;
};

}

#endif // Texture_H
