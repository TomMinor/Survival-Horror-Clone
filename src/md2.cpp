#include "md2.h"

#include <fstream>
#include "GLinc.h"

/*
 * Modified from http://tfc.duke.free.fr/old/models/md2.htm
 */



// Pre-calculated tables
const MeshMd2::fVec3 MeshMd2::m_normals[NUMVERTEXNORMALS] = {
  #include "anorms.h"
};
const float MeshMd2::m_normalsDot[SHADEDOT_QUANT][256] = {
  #include "anormtab.h"
};

static const float* shadeDots = MeshMd2::m_normalsDot[0];  // Used by process lighting
static MeshMd2::fVec3  lightColour;     // Final light colour

const MeshMd2::anim MeshMd2::m_animList[21] =
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

  fileStream.open(_filename.c_str(), std::ios::in | std::ios::binary);
  if( !fileStream.is_open() )
  {
    return false;
  }

  // Store the header
  fileStream.read( reinterpret_cast<char*>(&header), sizeof(MD2Header) );

  // Check the file's header signature to make sure this is a valid md2 file
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
  char*  keyFrameBuffer  = new char[ header.frameSize * m_totalFrames ];
  m_Vertices      = new fVec3[ m_totalVertices * m_totalFrames ];
  m_lightNormals  = new int[ m_totalVertices * m_totalFrames ];
  m_GLcmds        = new int[ m_totalGLcmds ];

  // Fill the keyFrameBuffer with an array of (KeyFrame) data from the file at header.offsetFrames,
  // each element will be cast to type KeyFrame later and used to transform the vertices into each frame
  fileStream.seekg( header.frameOffset, std::ios::beg );
  fileStream.read( reinterpret_cast<char*>(keyFrameBuffer),
                   m_totalFrames*header.frameSize );

  // Read GL commands, which will be used when rendering to decide on the most optimal draw method (FAN or STRIP)
  fileStream.seekg( header.glCmdsOffset, std::ios::beg );
  fileStream.read( reinterpret_cast<char*>(m_GLcmds),
                   m_totalGLcmds*(sizeof(m_GLcmds[0])) );

  // Decompress the vertex positions for each frame by transforming them
  // using the translation/scale of the KeyFrame
  for( int j=0; j<m_totalFrames; ++j )
  {
    int frameIndex  = m_totalVertices*j;
    int* normals    = &m_lightNormals[frameIndex];
    fVec3* verts    = &m_Vertices[frameIndex];
    KeyFrame* frame = reinterpret_cast<KeyFrame*>(&keyFrameBuffer[header.frameSize*j]);

    for( int i=0; i<m_totalVertices; ++i )
    {
      verts[i][0] = ( frame->vertices[i].v[0] * frame->scale[0] + frame->translate[0] );
      verts[i][1] = ( frame->vertices[i].v[1] * frame->scale[1] + frame->translate[1] );
      verts[i][2] = ( frame->vertices[i].v[2] * frame->scale[2] + frame->translate[2] );

      normals[i] = frame->vertices[i].lightNormalIndex;
    }
  }

  delete[] keyFrameBuffer;

  fileStream.close();
  return true;
}

void MeshMd2::drawMesh(float _time)
{
  if(_time>0.0f) { animate(_time ); }

  glPushMatrix();
    // Correct the mesh rotation
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

    renderFrame();
  glPopMatrix();
}

//void MeshMd2::drawFrame(int _frame)
//{
//  m_anim.currentAnim.firstFrame = _frame;
//  m_anim.currentAnim.lastFrame  = _frame;
//  m_anim.nextFrame = _frame;
//  m_anim.currentAnim.fps = 1;
//  m_anim.type  = -1;

//  drawMesh(1.0f);
//}

void MeshMd2::setAnimation(int _type)
{
  if((_type < 0) || (_type>MAX_ANIMATIONS)) { _type=0; }

  m_anim.currentAnim.firstFrame = m_animList[_type].firstFrame;
  m_anim.currentAnim.lastFrame  = m_animList[_type].lastFrame;
  m_anim.currentAnim.fps        = m_animList[_type].fps;
  m_anim.nextFrame = m_anim.currentAnim.firstFrame + 1;

  m_anim.type= _type;
}

void MeshMd2::animate(float _time)
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

void MeshMd2::processLighting()
{
  MeshMd2::fVec3  gLightColor  = {1.0f, 1.0f, 1.0f};

  float gAngle = 180.0f;
  float gShadeLight   = 128;
  int   gAmbientLight = 32;
  float lightVar = static_cast<float>((gShadeLight + gAmbientLight)/256.0f);

  lightColour[0] = gLightColor[0] - lightVar;
  lightColour[1] = gLightColor[1] - lightVar;
  lightColour[2] = gLightColor[2] - lightVar;

  shadeDots = m_normalsDot[static_cast<int>((gAngle*(SHADEDOT_QUANT/360.0f))) & (SHADEDOT_QUANT-1)];
}

void MeshMd2::interp(fVec3* _vtxList)
{
  fVec3* crntVtx = &m_Vertices[m_totalVertices*m_anim.currentFrame];
  fVec3* nextVtx = &m_Vertices[m_totalVertices*m_anim.nextFrame];

  // Interpolate between the current & next vertex, then scale the result
  for(int i=0; i<m_totalVertices; ++i)
  {
    _vtxList[i][0] = m_scale * (crntVtx[i][0] + m_anim.interp * (nextVtx[i][0]-crntVtx[i][0]));
    _vtxList[i][1] = m_scale * (crntVtx[i][1] + m_anim.interp * (nextVtx[i][1]-crntVtx[i][1]));
    _vtxList[i][2] = m_scale * (crntVtx[i][2] + m_anim.interp * (nextVtx[i][2]-crntVtx[i][2]));
  }
}

void MeshMd2::renderFrame()
{

  fVec3  vertList[m_totalVertices];
  int*   triCmds = m_GLcmds;

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

//      float l = shadeDots[m_lightNormals[triCmds[2]]];
//      glColor3f( l*lColour[0], l*lColour[1], l*lColour[2] );
      glColor3f(1.0f, 1.0f, 1.0f);
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
