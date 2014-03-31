#include "md2mesh.h"
#include "md2loader.h"

namespace Md2
{

// Pre-calculated tables
const Vec3 m_normals[TOTALVERTEXNORMALS] = {
  #include "anorms.h"
};
static const float m_normalsDot[SHADEDOT_QUANT][256] = {
  #include "anormtab.h"
};

const float* Mesh::m_shadeDots = m_normalsDot[0];
Vec3  lightColour;     // Final light colour

Mesh::Mesh(std::string _meshPath, std::string _texturePath, float _scale) :
  m_scale(_scale),
  m_skin(_texturePath)
{
  loadMesh(_meshPath);

  memset(&m_anim, 0, sizeof(m_anim));
  setAnimation(Animation::STAND);
}

Mesh::~Mesh()
{
  delete[] m_Vertices;
  delete[] m_GLcmds;
  delete[] m_lightNormals;
}

void Mesh::loadMesh(std::string _filename)
{
  Loader model(_filename, this);
  model.load();
}

void Mesh::drawMesh(float _time)
{
  if(_time>0.0f) { animate(_time ); }

  glPushMatrix();
    // Correct the mesh rotation
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

    renderFrame();
  glPopMatrix();
}

void Mesh::setAnimation(int _type)
{
  if((_type < 0) || (_type>Animation::MAX_ANIMATIONS)) { _type=0; }

  m_anim.currentAnim.firstFrame = Animation::animList[_type].firstFrame;
  m_anim.currentAnim.lastFrame  = Animation::animList[_type].lastFrame;
  m_anim.currentAnim.fps        = Animation::animList[_type].fps;
  m_anim.nextFrame = m_anim.currentAnim.firstFrame + 1;

  m_anim.type= _type;
}

void Mesh::animate(float _time)
{
  m_anim.currentTime = _time;

  if(m_anim.currentTime - m_anim.lastTime > (1.0f/m_anim.currentAnim.fps))
  {

    m_anim.currentFrame = m_anim.nextFrame++;

    // Loop the animation
    if(m_anim.nextFrame > m_anim.currentAnim.lastFrame)
    {
       m_anim.nextFrame = m_anim.currentAnim.firstFrame;
    }

    m_anim.lastTime = m_anim.currentTime;
  }

  // Prevent the frame counters from overflowing the vertex arrays
  if(m_anim.currentFrame > (m_totalFrames-1)) { m_anim.currentFrame = 0; }
  if(m_anim.nextFrame    > (m_totalFrames-1)) { m_anim.nextFrame = 0;    }

  m_anim.interp = m_anim.currentAnim.fps * (m_anim.currentTime - m_anim.lastTime);
}

void Mesh::processLighting()
{
  Vec3  gLightColor  = {1.0f, 1.0f, 1.0f};

  float gAngle = 180.0f;
  float gShadeLight   = 128;
  int   gAmbientLight = 32;
  float lightVar = static_cast<float>((gShadeLight + gAmbientLight)/256.0f);

  lightColour[0] = gLightColor[0] + lightVar;
  lightColour[1] = gLightColor[1] + lightVar;
  lightColour[2] = gLightColor[2] + lightVar;

  m_shadeDots = m_normalsDot[static_cast<int>((gAngle*(SHADEDOT_QUANT/360.0f))) & (SHADEDOT_QUANT-1)];
}

void Mesh::interp(Vec3* _vtxList)
{
  Vec3* crntVtx = &m_Vertices[m_totalVertices*m_anim.currentFrame];
  Vec3* nextVtx = &m_Vertices[m_totalVertices*m_anim.nextFrame];

  // Interpolate between the current & next vertex, then scale the result
  for(int i=0; i<m_totalVertices; ++i)
  {
    _vtxList[i][0] = m_scale * (crntVtx[i][0] + m_anim.interp * (nextVtx[i][0]-crntVtx[i][0]));
    _vtxList[i][1] = m_scale * (crntVtx[i][1] + m_anim.interp * (nextVtx[i][1]-crntVtx[i][1]));
    _vtxList[i][2] = m_scale * (crntVtx[i][2] + m_anim.interp * (nextVtx[i][2]-crntVtx[i][2]));
  }
}

void Mesh::renderFrame()
{
  Vec3  vertList[m_totalVertices];
  int*  triCmds = m_GLcmds;

  // Reverse face orientation to counter glCmd's clockwise winding
  glPushAttrib(GL_POLYGON_BIT);
  glFrontFace(GL_BACK);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_COLOR);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  processLighting();

  interp(vertList);

  m_skin.setCurrent();

  // Draw triangles
  while(int i = *(triCmds++))
  {
    if(i < 0)
    {
      glBegin(GL_TRIANGLE_FAN);
      i = -i;
    }
    else
    {
      glBegin(GL_TRIANGLE_STRIP);
    }

    for(  ; i>0; --i, triCmds+=3)
    {
      //  triCmds[0] : texcoord U
      //  triCmds[1] : texcoord V
      //  triCmds[2] : Vtx id

      float l = m_shadeDots[m_lightNormals[triCmds[2]]];
      glColor3f( l*lightColour[0], l*lightColour[1], l*lightColour[2] );
      glTexCoord2f( reinterpret_cast<float*>(triCmds)[0],
                    reinterpret_cast<float*>(triCmds)[1] );

      // GL lighting
      glNormal3fv( m_normals[m_lightNormals[triCmds[2]]] );

      glVertex3fv(vertList[triCmds[2]]);
    }

    glEnd();
  }

  glDisable(GL_TEXTURE_2D);
  //glDisable(GL_CULL_FACE);
  glPopAttrib();
}

}
