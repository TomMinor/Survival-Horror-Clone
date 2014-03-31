#ifndef MD2_H
#define MD2_H

/*
 * Modified from http://tfc.duke.free.fr/old/models/md2.htm
 * Split into 2 separate classes, Loader and Mesh
 */

namespace Md2
{

// Used to verify the file we're loading is the correct type/version
const int IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');  //'IDP2'
const int VERSION = 8;

// Number of precaculated normals (in "anorms.h")
const int TOTALVERTEXNORMALS = 162;
// Number of precaculated dot products (in "anormtab.h")
const int SHADEDOT_QUANT = 16;

typedef float Vec3[3];  // Simple float Vec3

typedef struct
{
  int firstFrame;
  int lastFrame;
  int fps;
} anim;

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

struct Header
{
  int ident;          // 'IDP2'
  int version;

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
  int frameOffset;
  int glCmdsOffset;
  int offsetEof;      // Unused

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

}

#endif // MD2_H
