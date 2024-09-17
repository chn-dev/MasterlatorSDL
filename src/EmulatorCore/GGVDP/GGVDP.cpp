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


/* GGVDP.c */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Z80/Z80.h"
#include "GGVDP.h"

extern int debug;


GGVDP *GGVDP::create()
{
   GGVDP *pVDP = new GGVDP();
   pVDP->reset();

   pVDP->m_render = true;
   pVDP->m_pVRAM = (u8*)malloc( 16 * 1024 );
   pVDP->m_gg = false;
   pVDP->m_screenw = 256;
   pVDP->m_screenh = 192;
   pVDP->m_pLinedata = 0;

   return( pVDP );
}


GGVDP::GGVDP()
{
//   memset( this, 0, sizeof( GGVDP ) );
}


GGVDP::~GGVDP()
{
   if( m_pVRAM )
   {
      free( m_pVRAM );
      m_pVRAM = 0;
   }

   if( m_pLinedata )
   {
      free( m_pLinedata );
      m_pLinedata = 0;
   }
}


void GGVDP::reset()
{
   m_address = 0;
   m_t = 0;
   m_hilo = 0;
   m_line_counter = 0;
   m_vcount = 0;
   m_clatch = 0;
   m_regs[0] = 0x00;
   m_regs[1] = 0x00;
   m_regs[2] = 0x00;
   m_regs[3] = 0x00;
   m_regs[4] = 0x00;
   m_regs[5] = 0x00;  /* Sprite table at 0x3f00 */
   m_regs[6] = 0x00;  /* Sprite tiles at 0x2000 */
   m_regs[7] = 0x00;
   m_regs[8] = 0x00;
   m_regs[9] = 0x00;
   m_regs[10] = 0x00;
   m_regs[11] = 0x00;
   m_regs[12] = 0x00;
   m_regs[13] = 0x00;
   m_regs[14] = 0x00;
   m_regs[15] = 0x00;
   m_line_irq = 0;
   m_frame_irq = 0;
   m_irq = 0;
   m_collision = 0;
   m_paletteChanged = false;
   m_render = true;

   forceDirtyTileCache();
}


u8 *GGVDP::vram() const
{
   return( m_pVRAM );
}


u8 GGVDP::registerValue( int n ) const
{
   return( m_regs[n] );
}


void GGVDP::pushTileCache( u32 tile )
{
   if( !m_tileCacheDirty[tile] )
   {
      m_tileCacheDirty[tile] = true;
      m_tileCacheTop++;
      m_tileCacheTiles[m_tileCacheTop] = tile;
   }
}


u32 GGVDP::popTileCache()
{
   int tile;

   if( m_tileCacheTop < 0 )
      return( 0xffffffff );

   tile = m_tileCacheTiles[m_tileCacheTop--];
   m_tileCacheDirty[tile] = false;

   return( tile );
}


void GGVDP::forceDirtyTileCache()
{
   m_tileCacheTop = -1;

   for( int i = 0; i < GGVDP_NUM_TILES * 8; i++ )
   {
      m_tileCacheDirty[i] = false;
   }

   for( int i = 0; i < GGVDP_NUM_TILES * 8; i++ )
   {
      pushTileCache( i );
   }
}


void GGVDP::renderTileLine( u32 tile, u32 line )
{
   u8 *src;
   int y1 = 8 * line;
   int y2 = 8 * (7 - line);
   int x, nfx;

   src = &m_pVRAM[( tile * 4 * 8 ) + ( 4 * line )];

   for( x = 0, nfx = 7; x < 8; x++, nfx-- )
   {
      u8 col = ( ( ( src[0] >> nfx ) & 1 ) << 0 ) |
               ( ( ( src[1] >> nfx ) & 1 ) << 1 ) |
               ( ( ( src[2] >> nfx ) & 1 ) << 2 ) |
               ( ( ( src[3] >> nfx ) & 1 ) << 3 );

      m_tileCache[0][tile][y1 + x] =
      m_tileCache[1][tile][y1 + nfx] =
      m_tileCache[2][tile][y2 + x] =
      m_tileCache[3][tile][y2 + nfx] = col;

      m_tileCache[4][tile][y1 + x] =
      m_tileCache[5][tile][y1 + nfx] =
      m_tileCache[6][tile][y2 + x] =
      m_tileCache[7][tile][y2 + nfx] = col | 16;
   }
}


