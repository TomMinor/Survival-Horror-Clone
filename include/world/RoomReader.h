#ifndef ROOMREADER_H
#define ROOMREADER_H
#include <SDL.h>

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>

#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "stringUtilities.h"

class RoomReader
{
public:
  RoomReader(const std::string& _fileName);
  ~RoomReader();
  Room* load();

private:
  enum { TRIGGER, BBOX, CAMERA, EXIT, BACKGROUND, SPAWN, ERROR };
  static const uint c_identifierSize[];

  std::string m_fileName;
  std::fstream m_fileStream;

  // To avoid parsing the file twice to get all the background IDs,
  // use the bgIDs as a key as we find them then copy the data into
  // a single Room object later
  std::map<int, Camera> m_roomCameras;
  std::map<int, BBox> m_roomTriggers;
  std::map<int, std::string> m_roomBackground;

  std::vector<Door> m_exits;
  std::vector<BBox> m_roomBounds;

  Vec4 m_spawnPosition;

  unsigned int getIdentifier(const std::string& i_token) const;

  float tokenToFloat(const std::string& _token) const;
  BBox parseBBox(const std::vector<std::string>& i_tokens) const;
  void parseBgID(const std::string& i_token, int &o_backgroundID) const;
  void addCamera(const std::vector<std::string>& i_tokens, int &o_backgroundID);
  void addExit(const std::vector<std::string>& i_tokens);
  void setSpawn(const std::vector<std::string>& i_tokens);

};

/*  ----- Example input file ------
//bbox <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax>
bbox 0 0 0 1 1 1
bbox 2 2 0 3 3 1
bbox 0 0 0 -1 -1 1
bbox -2 0 3 -3 -1

//camera <pitch> <yaw> <roll> <offsetX> <offsetY> <offsetZ> <fov> <bgID>
camera 0 0 0 1 1 1 75 1

//bg <bgID> <BackgroundFileName>
bg 1 BG_01_bg.tga

//trigger <Xmin> <Ymin> <Zmin> <Xmax> <Ymax> <Zmax> <bgID>
trigger 0 0 0 1 1 1 1

// spawn x y z
spawn 0 0 0

//exit <offsetX> <offsetY> <offsetZ> <roomFileName>
exit 0.5 1 0 ROOM_02.room

*/

#endif // ROOMREADER_H
