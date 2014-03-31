#ifndef MD2LOADER_H
#define MD2LOADER_H

#include <fstream>
#include "md2.h"
#include "md2mesh.h"

namespace Md2
{

class Loader
{
public:
  Loader(std::string _meshPath, Mesh * const o_mesh );
  ~Loader();

  void load();

private:
  std::ifstream m_fileStream;

private:
  Header m_header;
  Mesh * const m_mesh;

  // Data arrays
  char*  m_keyFrameBuffer;

};

}
#endif // MD2LOADER_H
