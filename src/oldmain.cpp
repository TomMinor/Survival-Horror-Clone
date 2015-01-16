#include "GLinc.h"
#include <SDL.h>

#include <iostream>
#include <cstdlib>
#include "GLFunctions.h"
#include "World.h"

#include "3dUtilities.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "Actor.h"

/* TODO
 * - Add copy constructors to all classes with pointers
 * - Ensure all pointers are freed in the destructor
 * - Fix collision (duh)
 * - Document
 * - Figure out a nice way to get input
 * - Put the window size somewhere nice
 * - Make some example rooms, no need to worry about converting from mayas coordinate system
 * if there is no time, just blag it manually
 */


int main2()
{
  // rand() is later used to randomly choose colours for trigger volumes
  srand((unsigned int)time(NULL));

  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    //SDLErrorExit("Unable to initialize SDL");
  }

  // Use a 4:3 aspect ratio to match the backgrounds
  SDL_Rect windowBounds = {0, 0, 800, 600};

  SDL_Window *window=SDL_CreateWindow("Survival Horror Clone",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      windowBounds.w,
                                      windowBounds.h,
                                      SDL_WINDOW_OPENGL
                                     );
  // check to see if that worked or exit
  if (!window)
  {
    //SDLErrorExit("Unable to create window");
  }

  // Create our opengl context and attach it to our window

   SDL_GLContext glContext;//=createOpenGLContext(window);
   if(!glContext)
   {
     //SDLErrorExit("Problem creating OpenGL context");
   }

  // make this our current GL context (we can have more than one window but in this case not)
  SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);

  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

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
  Game::World* world;

  // Instantiate the world and try to load the first room,
  // if this room can't be loaded consider it a critical error and exit
  try
  {
    world = new Game::World;
  }
  catch(std::runtime_error &msg)
  {
    std::cerr << msg.what() << "\n";
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  // Read input
  const Uint8* keystate = SDL_GetKeyboardState(0);

//  Vec4 position(-1.3169258356997249, 1.9582303618833887, 1.785959369300033);
//  Vec4 rotation(0.0, -7.545061101644754, 0.0);

//  Vec4 position(6.91449,-4.21061,-2.78206);
//  Vec4 rotation(199,10,-2,1);


  Vec4 position;
  Vec4 rotation;

  Vec4 cameraPos(position);
  Vec4 cameraRot(rotation);

  Game::Camera tmpCamera(  Vec4(position.m_x, position.m_y, position.m_z),
                      Vec4(rotation.m_y, rotation.m_x, rotation.m_z),
                      90.0f);

//  Game::Actor test(Vec4(1.0f, 1.0f, 1.0f), Vec4());
//  test.forceAnimation(Md2::Animation::DEATH_IDLE);
//  float time = 0.0f;
//  Vec4 cameraPos;

  int i = 2;
  std::string rooms[] = { "ROOM_01.room",
                          "ROOM_02.room",
                          "ROOM_03.room"
                        };
  world->loadRoom(rooms[(i)]);


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

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_o : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_p : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
            case SDLK_b : Game::BBox::toggleDebugDraw(); break;
            case SDLK_PAGEUP :
            {
              i++; i%=4;

              world->loadRoom(rooms[i]);
              break;
            }
            case SDLK_PAGEDOWN :
            {
              if(i > 0)
              {
                i--; i%=4;
              }

              //if(i == -1) i+=sizeof(rooms)/sizeof(rooms[0]);
              world->loadRoom(rooms[(i)]);
              break;
            }
            default : break;
          } // end of key process
        } // end of keydown*
        default : break;
      } // end of event switch
    } // end of poll events


    if( world->getElapsedTime() >= worldUpdateDelay)
    {
      if(keystate[SDL_SCANCODE_UP])           { world->playerWalk(0.1);    }
      if(keystate[SDL_SCANCODE_DOWN])         { world->playerWalk(-0.1);   }
      if(keystate[SDL_SCANCODE_RIGHT])        { world->playerTurn(-4);     }
      if(keystate[SDL_SCANCODE_LEFT])         { world->playerTurn(4);      }
      if(keystate[SDL_SCANCODE_LSHIFT])       { world->playerDash();       }

      if(keystate[SDL_SCANCODE_W])            { cameraPos.m_z+=0.25;   }
      if(keystate[SDL_SCANCODE_S])            { cameraPos.m_z-=0.25;   }
      if(keystate[SDL_SCANCODE_Q])            { cameraPos.m_y+=0.25;   }
      if(keystate[SDL_SCANCODE_E])            { cameraPos.m_y-=0.25;   }
      if(keystate[SDL_SCANCODE_D])            { cameraPos.m_x+=0.25;   }
      if(keystate[SDL_SCANCODE_A])            { cameraPos.m_x-=0.25;   }

      if(keystate[SDL_SCANCODE_J])            { cameraRot.m_z+=0.25;   }
      if(keystate[SDL_SCANCODE_L])            { cameraRot.m_z-=0.25;   }

      if(keystate[SDL_SCANCODE_I])            { cameraRot.m_y+=0.25;   }
      if(keystate[SDL_SCANCODE_K])            { cameraRot.m_y-=0.25;   }

      if(keystate[SDL_SCANCODE_U])            { cameraRot.m_x+=0.25;   }
      if(keystate[SDL_SCANCODE_O])            { cameraRot.m_x-=0.25;   }

      if(keystate[SDL_SCANCODE_SPACE])        { world->damagePlayer(10);   }

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      Game::BBox a(Vec4(0,0,0), Vec4(1.0f, 1.0f, 1.0f));
      Game::BBox b(Vec4(0,1.0f,0), Vec4(2.0f, 2.0f, 1.0f));

//      glPushMatrix();
//        //glTranslatef(2.0f, 0.0f, -2.0f);
//        if(a.checkIntersectOrTouch(b))
//        {
//          Vec4 offset = -a.getIntersectOffset(b);
//          std::cout << offset;
//          if(!a.checkTouch(b))
//          {
//            Vec4 offset = -a.getIntersectOffset(b);
//            std::cout << offset;
//            b.move(Vec4(0, -0.2, 0.0));
////            if(a.checkTouch(b))
////            {
////              std::cout << "boop";
////            }
//          }
//          a.draw();
//          b.draw();
//          util::drawWorldAxis();
//        }
//      glPopMatrix();

      //tmpCamera.setTransform(cameraPos, cameraRot);
//      std::cout << cameraPos << "\n";
//      std::cout << cameraRot << "\n";
      //tmpCamera.setView();
      world->update();
      world->draw();
//      tmpCamera.setView();

      // swap the buffers
      SDL_GL_SwapWindow(window);
    }
  }

  // now tidy up and exit SDL
  SDL_Quit();
}