void GGVDP::renderTile( u32 tile )
{
   for( u32 line = 0; line < 8; line++ )
   {
      renderTileLine( tile, line );
   }
}


void GGVDP::processTileCache()
{
   u32 tile;

   while( ( tile = popTileCache() ) != 0xffffffff )
   {
      renderTileLine( tile >> 3, tile & 7 );
   }
}


/*
void GGVDP_RenderTile(GGVDP *vdp, int tile) {

	int y;
	u8 *dest;
	u8 *src;

	if (!vdp)
		return;

	dest = vdp->tileCache[tile];
	src = &vdp->vram[tile * 4 * 8];

	for (y = 0; y < 8; y++) {

		int x;

		for (x = 0; x < 8; x++) {

			int nfx = 7 - x;

			dest[x + (y * 8)] = (((src[(y * 4) + 0] >> nfx) & 1) << 0) |
		                      (((src[(y * 4) + 1] >> nfx) & 1) << 1) |
		                      (((src[(y * 4) + 2] >> nfx) & 1) << 2) |
		                      (((src[(y * 4) + 3] >> nfx) & 1) << 3);
	  }
	}

	vdp->tileCacheValid[tile] = TRUE;
}

void GGVDP_RenderAllTiles(GGVDP *vdp) {

	int i;

	if (!vdp)
		return;

	for (i = 0; i < GGVDP_NUM_TILES; i++)
		GGVDP_RenderTile(vdp, i);

	vdp->tileCacheAllInvalid = FALSE;
}
*/

void GGVDP::setGG( bool en )
{
   m_gg = en;
   m_screenw = ( en ? 160 : 256 );
   m_screenh = ( en ? 144 : 192 );
}


bool GGVDP::isGG() const
{
   return( m_gg );
}


bool GGVDP::frameIRQ() const
{
   return( m_frame_irq != 0 );
}


void GGVDP::clearFrameIRQ()
{
   m_frame_irq = 0;
}


bool GGVDP::lineIRQ() const
{
   return( m_line_irq != 0 );
}


void GGVDP::clearLineIRQ()
{
   m_line_irq = 0;
}


bool GGVDP::collision() const
{
   return( m_collision != 0 );
}


void GGVDP::clearCollision()
{
   m_collision = 0;
}


bool GGVDP::irqPending() const
{
   return( m_irq != 0 );
}


void GGVDP::irqClear()
{
   m_irq = 0;
}


u16 GGVDP::vcount() const
{
   return( m_vcount );
}


bool GGVDP::paletteChanged()
{
   bool result = m_paletteChanged;
   m_paletteChanged = false;

   return( result );
}


int GGVDP::screenWidth() const
{
   return( m_screenw );
}


int GGVDP::screenHeight() const
{
   return( m_screenh );
}


GGVDP::Color GGVDP::getColor( int n ) const
{
   Color c;

   if( isGG() )
   {
      c.red   = ( ( m_cram[( n << 1 ) + 0] << 4 ) & 0xe0 );
      c.green = ( ( m_cram[( n << 1 ) + 0] << 0 ) & 0xe0 );
      c.blue  = ( ( m_cram[( n << 1 ) + 1] << 4 ) & 0xe0 );
   } else
   {
      c.red   = ( m_cram[n] << 6 ) & 0xc0;
      c.green = ( m_cram[n] << 4 ) & 0xc0;
      c.blue  = ( m_cram[n] << 2 ) & 0xc0;
   }

   return( c );
}


void GGVDP::enableRendering( bool en )
{
   m_render = en;
}


