#include "md2.h"

#include <fstream>

/*
 * Modified from http://tfc.duke.free.fr/old/models/md2.htm
 */

const int MD2_IDENT[]   = {'I', 'D', 'P', '2'};
const int MD2_VERSION = 8;
const int NUMVERTEXNORMALS = 162;
const int SHADEDOT_QUANT = 16;


MeshMd2::~MeshMd2()
{
  delete[] m_Vertices;
  delete[] m_GLcmds;
  delete[] m_lightNormals;
}

//BUG make this RAII compatible
bool MeshMd2::loadMesh(std::string _filename)
{
  std::ifstream fileStream;
  MD2Header   header;

  char*       frameBuffer;
  KeyFrame*   frame;
  fVec3*      verts;
  int*        normals;

  fileStream.open(_filename.c_str(), std::ios::in | std::ios::binary);
  if( !fileStream.is_open() )
  {
    return false;
  }

  fileStream.read( reinterpret_cast<char*>(&header), sizeof(MD2Header) );

  // Check ident & version to verify this is an MD2 file
  if( (header.ident != MD2_IDENT) || (header.version != MD2_VERSION) )
  {
    fileStream.close();
    return false;
  }

  // init attributes
  m_totalFrames   = header.numFrames;
  m_totalVertices = header.numVertices;
  m_totalGLcmds   = header.numGlcmds;

  // allocate buffers
  m_Vertices      = new fVec3[ m_totalVertices*m_totalFrames ];
  m_GLcmds        = new int[ m_totalGLcmds ];
  m_lightNormals  = new int[ m_totalVertices * m_totalFrames ];
  frameBuffer     = new char[ m_totalFrames * header.frameSize ];

  // Read frame data
  fileStream.seekg( header.offsetFrames, std::ios::beg );
  fileStream.read( reinterpret_cast<char*>(frameBuffer),
                   m_totalFrames*header.frameSize );

  // Read GL commands
  fileStream.seekg( header.offsetGlcmds, std::ios::beg );
  fileStream.read( reinterpret_cast<char*>(m_GLcmds),
                   m_totalGLcmds*(sizeof(m_GLcmds[0])) );

  // Initialise the decompressed vertex positions for each frame
  for( int j=0; j<m_totalFrames; ++j )
  {
    // Update pointers to new offsets for this frame
    int frameIndex = m_totalVertices*j;
    verts   = &m_Vertices[frameIndex];
    normals = &m_lightNormals[frameIndex];
    frame   = reinterpret_cast<KeyFrame*>(&frameBuffer[header.frameSize*j]);

    // Translate & scale each vertex
    for( int i=0; i<m_totalVertices; ++i )
    {
      verts[i][0] = ( frame->vertices[i].v[0] * frame->scale[0] + frame->translate[0] );
      verts[i][1] = ( frame->vertices[i].v[1] * frame->scale[1] + frame->translate[1] );
      verts[i][2] = ( frame->vertices[i].v[2] * frame->scale[2] + frame->translate[2] );

      normals[i] = frame->vertices[i].lightNormalIndex;
    }
  }

  delete[] frameBuffer;

  fileStream.close();
  return true;
}

void MeshMd2::drawMesh(float _time)
{
  glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

    renderFrame();
  glPopMatrix();
}

void MeshMd2::drawFrame(int _frame)
{

}

void MeshMd2::setAnimation(int _type)
{

}

void MeshMd2::animate(float _time)
{

}

void MeshMd2::processLighting()
{

}

void MeshMd2::interp(fVec3* _vertList)
{
  for(int i=0; i<m_totalVertices; ++i)
  {
    _vertList[i][0] = m_Vertices[i+(m_totalVertices*m_anim.currentFrame)][0]*m_scale;
    _vertList[i][1] = m_Vertices[i+(m_totalVertices*m_anim.currentFrame)][1]*m_scale;
    _vertList[i][2] = m_Vertices[i+(m_totalVertices*m_anim.currentFrame)][2]*m_scale;
  }
}

void MeshMd2::renderFrame()
{

}
