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


#include "Debugger.h"
#include "DebuggerSection.h"
#include "ArchSDL.h"
#include "Font.h"


Debugger::Section::Section( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   m_Activated( false ),
   m_pDebugger( pDebugger ),
   m_Name( name ),
   m_xPos( xp ),
   m_yPos( yp ),
   m_Width( width ),
   m_Height( height )
{
}


Debugger::Section::~Section()
{
}


std::string Debugger::Section::name() const
{
   return( m_Name );
}


Debugger *Debugger::Section::debugger() const
{
   return( m_pDebugger );
}


int Debugger::Section::getXPos() const
{
   return( m_xPos );
}


int Debugger::Section::getYPos() const
{
   return( m_yPos );
}


int Debugger::Section::getWidth() const
{
   return( m_Width );
}


int Debugger::Section::getHeight() const
{
   return( m_Height );
}


void Debugger::Section::activate( bool ac )
{
   if( m_Activated != ac )
   {
      m_Activated = ac;
   }
}


bool Debugger::Section::isActivated() const
{
   return( m_Activated );
}


bool Debugger::Section::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pDebugger->singleInstructionStep( pScreenBuffer ) );
}


bool Debugger::Section::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   print8x8( getScreen(), getXPos() + 2, getYPos() + 2, 130, isCurrentSection, m_Name );
   u8 c = isCurrentSection ? 130 : 131;

   for( int x = getXPos(); x < getXPos() + getWidth(); x++ )
   {
      getScreen()->pBuffer[( getYPos() * getScreen()->width ) + x] = c;
      getScreen()->pBuffer[( ( getYPos() + getHeight() ) * getScreen()->width ) + x] = c;
   }

   for( int y = getYPos(); y <= getYPos() + getHeight(); y++ )
   {
      getScreen()->pBuffer[( y * getScreen()->width ) + getXPos() ] = c;
      getScreen()->pBuffer[( y * getScreen()->width ) + getXPos() + getWidth() ] = c;
   }
   
   return( false );
}

