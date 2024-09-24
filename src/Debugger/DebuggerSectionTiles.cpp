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
\file DebuggerSectionTiles.h
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the tiles section.
*/
/*----------------------------------------------------------------------------*/
#include "Debugger.h"
#include "DebuggerSectionTiles.h"
#include "ArchSDL.h"
#include "Font.h"


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
Debugger::SectionTiles::SectionTiles( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   Section( pDebugger, name, xp, yp, width, height ),
   m_n( 0 )
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
*/
/*----------------------------------------------------------------------------*/
void Debugger::SectionTiles::printSprites()
{
   GGVDP *pVDP = debugger()->machine()->vdp();
   u8 *pSprites = &pVDP->vram()[( pVDP->registerValue( 5 ) << 7 ) & 0x3f00];

   for( int i = 0; i < 64; i++ )
   {
      char tmp[256];

      /* y position */
      int y = pSprites[i] - ( pVDP->registerValue( 0 ) & 8 );
      /* Height */
      int dy = pVDP->registerValue( 1 ) & 0x02 ? 16 : 8;
      /* x position */
      int x = pSprites[128 + ( i << 1 )];

      /* Tile number */
      int tile = ( pSprites[128 + ( i << 1 ) + 1] | ( ( pVDP->registerValue( 6 ) & 4 ) << 6 ) ) &
                  /* Ignore bit 0 for 8x16 sprites */
                  ( dy == 8 ? ~0 : ~1 );

      sprintf( tmp, "%02d:%02x/%02x", i, x, y );
      int xp = getXPos() + 2 + ( i / 22 ) * 84;
      int yp = getYPos() + 12 + ( ( i % 22 ) * 10 );
      print8x8( getScreen(), xp, yp, 129, false, tmp );
      paintTile( xp + 66, yp, tile );
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
\return true if the section should be visible even if the debugger is inactive
*/
/*----------------------------------------------------------------------------*/
bool Debugger::SectionTiles::isAlwaysVisible()
{
   return( true );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
Execute the section. This implementation of exec() only draws borders around the section 
and should be overriden.
\param pScreenBuffer Pointer to the output screen buffer
\param isCurrentSection true if this is the currently selected section
\return true if the screen shall be blanked
*/
/*----------------------------------------------------------------------------*/
bool Debugger::SectionTiles::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   Section::exec( pScreenBuffer, isCurrentSection );

   if( m_n == 0 )
   {
      m_Name = "VDP: Tiles";
      paintTiles();
   } else
   if( m_n == 1 )
   {
      m_Name = "VDP: Sprites";
      printSprites();
   }

   if( isCurrentSection )
   {
      if( keyHasBeenPressed( SDLK_LEFT ) )
      {
         m_n--;
         if( m_n < 0 )
            m_n = 0;
      }

      if( keyHasBeenPressed( SDLK_RIGHT ) )
      {
         m_n++;
         if( m_n > 1 )
            m_n = 1;
      }
   }

   return( false );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
*/
/*----------------------------------------------------------------------------*/
void Debugger::SectionTiles::paintTiles()
{
   for( int yt = 0; yt < 16; yt++ )
   {
      for( int xt = 0; xt < 28; xt++ )
      {
         paintTile( getXPos() + 4 + ( xt * 9 ), getYPos() + 12 + ( yt * 9 ), xt + ( 28 * yt ) );
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-24
\param x x position
\param y y position
\param tile The tile number
*/
/*----------------------------------------------------------------------------*/
void Debugger::SectionTiles::paintTile( int x, int y, int tile )
{
   u8 *pSrc = &debugger()->machine()->vdp()->vram()[tile * 8 * 4];
   u8 *pDest = &getScreen()->pBuffer[( y * getScreen()->width ) + x];
   for( int yp = 0; yp < 8; yp++ )
   {
      for( int xp = 0; xp < 8; xp++ )
      {
         int nfx = 7 - xp;
         u8 col = ( ( ( pSrc[0] >> nfx ) & 1 ) << 0 ) |
                  ( ( ( pSrc[1] >> nfx ) & 1 ) << 1 ) |
                  ( ( ( pSrc[2] >> nfx ) & 1 ) << 2 ) |
                  ( ( ( pSrc[3] >> nfx ) & 1 ) << 3 );
         pDest[xp] = col | ( tile >= 256 ? 16 : 0 );
      }

      pDest += getScreen()->width;
      pSrc += 4;
   }
}

