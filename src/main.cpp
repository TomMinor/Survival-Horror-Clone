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
#include "Actor.h"

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

  // Use a 4:3 aspect ratio to match the backgrounds
  SDL_Rect windowBounds = {0, 0, 800, 600};

  //SDL_GetDisplayBounds(0,&rect);
  // now create our window
  SDL_Window *window=SDL_CreateWindow("SDLGL",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      windowBounds.w,
                                      windowBounds.h,
                                      SDL_WINDOW_OPENGL
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
  //GLFunctions::perspective(90, float(4/3), 0.01, 500);

  GLFunctions::perspective(90, float(800.0f/600.0f), 0.01, 500);
  SDL_GL_SwapWindow(window);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColor3f(1,0,1);


  // Flag to indicate if we need to exit
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

  Vec4 position(-1.3169258356997249, 1.9582303618833887, 1.785959369300033);
  Vec4 rotation(0.0, -7.545061101644754, 0.0);

//  Vec4 position(1.7525374190842373, 1.624190772456883, 1.154906337937287);
//  Vec4 rotation(0.0, 86.090, 0.0);

  Game::Camera tits(  Vec4(-position.m_x, -position.m_y, -position.m_z),
                      Vec4(-rotation.m_y, -rotation.m_x, -rotation.m_z),
                      90.0f);

//  Game::Actor test(Vec4(1.0f, 1.0f, 1.0f), Vec4());
//  test.forceAnimation(Md2::Animation::DEATH_IDLE);
  float time = 0.0f;
  Vec4 cameraPos;

  //float tmp[] = { 0.6962864869302063, 0.6851291113894016, 0.21397015877257444, 0.0, -0.3256486830215435, 0.5672002014874894, -0.7564633941433556, 0.0, -0.6396390101961622, 0.4570361387891777, 0.6180452285035081, 0.0, 10.019665073440814, 7.789568673257114, 9.253249521312483, 1.0 };
  //float tmp[] = { 0.952889998609198, 0.0, 0.303316090161011, 0.0, -0.0, 1.0, 0.0, 0.0, -0.303316090161011, -0.0, 0.952889998609198, 0.0, 0.0, 0.0, -2.1060597064697046, 1.0 };
  //float tmp[] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -0.8768467576273767, 1.3902824917133552, 1.7579394038775344, 1.0} ;
//  Mat4  tmp2;
//  memcpy(tmp2.m_openGL, tmp, sizeof(tmp));

//  tmp2.m_m[2][3] *= -1;
//  tmp2.m_m[3][2] *= -1;

  //tmp2.transpose();

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
            case SDLK_b : world.toggleBBox(); break;
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
      if(keystate[SDL_SCANCODE_PAGEUP])       { world.nextRoom(); }

//      if(keystate[SDL_SCANCODE_W])            { cameraPos.m_z++;   }
//      if(keystate[SDL_SCANCODE_S])            { cameraPos.m_z--;   }
//      if(keystate[SDL_SCANCODE_Q])            { cameraPos.m_y++;   }
//      if(keystate[SDL_SCANCODE_E])            { cameraPos.m_y--;   }
//      if(keystate[SDL_SCANCODE_D])            { cameraPos.m_x++;   }
//      if(keystate[SDL_SCANCODE_A])            { cameraPos.m_x--;   }
//      if(keystate[SDL_SCANCODE_J])        { current.setView();   }
      //if(keystate[SDL_SCANCODE_K])        { tits.setView();   }
      //std::cerr << fmod(yaw, 360.0) << "\n";
      // x: 270 & 90
      // z: 0 & 180

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      tits.setView();

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
