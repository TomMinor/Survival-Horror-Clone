#include "GLinc.h"
#include <SDL.h>

#include <iostream>
#include <cstdlib>
#include "GLFunctions.h"
#include "model.h"
#include "World.h"

#include "3dUtilities.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "md2.h"

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);

int main()
{
  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
  }

  // now get the size of the display and create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // now create our window
  SDL_Window *window=SDL_CreateWindow("SDLGL",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      rect.w/2,
                                      rect.h/2,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext=createOpenGLContext(window);
   if(!glContext)
   {
     SDLErrorExit("Problem creating OpenGL context");
   }
   // make this our current GL context (we can have more than one window but in this case not)
   SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);

  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  GLFunctions::perspective(75,float(800/600),0.01,500);

  SDL_GL_SwapWindow(window);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColor3f(1,1,0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;

  const uint worldUpdateDelay = 30;

  Game::World world;

  // Exit if we fail to load for some reason
  if ( !world.init() )
      return EXIT_FAILURE;

  // Read input
  const Uint8* keystate = SDL_GetKeyboardState(0);

//  Game::Camera current(Vec4(-6.005612475588933f, 8.97664525811908f, 24.51080654590031f),
//                       Vec4(-14.757877717629409f, -12.537379551671203f, 0.0f),
//                       53.953475f);

  Game::Camera current(Vec4(0,-2,-4), Vec4(-58, -15, -2), 50);

  MeshMd2 test("assets/actor/monkey.md2", "assets/actor/brute.jpg", 0.02f);
  test.setAnimation(MeshMd2::BOOM);

  float i = 0.0f;

  while(!quit)
  {
    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_o : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_p : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_b : world.toggleBBox();
            default : break;
          } // end of key process
        } // end of keydown*
        default : break;
      } // end of event switch
    } // end of poll events

    if( world.getElapsedTime() >= worldUpdateDelay)
    {
      if(keystate[SDL_SCANCODE_UP])           { world.playerWalk(0.1);    }
      if(keystate[SDL_SCANCODE_DOWN])         { world.playerWalk(-0.1);   }
      if(keystate[SDL_SCANCODE_RIGHT])        { world.playerTurn(-4);     }
      if(keystate[SDL_SCANCODE_LEFT])         { world.playerTurn(4);      }
      if(keystate[SDL_SCANCODE_LSHIFT])       { world.playerDash();       }

      if(keystate[SDL_SCANCODE_PAGEDOWN])     { world.prevRoom();       }
      if(keystate[SDL_SCANCODE_PAGEUP])       { world.nextRoom();       }

      //std::cerr << fmod(yaw, 360.0) << "\n";

      // x: 270 & 90
      // z: 0 & 180

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //util::drawWorldAxis();

      i+=0.1;
      glPushMatrix();
        glTranslatef(1.0f, 0.0f, 0.5*sin(i));
        test.drawMesh(i);
      glPopMatrix();

      current.setView();

      world.update();
      world.draw();

      // swap the buffers
      SDL_GL_SwapWindow(window);
    }

  }

  // now tidy up and exit SDL
  SDL_Quit();
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
  #ifdef DARWIN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
