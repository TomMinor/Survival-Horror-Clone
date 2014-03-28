#ifndef MD2_H
#define MD2_H

#include <iostream>
#include <string>
#include "Texture.h"

class MeshMd2
{
  // Used to verify the file we're loading is the correct type/version
  #define MD2_IDENT         (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')  //'IDP2'
  #define MD2_VERSION       8
  #define NUMVERTEXNORMALS  162
  #define SHADEDOT_QUANT    16

public:
  typedef float fVec3[3];  // Simple float Vec3
  typedef struct
  {
    int firstFrame;
    int lastFrame;
    int fps;
  } anim;

  MeshMd2(std::string _meshPath, std::string _texturePath) :
    m_totalFrames(0),
    m_totalVertices(0),
    m_totalGLcmds(0),
    m_Vertices(0),
    m_GLcmds(0),
    m_lightNormals(0),
    m_texID(0),
    m_scale(1.0f),
    m_skin(_texturePath)
  {
    setAnimation(0);
    loadMesh(_meshPath);
  }

  ~MeshMd2();

  bool loadMesh(std::string _filename);

  void drawMesh(float _time);
  void drawFrame(int _frame);

  void setAnimation(int _type);

private:
  void animate(float _time);
  void processLighting();
  void interp(fVec3* _vertList);
  void renderFrame();


public:
  // Precalculated arrays applicable to every md2 mesh
  static const fVec3 m_normals[ NUMVERTEXNORMALS ];
  static const float m_normalsDot[ SHADEDOT_QUANT][256];  // Dot products

  static const anim m_animList[21];

private:
  typedef struct
  {
    anim    currentAnim;

    float   currentTime;
    float   lastTime;
    float   interp;

    int     type;

    int     currentFrame;
    int     nextFrame;
  } animState;

  int   m_totalFrames;
  int   m_totalVertices;
  int   m_totalGLcmds;

  // Data arrays
  fVec3*  m_Vertices;
  int*    m_GLcmds;
  int*    m_lightNormals;

  unsigned int  m_texID;
  animState     m_anim;
  float         m_scale;

  Game::Texture m_skin;

  struct MD2Header
  {
    int ident;       // 'IDP2'
    int version;        // md2 version = 8

    int skinWidth;
    int skinHeight;
    int frameSize;

    int numSkins;
    int numVertices;
    int numUVs;
    int numTriangles;
    int numGlcmds;
    int numFrames;

    int offsetSkins;
    int offsetUV;
    int offsetTriangles;
    int offsetFrames;
    int offsetGlcmds;
    int offsetEof;
  };

  // Stores compressed verts & the (unused) normal lookup index
  typedef struct
  {
    unsigned char v[3];
    unsigned char lightNormalIndex;
  } Vertex;

  // Stores compressed coords
  typedef struct
  {
    short u;
    short v;
  } UvCoord;

  typedef struct
  {
    // Use these to decompress the Vertex data by scaling &
    // translating each keyframe
    float   scale[3];
    float   translate[3];
    char    name[16];
    Vertex  vertices[1];
  } KeyFrame;

  // Stores the vertex/UV indices for a triangle
  typedef struct
  {
    short   indexVertices[3];
    short   indexUVs[3];
  } Triangle;
};

#endif // MD2_H
