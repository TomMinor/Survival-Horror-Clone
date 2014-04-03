#include "FileSystem.h"

namespace Game
{
// Root folder for assets
const std::string FileSystem::m_assetPath = "assets/";

// Child folders
const std::string FileSystem::m_roomPath = "backgrounds/";
const std::string FileSystem::m_actorPath = "actor/";

// Hard-coded resource files
const std::string FileSystem::m_fallbackTexture = "error.png";

std::string FileSystem::roomPath(const std::string& _fileName)
{
  return (m_assetPath + m_roomPath  + _fileName);
}

std::string FileSystem::actorPath(const std::string& _fileName)
{
  return (m_assetPath + m_actorPath + _fileName);
}

}
