#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "World.h"

/*@todo
 * - Convert to modern GL
 *   + Better error messages (shader logs etc)
 * - Fix wall collision
 * - Fix camera views
 * - Remove Game:: namespace, it's useless
 */

/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);

bool createShaders();

void draw();

// Fix this
GLuint gProgramID = 0;
GLint  gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

int main()
{
  // rand() is later used to randomly choose colours for trigger volumes
  srand((unsigned int)time(NULL));

  /*------------------Initialize SDL's Video subsystem--------------------*/
  /*----------------------------------------------------------------------*/
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    // Or die on error
    SDLErrorExit("Unable to initialize SDL");
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
  if (!window)
  {
    SDLErrorExit("Unable to create window");
  }

  /*------------------Create OpenGL Context--------------------*/
  /*-----------------------------------------------------------*/
  SDL_GLContext glContext = createOpenGLContext(window);
  if(!glContext)
  {
    SDLErrorExit("Problem creating OpenGL context");
  }


  /*------------------Init GLEW--------------------*/
  /*-----------------------------------------------*/
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
  {
    SDLErrorExit("Unable to initialize GLEW");
  }

  if( SDL_GL_SetSwapInterval(1) < 0 )
  {
    SDLErrorExit("Unable to set VSync");
  }

  SDL_GL_MakeCurrent(window, glContext);
  SDL_GL_SetSwapInterval(1);  /* This makes our buffer swap syncronized with the monitor's vertical refresh */

  glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);


  /*------------------Create Shaders--------------------*/
  /*----------------------------------------------------*/
  createShaders();

  SDL_GL_SwapWindow(window);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColor3f(1,0,1);

  /*------------------Initialise World------------------*/
  /*----------------------------------------------------*/

  const uint worldUpdateDelay = 30;
  Game::World* world;

  // Instantiate the world and try to load the first room,
  // if this room can't be loaded then consider it a critical error and exit
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

  /*------------------Game Loop--------------------*/
  /*-----------------------------------------------*/

  // Input keys array
  const Uint8* keystate = SDL_GetKeyboardState(0);

  bool quit=false;  // Flag to indicate if we need to exit
  SDL_Event event; // Store SDL events

  while(!quit)
  {
    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT:
        {
          quit = true; break;
        }

          // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            case SDLK_ESCAPE:
            {
              quit = true; break;
            }

            case SDLK_o : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
            case SDLK_p : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
          }
        } /* End of keydown */
        default: break;
      }/* End event switch */
    } /* End event polling */

    if( world->getElapsedTime() >= worldUpdateDelay)
    {
      if(keystate[SDL_SCANCODE_UP])           { world->playerWalk(0.1);    }
      if(keystate[SDL_SCANCODE_DOWN])         { world->playerWalk(-0.1);   }
      if(keystate[SDL_SCANCODE_RIGHT])        { world->playerTurn(-4);     }
      if(keystate[SDL_SCANCODE_LEFT])         { world->playerTurn(4);      }
      if(keystate[SDL_SCANCODE_LSHIFT])       { world->playerDash();       }

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//      draw();
      world->update();
      world->draw();

      // swap the buffers
      SDL_GL_SwapWindow(window);
    }
  }

  // now tidy up and exit SDL
  SDL_Quit();
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
#ifdef DARWIN
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
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

//@todo Split this up into it's own class
bool createShaders()
{
  gProgramID = glCreateProgram();

  /*------------------Vertex Shader--------------------*/

  GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
  const GLchar* vtxShaderSrc[] = {
    "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
  };

  glShaderSource(vertexShader, 1, vtxShaderSrc, NULL);

  glCompileShader(vertexShader);

  GLint vShaderCompiled = GL_FALSE;
  glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
  if( vShaderCompiled != GL_TRUE )
  {
    SDLErrorExit("Unable to compile vertex shader");
    return false;
  }

  glAttachShader( gProgramID, vertexShader);

  /*------------------Fragment Shader--------------------*/


  GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );

  const GLchar* fragShaderSrc[]= {
    "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
  };

  glShaderSource( fragShader, 1, fragShaderSrc, NULL );

  glCompileShader(fragShader);

  GLint fShaderCompiled = GL_FALSE;
  glGetShaderiv( fragShader, GL_COMPILE_STATUS, &fShaderCompiled );
  if( fShaderCompiled != GL_TRUE )
  {
    SDLErrorExit("Unable to compile fragment shader");
    return false;
  }

  glAttachShader( gProgramID, fragShader);

  /*------------------Link Program--------------------*/

  glLinkProgram( gProgramID );

  GLint programSuccess = GL_TRUE;
  glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
  if( programSuccess != GL_TRUE )
  {
    SDLErrorExit("Error linking program");
    return false;
  }

  /*------------------LVertexPos2D Attribute--------------------*/

  gVertexPos2DLocation = glGetAttribLocation( gProgramID, "LVertexPos2D" );
  if( gVertexPos2DLocation  == -1 )
  {
    SDLErrorExit("LVertexPos2D is invalid glsl variable");
    return false;
  }

  /*------------------Vertex Data--------------------*/

  glClearColor( 0.f, 0.f, 0.f, 1.f );

  GLfloat vtxData[] =
  {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f,  0.5f,
    -0.5f,  0.5f
  };

  GLuint indexData[] = { 0, 1, 2, 3 };

  // Create VBO
  glGenBuffers(1, &gVBO);
  glBindBuffer( GL_ARRAY_BUFFER, gVBO );
  glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vtxData, GL_STATIC_DRAW );

  // Create IBO
  glGenBuffers(1, &gIBO);
  glBindBuffer( GL_ARRAY_BUFFER, gIBO );
  glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

  return true;
}

void draw()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glUseProgram( gProgramID );

  glEnableVertexAttribArray( gVertexPos2DLocation );

  glBindBuffer( GL_ARRAY_BUFFER, gVBO );
  glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), NULL );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

  glDisableVertexAttribArray( gVertexPos2DLocation );

  // Unbind program
  glUseProgram(NULL);
}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr << _msg << "\n";
  std::cerr << SDL_GetError() << "\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
