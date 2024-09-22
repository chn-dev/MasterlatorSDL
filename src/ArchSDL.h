/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of Masterlator.                                         *
 *                                                                             *
 *  Masterlator is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *
 *  Masterlator is distributed in the hope that it will be useful, but         *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Masterlator. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/*----------------------------------------------------------------------------*/
/*!
\file ArchSDL.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for SDL-specific functions.
*/
/*----------------------------------------------------------------------------*/
#ifndef __ARCHSDL_H__
#define __ARCHSDL_H__

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
bool runSDL( GGMS *pMachine, const Config *pConfig, u8 clearColor );
void quitSDL();

#endif

