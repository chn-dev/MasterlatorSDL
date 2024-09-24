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
\file DebuggerSectionMemoryDump.cpp
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the memory dump section.
*/
/*----------------------------------------------------------------------------*/
#include "Debugger.h"
#include "DebuggerSectionMemoryDump.h"
#include "Font.h"
#include "ArchSDL.h"


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
Constructor
\param pDebugger Pointer to the debugger object
\param name The section's name
\param xp The section's x position
\param yp The section's y position
\param width The section's width
\param height The section's height
*/
/*----------------------------------------------------------------------------*/
Debugger::SectionMemoryDump::SectionMemoryDump( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   Section( pDebugger, name, xp, yp, width, height ),
   m_n( 0 ),
   m_Location( 0 ),
   m_Offset( 0 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
\return true if the section should be visible even if the debugger is inactive
*/
/*----------------------------------------------------------------------------*/
bool Debugger::SectionMemoryDump::isAlwaysVisible()
{
   return( false );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute the section. This implementation of exec() only draws borders around the section 
and should be overriden.
\param pScreenBuffer Pointer to the output screen buffer
\param isCurrentSection true if this is the currently selected section
\return true if the screen shall be blanked
*/
/*----------------------------------------------------------------------------*/
bool Debugger::SectionMemoryDump::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   Section::exec( pScreenBuffer, isCurrentSection );

   bool screenBlank = false;

   printMemoryDump( "abc", getXPos() + 2, getYPos() + 12 );

   if( isCurrentSection )
   {
      if( keyHasBeenPressed( SDLK_UP ) )
      {
         if( m_Regs[m_n].first == SectionRegisters::Z80Register_None )
            m_Location -= 16;
         else
            m_Offset -= 16;
      }

      if( keyHasBeenPressed( SDLK_DOWN ) )
      {
         if( m_Regs[m_n].first == SectionRegisters::Z80Register_None )
            m_Location += 16;
         else
            m_Offset += 16;
      }

      if( keyHasBeenPressed( SDLK_PAGEUP ) )
      {
         if( m_Regs[m_n].first == SectionRegisters::Z80Register_None )
            m_Location -= 16 * 12;
         else
            m_Offset -= 16 * 12;
      }

      if( keyHasBeenPressed( SDLK_PAGEDOWN ) )
      {
         if( m_Regs[m_n].first == SectionRegisters::Z80Register_None )
            m_Location += 16 * 12;
         else
            m_Offset += 16 * 12;
      }

      if( keyHasBeenPressed( SDLK_HOME ) && ( m_Regs[m_n].first != SectionRegisters::Z80Register_None ) )
      {
         m_Offset = 0;
      }

      if( keyHasBeenPressed( SDLK_LEFT ) )
      {
         m_n--;
         if( m_n < 0 )
            m_n = 0;
         m_Offset = 0;
      }

      if( keyHasBeenPressed( SDLK_RIGHT ) )
      {
         m_n++;
         if( m_n >= m_Regs.size() )
            m_n = (int)m_Regs.size() - 1;
         m_Offset = 0;
      }
   }

   return( screenBlank );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
Retrieve a 16bit register value of the Z80 CPU
\param reg Specifies the register
\return The register's value
*/
/*----------------------------------------------------------------------------*/
u16 Debugger::SectionMemoryDump::getRegisterValue16( SectionRegisters::Z80Register reg ) const
{
   if( reg == SectionRegisters::Z80Register_BC )
   {
      return( debugger()->machine()->cpu()->getBC().aword );
   } else
   if( reg == SectionRegisters::Z80Register_DE )
   {
      return( debugger()->machine()->cpu()->getDE().aword );
   } else
   if( reg == SectionRegisters::Z80Register_HL )
   {
      return( debugger()->machine()->cpu()->getHL().aword );
   } else
   if( reg == SectionRegisters::Z80Register_IX )
   {
      return( debugger()->machine()->cpu()->getIX().aword );
   } else
   if( reg == SectionRegisters::Z80Register_IY )
   {
      return( debugger()->machine()->cpu()->getIY().aword );
   } else
   if( reg == SectionRegisters::Z80Register_SP )
   {
      return( debugger()->machine()->cpu()->getSP().aword );
   } else
   if( reg == SectionRegisters::Z80Register_PC )
   {
      return( debugger()->machine()->cpu()->getPC().aword );
   }

   return( -1 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
Print a memory dump
\param title
\param x The x position
\param y The y position
*/
/*----------------------------------------------------------------------------*/
void Debugger::SectionMemoryDump::printMemoryDump( std::string title, int x, int y )
{
   u16 loc = m_Location;
   int bytesPerLine = 16;
   char tmp[256];

   if( m_Regs[m_n].first != SectionRegisters::Z80Register_None  )
   {
      loc = getRegisterValue16( m_Regs[m_n].first ) + m_Offset;
      u16 a = m_Offset;
      bool neg = ( a & 0x8000 ) != 0;
      if( neg )
         a = -a;
      char tmp[10];
      sprintf( tmp, "%c%04xh", neg ? '-' : '+', a );
      m_Name = std::string( "Memory Dump (" ) + m_Regs[m_n].second + std::string( tmp ) + std::string( ")" );
   }

   for( int line = 0; line < 12; line++ )
   {
      std::string l;

      sprintf( tmp, "%04x: ", loc );
      l = tmp;

      for( int i = 0; i < bytesPerLine; i++ )
      {
         if( i > 0 )
            l.append( " " );

         sprintf( tmp, "%02x", debugger()->machine()->z80_readMem( loc + i ) );
         l.append( tmp );
      }

      l.append( " " );
      for( int i = 0; i < bytesPerLine; i++ )
      {
         u8 d = debugger()->machine()->z80_readMem( loc + i );
         if( isprint( d ) )
            l.push_back( (char)d );
         else
            l.append( "." );
      }

      print8x8( getScreen(), x, y + ( 10 * line ), 129, false, l );

      loc += bytesPerLine;
   }
}

