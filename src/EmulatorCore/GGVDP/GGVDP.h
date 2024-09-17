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


/* GGVDP.h */

#ifndef __GGVDP_H__
#define __GGVDP_H__

#include "../defines.h"

#define GGVDP_NUM_TILES 512
#define GGVDP_CYCLE_IRQ 1
#define GGVDP_CYCLE_FINISHEDFRAME 2

class GGVDP
{
public:
   typedef struct
   {
      u8 red;
      u8 green;
      u8 blue;
   } Color;

   ~GGVDP();
   static GGVDP *create();
   void reset();
   int cycle( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );

   int screenWidth() const;
   int screenHeight() const;
   void setGG( bool );
   bool isGG() const;
   int saveState( u8 *d );
   int loadState( u8 *d );

   void writeControl( u8 );
   void writeData( u8 );
   u8 readData();

   void enableRendering( bool en );

   bool frameIRQ() const;
   void clearFrameIRQ();
   bool lineIRQ() const;
   void clearLineIRQ();
   bool collision() const;
   void clearCollision();
   bool irqPending() const;
   void irqClear();
   bool paletteChanged();
   Color getColor( int n ) const;
   u16 vcount() const;

   u8 *vram() const;
   u8 registerValue( int n ) const;

private:
   GGVDP();
   void pushTileCache( u32 tile );
   u32 popTileCache();
   void renderTileLine( u32 tile, u32 line );
   void processTileCache();
   void renderBG( int cl, u8 *oprint, u8 *destline );

   void forceDirtyTileCache();
   void renderTile( u32 );
   void renderLine( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );

   u16 m_address;  /* Current address counter */
   u8  m_t;    /* Temporary byte for selecting */
   /* internal register or address */
   u8  m_hilo;   /* 0 if the next write to the */
                 /* address port will be the lower byte */
   u8 *m_pVRAM;    /* 16kbyte of VRAM */
   u8 m_line_counter;  /* Counter for line interrupts */
   u16 m_vcount;   /* The line currently being rendered */
   u8 m_clatch;    /* Color data latch (GG only) */
   u8 m_cram[64];  /* 64 bytes of CRAM (32 byte on SMS) */
   u8 m_regs[16];  /* 16 registers (0x80..0x8f) */
   bool m_gg;    /* TRUE if in GG mode */
   int m_frame_irq;  /* Frame Interrupt pending */
   int m_line_irq; /* Line Interrupt pending */
   int m_irq;    /* Interrupt pending (no INT acknowledge from Z80 yet) */

   u16 m_screenw;      /* Screen width of the MS/GG */
   u16 m_screenh;      /* Screen height of the MS/GG */

   bool m_paletteChanged;

   bool  m_render;      /* TRUE if the screen should be rendered */
   u8 *m_pLinedata;    /* Pointer to the gfx data of one rendered line (incl. zoom) */
   int m_collision;

   bool m_tileCacheDirty[GGVDP_NUM_TILES * 8]; /* Indicates whether each tile is still valid */
   u32 m_tileCacheTiles[( GGVDP_NUM_TILES * 8 ) + 1];
   int m_tileCacheTop;
   u8 m_tileCache[8][GGVDP_NUM_TILES][8 * 8];
   u8 *m_pBgCache[32 * 28]; /* Pointers to tiles in the tileCache */
   int m_vidmode;
};

#endif
