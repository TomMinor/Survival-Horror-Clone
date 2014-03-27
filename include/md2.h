#ifndef MD2_H
#define MD2_H

class MeshMd2
{
public:

private:
  struct MD2Header
  {
    char ident[4];
    int version;
    int skinWidth;
    int skinHeight;
    int frameSize;
    int numSkins;
    int numVertices;
    int numTextureCoords;
    int numTriangles;
    int numGLCmds;
    int numFrames;
    int skinOffset;
    int texCoordOffset;
    int triangleOffset;
    int fraemOffset;
    int GLCmdOffset;
    int eofOffset;
  };

  struct Skin
  {
    char name[64];
  };

  struct TexCoord
  {
    short u;
    short v;
  };


};

#endif // MD2_H
