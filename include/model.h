#ifndef MODEL_H
#define MODEL_H
#include "GLinc.h"

#include <vector>
#include <string>

#include "Vec4.h"


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
