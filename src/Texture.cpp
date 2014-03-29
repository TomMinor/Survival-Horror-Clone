#include "Texture.h"
#include <stdexcept>

namespace Game {

  Texture::Texture(std::string _fileName) : m_texID(0), m_texture(NULL)
  {
    glGenTextures(1,&m_texID);

    // BUG crashes when i use m_texture, why?
    m_texture=0;

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
    glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, texture->w, texture->h, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //m_texture = texture;
  }

  Texture::~Texture()
  {
    free(m_texture);
    glDeleteTextures(1,&m_texID);
  }

  void Texture::setCurrent() const
  {
    glBindTexture(GL_TEXTURE_2D, m_texID);
  }

}
