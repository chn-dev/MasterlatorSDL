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
\file DebuggerSection.h
\author Christian Nowak <chnowak@web.de>
\brief Any debugger sections should be derived from the DebuggerSection class.
*/
/*----------------------------------------------------------------------------*/
#ifndef __DEBUGGERSECTION_H__
#define __DEBUGGERSECTION_H__

#include "Debugger.h"

/*----------------------------------------------------------------------------*/
/*!
\class DebuggerSection
\date  2024-09-23
*/
/*----------------------------------------------------------------------------*/
class Debugger::Section
{
public:
   Section( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );
   ~Section();

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );
   bool isActivated() const;

   virtual bool isAlwaysVisible() = 0;

   bool singleInstructionStep( u8 *pScreenBuffer );

   int getXPos() const;
   int getYPos() const;
   int getWidth() const;
   int getHeight() const;
   std::string name() const;

protected:
   Debugger *debugger() const;
   std::string m_Name;

private:
   bool m_Activated;
   Debugger *m_pDebugger;
   int m_xPos;
   int m_yPos;
   int m_Width;
   int m_Height;
};

#endif
