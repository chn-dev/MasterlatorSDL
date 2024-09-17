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
#include "DebuggerSectionRegisters.h"
#include "ArchSDL.h"
#include "Font.h"


Debugger::SectionRegisters::SectionRegisters( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   Section( pDebugger, name, xp, yp, width, height ),
   m_CurrentRegister( Z80Register_A ),
   m_CurrentDigit( 0 )
{
   m_RegKeyMap[SDLK_DOWN][Z80Register_A] = Z80Register_BC;
   m_RegKeyMap[SDLK_DOWN][Z80Register_BC] = Z80Register_DE;
   m_RegKeyMap[SDLK_DOWN][Z80Register_DE] = Z80Register_HL;
   m_RegKeyMap[SDLK_DOWN][Z80Register_HL] = Z80Register_IX;
   m_RegKeyMap[SDLK_DOWN][Z80Register_IX] = Z80Register_IY;
   m_RegKeyMap[SDLK_DOWN][Z80Register_IY] = Z80Register_A;
   m_RegKeyMap[SDLK_DOWN][Z80Register_F] = Z80Register_PC;
   m_RegKeyMap[SDLK_DOWN][Z80Register_PC] = Z80Register_SP;
   m_RegKeyMap[SDLK_DOWN][Z80Register_SP] = Z80Register_F;

   m_RegKeyMap[SDLK_UP][Z80Register_A] = Z80Register_IY;
   m_RegKeyMap[SDLK_UP][Z80Register_BC] = Z80Register_A;
   m_RegKeyMap[SDLK_UP][Z80Register_DE] = Z80Register_BC;
   m_RegKeyMap[SDLK_UP][Z80Register_HL] = Z80Register_DE;
   m_RegKeyMap[SDLK_UP][Z80Register_IX] = Z80Register_HL;
   m_RegKeyMap[SDLK_UP][Z80Register_IY] = Z80Register_IX;
   m_RegKeyMap[SDLK_UP][Z80Register_F] = Z80Register_SP;
   m_RegKeyMap[SDLK_UP][Z80Register_PC] = Z80Register_F;
   m_RegKeyMap[SDLK_UP][Z80Register_SP] = Z80Register_PC;

   m_RegKeyMap[SDLK_LEFT][Z80Register_A] = Z80Register_F;
   m_RegKeyMap[SDLK_LEFT][Z80Register_BC] = Z80Register_PC;
   m_RegKeyMap[SDLK_LEFT][Z80Register_DE] = Z80Register_SP;
   m_RegKeyMap[SDLK_LEFT][Z80Register_HL] = Z80Register_HL;
   m_RegKeyMap[SDLK_LEFT][Z80Register_IX] = Z80Register_IX;
   m_RegKeyMap[SDLK_LEFT][Z80Register_IY] = Z80Register_IY;
   m_RegKeyMap[SDLK_LEFT][Z80Register_F] = Z80Register_A;
   m_RegKeyMap[SDLK_LEFT][Z80Register_PC] = Z80Register_BC;
   m_RegKeyMap[SDLK_LEFT][Z80Register_SP] = Z80Register_DE;

   m_RegKeyMap[SDLK_RIGHT][Z80Register_A] = Z80Register_F;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_BC] = Z80Register_PC;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_DE] = Z80Register_SP;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_HL] = Z80Register_HL;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_IX] = Z80Register_IX;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_IY] = Z80Register_IY;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_F] = Z80Register_A;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_PC] = Z80Register_BC;
   m_RegKeyMap[SDLK_RIGHT][Z80Register_SP] = Z80Register_DE;
}


bool Debugger::SectionRegisters::isAlwaysVisible()
{
   return( false );
}


