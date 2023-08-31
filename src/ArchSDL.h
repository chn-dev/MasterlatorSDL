#ifndef ARCHSDL_H
#define ARCHSDL_H

#include "CommandLine.h"
#include "EmulatorCore/GGMS.h"

bool initSDL( GGMS *pMachine, const Config *pConfig );
bool runSDL( GGMS *pMachine, const Config *pConfig, u8 *pScreenBuffer, Uint32 *pPalette );
void quitSDL();

#endif

