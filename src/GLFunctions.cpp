#include "GLFunctions.h"
#include <iostream>
#include "Vec4.h"
#include "Mat4.h"
#include <cmath>
#include <cstdlib>

void GLFunctions::cube(GLfloat _w, GLfloat _h, GLfloat _d)
{
  // assume the  cube centered at the origin
  GLfloat w=_w/2.0f;
  GLfloat h=_h/2.0f;
  GLfloat d=_d/2.0f;


  glBegin(GL_QUADS);
    // front face
    glNormal3f(0,0,1);
    glVertex3f(-w,h,d);
    glVertex3f(w,h,d);
    glVertex3f(w,-h,d);
    glVertex3f(-w,-h,d);
    // back face
    glNormal3d(0,0,-1);
    glVertex3f(-w,h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,-h,-d);
    glVertex3f(-w,-h,-d);
    // Left face
    glNormal3f(1,0,0);
    glVertex3f(-w,-h,d);
    glVertex3f(-w,-h,-d);
    glVertex3f(-w,h,-d);
    glVertex3f(-w,h,d);
    // Right face
    glNormal3f(-1,0,0);
    glVertex3f(w,-h,d);
    glVertex3f(w,-h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,h,d);
    // Top face
    glNormal3f(0,1,0);
    glVertex3f(-w,h,d);
    glVertex3f(-w,h,-d);
    glVertex3f(w,h,-d);
    glVertex3f(w,h,d);
    // Bottom face
    glNormal3f(0,-1,0);
    glVertex3f(-w,-h,d);
    glVertex3f(-w,-h,-d);
    glVertex3f(w,-h,-d);
    glVertex3f(w,-h,d);

  glEnd();
}

void GLFunctions::lookAt(Vec4 _eye, Vec4 _look, Vec4 _up)
{
  Vec4 n =   _look-_eye;
  Vec4 u = _up;
  Vec4 v = n.cross(u);
  u = v.cross(n);
  n.normalize();
  v.normalize();
  u.normalize();

  Mat4 mv(1.0f);
  mv.m_00= v.m_x;
  mv.m_10= v.m_y;
  mv.m_20= v.m_z;
  mv.m_01= u.m_x;
  mv.m_11= u.m_y;
  mv.m_21= u.m_z;
  mv.m_02=-n.m_x;
  mv.m_12=-n.m_y;
  mv.m_22=-n.m_z;
  mv.m_30=-_eye.dot(v);
  mv.m_31=-_eye.dot(u);
  mv.m_32= _eye.dot(n);
  mv.loadModelView();
}


