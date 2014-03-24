#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include "GLinc.h"

namespace Game {

class Image
{
public:
  Image(std::string _fileName);
  ~Image();

private:
  GLuint m_texID;
};

}

#endif // IMAGE_H
