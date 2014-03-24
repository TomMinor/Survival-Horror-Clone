#include "Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

namespace Game {

  Texture::Texture(std::string _fileName) : m_texID(0)
  {
    glGenTextures(1,&m_texID);

    SDL_Surface* texture = IMG_Load(_fileName.c_str());

    if(!texture)
    {
      // Attempt to load a fallback texture that should be guaranteed to exist
      if( !(texture = IMG_Load("assets/error.png")) )
      {
        throw std::invalid_argument(" Could not load fallback material ");
      }
      std::cerr << SDL_GetError() << ", using fallback material\n";
    }

    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, texture->w, texture->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  Texture::~Texture()
  {
    glDeleteTextures(1,&m_texID);
  }

  void Texture::setCurrent() const
  {
    glBindTexture(GL_TEXTURE_2D, m_texID);
  }

}
