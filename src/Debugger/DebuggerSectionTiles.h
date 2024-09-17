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


#ifndef __DEBUGGERSECTIONTILES_H__
#define __DEBUGGERSECTIONTILES_H__

#include "Debugger.h"
#include "DebuggerSection.h"

class Debugger::SectionTiles : public Section
{
public:
   SectionTiles( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

   virtual bool isAlwaysVisible();

private:
   void printSprites();
   void paintTiles();
   void paintTile( int x, int y, int tile );

   int m_n;
};

#endif
