#include "RoomReader.h"
#include <stdexcept>
#include <iostream>
#include "stringUtilities.h"

namespace Game {

// Expected amount of tokens for respective line identifier - 1(identifier) + parameters
const int RoomReader::c_identifierSize[] = { 1+10, //  TRIGGER
                                             1+9,  //  BBOX
                                             1+8,  //  CAMERA
                                             1+4,  //  EXIT
                                             1+3,  //  BACKGROUND
                                             1+3,  //  SPAWN
                                             ~0    //  ERROR
                                           };

RoomReader::RoomReader(const std::string& _fileName, std::vector<Room> _roomsContainer )
{
  m_fileStream.open( _fileName, std::ios::in );
  if( !m_fileStream.is_open() )
  {
    throw std::invalid_argument("Could not load background file : " + _fileName + "\n");
  }
}

RoomReader::~RoomReader()
{
  m_fileStream.close();
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

bool RoomReader::load()
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
      uint identifier = getIdentifier(token[0]);
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
              roomBounds.push_back( parseBBox(tokens) );
              break;
            }
            case TRIGGER: //trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <offsetX> <offsetY> <offsetZ> <bgID>
            {
              if(parseBgID(tokens[10], bgID))
              {
                roomTriggers[bgID] = parseBBox(tokens);
              }
              else
              {
                throw std::runtime_error("Non-integer background ID\n");
              }
              break;
            }
            case BACKGROUND: //bg <bgID> <ForegroundFileName> <BackgroundFileName>
            {
              if(parseBgID(tokens[1], bgID))
              {
                addBackground(tokens, bgID);
              }
              else
              {
                throw std::runtime_error("Non-integer background ID\n");
              }
              break;
            }
            case CAMERA: //camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
            {
              if(parseBgID(tokens[8], bgID))
              {
                addCamera(tokens, bgID);
              }
              else
              {
                throw std::runtime_error("Non-integer background ID\n");
              }
              break;
            }
            case EXIT: //exit <offsetX> <offsetY> <offsetZ> <roomFileName>
            {
              break;
            }
            case SPAWN: // spawn x y z
            {
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
          std::cerr << msg.what() << "\n";
        }
      } // end token count check
    } // end empty line/comment check
  }// end get line
}



}
