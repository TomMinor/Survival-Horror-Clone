#include "RoomReader.h"
#include "stringUtilities.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace Game {

// Expected amount of tokens for respective line identifier - 1(identifier) + parameters
const uint RoomReader::c_identifierSize[] = { 1+ 7, //  TRIGGER
                                              1+ 6,  //  BBOX
                                              1+ 8,  //  CAMERA
                                              1+ 4,  //  EXIT
                                              1+ 2,  //  BACKGROUND
                                              1+ 3,  //  SPAWN
                                              ~0    //  ERROR
                                            };

RoomReader::RoomReader(const std::string& _fileName)
  : m_fileName(_fileName)
{
  std::string filePath = FileSystem::instance().roomPath(_fileName);
  m_fileStream.open( filePath.c_str(), std::ios::in );
  if( !m_fileStream.is_open() )
  {
    throw std::invalid_argument("Could not load background file : " + filePath);
  }
}

RoomReader::~RoomReader()
{
  m_fileStream.close();
}

Room* RoomReader::load()
{
  int bgID = 0;
  int maxbgID = 0;
  int lineCount = 0;
  std::string line;

  while(std::getline(m_fileStream, line))
  {
    std::vector<std::string> tokens;
    util::tokenize(line, tokens, " ");
    lineCount++;

    // Try to parse the line if it isn't empty or a comment
    if( !tokens.empty() && (tokens[0].substr(0, 2) != "//") )
    {
      uint identifier = getIdentifier(tokens[0]);
      // We cannot rely on the input file having the correct data for each field,
      // hence the various parsing functions are expected to throw exceptions if bad data is encountered
      try
      {
        if( tokens.size() == c_identifierSize[ identifier ] || identifier == ERROR) //Allow the switch statement to deal with the error
        {
          switch(identifier)
          {
            case BBOX:    //bbox    <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ>
            {
              // The order the room bounds are stored does not matter
              m_roomBounds.push_back( parseBBox(tokens) );
              break;
            }
            // Each background is linked to a trigger and camera, bgID is used to link them
            case TRIGGER: //trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ> <bgID>
            {
              parseBgID(tokens[7], bgID);
              m_roomTriggers[bgID] = parseBBox(tokens);
              break;
            }
            case BACKGROUND: //bg <bgID> <BackgroundFileName>
            {
              parseBgID(tokens[1], bgID);
              m_roomBackground[bgID]= tokens[2];
              break;
            }
            case CAMERA: //camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
            {
              parseBgID(tokens[8], bgID);
              addCamera(tokens, bgID);
              break;
            }
            case EXIT: //exit <offsetX> <offsetY> <offsetZ> <roomFileName>
            {
              addExit(tokens);
              break;
            }
            case SPAWN: // spawn x y z
            {
              setSpawn(tokens);
              break;
            }
            case ERROR:
            {
              throw std::runtime_error( "Invalid identifier \"" + tokens[0] + "\"" );
              break;
            }
          }
        }
        else
        {
          throw std::runtime_error( "Invalid token amount" );
        } // end token count check
      }
      catch(std::runtime_error &msg)
      {
        std::stringstream error;  error << "Line " << lineCount << " : " << msg.what() << "\n";
        throw std::runtime_error( error.str() );
      }
    } // end empty line/comment check

    // Keep track of the maximum background ID
    if (bgID > maxbgID) { maxbgID = bgID; }
  }// end get line

  if(lineCount == 0)
  {
    throw std::runtime_error( "Empty file" );
  }

  std::vector<Background> roomBackgrounds;
  for( int bgID = 1; bgID <= maxbgID; ++bgID )
  {
    if( (m_roomBackground.count(bgID)>0) &&
        (m_roomTriggers.count(bgID)>0) &&
        (m_roomCameras.count(bgID)>0) )
    {
      roomBackgrounds.push_back( Background(m_roomTriggers[bgID],
                                            m_roomBackground[bgID],
                                            m_roomCameras[bgID] ) );
    }
  }

  // Disallow incomplete rooms
  if(roomBackgrounds.empty()) { throw std::runtime_error( "No backgrounds found" ); }
  if(m_roomBounds.empty()   ) { throw std::runtime_error( "No bounds found" );      }
  if(m_exits.empty()        ) { throw std::runtime_error( "No room exits found" );  }

  return new Room(m_fileName, m_spawnPosition, m_roomBounds, roomBackgrounds, m_exits);
}

