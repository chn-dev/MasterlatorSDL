/* GGMS.h */
#ifndef __GGMS_H__
#define __GGMS_H__

#include "Z80/Z80.h"
#include "GGVDP/GGVDP.h"
#include "SN76489/SN76489.h"
#include "crc32.h"
#include "defines.h"

class GGMS : public Z80::MemoryInterface
{
public:
   virtual ~GGMS();

   enum PageType
   {
      PageType_NONE,
      PageType_ROM,
      PageType_RAM,
      PageType_SRAM,
      PageType_BIOS
   };

   class ReadPage
   {
   public:
      ReadPage();
      ReadPage( PageType pageType, int offset );
      ~ReadPage();

      static std::string toString( PageType pt );

      bool operator==( const ReadPage &other ) const;
      bool operator<( const ReadPage &other ) const;

      int offset() const;
      PageType pageType() const;

   private:
      PageType m_PageType;
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
   ReadPage addressToReadPage( u16 address );

   virtual u8 z80_in( u8 loc );
   virtual void z80_out( u8 loc, u8 d );
   virtual u8 z80_readMem( u16 loc ) const;
   virtual void z80_writeMem( u16 loc, u8 d );

private:
   GGMS();
   u8 *toPointer( const ReadPage &readPage );
   bool run( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs );
   void updateAllPages();
   void updatePage( int );
   ReadPage getReadPage( int page );
   int addy2ROM( u16 );

   bool          m_debug;

   Z80          *m_pCPU;
   GGVDP        *m_pVDP;
   SN76489      *m_pSND;

   u8           *m_pROM;
   unsigned int  m_romsize;

   u8           *m_pRAM;
   unsigned int  m_ramsize;

   u8           *m_pSRAM;
   unsigned int  m_sramsize;

   bool          m_jap;      /* TRUE if japanese, FALSE if european/american */
   u8            m_countrydetect; /* Used as a temporary buffer */
   u8            m_memcontrol;
   u8            m_banks_rom[4];
   u8            m_banks_bios[4];
   u8           *m_pBanks;
   u8            m_p1_keys[8]; /* left, right, up, down, a, b, start (gg only) */
   u8            m_p2_keys[8];
   bool          m_bios_en;
   bool          m_sram_changed;
   u8           *m_pPages[64];
   ReadPage      m_ReadPages[64];
   const char   *m_pGameName;
   u32           m_CRC32;
   int           m_pause;
   GGVDP::Color  m_Palette[32];
   int           m_cyclesPerLine;
};

#endif