void GLFunctions::perspective(float _fovy,float _aspect, float _zNear, float _zFar)
{
  float range = tan(radians(_fovy / 2.0)) * _zNear;
  float left = -range * _aspect;
  float right = range * _aspect;
  float bottom = -range;
  float top = range;
  Mat4 result;
  result.m_00 = (2.0f * _zNear) / (right - left);
  result.m_11 = (2.0f * _zNear) / (top - bottom);
  result.m_22 = - (_zFar + _zNear) / (_zFar - _zNear);
  result.m_23 = - 1.0f;
  result.m_32 = - (2.0f* _zFar * _zNear) / (_zFar - _zNear);
  result.loadProjection();
}

 float GLFunctions::radians(float _deg )
{
  return (_deg/180.0f) * M_PI;
}



 void GLFunctions::sphere(float _radius, int _precision )
 {
  //  Sphere code based on a function Written by Paul Bourke.
  //  http://astronomy.swin.edu.au/~pbourke/opengl/sphere/
  // the next part of the code calculates the P,N,UV of the sphere for tri_strips

  float theta1 = 0.0;
  float theta2 = 0.0;
  float theta3 = 0.0;
  Vec4 normal;
  Vec4 vertex;
  glBegin(GL_TRIANGLE_STRIP);

  if( _radius < 0 )
   {
       _radius = -_radius;
   }
   // Disallow a negative number for _precision.
   if( _precision < 4 )
   {
       _precision = 4;
   }
  // now fill in a vertData structure and add to the data list for our sphere
  for( int i = 0; i < _precision/2; ++i )
  {
   theta1 = i * TWO_PI / _precision - PI2;
   theta2 = (i + 1) * TWO_PI / _precision - PI2;

   for( int j = 0; j <= _precision; ++j )
   {
     theta3 = j * TWO_PI / _precision;
     normal.set(cosf(theta2) * cosf(theta3),sinf(theta2),cosf(theta2) * sinf(theta3));
     vertex=normal*_radius;
     normal.normalGL();
     vertex.vertexGL();
     // vert / normal 2
     normal.set(cosf(theta1) * cosf(theta3),sinf(theta1),cosf(theta1) * sinf(theta3));
     vertex=normal*_radius;
     normal.normalGL();
     vertex.vertexGL();
    } // end inner loop
  }// end outer loop
  glEnd();
  // end citation
 }



 void GLFunctions::capsule(float _radius,  float _height,   int _precision  )

 {
  // based on code from here
  // https://code.google.com/p/rgine/source/browse/trunk/RGine/opengl/src/RGLShapes.cpp
  /// @todo add UV's at some stage
  glBegin(GL_TRIANGLES);
  float x,y,z,nx,ny,nz;
  float h = _height / 2;
  float s, c, s1, c1, o;
  float sb, cb, sb1, cb1;
  float ang = (1.0 / _precision) * (M_PI);
  Vec4 normal,vertex;
  for (int i = 0; i < 2 * _precision; ++i)
  {

    c = _radius * cos(ang * i);
    c1= _radius * cos(ang * (i + 1));
    s = _radius * sin(ang * i);
    s1 =_radius * sin(ang * (i + 1));

    //side top
    vertex.set(c1,h,s1);
    normal.set(c1,0.0f,s1);
    normal.normalGL();
    vertex.vertexGL();

    vertex.set(c,h,s);
    normal.set(c,0.0f,s);
    normal.normalGL();
    vertex.vertexGL();

    vertex.set(c,-h,s);
    normal.set(c,0.0f,s);
    normal.normalGL();
    vertex.vertexGL();


    //side bot
    // same vert again
    vertex.vertexGL();

    vertex.set(c1,-h,s1);
    normal.set(c1,0.0f,s1);
    normal.normalGL();
    vertex.vertexGL();


    vertex.set(c1,h,s1);
    normal.set(c1,0.0f,s1);
    normal.normalGL();
    vertex.vertexGL();
  // top and bottom caps are the 2 halves of the sphere translated.

  for (int j = 0; j <=_precision; ++j)
  {
    if (j < _precision / 2)
    {
      o = h;
    }
    else
    {
      o = -h;
    }
    //longitude
    s = -sin(ang * i);
    s1 = -sin(ang * (i + 1));
    c = cos(ang * i);
    c1 = cos(ang * (i + 1));
    //latitude
    sb = _radius * sin(ang * j);
    sb1 = _radius * sin(ang * (j + 1));
    cb = _radius * cos(ang * j);
    cb1 = _radius * cos(ang * (j + 1));
    if (j != _precision - 1)
    {
     vertex.set(sb*c,cb+o,sb*s);
     normal=vertex;
     normal.normalGL();
     vertex.vertexGL();

     vertex.set(sb1*c,cb1+o,sb1*s);
     normal=vertex;
     normal.normalGL();
     vertex.vertexGL();

     vertex.set(sb1*c1,cb1+o,sb1*s1);
     normal=vertex;
     normal.normalGL();
     vertex.vertexGL();
    }
    if (j != 0)
    {
      vertex.set(sb*c,cb+o,sb*s);
      normal=vertex;
      normal.normalGL();
      vertex.vertexGL();

      vertex.set(sb1*c1,cb1+o,sb1*s1);
      normal=vertex;
      normal.normalGL();
      vertex.vertexGL();

      nx=x = sb * c1;
      ny=y = cb + o;
      nz=z = sb * s1;
      vertex.set(sb*c1,cb+o,sb*s1);
      normal=vertex;
      normal.normalGL();
      vertex.vertexGL();
    }
    }
  }
 glEnd();
  // end citation

 }



 /*----------------------------------------------------------------------------------------------------------------------
  * Compute lookup table of cos and sin values forming a cirle
  * borrowed from free glut implimentation of primitive drawing
  *
  * Notes:
  *    It is the responsibility of the caller to free these tables
  *    The size of the table is (n+1) to form a connected loop
  *    The last entry is exactly the same as the first
  *    The sign of n can be flipped to get the reverse loop
  */
 //----------------------------------------------------------------------------------------------------------------------
