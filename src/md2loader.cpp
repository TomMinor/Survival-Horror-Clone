#include "md2loader.h"

#include <fstream>
#include <stdexcept>

namespace Md2
{

Loader::Loader(std::string _meshPath, Mesh * const o_mesh ) :
  m_mesh(o_mesh), m_keyFrameBuffer(0)
{
  m_fileStream.open(_meshPath.c_str(), std::ios::in | std::ios::binary);
  if( !m_fileStream.is_open() )
  {
    const std::string tmp = "Error opening file " + _meshPath;
    throw std::runtime_error(tmp.c_str());
  }

  // Store the header
  m_fileStream.read( reinterpret_cast<char*>(&m_header), sizeof(Md2::Header) );

  // Check the file's header signature to make sure this is a valid md2 file
  if( (m_header.ident != Md2::IDENT) )
  {
    throw std::runtime_error("Invalid file ident");
  }

  if( (m_header.version != Md2::VERSION) )
  {
    throw std::runtime_error("Invalid file version");
  }
}

Loader::~Loader()
{
  delete[] m_keyFrameBuffer;
  m_fileStream.close();
}

void Loader::load()
{
  // init attributes
  m_mesh->m_totalFrames   = m_header.numFrames;
  m_mesh->m_totalVertices = m_header.numVertices;
  m_mesh->m_totalGLcmds   = m_header.numGlcmds;

  // allocate buffers
  m_mesh->m_Vertices      = new Vec3[m_header.numVertices * m_header.numFrames ];
  m_mesh->m_lightNormals  = new int[ m_header.numVertices * m_header.numFrames ];
  m_mesh->m_GLcmds        = new int[ m_header.numGlcmds ];
  m_keyFrameBuffer= new char[ m_header.frameSize * m_header.numFrames ];

  // Fill the keyFrameBuffer with an array of (KeyFrame) data from the file at header.offsetFrames,
  // each element will be cast to type KeyFrame later and used to transform the vertices into each frame
  m_fileStream.seekg( m_header.frameOffset, std::ios::beg );
  m_fileStream.read(reinterpret_cast<char*>( m_keyFrameBuffer ),
                    m_header.numFrames*m_header.frameSize );

  // Read GL commands, which will be used when rendering to decide on the most optimal draw method (FAN or STRIP)
  m_fileStream.seekg( m_header.glCmdsOffset, std::ios::beg );
  m_fileStream.read(reinterpret_cast<char*>( m_mesh->m_GLcmds ),
                    m_header.numGlcmds*(sizeof( m_mesh->m_GLcmds[0] )) );

  // Decompress the vertex positions for each frame by transforming them
  // using the translation/scale of the KeyFrame
  for( int j=0; j<m_header.numFrames; ++j )
  {
    int frameIndex  = m_header.numVertices*j;
    int* normals    = &m_mesh->m_lightNormals[frameIndex];
    Vec3* verts    = &m_mesh->m_Vertices[frameIndex];
    KeyFrame* frame = reinterpret_cast<KeyFrame*>(&m_keyFrameBuffer[m_header.frameSize*j]);

    for( int i=0; i<m_header.numVertices; ++i )
    {
      verts[i][0] = ( frame->vertices[i].v[0] * frame->scale[0] + frame->translate[0] );
      verts[i][1] = ( frame->vertices[i].v[1] * frame->scale[1] + frame->translate[1] );
      verts[i][2] = ( frame->vertices[i].v[2] * frame->scale[2] + frame->translate[2] );

      normals[i] = frame->vertices[i].lightNormalIndex;
    }
  }
}

}
