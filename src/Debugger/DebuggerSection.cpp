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
\file DebuggerSection.cpp
\author Christian Nowak <chnowak@web.de>
\brief Any debugger sections should be derived from the DebuggerSection class.
*/
/*----------------------------------------------------------------------------*/
#include "Debugger.h"
#include "DebuggerSection.h"
#include "ArchSDL.h"
#include "Font.h"


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Constructor
\param pDebugger Pointer to the debugger object
\param name The section's name
\param xp The section's x position
\param yp The section's y position
\param width The section's width
\param height The section's height
*/
/*----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
Debugger::Section::~Section()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The section's name
*/
/*----------------------------------------------------------------------------*/
std::string Debugger::Section::name() const
{
   return( m_Name );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The debugger
*/
/*----------------------------------------------------------------------------*/
Debugger *Debugger::Section::debugger() const
{
   return( m_pDebugger );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The section's x position
*/
/*----------------------------------------------------------------------------*/
int Debugger::Section::getXPos() const
{
   return( m_xPos );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The section's y position
*/
/*----------------------------------------------------------------------------*/
int Debugger::Section::getYPos() const
{
   return( m_yPos );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The section's width
*/
/*----------------------------------------------------------------------------*/
int Debugger::Section::getWidth() const
{
   return( m_Width );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return The section's height
*/
/*----------------------------------------------------------------------------*/
int Debugger::Section::getHeight() const
{
   return( m_Height );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
De/activate the section
\param ac true to activate
*/
/*----------------------------------------------------------------------------*/
void Debugger::Section::activate( bool ac )
{
   if( m_Activated != ac )
   {
      m_Activated = ac;
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return true if the section is activated
*/
/*----------------------------------------------------------------------------*/
bool Debugger::Section::isActivated() const
{
   return( m_Activated );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute a single Z80 instruction
\param pScreenBuffer Pointer to the output screen buffer
\return true if a complete screen frame has been finished rendering
*/
/*----------------------------------------------------------------------------*/
bool Debugger::Section::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pDebugger->singleInstructionStep( pScreenBuffer ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute the section. This implementation of exec() only draws borders around the section 
and should be overriden.
\param pScreenBuffer Pointer to the output screen buffer
\param isCurrentSection true if this is the currently selected section
\return false
*/
/*----------------------------------------------------------------------------*/
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

