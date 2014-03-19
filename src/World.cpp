#ifdef LINUX
  #include <GL/gl.h>
#endif
#ifdef DARWIN
  #include <OpenGL/gl.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "SDL.h"
#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "stringUtilities.h"

namespace Game {

static bool m_init = false;

bool World::init(const std::string& _assetpath)
{
  if(!m_init)
  {
    // Read assets
    //    Store in m_rooms
    m_assetPath = _assetpath;
    printf("Loading assets : %s\n", m_assetPath.c_str());

    loadRooms();

    m_init = true;
  }
  else
  {
    printf("World already loaded, not reinitialising\n");
  }

  updateTime();

  return true;
}

// Draw actors and room
void World::draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // now draw etc

  glPushMatrix();
    glTranslatef(0, 0, 0.0f);
    glPushMatrix();
      glRotatef(0, 0.0f, 0.0f, 1.0f);
      GLFunctions::cube(2.0f,1.0f,1.0f);
    glPopMatrix();
  glPopMatrix();


}

// Update actor states
void World::update()
{

}

void World::loadRooms()
{
  std::ifstream manifestFile;
  std::string path = m_assetPath + "manifest.bg";
  std::string line;

  manifestFile.open(path.c_str(), std::ios::in);
  if( manifestFile.is_open() )
  {
    while(getline(manifestFile, line))
    {
      if(!loadRoom(line))
      {
        std::cout << "Could not load background file : " << m_assetPath + line << std::endl;
      }
    }
    manifestFile.close();
  }
  else
  {
    // Throw an error
    std::cout << "Could not load manifest file\n";
    return;
  }
}

bool World::loadRoom(const std::string& _fileName)
{
  /*  ----- Example input file ------
  //bbox <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ>
  bbox 0 0 0 1 1 1 0 0 0
  bbox 2 2 0 3 3 1 1 1 0
  bbox 0 0 0 -1 -1 1 0 0 0
  bbox -2 0 3 -3 -1 -1 1 0

  //camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
  camera 0 0 0 1 1 1 75 1

  //bg <bgID> <texturename>
  bg 1 BG_00_01.tga
  */

  std::ifstream backgroundFile;
  std::string path = m_assetPath + _fileName;
  std::string line;

  backgroundFile.open(path.c_str(), std::ios::in);
  if( backgroundFile.is_open() )
  {
    while(getline(backgroundFile, line))
    {
      std::vector<std::string> tokens;
      stringUtils::tokenize(line, tokens, " ");

      if( !tokens.empty() )
      {
        if(tokens[0] == "bbox")
        {
          std::cout << tokens[0];
          for(std::vector<std::string>::iterator it = ++tokens.begin(); it != tokens.end(); ++it)
          {
            std::cout << *it;
          }
          std::cout << std::endl;
        }

        else if(tokens[0] == "camera")
        {
          for(std::vector<std::string>::iterator it = ++tokens.begin(); it != tokens.end(); ++it)
          {
            std::cout << *it;
          }
          std::cout << std::endl;
        }

        else if(tokens[0] == "bg")
        {
          for(std::vector<std::string>::iterator it = ++tokens.begin(); it != tokens.end(); ++it)
          {
            std::cout << *it;
          }
          std::cout << std::endl;
        }
      }
    }
    backgroundFile.close();
  }
  else
  {
    // Throw an error
    return false;
  }

  return true;
}

}
