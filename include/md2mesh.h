#ifndef MD2MESH_H
#define MD2MESH_H

#include <stdexcept>
#include "md2.h"
#include "Texture.h"

namespace Md2
{

  namespace Animation
  {
  const anim animList[21] =
  {
    // Frame 0,   Frame n,  FPS
    {  11, 11, 9 },    // STAND
    {  0,  7,  5  },   // WALK
    {  0,  7,  10 },   // RUN
    {  13, 13, 10 },   // ATTACK
    {  7,  13, 10 },   // PREP_ATTACK
    {  15, 19, 6 },    // PAIN
    {  19, 25, 10 },   // DEATH
    {  25, 25, 10 },   // DEATH_IDLE
  };

  typedef enum
  {
    STAND,
    WALK,
    RUN,
    ATTACK,
    PREP_ATTACK,
    PAIN,
    DEATH,
    DEATH_IDLE,

    MAX_ANIMATIONS
  } Sequence;
  }

class Loader;

class Mesh
{
public:
  Mesh(std::string _meshPath, std::string _texturePath, float _scale=1.0f);
  ~Mesh();

  void loadMesh(std::string _filename);

  void drawMesh(float _time);

  void setAnimation(int _type);
  int currentAnimation()  { return m_anim.type; }
  int timesLooped()       { return m_loopCount; }

  // The Loader class is used to move all the file handling out of this
  // mesh class
  friend class Loader;
private:
  void animate(float _time);
  void processLighting();
  void interp(Vec3* _vertList);
  void renderFrame();

private:
  static const float* m_shadeDots; // Used by process lighting

  // Data arrays
  Vec3* m_Vertices;
  int*  m_GLcmds;
  int*  m_lightNormals;
  int   m_totalFrames;
  int   m_totalVertices;
  int   m_totalGLcmds;

  float      m_scale;
  animState  m_anim;
  int        m_loopCount;

  Game::Texture m_skin;
};

}

#endif // MD2MESH_H
