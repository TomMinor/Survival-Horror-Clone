#include "md2.h"

#include <fstream>
#include "GLinc.h"

/*
 * Modified from http://tfc.duke.free.fr/old/models/md2.htm
 */

#define SHADEDOT_QUANT  16

const MeshMd2::fVec3 MeshMd2::m_normals[NUMVERTEXNORMALS] = {
  //#include "anorms.h";
};

const float MeshMd2::m_normalsDot[SHADEDOT_QUANT][256] = {
  //#include "anormstab.h";
};

static const float* shadeDots = MeshMd2::m_normalsDot[0];  // Used by process lighting
static MeshMd2::fVec3  lColour;     // Final light colour

///////////////////////////////////////////

MeshMd2::fVec3  gLightColor   = {1.0f, 1.0f, 1.0f};
int             gAmbientLight = 32;
float           gShadeLight   = 128;
float           gAngle        = 0.0f;

//////////////////////////////////////////////

#define MAX_MD2_VERTS 2048

const MeshMd2::anim MeshMd2::m_animList[21] =
{
  // Frame 0,   Frame n,  FPS

  {   0,  39,  9 },   // STAND
  {  40,  45, 10 },   // RUN
  {  46,  53, 10 },   // ATTACK
  {  54,  57,  7 },   // PAIN_A
  {  58,  61,  7 },   // PAIN_B
  {  62,  65,  7 },   // PAIN_C
  {  66,  71,  7 },   // JUMP
  {  72,  83,  7 },   // FLIP
  {  84,  94,  7 },   // SALUTE
  {  95, 111, 10 },   // FALLBACK
  { 112, 122,  7 },   // WAVE
  { 123, 134,  6 },   // POINT
  { 135, 153, 10 },   // CROUCH_STAND
  { 154, 159,  7 },   // CROUCH_WALK
  { 160, 168, 10 },   // CROUCH_ATTACK
  { 196, 172,  7 },   // CROUCH_PAIN
  { 173, 177,  5 },   // CROUCH_DEATH
  { 178, 183,  7 },   // DEATH_FALLBACK
  { 184, 189,  7 },   // DEATH_FALLFORWARD
  { 190, 197,  7 },   // DEATH_FALLBACKSLOW
  { 198, 198,  5 },   // BOOM
};

typedef enum
{
  STAND,
  RUN,
  ATTACK,
  PAIN_A,
  PAIN_B,
  PAIN_C,
  JUMP,
  FLIP,
  SALUTE,
  FALLBACK,
  WAVE,
  POINT,
  CROUCH_STAND,
  CROUCH_WALK,
  CROUCH_ATTACK,
  CROUCH_PAIN,
  CROUCH_DEATH,
  DEATH_FALLBACK,
  DEATH_FALLFORWARD,
  DEATH_FALLBACKSLOW,
  BOOM,

  MAX_ANIMATIONS
} animType;

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
  if(_time>0.0f) { animate(_time ); }

  glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

    renderFrame();
  glPopMatrix();
}

void MeshMd2::drawFrame(int _frame)
{
  m_anim.currentAnim.firstFrame = _frame;
  m_anim.currentAnim.lastFrame  = _frame;
  m_anim.nextFrame = _frame;
  m_anim.currentAnim.fps = 1;
  m_anim.type  = -1;

  drawMesh(1.0f);
}

void MeshMd2::setAnimation(int _type)
{
  if((_type<0) || (_type>MAX_ANIMATIONS))
  {
    _type=0;
  }

  m_anim.currentAnim.firstFrame = m_animList[_type].firstFrame;
  m_anim.currentAnim.lastFrame  = m_animList[_type].lastFrame;
  m_anim.currentAnim.fps        = m_animList[_type].fps;
  m_anim.nextFrame = m_animList[_type].firstFrame+1;
  m_anim.type= _type;
}

void MeshMd2::animate(float _time)
{
  m_anim.currentTime = _time;

  if(m_anim.currentTime - m_anim.lastTime > (1.0f/m_anim.currentAnim.fps))
  {
    //m_anim.currentFrame = m_anim.nextFrame++;
    m_anim.currentFrame = m_anim.nextFrame;
    m_anim.nextFrame++;

    if(m_anim.nextFrame > m_anim.currentAnim.lastFrame)
    {
      m_anim.nextFrame = m_anim.currentAnim.firstFrame;
    }

    m_anim.lastTime = m_anim.currentTime;
  }

  if(m_anim.currentFrame > (m_totalFrames-1)) { m_anim.currentFrame = 0; }
  if(m_anim.nextFrame > (m_totalFrames-1))    { m_anim.nextFrame = 0; }

  m_anim.interp = m_anim.currentAnim.fps * (m_anim.currentTime - m_anim.lastTime);
}

void MeshMd2::processLighting()
{
  float lightVar = static_cast<float>((gShadeLight + gAmbientLight)/256.0f);

  lColour[0] = gLightColor[0] + lightVar;
  lColour[1] = gLightColor[1] + lightVar;
  lColour[2] = gLightColor[2] + lightVar;

  shadeDots = m_normalsDot[static_cast<int>((gAngle*(SHADEDOT_QUANT/360.0f))) & (SHADEDOT_QUANT-1)];
}

void MeshMd2::interp(fVec3* _vertList)
{
  fVec3* currentV;
  fVec3* nextV;

  currentV = &m_Vertices[m_totalVertices*m_anim.currentFrame];
  nextV    = &m_Vertices[m_totalVertices*m_anim.nextFrame];

  for(int i=0; i<m_totalVertices; ++i)
  {
    _vertList[i][0] = (currentV[i][0] + m_anim.interp * (nextV[i][0] - currentV[i][0]) * m_scale);
    _vertList[i][1] = (currentV[i][1] + m_anim.interp * (nextV[i][1] - currentV[i][1]) * m_scale);
    _vertList[i][2] = (currentV[i][2] + m_anim.interp * (nextV[i][2] - currentV[i][2]) * m_scale);
  }
}

void MeshMd2::renderFrame()
{
  // Since the array size is constant each time drawFrame is called,
  //it is declared static so it persists between calls
  static fVec3  vertList[MAX_MD2_VERTS];
  int*          triCmds = m_GLcmds;

  // Reverse face orientation to counter glCmd's clockwise winding
  glPushAttrib(GL_POLYGON_BIT);
  glFrontFace(GL_BACK);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  processLighting();

  interp(vertList);

  glBindTexture(GL_TEXTURE_2D, m_texID);

  // Draw triangles
  while(int i = *(triCmds++))
  {
    if(i<0)
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
      //  triCmds[0] : U
      //  triCmds[1] : V
      //  triCmds[2] : Vtx id

      float l = shadeDots[m_lightNormals[triCmds[2]]];

      // Pseudo vertex lighting
      glColor3f( l*lColour[0], l*lColour[1], l*lColour[2] );

      glTexCoord2f( reinterpret_cast<float*>(triCmds)[0],
                    reinterpret_cast<float*>(triCmds)[1] );

      // GL lighting
      glNormal3fv( m_normals[m_lightNormals[triCmds[2]]] );

      glVertex3fv(vertList[triCmds[2]]);
    }

    glEnd();
  }

  glDisable(GL_CULL_FACE);
  glPopAttrib();
}
