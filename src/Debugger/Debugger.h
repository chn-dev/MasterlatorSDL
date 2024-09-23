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
\file Debugger.h
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the GGMS debugger
*/
/*----------------------------------------------------------------------------*/
#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

#include <set>
#include <map>
#include <EmulatorCore/GGMS.h>

/*----------------------------------------------------------------------------*/
/*!
\class Debugger
\date  2024-09-23
*/
/*----------------------------------------------------------------------------*/
class Debugger : public Z80::DebuggerInterface
{
public:
   Debugger();
   Debugger( GGMS *pMachine );
   ~Debugger();

   void activate(  bool ac );
   bool isActivated() const;
   bool doDebug( u8 *pScreenBuffer );
   bool singleInstructionStep( u8 *pScreenBuffer );

   void loadSymbolsFile( std::string fname );

   GGMS *machine() const;

   virtual void z80_execStart( u16 loc );
   virtual void z80_execFinish( u16 loc );
   virtual bool z80_break();
   virtual std::string z80_locationToLabel( u16 loc );

private:
   bool isAtBreakpoint( u16 loc ) const;
   bool execSections( u8 *pScreenBuffer );

   bool m_Break;

   GGMS *m_pMachine;
   bool m_Activated;

   class Section;
   class SectionDisassembly;
   class SectionRegisters;
   class SectionMemoryDump;
   class SectionTiles;
   std::vector<Section *> m_Sections;
   std::vector<Section *> m_StaticSections;
   SectionDisassembly *sectionDisassembly() const;
   int m_CurrentSection;
};

#endif