/// borrowed from glut source
 void GLFunctions::fghCircleTable(double **io_sint, double **io_cost, const int _n  )
 {
   int i;
   /* Table size, the sign of n flips the circle direction */
   const int size = abs(_n);
   /* Determine the angle between samples */
   const double angle = 2*M_PI/(double)( ( _n == 0 ) ? 1 : _n );

   /* Allocate memory for n samples, plus duplicate of first entry at the end */
   *io_sint = new double[size+1];
   *io_cost = new double[size+1];

   /* Compute cos and sin around the circle */
   (*io_sint)[0] = 0.0;
   (*io_cost)[0] = 1.0;

   for (i=1; i<size; ++i)
   {
     (*io_sint)[i] = sin(angle*i);
     (*io_cost)[i] = cos(angle*i);
   }
   /* Last sample is duplicate of the first */
   (*io_sint)[size] = (*io_sint)[0];
   (*io_cost)[size] = (*io_cost)[0];
 }
// end citation

 void GLFunctions::cylinder( float _radius,const float _height, int _slices, int _stacks )
 {
  /* Step in z and radius as stacks are drawn. */

  double z0,z1;
  const double zStep = _height / ( ( _stacks > 0 ) ? _stacks : 1 );

  /* Pre-computed circle */

  double *sint,*cost;
  Vec4 vertex;
  Vec4 normal;
  fghCircleTable(&sint,&cost,-_slices);
  /* Do the stacks */
  z0 = 0.0;
  z1 = zStep;
  glBegin(GL_TRIANGLES);
    for(int i=1; i<=_stacks+1; ++i )
    {
      if(i==_stacks)
      {
        z1 = _height;
      }
      for(int j=0; j<=_slices-1; ++j)
      {
        // vert 1;
        normal.set(sint[j],cost[j],0.0f);
        vertex.set(sint[j]*_radius,cost[j]*_radius,-z0/2.0);
        normal.normalGL();
        vertex.vertexGL();
        // vert 2
        vertex.set(sint[j]*_radius,cost[j]*_radius,-z1/2.0);
        vertex.vertexGL();

        vertex.set(sint[j+1]*_radius,cost[j+1]*_radius,-z0/2.0);
        normal.set(sint[j+1],cost[j+1],0.0f);
        normal.normalGL();
        vertex.vertexGL();

        vertex.set(sint[j+1]*_radius,cost[j+1]*_radius,-z0/2.0);
        normal.set(sint[j+1],cost[j+1],0.0f);
        normal.normalGL();
        vertex.vertexGL();

        vertex.set(sint[j]*_radius,cost[j]*_radius,-z1/2.0);
        normal.set(sint[j],cost[j],0.0f);
        normal.normalGL();
        vertex.vertexGL();

        vertex.set(sint[j+1]*_radius,cost[j+1]*_radius,-z1/2.0);
        normal.set(sint[j+1],cost[j+1],0.0f);
        normal.normalGL();
        vertex.vertexGL();

      }
    z0 = z1; z1 += zStep;
    }
  glEnd();
  /* Release sin and cos tables */

  delete [] sint;
  delete [] cost;
 }

 void GLFunctions::cone(float _base,float _height,  int _slices, int _stacks  )
 {
  /* Step in z and radius as stacks are drawn. */
  double z0,z1;
  double r0,r1;

  const double zStep = _height / ( ( _stacks > 0 ) ? _stacks : 1 );
  const double rStep = _base / ( ( _stacks > 0 ) ? _stacks : 1 );


  /* Pre-computed circle */
  glBegin(GL_TRIANGLE_STRIP);
  double *sint,*cost;
  fghCircleTable(&sint,&cost,-_slices);
  z0 = 0.0;
  z1 = zStep;

  r0 = _base;
  r1 = r0 - rStep;

  float phi = (float)atan(_base/_height);
  float sphi= (float)sin(phi);
  Vec4 vertex;
  Vec4 normal;

  for(int i=0; i<_stacks; i++ )
  {
    for(int j=0; j<=_slices; j++)
    {
      float theta = j == _slices ? 0.f : (float) j / _slices * TWO_PI;
      float ctheta = (float)cos(theta);
      float stheta = (float)sin(theta);

      vertex.set(cost[j]*r0,sint[j]*r0,z0);
      normal.set(ctheta,-stheta,sphi);
      normal.normalGL();
      vertex.vertexGL();
      // nornal the same so just set vertex
      vertex.set(cost[j]*r1,sint[j]*r1,z1);
      vertex.vertexGL();
    }
    z0 = z1; z1 += zStep;
    r0 = r1; r1 -= rStep;

  }
   glEnd();
  // Release sin and cos tables

  delete [] sint;
  delete [] cost;
 }


 void GLFunctions::disk(float _radius,  int _slices )
 {
     /* Pre-computed circle */
     double *sint,*cost;

     fghCircleTable(&sint,&cost,-_slices);
     // as were using a triangle fan its  vert at the center then
     // the points
     glBegin(GL_TRIANGLE_FAN);
     // as we are doing a tri fan this is the center
    Vec4 normal(0.0f,0.0f,1.0f);
    Vec4 vertex(0.0f,0.0f,0.0f);
    normal.normalGL();
    vertex.vertexGL();

     for (int j=0; j<=_slices; ++j)
     {
      // normals set above
      vertex.set(cost[j]*_radius,sint[j]*_radius,0.0f);
      vertex.vertexGL();
     }
     glEnd();
     /* Release sin and cos tables */
     delete [] sint;
     delete [] cost;
 }



 void GLFunctions::torus( float _minorRadius, float _majorRadius,int _nSides, int _nRings)
 {
  float  iradius = _minorRadius, oradius = _majorRadius, phi, psi, dpsi, dphi;

  float spsi, cpsi, sphi, cphi ;

  if ( _nSides < 1 ) { _nSides = 1; }
  if ( _nRings < 1 ) { _nRings = 1; }

  /* Increment the number of sides and rings to allow for one more point than surface */
  _nSides ++ ;
  _nRings ++ ;
  Vec4 *vertexTable = new Vec4[_nSides * _nRings+1];
  Vec4 *normalTable = new Vec4[_nSides * _nRings+1];
  dpsi =  2.0 * M_PI / (double)(_nRings - 1) ;
  dphi = -2.0 * M_PI / (double)(_nSides - 1) ;
  psi  = 0.0;
  // pre compute the values for the torus
  for(int j=0; j<_nRings; ++j )
  {
    cpsi = cos ( psi ) ;
    spsi = sin ( psi ) ;
    phi = 0.0;
    for(int i=0; i<_nSides; ++i )
    {
      int offset = 3 * ( j * _nSides + i ) ;
      cphi = cos ( phi ) ;
      sphi = sin ( phi ) ;
      vertexTable[offset].set(cpsi * ( oradius + cphi * iradius ),spsi * ( oradius + cphi * iradius ), sphi * iradius);
      normalTable[offset].set(cpsi * cphi,spsi * cphi,sphi);
      phi += dphi;
    } // end of _nSides loop
    psi += dpsi;
   }  // end of _nRings loop
   // now draw
   glBegin(GL_TRIANGLES);
    for(int i=0; i<_nSides-1; ++i )
    {
      for(int j=0; j<_nRings-1; ++j )
      {
        int offset = 3 * ( j * _nSides + i ) ;
        normalTable[offset].normalGL();
        vertexTable[offset].vertexGL();
        //V1
        normalTable[offset+3].normalGL();
        vertexTable[offset+3].vertexGL();

        normalTable[offset + 3 * _nSides + 3].normalGL();
        vertexTable[offset + 3 * _nSides + 3].vertexGL();


        normalTable[offset].normalGL();
        vertexTable[offset].vertexGL();


        normalTable[offset + 3 * _nSides + 3].normalGL();
        vertexTable[offset + 3 * _nSides + 3].vertexGL();

        normalTable[offset + 3 * _nSides].normalGL();
        vertexTable[offset + 3 * _nSides ].vertexGL();

      } // end _nRings
    } // end _nSides
  glEnd();
  // now clean up
  delete [] vertexTable;
  delete [] normalTable;

 }