int GGVDP::saveState( u8 *d )
{
   int s = 0;

   memcpy( &d[s], m_pVRAM, 16384 ); s += 16384;
   memcpy( &d[s], &m_address, sizeof( m_address ) ); s += sizeof( m_address );
   memcpy( &d[s], &m_t, sizeof( m_t ) ); s += sizeof( m_t );
   memcpy( &d[s], &m_hilo, sizeof( m_hilo ) ); s += sizeof( m_hilo );
	memcpy( &d[s], &m_line_counter, sizeof( m_line_counter ) ); s += sizeof( m_line_counter );
   memcpy( &d[s], &m_vcount, sizeof( m_vcount ) ); s += sizeof( m_vcount );
   memcpy( &d[s], m_cram, sizeof( m_cram ) ); s += sizeof( m_cram );
   memcpy( &d[s], m_regs, sizeof( m_regs ) ); s += sizeof( m_regs );
   memcpy( &d[s], &m_frame_irq, sizeof( m_frame_irq ) ); s += sizeof( m_frame_irq );
   memcpy( &d[s], &m_line_irq, sizeof( m_line_irq ) ); s += sizeof( m_line_irq );
   memcpy( &d[s], &m_irq, sizeof( m_irq ) ); s += sizeof( m_irq );
   memcpy( &d[s], &m_collision, sizeof( m_collision ) ); s += sizeof( m_collision );

   return( s );
}


int GGVDP::loadState( u8 *d )
{
   int s = 0;

   memcpy( m_pVRAM, &d[s], 16384 ); s += 16384;
   memcpy( &m_address, &d[s], sizeof( m_address ) ); s += sizeof( m_address );
   memcpy( &m_t, &d[s], sizeof( m_t ) ); s += sizeof( m_t );
   memcpy( &m_hilo, &d[s], sizeof( m_hilo ) ); s += sizeof( m_hilo );
   memcpy( &m_line_counter, &d[s], sizeof( m_line_counter ) ); s += sizeof( m_line_counter );
   memcpy( &m_vcount, &d[s], sizeof( m_vcount ) ); s += sizeof( m_vcount );
   memcpy( m_cram, &d[s], sizeof( m_cram ) ); s += sizeof( m_cram );
   memcpy( m_regs, &d[s], sizeof( m_regs ) ); s += sizeof( m_regs );
   memcpy( &m_frame_irq, &d[s], sizeof( m_frame_irq ) ); s += sizeof( m_frame_irq );
   memcpy( &m_line_irq, &d[s], sizeof( m_line_irq ) ); s += sizeof( m_line_irq );
   memcpy( &m_irq, &d[s], sizeof( m_irq ) ); s += sizeof( m_irq );
   memcpy( &m_collision, &d[s], sizeof( m_collision ) ); s += sizeof( m_collision );

   forceDirtyTileCache();

   return( s );
}


/* Write to the address/control register */
void GGVDP::writeControl( u8 d )
{
   if( m_hilo == 0 )
   {
      m_hilo = 1;
      m_t = d;
   } else
   {
      m_hilo = 0;

      if( ( d & 0xf0 ) == 0x80 )
      {
         /* Register write */
         m_regs[d & 0x0f] = m_t;
         m_address = 0;
      } else
      {
         /* VRAM address write */
         m_address = m_t | ( d << 8 );
      }
   }
}


void GGVDP::writeData( u8 d )
{
   /*GGMS * machine = (GGMS*)vdp->val;*/
   u16 t = ( m_address >> 14 ) & 3;
   u16 address = m_address & 0x3fff;
   u8 *pDest;
   /* Reset internal flag */
   m_hilo = 0;

   switch( t )
   {
      case 0x00:
      case 0x01:
      case 0x02:
         /* VRAM */
         pDest = &m_pVRAM[address];

         if( *pDest != d )
         {
            *pDest = d;

            if( address < 0x3800 )
               pushTileCache( address >> 2 );
         }
         break;

      case 0x03:
         m_cram[m_address & ( m_gg ? 0x3f : 0x1f )] = d;
         m_paletteChanged = true;
         break;
   }

   m_address++;
}


u8 GGVDP::readData()
{
   u8 d = 0xff;

   /* Reset internal flag */
   m_hilo = 0;

   if( m_address < 0x8000 )
   {
      /* Video Mem */
      d = m_pVRAM[m_address & 0x3fff];
   } else
   if( ( m_address & 0xc000 ) == 0xc000 )
   {
      /* CRAM */
      d = m_cram[m_address & 0x003f];
   }

   m_address++;
   m_address &= 0xffff;

   return( d );
}


