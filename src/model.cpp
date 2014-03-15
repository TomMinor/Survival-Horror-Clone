#include "model.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef LINUX
	#include <GL/gl.h>
#endif
#ifdef DARWIN
	#include <OpenGL/gl.h>
#endif

void Model::draw() const
{

}

void Model::load(std::string path)
{
  std::ifstream mesh( path.c_str() );

  std::string identifier;
  std::string line;
  while(std::getline(mesh, line))
  {
    std::istringstream iss(line);
    iss >> identifier;

    if(identifier == "v")
    {
      //std::cout << "Vertex ";
      float x, y, z;
      iss >> x >> y >> z;
      //std::cout << x << y << z << std::endl;

      vertices.push_back( Vec4(x, y, z) );
    }

    if(identifier == "vn")
    {
      //std::cout << "Vertex Normal";
      float x, y, z;
      iss >> x >> y >> z;
      //std::cout << x << y << z << std::endl;

      vnormals.push_back( Vec4(x, y, z) );
    }

    if(identifier == "vt")
    {
      //std::cout << "Vertex Texture";
      float u, v;
      iss >> u >> v;
      //std::cout << u << v << std::endl;

      vtexture.push_back( Vec4(u, v) );
    }

    if(identifier == "f")
    {
      std::cout << "Face \n";
      std::string ID;

      std::string indices[3];
      do
      {
        iss >> ID;

        for(int i = 0; i < 3; ++i)
        {
          //std::cout << ID << " ";
          int index = ID.rfind('/');
          indices[i] = ID.substr(index + 1);
          ID = ID.substr(index + 1, std::string::npos);
        }

        glVerts.push_back( vertices[atof(indices[0].c_str())].m_x );
        glVerts.push_back( vertices[atof(indices[0].c_str())].m_y );
        glVerts.push_back( vertices[atof(indices[0].c_str())].m_z );

//        std::cout << "\nVertex  ID : " << glVerts[0]
//                << "\nNormal  ID : " << glNorm [glNorm.back() ]
//                << "\nTexture ID : " << glTex  [glTex.back()  ];

      }while(iss);

      std::cout << std::endl;
    }
  }
}
