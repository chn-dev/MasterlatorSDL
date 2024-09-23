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
\file DebuggerSectionDisassembly.h
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the disassembly section.
*/
/*----------------------------------------------------------------------------*/
#ifndef __DEBUGGERSECTIONDISASSEMBLY_H__
#define __DEBUGGERSECTIONDISASSEMBLY_H__

#include "Debugger.h"
#include "DebuggerSection.h"
#include "EmulatorCore/GGMS.h"

/*----------------------------------------------------------------------------*/
/*!
\class SectionDisassembly
\date  2024-09-23
*/
/*----------------------------------------------------------------------------*/
class Debugger::SectionDisassembly : public Section
{
public:
   SectionDisassembly( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );

   virtual bool isAlwaysVisible();

   bool isAtBreakpoint( u16 loc ) const;

   void loadSymbolsFile( std::string fname );

   std::string locationToLabel( u16 loc );

private:
   void printDisassembly( int x, int y ) const;
   u16 getNextPC( u16 curPC, int step );

   std::set<GGMS::MemoryLocation> m_Breakpoints;
   u16 m_NextInstrBreak;
   bool m_NextInstrBreakIsSet;
   void updateDisassembly();
   void updateDisassembly( int nInstructions, int cursorPos );
   std::vector<Z80::Instruction> m_Disassembly;
   u16 m_CursorPosition;
   int m_CursorOffset;
   std::map<GGMS::MemoryLocation, std::string> m_Labels;
};

#endif
