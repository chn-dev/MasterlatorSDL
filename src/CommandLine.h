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
\file CommandLine.h
\author Christian Nowak <chnowak@web.de>
\brief Headerfile for command line functions.
*/
/*----------------------------------------------------------------------------*/
#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

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

