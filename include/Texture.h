#ifndef Texture_H
#define Texture_H
#include <iostream>
#include "GLinc.h"

namespace Game {

class Texture
{
public:
  Texture(std::string _fileName);
  ~Texture();

  void setCurrent() const;

private:
  GLuint m_texID;
};

}

#endif // Texture_H
