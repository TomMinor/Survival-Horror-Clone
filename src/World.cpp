#ifdef LINUX
  #include <GL/gl.h>
#endif
#ifdef DARWIN
  #include <OpenGL/gl.h>
#endif

#include <iostream>
#include <string>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "RoomReader.h"

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

    try
    {
      loadRooms();
    }
    catch(std::ios_base::failure &msg)
    {
      std::cerr << msg.what() << "\n";
      return false;
    }

    updateTime();
    m_init = true;
  }
  else
  {
    printf("World already loaded, not reinitialising\n");
  }

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
      try
      {
        RoomReader(m_assetPath+line, m_rooms).load();
      }
      catch(std::invalid_argument &msg)
      {
        std::cerr << msg.what() << "\n";
      }

    }
    manifestFile.close();
  }
  else
  {
    // Throw an error
    throw std::ios_base::failure("Could not load manifest file\n");
  }
}

bool World::loadRoom(const std::string& _fileName)
{
        // identifier enum values store the expected amount of tokens
        if(tokens.size() == identifier)
        {
          switch(identifier)
          {
            case BBOX:    //bbox    <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ>
            case TRIGGER: //trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ> <bgID>
            {

              BBox bound = BBox(atof(tokens[1].c_str()),  atof(tokens[2].c_str()),  atof(tokens[3].c_str()),
                                atof(tokens[4].c_str()),  atof(tokens[5].c_str()),  atof(tokens[6].c_str()),
                                Vec4(atof(tokens[7].c_str()), atof(tokens[8].c_str()), atof(tokens[9].c_str())) );

              if(identifier == TRIGGER)
              {
                if(isdigit(tokens[10]))
                bgID = atof(tokens[10].c_str());

                roomTriggers[bgID] = bound;
              }
              else
              {
                roomBounds.push_back( bound );
              }

              break;
            }
            case BACKGROUND: //bg <bgID> <ForegroundFileName> <BackgroundFileName>
            {
              bgID = atof(tokens[1].c_str());

              roomForeground[bgID]= tokens[2];
              roomBackground[bgID]= tokens[3];

              break;
            }
            case CAMERA: //camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
            {
              bgID  = atof(tokens[8].c_str());

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
            case EXIT: //exit <offsetX> <offsetY> <offsetZ> <roomFileName>
            {
              std::string exitRoomPath = tokens[4];
              Vec4 offset = Vec4( atof(tokens[1].c_str()),
                                  atof(tokens[2].c_str()),
                                  atof(tokens[3].c_str()) );

              roomExits.push_back( exitRoomPath );
              roomExitPosition.push_back( offset );
              break;
            }
            case SPAWN: // spawn x y z
            {
              float x = atof(tokens[0].c_str());
              float y = atof(tokens[1].c_str());

              spawnPosition = Vec4(x,y);
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
          std::cout << _fileName << " : " << tokens[0]  << " : malformed line " << lineCount << "\n";
        }

        if (maxbgID < bgID) { maxbgID = bgID; }
        //maxbgID = (maxbgID < bgID) ? bgID : maxbgID;

      } // End of empty line/comment check
    }
    backgroundFile.close();
  }
  else
  {
    // Throw an error
    return false;
  }

  std::vector<Background> roomBackgrounds;
  for( unsigned int bgID = 0; bgID < maxbgID; ++bgID )
  {
    roomBackgrounds.push_back( Background(roomTriggers[bgID],
                                          roomBackground[bgID],
                                          roomForeground[bgID],
                                          roomCameras[bgID] ) );
  }

//  std::map<int, Camera> roomCameras;
//  std::map<int, BBox> roomTriggers;
//  std::map<int, std::string> roomForeground;
//  std::map<int, std::string> roomBackground;

  //roomTriggers, roomForeground,

  //for(  )

  std::vector<Door> roomDoors;

  // sort out the room here
  //m_rooms.push_back( Room(spawnPosition, roomBounds, roomBackgrounds, roomDoors) );

  return true;
}

}
