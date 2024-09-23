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
\file Debugger.cpp
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the GGMS Debugger
*/
/*----------------------------------------------------------------------------*/
#include "Debugger.h"
#include "DebuggerSectionDisassembly.h"
#include "DebuggerSectionRegisters.h"
#include "DebuggerSectionMemoryDump.h"
#include "DebuggerSectionTiles.h"

#include "ArchSDL.h"
#include "Font.h"


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Constructor
*/
/*----------------------------------------------------------------------------*/
Debugger::Debugger() :
   m_pMachine( 0 ),
   m_Activated( false ),
   m_Break( false ),
   m_CurrentSection( -1 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Constructor
*/
/*----------------------------------------------------------------------------*/
Debugger::Debugger( GGMS *pMachine ) :
   m_pMachine( pMachine ),
   m_Activated( false ),
   m_Break( false ),
   m_CurrentSection( 1 )
{
   m_Sections.push_back( new SectionRegisters(   this, "Registers",   10, 10,  562, 70 ) );
   m_Sections.push_back( new SectionDisassembly( this, "Disassembly", 10, 90,  562, 240 ) );
   m_Sections.push_back( new SectionMemoryDump(  this, "Memory Dump", 10, 340, 562, 130 ) );

   m_Sections.push_back( new SectionTiles( this, "Tiles", 587, 212, 257, /*156*/258 ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
Debugger::~Debugger()
{
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      Section *pSection = m_Sections[i];
      delete pSection;
   }

   m_Sections.clear();

   for( int i = 0; i < m_StaticSections.size(); i++ )
   {
      Section *pSection = m_StaticSections[i];
      delete pSection;
   }

   m_StaticSections.clear();
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Load a wla-dx compatible .sym file
\param fname The symbols file name
*/
/*----------------------------------------------------------------------------*/
void Debugger::loadSymbolsFile( std::string fname )
{
   sectionDisassembly()->loadSymbolsFile( fname );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The GGMS object
*/
/*----------------------------------------------------------------------------*/
GGMS *Debugger::machine() const
{
   return( m_pMachine );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Callback function called by the Z80 core
*/
/*----------------------------------------------------------------------------*/
void Debugger::z80_execStart( u16 loc )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Callback function called by the Z80 core
*/
/*----------------------------------------------------------------------------*/
void Debugger::z80_execFinish( u16 loc )
{
   if( isActivated() )
      return;

   m_Break = isAtBreakpoint( loc );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Callback function called by the Z80 core when disassembling code
*/
/*----------------------------------------------------------------------------*/
std::string Debugger::z80_locationToLabel( u16 loc )
{
   return( sectionDisassembly()->locationToLabel( loc ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Check whether a specific address has a breakpoint, taking into account paging
\param loc The 16bit memory adress
\return true if that address has a breakpoint
*/
/*----------------------------------------------------------------------------*/
bool Debugger::isAtBreakpoint( u16 loc ) const
{
   SectionDisassembly *pSecDis = sectionDisassembly();
   if( !pSecDis )
      return( false );
   return( pSecDis->isAtBreakpoint( loc ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The disassembly section
*/
/*----------------------------------------------------------------------------*/
Debugger::SectionDisassembly *Debugger::sectionDisassembly() const
{
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      SectionDisassembly *pSecDis = dynamic_cast<SectionDisassembly *>( m_Sections[i] );
      if( pSecDis )
         return( pSecDis );
   }

   return( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Callback function called by the Z80 core to check whether it should break
*/
/*----------------------------------------------------------------------------*/
bool Debugger::z80_break()
{
   return( m_Break );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
De/activate the debugger
\param ac true if the debugger shall be activated
*/
/*----------------------------------------------------------------------------*/
void Debugger::activate( bool ac )
{
   if( m_Activated == ac )
      return;

   for( int i = 0; i < m_Sections.size(); i++ )
   {
      m_Sections[i]->activate( ac );
   }

   m_Activated = ac;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return true if the debugger is activated
*/
/*----------------------------------------------------------------------------*/
bool Debugger::isActivated() const
{
   return( m_Activated );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute all debugger sections
\param pScreenBuffer Pointer to the output screen buffer
\return true if the screen shall be blanked
*/
/*----------------------------------------------------------------------------*/
bool Debugger::execSections( u8 *pScreenBuffer )
{
   bool screenBlank = false;

/*   if( !isActivated() )
      return( false );*/

   Section *pActiveSection = 0;
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      if( i == m_CurrentSection )
         pActiveSection = m_Sections[i];
      else
      {
         if( isActivated() || m_Sections[i]->isAlwaysVisible() )
         {
            screenBlank = screenBlank || m_Sections[i]->exec( pScreenBuffer, false );
         }
      }
   }

   if( pActiveSection )
   {
      if( isActivated() || pActiveSection->isAlwaysVisible() )
      {
         screenBlank = screenBlank || pActiveSection->exec( pScreenBuffer, true && isActivated() );
      }
   }

   return( screenBlank && isActivated() );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute the debugger
\param pScreenBuffer Pointer to the output screen buffer
\return true if the screen shall be blanked
*/
/*----------------------------------------------------------------------------*/
bool Debugger::doDebug( u8 *pScreenBuffer )
{
   bool screenBlank = execSections( pScreenBuffer );

   if( !isActivated() )
      return( false );

   if( keyHasBeenPressed( SDLK_TAB ) )
   {
      if( isKeyPressed( SDLK_RSHIFT ) || isKeyPressed( SDLK_LSHIFT ) )
      {
         m_CurrentSection--;
         if( m_CurrentSection < 0 )
            m_CurrentSection = (int)m_Sections.size() - 1;
      } else
      {
         m_CurrentSection++;
         if( m_CurrentSection >= m_Sections.size() )
            m_CurrentSection = 0;
      }
   }

//   printMemoryDump( "Memory Dump", 10, 300 );
//   printMemoryDump( "Memory Dump", 300, 300 );
//   printMemoryDump( "Memory Dump", 600, 300 );

   return( screenBlank );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute a single Z80 instruction
\param pScreenBuffer Pointer to the output screen buffer
\return true if a complete screen frame has been finished rendering
*/
/*----------------------------------------------------------------------------*/
bool Debugger::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pMachine->singleInstructionStep(
      pScreenBuffer,
      m_pMachine->screenWidth(), m_pMachine->screenHeight(),
      0, 0 ) );
}

