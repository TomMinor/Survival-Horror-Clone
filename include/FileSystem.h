#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>

class FileSystem
{
public:
  static FileSystem* instance()
  {
    static FileSystem* instance = new FileSystem();
    return instance;
  }

  std::string roomPath(const std::string& _fileName);
  std::string actorPath(const std::string& _fileName);

  std::string assetFolder()     { return m_assetPath;   }

  std::string fallbackTexture() { return (m_assetPath + m_actorPath + m_fallbackTexture); }
  std::string roomManifest()    { return (m_assetPath + m_roomPath + m_roomManifest);    }

private:
  FileSystem();

  // Root folder for assets
  std::string m_assetPath;

  // Child folders
  std::string m_roomPath;
  std::string m_actorPath;

  // Hard-coded resource files
  std::string m_roomManifest;
  std::string m_fallbackTexture;
};

#endif // FILESYSTEM_H
