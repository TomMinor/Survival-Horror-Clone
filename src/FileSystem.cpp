#include "FileSystem.h"

FileSystem::FileSystem() :
  m_assetPath("assets"), m_roomPath("backgrounds"), m_actorPath("actor"),
  m_roomManifest("manifest.bg"), m_fallbackTexture("error.png")
{
  // Add / to each path if it does not exist
  if(m_assetPath[m_assetPath.size()-1] !='/') { m_assetPath+="/"; }
  if(m_roomPath[m_roomPath.size()-1]   !='/') { m_roomPath +="/"; }
  if(m_actorPath[m_actorPath.size()-1] !='/') { m_actorPath+="/"; }
}

std::string FileSystem::roomPath(const std::string& _fileName)
{
  return (m_assetPath + m_roomPath  + _fileName);
}

std::string FileSystem::actorPath(const std::string& _fileName)
{
  return (m_assetPath + m_actorPath + _fileName);
}

//const std::string& combinePath(const std::vector<std::string>& _pathTokens)
//{
//  std::string path;
//  if(!_pathTokens.empty())
//  {
//    // Combine the folders into an A/B/C/ structure
//    for(std::vector<std::string>::iterator token = _pathTokens.begin();
//        token != _pathTokens.end()-1;
//        ++token)
//    {
//      path += *token + "/";
//    }

//    // Add the filename to the end to get A/B/C/d.file
//    path += _pathTokens.end();
//  }

//  return path;
//}
