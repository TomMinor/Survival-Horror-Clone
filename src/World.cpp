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
#include <map>

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

  //bg <bgID> <ForegroundFileName> <BackgroundFileName>
  bg 1 BG_01_fg.tga BG_01_bg.tga

  //trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ> <bgID>
  trigger 0 0 0 1 1 1 0 0 0 1

  // spawn x y z
  spawn 0 0 0
  */

  // Values store expected amount of tokens for each identifier
  // (+1 to include identifier itself)
  enum {
    TRIGGER     = 1+10,
    BBOX        = 1+9,
    CAMERA      = 1+8,
    BACKGROUND  = 1+3,
    SPAWN       = 1,
    ERROR       = ~0
  };

  unsigned int lineCount = 0;
  std::ifstream backgroundFile;
  std::string path = m_assetPath + _fileName;
  std::string line;

  backgroundFile.open(path.c_str(), std::ios::in);
  if( backgroundFile.is_open() )
  {
    // These are all linked using ID's
    std::map<int, Camera> roomCameras;
    std::map<int, BBox> roomTriggers;
    std::map<int, std::string> roomBackgroundFG;
    std::map<int, std::string> roomBackgroundBG;

    std::vector<BBox> roomBounds;
    std::vector<Door> roomDoors;
    Vec4 spawnPosition;

    while(getline(backgroundFile, line))
    {
      std::vector<std::string> tokens;
      stringUtils::tokenize(line, tokens, " ");
      lineCount++;

      // Try to parse the line if it isn't empty or a comment
      if( !tokens.empty() && !(tokens[0].substr(2) == "//") )
      {
        unsigned int identifier;
             if(tokens[0] == "bbox")    { identifier = BBOX;      }
        else if(tokens[0] == "trigger") { identifier = TRIGGER;   }
        else if(tokens[0] == "camera")  { identifier = CAMERA;    }
        else if(tokens[0] == "bg")      { identifier = BACKGROUND;}
        else if(tokens[0] == "spawn")   { identifier = SPAWN;     }
        else                            { identifier = ERROR;     } // None-empty line, but invalid identifier

        // identifier enum values store the expected amount of tokens
        if(tokens.size() == identifier)
        {
          switch(identifier)
          {
            case BBOX:    //bbox    <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ>
            case TRIGGER: //trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ> <bgID>
            {
              float xmin = atof(tokens[1].c_str());
              float ymin = atof(tokens[2].c_str());
              float zmin = atof(tokens[3].c_str());
              float xmax = atof(tokens[4].c_str());
              float ymax = atof(tokens[5].c_str());
              float zmax = atof(tokens[6].c_str());
              Vec4 pos(atof(tokens[7].c_str()),
                       atof(tokens[8].c_str()),
                       atof(tokens[9].c_str()) );

              if(identifier == TRIGGER)
              {
                unsigned int bgID = atof(tokens[10].c_str());
                roomTriggers[bgID] =  BBox(xmin, ymin, zmin, xmax, ymax, zmax);
              }
              else
              {
                roomBounds.push_back( BBox(xmin, ymin, zmin, xmax, ymax, zmax) );
              }

              break;
            }
            case BACKGROUND: //bg <bgID> <ForegroundFileName> <BackgroundFileName>
            {
              unsigned int bgID = atof(tokens[1].c_str());
              roomBackgroundFG[bgID]= tokens[2];
              roomBackgroundBG[bgID]= tokens[3];

              break;
            }
            case CAMERA: //camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
            {
              unsigned int bgID  = atof(tokens[8].c_str());

              float pitch = atof(tokens[1].c_str());
              float yaw   = atof(tokens[2].c_str());
              float roll  = atof(tokens[3].c_str());

              Vec4 offset(atof(tokens[4].c_str()),
                          atof(tokens[5].c_str()),
                          atof(tokens[6].c_str()));

              float fov = atof(tokens[7].c_str());

              roomCameras[bgID] = Camera(offset, pitch, yaw, roll, fov) ;
              break;
            }
            case SPAWN:
            {
              float x = atof(tokens[0].c_str());
              float y = atof(tokens[1].c_str());
              float z = atof(tokens[2].c_str());

              spawnPosition = Vec4(x,y,z);
              break;
            }
            case ERROR:
            {
              std::cout << "Line " << lineCount << " : invalid identifier\n";
              break;
            }
          }
        }
        else
        {
          std::cout << tokens[0]  << " : malformed line " << lineCount << "\n";
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