unsigned int RoomReader::getIdentifier(const std::string& _token) const
{
       if(_token == "bbox")    { return BBOX;      }
  else if(_token == "trigger") { return TRIGGER;   }
  else if(_token == "camera")  { return CAMERA;    }
  else if(_token == "bg")      { return BACKGROUND;}
  else if(_token == "spawn")   { return SPAWN;     }
  else if(_token == "exit")    { return EXIT;      }
  // None-empty line, but invalid identifier
  else                         { return ERROR;     }
}

BBox RoomReader::parseBBox(const std::vector<std::string>& i_tokens) const
{
  float bounds[6]; // xyz min/max

  for(int i=0; i<6; ++i)
  {
    // Try to parse the rotation
    try
    {
      bounds[i] = util::tokenToFloat( i_tokens[i+1]);
    }
    catch(std::runtime_error)
    {
      std::cerr << "Invalid bbox bound\n";
      throw;
    }
  }

  return BBox(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
}

void RoomReader::parseBgID(const std::string& i_token, int &o_backgroundID) const
{
  try
  {
    o_backgroundID = util::tokenToFloat(i_token);
  }
  catch(std::runtime_error &msg)
  {
    std::cerr << "Invalid background ID\n";
    throw;
  }

}

void RoomReader::addCamera(const std::vector<std::string>& i_tokens, int &o_backgroundID)
{
  float rotation[3]; // pitch, yaw, roll
  float offset[3]; // x,y,z
  float fov;

  for(int i=0; i<3; ++i)
  {
    // Try to parse the rotation
    try
    {
      rotation[i] = util::tokenToFloat( i_tokens[i+1]);
    }
    catch(std::runtime_error)
    {
      std::cerr << "Invalid camera rotation\n";
      throw;
    }

    try
    {
      offset[i] = util::tokenToFloat( i_tokens[i+4]);
    }
    catch(std::runtime_error)
    {
      std::cerr << "Invalid camera coordinates\n";
      throw;
    }
  }

  try
  {
    fov = util::tokenToFloat( i_tokens[7]);
  }
  catch(std::runtime_error)
  {
    std::cerr << "Invalid FOV\n";
    throw;
  }

  // BUG (maya uses different coord system) http://www.evl.uic.edu/ralph/508S98/coordinates.html
  m_roomCameras[o_backgroundID] = Camera( Vec4(offset[0], offset[1], offset[2]),
                                          Vec4(rotation[1], rotation[0], rotation[2]), fov );
}

void RoomReader::addExit(const std::vector<std::string>& i_tokens)
{
  float coord[3];
  std::string nextRoom;

  for(int i=0; i<3; ++i)
  {
    // Try to parse the offset
    try
    {
      coord[i] = util::tokenToFloat( i_tokens[i+1]);
    }
    catch(std::runtime_error)
    {
      std::cerr << "Invalid  exit position";
      throw;
    }
  }

  // File name of the room that should load when this exit is used
  nextRoom = i_tokens[4];

  m_exits.push_back( Door(nextRoom, Vec4(coord[0], coord[1], coord[2])) );
}

void RoomReader::setSpawn(const std::vector<std::string>& i_tokens)
{
  float coord[3];

  for(int i=0; i<3; ++i)
  {
    try
    {
      coord[i] = util::tokenToFloat( i_tokens[i+1]);
    }
    catch(std::runtime_error)
    {
      std::cerr << "Invalid spawn coordinate\n";
      throw;
    }
  }

  m_spawnPosition = Vec4(coord[0], coord[1], coord[2]);
}

}

