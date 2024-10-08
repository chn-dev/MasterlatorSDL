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
\file GGMS.h
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the class GGMS (main emulator)
*/
/*----------------------------------------------------------------------------*/
#ifndef __GGMS_H__
#define __GGMS_H__

#include "Z80/Z80.h"
#include "GGVDP/GGVDP.h"
#include "SN76489/SN76489.h"
#include "crc32.h"
#include "defines.h"

/*----------------------------------------------------------------------------*/
/*!
\class GGMS
\date  2024-09-17
*/
/*----------------------------------------------------------------------------*/
class GGMS : public Z80::MemoryInterface
{
public:
   virtual ~GGMS();

   enum MemoryType
   {
      MemoryType_NONE,
      MemoryType_ROM,
      MemoryType_RAM,
      MemoryType_SRAM,
      MemoryType_BIOS
   };

   /*----------------------------------------------------------------------------*/
   /*!
   \class MemoryLocation
   \date  2024-09-17
   */
   /*----------------------------------------------------------------------------*/
   class MemoryLocation
   {
   public:
      MemoryLocation();
      MemoryLocation( MemoryType memType, int offset );
      ~MemoryLocation();

      static std::string toString( MemoryType pt );
      bool isNull();

      bool operator==( const MemoryLocation &other ) const;
      bool operator<( const MemoryLocation &other ) const;

      int offset() const;
      MemoryType memoryType() const;

   private:
      MemoryType m_MemoryType;
      int m_Offset;
   };

   static GGMS *create( const char *fname, bool debug );

   Z80 *cpu() const;
   GGVDP *vdp() const;

   void reset();

   void renderFrame( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );
   bool singleInstructionStep( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );
   void renderAudio( void *pBuffer, int numSamples, int freq );

   bool isAtBreakpoint() const;

   int loadSRAM( const char * );
   int saveSRAM( const char * );
   int loadState( const char * );
   int saveState( const char * );
   bool sramChanged() const;
   void enableBIOS();
   void disableBIOS();

   void setP1LeftButton( bool b );
   void setP1RightButton( bool b );
   void setP1UpButton( bool b );
   void setP1DownButton( bool b );
   void setP1AButton( bool a );
   void setP1BButton( bool b );
   void setP1StartButton( bool start );

   int screenWidth() const;
   int screenHeight() const;
   const GGVDP::Color &getColor( int n ) const;
   MemoryLocation addressToReadPage( u16 address );

   virtual u8 z80_in( u8 loc );
   virtual void z80_out( u8 loc, u8 d );
   virtual u8 z80_readMem( u16 loc ) const;
   virtual void z80_writeMem( u16 loc, u8 d );

private:
   GGMS();
   u8 *toPointer( const MemoryLocation &memLoc );
   bool run( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );
   void updateAllPages();
   void updatePage( int );
   MemoryLocation getReadPage( int page );
   int addy2ROM( u16 );

   bool           m_debug;

   Z80           *m_pCPU;
   GGVDP         *m_pVDP;
   SN76489       *m_pSND;

   u8            *m_pROM;
   unsigned int   m_romsize;

   u8            *m_pRAM;
   unsigned int   m_ramsize;

   u8            *m_pSRAM;
   unsigned int   m_sramsize;

   bool           m_jap;      /* TRUE if japanese, FALSE if european/american */
   u8             m_countrydetect; /* Used as a temporary buffer */
   u8             m_memcontrol;
   u8             m_banks_rom[4];
   u8             m_banks_bios[4];
   u8            *m_pBanks;
   u8             m_p1_keys[8]; /* left, right, up, down, a, b, start (gg only) */
   u8             m_p2_keys[8];
   bool           m_bios_en;
   bool           m_sram_changed;
   u8            *m_pPages[64];
   MemoryLocation m_ReadPages[64];
   const char    *m_pGameName;
   u32            m_CRC32;
   int            m_pause;
   GGVDP::Color   m_Palette[32];
   int            m_cyclesPerLine;
};

#endif
