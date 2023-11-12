#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <SDL.h>
#include <string>

typedef struct Config
{
   std::string romFile;
   std::string symbolsFile;
   int xWidth = -1;
   int xHeight = -1;
   int windowWidth = -1;
   int windowHeight = -1;
   int screenBufferWidth = -1;
   int screenBufferHeight = -1;
   bool keepar = false;
   bool maximized = false;
   bool fullscreen = false;
   bool debug = false;
   SDL_Keycode aKey = SDLK_y;
   SDL_Keycode bKey = SDLK_x;
   SDL_Keycode startKey = SDLK_RETURN;
   SDL_Keycode upKey = SDLK_UP;
   SDL_Keycode downKey = SDLK_DOWN;
   SDL_Keycode leftKey = SDLK_LEFT;
   SDL_Keycode rightKey = SDLK_RIGHT;
} Config;

int readCommandLine( int argc, char *argv[], Config *pConfig );

#endif

