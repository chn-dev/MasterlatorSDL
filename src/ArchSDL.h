#ifndef ARCHSDL_H
#define ARCHSDL_H

#include "CommandLine.h"
#include "EmulatorCore/GGMS.h"

typedef struct Screen
{
   u8 *pBuffer;
   int width;
   int height;
   Uint32 pal[256];
} Screen;

bool isKeyPressed( SDL_Keycode keyCode );
bool keyHasBeenPressed( SDL_Keycode keyCode );
bool keyHasBeenReleased( SDL_Keycode keyCode );

Screen *getScreen();

bool initSDL( GGMS *pMachine, const Config *pConfig );
bool runSDL( GGMS *pMachine, const Config *pConfig );
void quitSDL();

#endif

