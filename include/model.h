#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Vec4.h"
#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

class Model
{
public:
  void load(std::string path);
  void draw() const;

private:
  std::vector<Vec4> vertices;
  std::vector<Vec4> vnormals;
  std::vector<Vec4> vtexture;
  std::vector<Vec4> faces;

  std::vector<GLfloat> glVerts;
  std::vector<GLfloat> glNorm;
  std::vector<GLfloat> glTex;

};

#endif // MODEL_H
