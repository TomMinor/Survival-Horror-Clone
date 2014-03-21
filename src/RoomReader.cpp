#include "RoomReader.h"
#include <stdexcept>
#include <iostream>
#include "stringUtilities.h"

namespace Game {

// Expected amount of tokens for respective line identifier - 1(identifier) + parameters
const uint RoomReader::c_identifierSize[] = { 1+10, //  TRIGGER
                                             1+9,  //  BBOX
                                             1+8,  //  CAMERA
                                             1+4,  //  EXIT
                                             1+3,  //  BACKGROUND
                                             1+3,  //  SPAWN
                                             ~0    //  ERROR
                                           };

RoomReader::RoomReader(const std::string& _fileName, std::vector<Room>& _roomsContainer )
  : m_roomsContainer(_roomsContainer)
{
  m_fileStream.open( _fileName.c_str(), std::ios::in );
  if( !m_fileStream.is_open() )
  {
    throw std::invalid_argument("Could not load background file : " + _fileName + "\n");
  }
}

RoomReader::~RoomReader()
{
  m_fileStream.close();
}

void RoomReader::load()
{
  int bgID;
  int maxbgID = 0;
  int lineCount = 0;
  std::string line;

  while(std::getline(m_fileStream, line))
  {
    std::vector<std::string> tokens;
    stringUtils::tokenize(line, tokens, " ");
    lineCount++;

    // Try to parse the line if it isn't empty or a comment
    if( !tokens.empty() && (tokens[0].substr(0, 2) != "//") )
    {
      uint identifier = getIdentifier(tokens[0]);
      if( tokens.size() == c_identifierSize[ identifier ] )
      {
        try
        {
          // We cannot rely on the input file having the correct data for each field,
          // hence the various parsing functions are expected to throw exceptions if bad data is encountered
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
              parseBgID(tokens[10], bgID);
              m_roomTriggers[bgID] = parseBBox(tokens);
              break;
            }
            case BACKGROUND: //bg <bgID> <ForegroundFileName> <BackgroundFileName>
            {
              parseBgID(tokens[1], bgID);
              addBackground(tokens, bgID);
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
              std::cerr << "Line " << lineCount << " : invalid identifier\n";
              break;
            }
          }
        }
        catch(std::runtime_error &msg)
        {
          std::cerr << "Line " << lineCount << " : " << msg.what() << "\n";
        }
      } // end token count check
    } // end empty line/comment check

    if (bgID > maxbgID) { maxbgID = bgID; }
  }// end get line

//  std::vector<Background> roomBackgrounds;
//  for( unsigned int bgID = 0; bgID < maxbgID; ++bgID )
//  {
//    roomBackgrounds.push_back( Background(roomTriggers[bgID],
//                                          roomBackground[bgID],
//                                          roomForeground[bgID],
//                                          roomCameras[bgID] ) );
//  }
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

void RoomReader::parseBgID(const std::string& i_token, int &o_backgroundID) const
{
//  std::istringstream ss(i_token);
//  int _ID;
//  // Check if the token is valid before placing it's value into bg ID
//  if( ss >> _ID )
//  {
//    ss >> o_backgroundID;
//  }
//  else
//  {
//    throw std::runtime_error("Non-integer background ID");
//  }
}

BBox RoomReader::parseBBox(const std::vector<std::string>& i_tokens) const
{
//  std::istringstream ss(i_token);
//  int _ID;
//  // Check if the token is valid before placing it's value into bg ID
//  if( ss >> _ID )
//  {
//    ss >> _backgroundID;
//  }
//  else
//  {
//    throw std::runtime_error("Non-integer background ID");
//  }
}

void RoomReader::addBackground(const std::vector<std::string>& i_tokens, int &o_backgroundID)
{
//  std::istringstream ss(i_token);
//  int _ID;
//  // Check if the token is valid before placing it's value into bg ID
//  if( ss >> _ID )
//  {
//    ss >> o_backgroundID;
//  }
//  else
//  {
//    throw std::runtime_error("Non-integer background ID");
//  }

//  bgID = atof(tokens[1].c_str());

//  roomForeground[bgID]= tokens[2];
//  roomBackground[bgID]= tokens[3];

//  break;
}

void RoomReader::addCamera(const std::vector<std::string>& i_tokens, int &o_backgroundID)
{
  std::stringstream ss(std::string());  // Empty stream
  float rotation[3]; // pitch, yaw, roll
  float offset[3]; // x,y,z
  float fov;

  // Try to parse the rotation
  for(int i=0; i<3; ++i)
  {
    if(ss << i_tokens[i+1])
    {
      ss >> rotation[i];
      ss.clear();
    }
    else
    {
      throw std::runtime_error("Invalid spawn rotation");
    }
  }

  // Try to parse the offset
  for(int i=0; i<3; ++i)
  {
    if(ss << i_tokens[i+4])
    {
      ss >> offset[i];
      ss.clear();
    }
    else
    {
      throw std::runtime_error("Invalid spawn coordinates");
    }
  }

  // FOV
  if(ss << i_tokens[7])
  {
    ss >> fov;
  }
  else
  {
    throw std::runtime_error("Invalid FOV");
  }

  roomCameras[o_backgroundID] = Camera(Vec4(offset[0], offset[1], offset[2]),
                                rotation[0], rotation[1], rotation[2], fov);
}

void RoomReader::addExit(const std::vector<std::string>& i_tokens)
{
//  std::istringstream ss(_token);
//  int _ID;
//  // Check if the token is valid before placing it's value into bg ID
//  if( ss >> _ID )
//  {
//    ss >> _backgroundID;
//  }
//  else
//  {
//    throw std::runtime_error("Non-integer background ID");
//  }

//  std::string exitRoomPath = tokens[4];
//  Vec4 offset = Vec4( atof(tokens[1].c_str()),
//                      atof(tokens[2].c_str()),
//                      atof(tokens[3].c_str()) );

//  roomExits.push_back( exitRoomPath );
//  roomExitPosition.push_back( offset );
}

void RoomReader::setSpawn(const std::vector<std::string>& i_tokens)
{
  std::stringstream ss;  // Empty stream
  float coord[3];

  // Try to parse the coordinates
  for(int i=0; i<3; ++i)
  {
    if(ss << i_tokens[i+1])
    {
      ss >> coord[i];
      ss.clear();//clear any bits set
      ss.str(std::string());
    }
    else
    {
      throw std::runtime_error("Invalid spawn coordinates");
    }
  }

  m_spawnPosition = Vec4(coord[0], coord[1], coord[2]);
}


}
