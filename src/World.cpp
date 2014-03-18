#include "World.h"
#include "SDL2/SDL.h"
#ifdef LINUX
  #include <GL/gl.h>
#endif
#ifdef DARWIN
  #include <OpenGL/gl.h>
#endif

#include "GLFunctions.h"

namespace Game {

bool World::init()
{
  // Read assets
  //    Store in m_rooms
  return true;
}

// Draw actors and room
void World::draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // now draw etc

  glPushMatrix();
    glTranslatef(0, 0, 0.0f);
    glPushMatrix();
      glRotatef(0, 0.0f, 0.0f, 1.0f);
      GLFunctions::cube(2.0f,1.0f,1.0f);
    glPopMatrix();
  glPopMatrix();


}

// Update actor states
void World::update()
{

}

}
