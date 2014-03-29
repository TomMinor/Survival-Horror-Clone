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
  Texture(std::string _fileName);
  ~Texture();

  void setCurrent() const;

  int textureHeight() const { return m_texture->h; }
  int textureWidth() const  { return m_texture->w; }

private:
  GLuint m_texID;
  SDL_Surface* m_texture;
};

}

#endif // Texture_H
