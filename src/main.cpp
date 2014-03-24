#include "GLinc.h"
#include <SDL.h>

#include <iostream>
#include <cstdlib>
#include "GLFunctions.h"
#include "model.h"
#include "World.h"

#include "BoundingBox.h"

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

  // now clear the screen and swap whilst NGL inits (which may take time)
  Vec4 red(1,0,0);
  Vec4 green(0,1,0);
  Vec4 blue(0,0,1);
  Vec4 white(1,1,1);
  Vec4 yellow(1,1,0);

  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  GLFunctions::perspective(75,float(1024/720),0.01,500);
  GLFunctions::lookAt(Vec4(0,4,4),Vec4(-4,2,0),Vec4(0,1,0));

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

  unsigned int delay = 30;

  // Exit if we fail to load for some reason
  // Maybe add exceptions here
  if ( !Game::World::instance().init() )
      return EXIT_FAILURE;

  Game::BBox a(-1,-1,-1,1,1,1);
  //Game::BBox b(0.5,-0.5,-0.5,1.5,1.5,1.5);
  Game::BBox b(-2,-2,-2, 3,3,3);

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

//            case SDLK_w : offset.x++; break;
//            case SDLK_s : offset.x--; break;
//            case SDLK_a : offset.angle += 5; break;
//            case SDLK_d : offset.angle -= 5; break;
            case SDLK_PAGEUP : break;
            case SDLK_PAGEDOWN : break;

            default : break;
          } // end of key process
        } // end of keydown*
        default : break;
      } // end of event switch
    } // end of poll events    

    if( Game::World::instance().getCurrentTime() > (Game::World::instance().getLastTime()  + delay))
    {
      Game::World::instance().updateTime();
      Game::World::instance().draw();

      a.draw();
      b.draw();
      if(a.checkCollision(b))
      {
        Vec4 offset(a.intersectionAmount(b));
        a.move( offset*-1 );
        std::cout << offset*-1;
      }


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