bool Debugger::SectionRegisters::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   Section::exec( pScreenBuffer, isCurrentSection );

   // Registers
   printRegisters( getXPos() + 2, getYPos() + 2 + 10, isCurrentSection );

   if( isCurrentSection )
   {
      for( auto e = m_RegKeyMap.begin(); e != m_RegKeyMap.end(); e++ )
      {
         SDL_Keycode keyCode = e->first;

         if( keyHasBeenPressed( keyCode ) )
         {
            if( m_RegKeyMap[keyCode].find( m_CurrentRegister ) != m_RegKeyMap[keyCode].end() )
            {
               m_CurrentRegister = m_RegKeyMap[keyCode][m_CurrentRegister];
               m_CurrentDigit = 0;
            }
         }
      }

      std::map<SDL_Keycode, int> hexChars
      {
         { SDLK_0, 0  },
         { SDLK_1, 1  },
         { SDLK_2, 2  },
         { SDLK_3, 3  },
         { SDLK_4, 4  },
         { SDLK_5, 5  },
         { SDLK_6, 6  },
         { SDLK_7, 7  },
         { SDLK_8, 8  },
         { SDLK_9, 9  },
         { SDLK_a, 10 },
         { SDLK_b, 11 },
         { SDLK_c, 12 },
         { SDLK_d, 13 },
         { SDLK_e, 14 },
         { SDLK_f, 15 }
      };

      for( auto k = hexChars.begin(); k != hexChars.end(); k++ )
      {
         SDL_Keycode kc = k->first;
         if( keyHasBeenPressed( kc ) )
         {
            int val = k->second;

            switch( m_CurrentRegister )
            {
               case Z80Register_A:
               {
                  union_word v = debugger()->machine()->cpu()->getAF();
                  v.abyte.high = ( ( v.abyte.high << 4 ) & 0xf0 ) | ( (u8)val & 0x0f );
                  debugger()->machine()->cpu()->setAF( v );
                  break;
               }
               case Z80Register_F:
               {
                  union_word v = debugger()->machine()->cpu()->getAF();
                  v.abyte.low = ( ( v.abyte.low << 4 ) & 0xf0 ) | ( (u8)val & 0x0f );
                  debugger()->machine()->cpu()->setAF( v );
                  break;
               }
               case Z80Register_BC:
               {
                  union_word v = debugger()->machine()->cpu()->getBC();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setBC( v );
                  break;
               }
               case Z80Register_DE:
               {
                  union_word v = debugger()->machine()->cpu()->getDE();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setDE( v );
                  break;
               }
               case Z80Register_HL:
               {
                  union_word v = debugger()->machine()->cpu()->getHL();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setHL( v );
                  break;
               }
               case Z80Register_IX:
               {
                  union_word v = debugger()->machine()->cpu()->getIX();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setIX( v );
                  break;
               }
               case Z80Register_IY:
               {
                  union_word v = debugger()->machine()->cpu()->getIY();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setIY( v );
                  break;
               }
               case Z80Register_SP:
               {
                  union_word v = debugger()->machine()->cpu()->getSP();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setSP( v );
                  break;
               }
               case Z80Register_PC:
               {
                  union_word v = debugger()->machine()->cpu()->getPC();
                  v.aword = ( ( v.aword << 4 ) & 0xfff0 ) | ( (u16)val & 0x000f );
                  debugger()->machine()->cpu()->setPC( v );
                  break;
               }
               default:
                  break;
            }
            break;
         }
      }
   }

   return( false );
}


void Debugger::SectionRegisters::dumpRegister( char *pDest, const char *pName, union_word reg )
{
   sprintf( pDest, "%s = %04xh = '%c%c' = %5du = %6di",
      pName,
      reg.aword,
      isprint( reg.abyte.high ) ? reg.abyte.high : ' ',
      isprint( reg.abyte.low ) ? reg.abyte.low : ' ',
      reg.aword,
      (s16)reg.aword );
}


void Debugger::SectionRegisters::printRegisters( int x, int y, bool isCurrentSection ) const
{
   union_word af = debugger()->machine()->cpu()->getAF();
   u8 a = af.abyte.high;
   u8 f = af.abyte.low;
   char tmp[256];
   char flags[10];
   sprintf( flags, "%c%c%c%c%c%c%c%c",
      f & FLAG_S ? 'S' : '-',
      f & FLAG_Z ? 'Z' : '-',
      f & FLAG_5 ? '5' : '-',
      f & FLAG_H ? 'H' : '-',
      f & FLAG_3 ? '3' : '-',
      f & FLAG_PV ? 'V' : '-',
      f & FLAG_N ? 'N' : '-',
      f & FLAG_C ? 'C' : '-' );
   sprintf( tmp, "A  =   %02xh =  '%c' = %5du = %6di",
      a, isprint( a ) ? a : ' ', a, (int)(s8)a );
   print8x8( getScreen(), x, y, 129, ( m_CurrentRegister == Z80Register_A ) && isCurrentSection, tmp );

   sprintf( tmp, "F  = %02xh = %s", f, flags );
   print8x8( getScreen(), x + 310, y, 129, ( m_CurrentRegister == Z80Register_F ) && isCurrentSection, tmp );

   sprintf( tmp, "PC = %04xh", debugger()->machine()->cpu()->getPC().aword );
   print8x8( getScreen(), x + 310, y + 10, 129, ( m_CurrentRegister == Z80Register_PC ) && isCurrentSection, tmp );

   sprintf( tmp, "SP = %04xh", debugger()->machine()->cpu()->getSP().aword );
   print8x8( getScreen(), x + 310, y + 20, 129, ( m_CurrentRegister == Z80Register_SP ) && isCurrentSection, tmp );

   dumpRegister( tmp, "BC", debugger()->machine()->cpu()->getBC() );
   print8x8( getScreen(), x, y + 10, 129, ( m_CurrentRegister == Z80Register_BC ) && isCurrentSection, tmp );

   dumpRegister( tmp, "DE", debugger()->machine()->cpu()->getDE() );
   print8x8( getScreen(), x, y + 20, 129, ( m_CurrentRegister == Z80Register_DE ) && isCurrentSection, tmp );

   dumpRegister( tmp, "HL", debugger()->machine()->cpu()->getHL() );
   print8x8( getScreen(), x, y + 30, 129, ( m_CurrentRegister == Z80Register_HL ) && isCurrentSection, tmp );

   dumpRegister( tmp, "IX", debugger()->machine()->cpu()->getIX() );
   print8x8( getScreen(), x, y + 40, 129, ( m_CurrentRegister == Z80Register_IX ) && isCurrentSection, tmp );

   dumpRegister( tmp, "IY", debugger()->machine()->cpu()->getIY() );
   print8x8( getScreen(), x, y + 50, 129, ( m_CurrentRegister == Z80Register_IY ) && isCurrentSection, tmp );

//   sprintf( tmp, "vcount = %d", debugger()->machine()->vdp()->vcount() );
//   print8x8( getScreen(), x, y + 60, 129, false, tmp );
}

