#ifndef ROOMREADER_H
#define ROOMREADER_H

#include <map>
#include <vector>
#include <fstream>
#include <string>

#include "SDL.h"
#include "GLFunctions.h"
#include "World.h"
#include "Vec4.h"
#include "stringUtilities.h"


namespace Game {

class RoomReader
{
public:
  RoomReader(const std::string& _fileName, std::vector<Room> _roomsContainer );
  ~RoomReader();
  bool load();

private:
  enum { TRIGGER, BBOX, CAMERA, EXIT, BACKGROUND, SPAWN, ERROR };
  static const int c_identifierSize[];

  std::fstream m_fileStream;

  // To avoid parsing the file twice to get all the background IDs,
  // use the IDs as keys as we find them then sort out the data into
  // a single Room object later
  std::map<int, Camera> roomCameras;
  std::map<int, BBox> roomTriggers;
  std::map<int, std::string> roomForeground;
  std::map<int, std::string> roomBackground;

  // Stores the filenames of the rooms the exits lead to
  std::vector<std::string> roomExits;
  std::vector<Vec4> roomExitPosition;
  std::vector<BBox> roomBounds;

  Vec4 spawnPosition;

  unsigned int getIdentifier(const std::string& _token) const;

  BBox parseBBox(const std::vector<std::string>& _tokens) const;
  bool parseBgID(const std::string& _token, int &_backgroundID) const;
  void addBackground(const std::vector<std::string>& _tokens, int &_backgroundID);
  void addCamera(const std::vector<std::string>& _tokens, int &_backgroundID);
  void addExit(const std::vector<std::string>& _tokens);

};

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

//exit <offsetX> <offsetY> <offsetZ> <roomFileName>
exit 0.5 1 0 ROOM_02.room

*/

}

#endif // ROOMREADER_H