void GGVDP::renderBG( int cl, u8 *oprint, u8 *destline )
{
   int xofs, yofs, i;
   int y;
   u8 *dest;
   int t;
   int mapy, mapx;
   int iofs;
   int tiley, tile, tilep;
   int mapsrcofs;

   u8 *map;
   u8 *overprint;
   u8 *tcache;
   u8 *dtemp;

   xofs = -m_regs[8];
   yofs =  m_regs[9];
   y = cl + yofs;
   mapy = y >> 3;

   if( mapy >= 28 )
      mapy -= 28;

   /* We are in the active scan period
      Render the current bg line */
   dest = destline;

   tiley = y & 7;

   mapsrcofs = ( ( ( (int)m_regs[2]) << 10 ) & 0x3800 ) + ( mapy << 6 );

   iofs = ( ( m_regs[0] & 0x40 ) && ( cl < 16 ) && !m_gg ? 0 : xofs ) +
            ( m_gg ? 48 : 0 );

   map = &( m_pVRAM[mapsrcofs] );

   mapx = ( ( (/*(8 - (iofs & 7)) +*/ iofs ) >> 3 ) & 31 ) << 1;

   tile = map[mapx];
   tilep = map[mapx + 1];
   tile |= (tilep & 1) << 8;
   tcache = &m_tileCache[( tilep >> 1 ) & 7][tile][tiley << 3];

   dtemp = dest;

   tcache += iofs & 7;

   if( !( m_regs[0] & 0x20 ) || m_gg )
   {
      for( i = 0; i < 8 - ( iofs & 7 ); i++ )
      {
         oprint[i] = ( tilep & 0x10 ) << 3;
         *dtemp++ = *tcache++;
      }
   }

   mapx = ( mapx + 2 ) & 63;

   t = m_screenw - ( iofs & 7 );

   i = 8 - ( iofs & 7 );
   overprint = &oprint[i];
   dtemp = &dest[i];

   for (/*i = 8 - (iofs & 7)*/; i < t; i += 8)
   {
      tile = map[mapx++];
      tilep = map[mapx++];
      mapx &= 63;
      tile |= ( tilep & 1 ) << 8;

      tcache = &m_tileCache[( tilep >> 1 ) & 7][tile][tiley << 3];

      overprint[0] =
      overprint[1] =
      overprint[2] =
      overprint[3] =
      overprint[4] =
      overprint[5] =
      overprint[6] =
      overprint[7] = ( tilep & 0x10 ) << 3;
      overprint += 8;

      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
      *dtemp++ = *tcache++;
   }

   tile = map[mapx];
   tilep = map[mapx + 1];
   tile |= ( tilep & 1 ) << 8;
   tcache = &m_tileCache[( tilep >> 1 ) & 7][tile][tiley << 3];

   dtemp = &dest[m_screenw - ( iofs & 7 )];

   for( i = m_screenw - ( iofs & 7 ); i < m_screenw; i++ )
   {
      oprint[i] = ( tilep & 0x10 ) << 3;
      *dtemp++ = *tcache++;
   }

   if( ( m_regs[0] & 0x20 ) && !m_gg )
   {
      dtemp = dest;

      oprint[0] =
      oprint[1] =
      oprint[2] =
      oprint[3] =
      oprint[4] =
      oprint[5] =
      oprint[6] =
      oprint[7] = 0x8f;

      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
      *dtemp++ = 0;
   }
}


