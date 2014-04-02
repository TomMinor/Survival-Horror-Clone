#include "Texture.h"
#include "FileSystem.h"
#include <stdexcept>

namespace Game {

  Texture::Texture(std::string _fileName) : m_texID(0)
  {
    glGenTextures(1,&m_texID);

    SDL_Surface* texture = IMG_Load(_fileName.c_str());

    if(!texture)
    {
      // Attempt to load a fallback texture that should be guaranteed to exist
      if( !(texture = IMG_Load(FileSystem::instance()->fallbackTexture().c_str())) )
      {
        throw std::invalid_argument(" Could not load fallback material ");
      }
      std::cerr << SDL_GetError() << ", using fallback material\n";
    }

    // ----------------------------------------------------------------------
    // Additional error checks from http://content.gpwiki.org/SDL%3aTutorials%3aUsing_SDL_with_OpenGL
    // ----------------------------------------------------------------------

    // Warn if the image is not a power of 2
    if((texture->w & (texture->w-1)) != 0) { std::cerr << _fileName << " : Non-power of 2 width \n"; }
    if((texture->h & (texture->h-1)) != 0) { std::cerr << _fileName << " : Non-power of 2 height \n"; }

    GLenum texFormat;
    GLint numColours = texture->format->BytesPerPixel;
    if(numColours == 4)   // Alpha
    {
      texFormat = GL_RGBA;
    }
    else if(numColours== 3)  // No alpha
    {
      texFormat = GL_RGB;
    }
    else
    {
      throw std::runtime_error("Invalid image format (Must be RGB or RGBA)");
    }

    glBindTexture(GL_TEXTURE_2D, m_texID);

    glTexImage2D(GL_TEXTURE_2D, 0 , numColours, texture->w, texture->h, 0,
                 texFormat, GL_UNSIGNED_BYTE, texture->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texHeight = texture->h;
    texWidth = texture->w;

    SDL_FreeSurface(texture);
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
