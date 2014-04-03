#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>

namespace Game
{

class FileSystem
{
public:
  static std::string roomPath(const std::string& _fileName);
  static std::string actorPath(const std::string& _fileName);

  static std::string assetFolder()     { return m_assetPath;   }

  static std::string fallbackTexture() { return (m_assetPath + m_actorPath + m_fallbackTexture); }
private:
  // Root folder for assets
  static const std::string m_assetPath;

  // Child folders
  static const std::string m_roomPath;
  static const std::string m_actorPath;

  // Hard-coded resource files
  static const std::string m_fallbackTexture;
};

}

#endif // FILESYSTEM_H
