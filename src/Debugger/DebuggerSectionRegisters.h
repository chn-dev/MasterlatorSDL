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
\file DebuggerSectionRegisters.h
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the registers section.
*/
/*----------------------------------------------------------------------------*/
#ifndef __DEBUGGERSECTIONREGISTERS_H__
#define __DEBUGGERSECTIONREGISTERS_H__

#include "Debugger.h"
#include "DebuggerSection.h"

/*----------------------------------------------------------------------------*/
/*!
\class SectionRegisters
\date  2024-09-23
*/
/*----------------------------------------------------------------------------*/
class Debugger::SectionRegisters : public Section
{
public:
   enum Z80Register
   {
      Z80Register_None,
      Z80Register_A,
      Z80Register_BC,
      Z80Register_DE,
      Z80Register_HL,
      Z80Register_IX,
      Z80Register_IY,
      Z80Register_F,
      Z80Register_PC,
      Z80Register_SP
   };

   SectionRegisters( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool isAlwaysVisible();

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

private:
   static void dumpRegister( char *pDest, const char *pName, union_word reg );
   void printRegisters( int x, int y, bool isCurrentSection ) const;

   Z80Register m_CurrentRegister;
   int m_CurrentDigit;
   std::map<SDL_Keycode, std::map<Z80Register, Z80Register> > m_RegKeyMap;
};

#endif

