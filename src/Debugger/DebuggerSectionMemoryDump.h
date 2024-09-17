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


#ifndef __DEBUGGERSECTIONMEMORYDUMP_H__
#define __DEBUGGERSECTIONMEMORYDUMP_H__

#include "Debugger.h"
#include "DebuggerSection.h"
#include "DebuggerSectionRegisters.h"

class Debugger::SectionMemoryDump : public Section
{
public:
   SectionMemoryDump( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

   virtual bool isAlwaysVisible();

private:
   void printMemoryDump( std::string title, int x, int y );
   u16 getRegisterValue16( SectionRegisters::Z80Register reg ) const;

   const std::vector<std::pair<SectionRegisters::Z80Register, std::string> > m_Regs
   {
      { SectionRegisters::Z80Register_None, "" },
      { SectionRegisters::Z80Register_BC, "BC" },
      { SectionRegisters::Z80Register_DE, "DE" },
      { SectionRegisters::Z80Register_HL, "HL" },
      { SectionRegisters::Z80Register_IX, "IX" },
      { SectionRegisters::Z80Register_IY, "IY" },
      { SectionRegisters::Z80Register_SP, "SP" },
      { SectionRegisters::Z80Register_PC, "PC" }
   };

   u16 m_Location;
   u16 m_Offset;
   int m_n;
};

#endif