/* Render the current raster line */
void GGVDP::renderLine( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs )
{
   int curLine, i,j;
   u8 *pSrc;
   u8 *pDest;
   u8 overprint[256];
   u8 *pOprint;
   u8 *pDestline;
   u8 collision[256];

   /* Get the sprite table */
   u8 *pSprites = &m_pVRAM[( m_regs[5] << 7 ) & 0x3f00];
   /* Stack with all sprites which are visible in the current rasterline */
   int spriteBuf[8];
   int spriteBufTop;

   /* Refresh the tile cache */
   processTileCache();

   /* Get the current line */
   curLine = m_vcount;

   /* Get a pointer to the memory where we are supposed to render the data */
   pDestline = &pDisplayBuffer[( ( curLine + displayBufferYOfs ) * displayBufferWidth ) + displayBufferXOfs];

   renderBG( curLine, overprint, pDestline );

   /* Get all visible sprites at the current rasterline */
   for( spriteBufTop = -1, i = 0; i < 64; i++ )
   {
      int y = pSprites[i];
      int dy = m_regs[1] & 2 ? 16 : 8;

      if( y == 208 )
      {
         break;
      } else
      if( y >= 240 )
      {
         y -= 256;
      }

      if( ( curLine >= y ) && ( curLine < ( y + dy ) ) )
      {
         spriteBufTop++;
         spriteBuf[spriteBufTop] = i;
         if( spriteBufTop == 7 )
         {
            break;
         }
      }
   }

   /* Render the sprites */
   while( spriteBufTop >= 0 )
   {
      /* Current sprite number */
      int n = spriteBuf[spriteBufTop--];

      /* y position */
      int y = pSprites[n] - ( m_regs[0] & 8 );

      /* Height */
      int dy = m_regs[1] & 0x02 ? 16 : 8;

      /* x position */
      int x = pSprites[128 + ( n << 1 )];

      /* Tile number */
      int tile = ( pSprites[128 + ( n << 1 ) + 1] | ( ( m_regs[6] & 4 ) << 6 ) ) &
                  /* Ignore bit 0 for 8x16 sprites */
                  ( dy == 8 ? ~0 : ~1 );

      /* Start and end pixel for the line */
      int spx1 = 0;
      int spx2 = 8;

      int spriteY;

      /* Wrap y position */
      if( y >= 240 ) y -= 256;

      /* Get y pos within the sprite */
      spriteY = curLine - y;

      if( m_gg) x -= 48;

      /* 8x16 sprites */
      if( spriteY >= 8 )
      {
         spriteY -= 8;
         tile++;
      }

      if( ! ( ( curLine >= y ) && ( curLine < ( y + dy ) ) ) )
         continue;

      /* Get source tile data */
      pSrc = &m_tileCache[4][tile][spriteY << 3];

      if( ( x + 8 ) >= m_screenw )
         spx2 = m_screenw - x;

      if( x < 0 )
      {
         spx1 = -x;
         x = 0;
      }

      pDest = &pDestline[x];

      pOprint = &overprint[x + spx1];

      for( j = spx1; j < spx2; j++ )
      {
         u8 u = 0;

         if( !( ( *pOprint & 0x80 ) && ( *pDest & 0xf ) ) )
         {
            u8 col = pSrc[j] | 16;

            if( col != 16 )
            {
               *pDest = col;
               u = 1;
            }
         }

         pDest++;
         pOprint++;

         if( collision[j + x] == 1 )
            m_collision = 1;

         collision[j + x] = u;
      }
   }
}


/* Returns 1 if an INT should be generated */
int GGVDP::cycle( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs )
{
   int result = 0;
   int y1 = ( m_gg ? 24 : 0 );
   int y2 = ( m_gg ? 168 : 192 );

   displayBufferYOfs -= y1;

   if( m_vcount <= 192 )
   {
      if( m_vcount == 192 )
         m_frame_irq = 1;

      if( m_vcount == 0 )
         m_line_counter = m_regs[10];

      if( m_line_counter == 0 )
      {
         m_line_counter = m_regs[10];
         m_line_irq = 1;

         if( m_regs[0] & 0x10 )
            m_irq = 1;
      } else
      {
         m_line_counter--;
      }

      if( ( m_regs[0] & 0x10 ) && ( m_line_irq == 1 ) )
      {
         m_irq = 1;
         result |= GGVDP_CYCLE_IRQ;
      }
   } else
   {
      if( ( m_vcount == 193) && ( m_regs[1] & 0x20 ) && ( m_frame_irq == 1 ) )
      {
         result |= GGVDP_CYCLE_IRQ;
         m_irq = 1;
      }

      m_line_counter = m_regs[10];
   }

   if( ( m_vcount >= y1 ) && ( m_vcount < y2 ) )
   {
      /* Render current line */
      if( ( m_regs[1] & 0x40 )  && m_render )
      {
         renderLine( pDisplayBuffer, displayBufferWidth, displayBufferHeight, displayBufferXOfs, displayBufferYOfs );
      }
   }

   /* Go to next line */
   m_vcount++;

   if( m_vcount > 261 )
   {
      m_vcount = 0;

      if( m_render )
      {
         result |= GGVDP_CYCLE_FINISHEDFRAME;
      }
   }

   return( result );
}
