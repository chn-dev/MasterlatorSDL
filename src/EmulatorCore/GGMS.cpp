/* GGMS.c */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "GGMS.h"
#include "SN76489/SN76489.h"
#include "Z80/Z80.h"
#include "smsbios.h"

extern int debug;

#define CYCLES_PER_LINE /*227*/253

static u8 *readFile(const char *fname, int *size)
{
   FILE *fh;
   u8 *d;

   if( !( fh = fopen( fname, "rb" ) ) )
      return( 0 );

   fseek( fh, 0, SEEK_END );
   *size = ftell( fh );
   fseek( fh, 0, SEEK_SET );
   d = (u8*)malloc( *size );
   fread( d, *size, 1, fh );
   fclose( fh );

   return( d );
}

static int writeFile( const char *fname, u8 *data, int size )
{
   FILE *fh;

   if( !( fh = fopen( fname, "wb" ) ) )
      return( -1 );

   fwrite( data, size, 1, fh );

   fclose( fh );

   return( 0 );
}


/*int stricmp(char *, char *);*/
static int stri_cmp( const char *a, const char *b )
{
   size_t l, i;

   if( ( l = strlen( a ) ) != strlen( b ) )
      return( -1 );

   for( i = 0; i < l; i++ )
   {
      if( tolower( a[i] ) != tolower( b[i] ) )
         return( -1 );
   }

   return( 0 );
}


static int fsize( const char *fname )
{
   FILE *f;
   int s;

   if( !( f = fopen( fname,"rb" ) ) )
      return( -1 );

   fseek( f, 0, SEEK_END );
   s = ftell( f );
   fclose( f );

   return( s );
}


GGMS::MemoryLocation::MemoryLocation() :
   m_MemoryType( GGMS::MemoryType_NONE ),
   m_Offset( 0 )
{
}


GGMS::MemoryLocation::MemoryLocation( MemoryType memType, int offset ) :
   m_MemoryType( memType ),
   m_Offset( offset )
{
}


GGMS::MemoryLocation::~MemoryLocation()
{
}


bool GGMS::MemoryLocation::isNull()
{
   return( m_MemoryType == GGMS::MemoryType_NONE );
}


bool GGMS::MemoryLocation::operator<( const GGMS::MemoryLocation &other ) const
{
   if( m_MemoryType == other.m_MemoryType )
   {
      return( m_Offset < other.m_Offset );
   } else
   {
      return( m_MemoryType < other.m_MemoryType );
   }
}


bool GGMS::MemoryLocation::operator==( const GGMS::MemoryLocation &other ) const
{
   return( ( m_MemoryType == other.m_MemoryType ) && ( m_Offset == other.m_Offset ) );
}


std::string GGMS::MemoryLocation::toString( MemoryType pt )
{
   switch( pt )
   {
      case MemoryType_BIOS:
         return( "BIOS" );
         break;
      case MemoryType_RAM:
         return( "RAM" );
         break;
      case MemoryType_ROM:
         return( "ROM" );
         break;
      case MemoryType_SRAM:
         return( "SRAM" );
         break;
      default:
         return( std::string() );
         break;
   }
}

int GGMS::MemoryLocation::offset() const
{
   return( m_Offset );
}


GGMS::MemoryType GGMS::MemoryLocation::memoryType() const
{
   return( m_MemoryType );
}


GGMS::GGMS()
{
}


GGMS *GGMS::create( const char *fname, bool debug )
{
   GGMS *pMachine = new GGMS();

   pMachine->m_cyclesPerLine = CYCLES_PER_LINE;
   pMachine->m_debug = debug;

   /* Attempt to load the ROM */
   pMachine->m_romsize = fsize( fname );
   pMachine->m_ramsize = 16384;
   pMachine->m_sramsize = 32768;

   if( pMachine->m_romsize == (unsigned int)-1 )
      return( 0 );

   pMachine->m_pRAM  = (u8*)calloc( pMachine->m_ramsize, 1 );
   pMachine->m_pSRAM = (u8*)calloc( pMachine->m_sramsize, 1 );
   pMachine->m_pROM  = (u8*)calloc( pMachine->m_romsize, 1 );
   pMachine->m_pCPU  = Z80::create( pMachine );
   pMachine->m_pVDP  = GGVDP::create();
   pMachine->m_pSND  = SN76489::create();

   if( !( pMachine->m_pRAM && pMachine->m_pROM &&
          pMachine->m_pCPU && pMachine->m_pVDP && pMachine->m_pSRAM) )
   {
      delete pMachine;
      return( 0 );
   }

   memset( pMachine->m_pSRAM, 0, 32768 );

   for( int i = 0; i < 8; i++ )
   {
      pMachine->m_p1_keys[i] = 1;
      pMachine->m_p2_keys[i] = 1;
   }

   pMachine->m_banks_rom[0] = 0x00;
   pMachine->m_banks_rom[1] = 0x00;
   pMachine->m_banks_rom[2] = 0x01;
   pMachine->m_banks_rom[3] = 0x00;
   pMachine->m_banks_bios[0] = 0x00;
   pMachine->m_banks_bios[1] = 0x00;
   pMachine->m_banks_bios[2] = 0x01;
   pMachine->m_banks_bios[3] = 0x00;
   pMachine->m_pBanks = pMachine->m_banks_bios;
   pMachine->m_jap = true;

   FILE *fhandle;
   if( !( fhandle = fopen( fname, "rb") ) )
   {
      delete pMachine;
      return( 0 );
   }

   /* Handle ROM Header */
   if( ( ( pMachine->m_romsize - 512 ) & 32767 ) == 0 )
   {
      fseek( fhandle, 512, SEEK_SET );
      pMachine->m_romsize -= 512;
   }

   fread( pMachine->m_pROM, pMachine->m_romsize, 1, fhandle );
   fclose( fhandle );

   /* GG or MS mode ? */
   pMachine->m_pVDP->setGG( false );
   if( strlen( fname ) >= 4 )
   {
      pMachine->m_pVDP->setGG(
         ( stri_cmp( &fname[strlen( fname ) - 3], ".gg" )  == 0 ) ||
         ( stri_cmp( &fname[strlen( fname ) - 4], ".ggg" ) == 0 ) );
   }

   if( pMachine->m_pVDP->isGG() )
   {
      pMachine->m_pBanks = pMachine->m_banks_rom;
   }

   pMachine->m_memcontrol = 0;
   pMachine->m_bios_en = true;
   pMachine->m_sram_changed = false;
   pMachine->m_pause = false;
   pMachine->m_countrydetect = 0;

   pMachine->updateAllPages();

   if( pMachine->m_pVDP->isGG() )
   {
      pMachine->m_pGameName = GG_GetGame( pMachine->m_pROM, pMachine->m_romsize );
   } else
   {
      pMachine->m_pGameName = SMS_GetGame( pMachine->m_pROM, pMachine->m_romsize );
   }

   pMachine->m_CRC32 = calccrc32( pMachine->m_pROM, pMachine->m_romsize, ~0 );

   return( pMachine );
}


GGMS::~GGMS()
{
   if( m_pSND )
   {
      delete m_pSND;
      m_pSND = 0;
   }

   if( m_pVDP )
   {
      delete m_pVDP;
      m_pVDP = 0;
   }

   if( m_pCPU )
   {
      delete m_pCPU;
      m_pCPU = 0;
   }

   if( m_pRAM )
   {
      free( m_pRAM );
      m_pRAM = 0;
   }

   if( m_pROM )
   {
      free( m_pROM );
      m_pROM = 0;
   }

   if( m_pSRAM )
   {
      free( m_pSRAM );
      m_pSRAM = 0;
   }
}


int GGMS::addy2ROM( u16 a )
{
   u32 i = ( ( ( (int)m_pBanks[( ( a >> 14 ) + 1 ) & 3] ) & ( ( m_romsize >> 14 ) - 1 ) ) << 14 ) + ( (int)a & 16383 );

   if( ( i >= m_romsize ) && ( ( m_memcontrol & 0x08 ) || m_pVDP->isGG() ) )
   {
      return( 0 );
   }

   return( i );
}


u8 *GGMS::toPointer( const MemoryLocation &memLoc )
{
   u8 *pPointer = 0;

   switch( memLoc.memoryType() )
   {
      case MemoryType_BIOS:
         pPointer = smsbios;
         break;
      case MemoryType_ROM:
         pPointer = m_pROM;
         break;
      case MemoryType_RAM:
         pPointer = m_pRAM;
         break;
      case MemoryType_SRAM:
         pPointer = m_pSRAM;
         break;
      default:
         pPointer = 0;
         break;
   }

   if( !pPointer )
      return( 0 );

   return( &pPointer[memLoc.offset()] );
}


GGMS::MemoryLocation GGMS::addressToReadPage( u16 address )
{
   MemoryLocation ml = m_ReadPages[address >> 10];
   return( MemoryLocation( ml.memoryType(), ml.offset() + ( address & 0x03ff ) ) );
}


GGMS::MemoryLocation GGMS::getReadPage( int page )
{
   unsigned int a = (unsigned int)page << 10;
   MemoryType memType = MemoryType_NONE;
   int offset = 0;

   if( a < 0x0400 )
   {
      if ( ( m_memcontrol & 0x08 ) || m_pVDP->isGG() )
      {
         if( a >= m_romsize )
         {
            memType = MemoryType_ROM;
            offset = 0;
         } else
         {
            memType = MemoryType_ROM;
            offset = a;
         }
      } else
      {
         memType = MemoryType_BIOS;
         offset = a;
      }
   } else
   if( a < 0xc000 )
   {
      if( ( (int)( m_pBanks[0] & 0x08 ) != 0 ) && ( a >= 0x8000 ) )
      {
         memType = MemoryType_SRAM;
         offset = ( a & 0x1fff ) | ( (u16)m_pBanks[0] & 0x04 ) << 12;
      } else
      {
         memType = ( ( m_memcontrol & 0x40 ) && !m_pVDP->isGG() ) ? MemoryType_BIOS : MemoryType_ROM;
         offset = addy2ROM( a );
      }
   } else
   {
      memType = MemoryType_RAM;
      offset = a & 0x1fff;
   }

   return( MemoryLocation( memType, offset ) );
}


void GGMS::updatePage( int page )
{
   if( page == 0 )
   {
      m_pPages[0] = m_pROM;
      m_ReadPages[0] = MemoryLocation( MemoryType_ROM, 0 );
   } else
   {
      m_ReadPages[page] = getReadPage( page );
      m_pPages[page] = toPointer( m_ReadPages[page] );
   }
}


void GGMS::updateAllPages()
{
   for( int i = 0; i < 64; i++ )
   {
      updatePage( i );
   }
}


void GGMS::reset()
{
   m_cyclesPerLine = CYCLES_PER_LINE;

   for( int i = 0; i < 8; i++ )
   {
      m_p1_keys[i] = 1;
      m_p2_keys[i] = 1;
   }

   m_banks_rom[0] = 0x00;
   m_banks_rom[1] = 0x00;
   m_banks_rom[2] = 0x01;
   m_banks_rom[3] = 0x00;
   m_banks_bios[0] = 0x00;
   m_banks_bios[1] = 0x00;
   m_banks_bios[2] = 0x01;
   m_banks_bios[3] = 0x00;

   m_pBanks[0] = 0x00;
   m_pBanks[1] = 0x00;
   m_pBanks[2] = 0x01;
   m_pBanks[3] = 0x00;
   m_jap = false;
   m_pause = false;

   m_pVDP->reset();
   m_pSND->reset();

   updateAllPages();

   m_pCPU->reset();
}


bool GGMS::singleInstructionStep( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs )
{
   bool finishedFrame = false;

   int cpuCycles = m_pCPU->step();
   m_cyclesPerLine -= cpuCycles;

   if( m_cyclesPerLine <= 0 )
   {
      while( m_cyclesPerLine <= 0 )
         m_cyclesPerLine += CYCLES_PER_LINE;

      if( m_pause )
      {
         /* Pause button */
         m_pCPU->interrupt( Z80_NMI );
         m_pause = 0;
      }

      int cycleResult = m_pVDP->cycle( pDisplayBuffer, displayBufferWidth, displayBufferHeight, displayBufferXOfs, displayBufferYOfs );
      if( cycleResult & GGVDP_CYCLE_IRQ )
      {
         m_pCPU->interrupt( 0x38 );
      }

      finishedFrame = ( cycleResult & GGVDP_CYCLE_FINISHEDFRAME ) != 0;

      if( finishedFrame )
      {
         if( m_pVDP->paletteChanged() )
         {
            /* Set palette */
            for( int i = 0; i < 32; i++ )
            {
               m_Palette[i] = m_pVDP->getColor( i );
            }
         }
      }
   }

   return( finishedFrame );
}


bool GGMS::run( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs )
{
   if( m_pause )
   {
      /* Pause button */
      m_pCPU->interrupt( Z80_NMI );
      m_pause = 0;
   }

   // run CPU for a raster line
   int cpuCycles = m_pCPU->run( m_cyclesPerLine );
   m_cyclesPerLine -= cpuCycles;
   bool ok = false;
   while( m_cyclesPerLine <= 0 )
   {
      ok = true;
      m_cyclesPerLine += CYCLES_PER_LINE;
   }

   int cycleResult = 0;
   if( ok )
   {
      cycleResult = m_pVDP->cycle( pDisplayBuffer, displayBufferWidth, displayBufferHeight, displayBufferXOfs, displayBufferYOfs );
      if( cycleResult & GGVDP_CYCLE_IRQ )
      {
         m_pCPU->interrupt( 0x38 );
      }
   }

   return( ( cycleResult & GGVDP_CYCLE_FINISHEDFRAME ) != 0 );
}


bool GGMS::isAtBreakpoint() const
{
   return( m_pCPU->isAtBreakpoint() );
}


void GGMS::renderFrame( u8 *pDisplayBuffer, int displayBufferWidth, int displayBufferHeight, int displayBufferXOfs, int displayBufferYOfs )
{
   while( !run( pDisplayBuffer, displayBufferWidth, displayBufferHeight, displayBufferXOfs, displayBufferYOfs ) )
   {
      if( isAtBreakpoint() )
         break;
   }

   if( m_pVDP->paletteChanged() )
   {
      /* Set palette */
      for( int i = 0; i < 32; i++ )
      {
         m_Palette[i] = m_pVDP->getColor( i );
      }
   }
}


void GGMS::renderAudio( void *pBuffer, int numSamples, int freq )
{
   m_pSND->cycle( freq, (u16*)pBuffer, numSamples );
}


const GGVDP::Color &GGMS::getColor( int n ) const
{
   if( n < 0 )
      n = 0;
   else
   if( n > 31 )
      n = 31;

   return( m_Palette[n] );
}

int GGMS::saveState( const char *fname )
{
   u8 temp;
   u8 *d = (u8*)malloc( 64 * 1024 );
   int s = 0;

   /* GGMS */
   memcpy( &d[s], m_pRAM, 16384 ); s += 16384;
   memcpy( &d[s], &m_countrydetect, sizeof( m_countrydetect ) ); s += sizeof( m_countrydetect );
   memcpy( &d[s], &m_memcontrol, sizeof( m_memcontrol ) ); s += sizeof( m_memcontrol );
   memcpy( &d[s], m_banks_rom, sizeof( m_banks_rom ) ); s += sizeof( m_banks_rom );
   memcpy( &d[s], m_banks_bios, sizeof( m_banks_bios ) ); s += sizeof( m_banks_bios );
   temp = m_pBanks == m_banks_rom ? 0 : 1;
   memcpy( &d[s], &temp, sizeof( temp ) ); s += sizeof( temp );

   /* Z80 */
   s += m_pCPU->saveState( &d[s] );

	/* VDP */
   s += m_pVDP->saveState( &d[s] );

   /* SN76489 */
   s += m_pSND->saveState( &d[s] );

   if( writeFile( fname, d, s ) < 0 )
   {
      free( d );
      return( -1 );
   }

   free( d );

   return( 0 );
}


int GGMS::loadState( const char *fname )
{
   u8 *d;
   int s = 0;
   int size;
   u8 temp = 0;

   if ( !( d = readFile( fname, &size ) ) )
   {
      return( -1 );
   }

   /* GGMS */
   memcpy( m_pRAM, &d[s], 16384 ); s += 16384;
   memcpy( &m_countrydetect, &d[s], sizeof( m_countrydetect ) ); s += sizeof( m_countrydetect );
   memcpy( &m_memcontrol, &d[s], sizeof( m_memcontrol ) ); s += sizeof( m_memcontrol );
   memcpy( m_banks_rom, &d[s], sizeof( m_banks_rom ) ); s += sizeof( m_banks_rom );
   memcpy( m_banks_bios, &d[s], sizeof( m_banks_bios ) ); s += sizeof( m_banks_bios );
   memcpy( &temp, &d[s], sizeof( temp ) ); s += sizeof( temp );

   if( !temp )
   {
      m_pBanks = m_banks_rom;
   } else
   {
      m_pBanks = m_banks_bios;
   }

   /* Z80 */
   s += m_pCPU->loadState( &d[s] );

   /* VDP */
   s += m_pVDP->loadState( &d[s] );

   /* SN76489 */
   s += m_pSND->loadState( &d[s] );

   free( d );

   updateAllPages();

   return( 0 );
}


int GGMS::saveSRAM( const char *fname )
{
   if( writeFile( fname, m_pSRAM, 32768 ) < 0 )
   {
      return( -1 );
   }

   m_sram_changed = false;

   return( 0 );
}


int GGMS::loadSRAM( const char *fname )
{
   u8 *sram;
   int size;

	if( !( sram = readFile( fname, &size ) ) )
   {
      return( -1 );
   }

   if ( size > 32768 )
   {
      return( -1 );
   }

   memcpy( m_pSRAM, sram, size );

   free( sram );

   m_sram_changed = false;

   return( 0 );
}


bool GGMS::sramChanged() const
{
   return( m_sram_changed );
}


void GGMS::enableBIOS()
{
   m_bios_en = true;
   updateAllPages();
}


void GGMS::disableBIOS()
{
   m_bios_en = false;
   m_memcontrol = 0x08;
   updateAllPages();
}


void GGMS::setP1LeftButton( bool b )
{
   if( b )
   {
      m_p1_keys[0] = b ? 0 : 1;
      m_p1_keys[1] = b ? 1 : 0;
   } else
   {
      m_p1_keys[0] = 1;
   }
}


void GGMS::setP1RightButton( bool b )
{
   if( b )
   {
      m_p1_keys[0] = b ? 1 : 0;
      m_p1_keys[1] = b ? 0 : 1;
   } else
   {
      m_p1_keys[1] = 1;
   }
}


void GGMS::setP1UpButton( bool b )
{
   if( b )
   {
      m_p1_keys[2] = ( b ? 0 : 1 );
      m_p1_keys[3] = ( b ? 1 : 0 );
   } else
   {
      m_p1_keys[2] = 1;
   }
}


void GGMS::setP1DownButton( bool b )
{
   if( b )
   {
      m_p1_keys[2] = ( b ? 1 : 0 );
      m_p1_keys[3] = ( b ? 0 : 1 );
   } else
   {
      m_p1_keys[3] = 1;
   }
}


void GGMS::setP1AButton( bool a )
{
   m_p1_keys[4] = ( a ? 0 : 1 );
}


void GGMS::setP1BButton( bool b )
{
   m_p1_keys[5] = ( b ? 0 : 1 );
}


void GGMS::setP1StartButton( bool start )
{
   m_p1_keys[6] = ( start ? 0 : 1 );
}


int GGMS::screenWidth() const
{
   return( m_pVDP->screenWidth() );
}


int GGMS::screenHeight() const
{
   return( m_pVDP->screenHeight() );
}


u8 GGMS::z80_in( u8 loc )
{
   u16 temp;
   switch( loc )
   {
      /* VDP Data Port */
      case 0xbe:
         return( m_pVDP->readData() );
         break;

      case 0xbd:
      case 0xbf:
      {
         u8 i = ( (u8)( m_pVDP->frameIRQ()  ? 1 : 0 ) << 7 ) |
                ( (u8)( m_pVDP->lineIRQ()   ? 1 : 0 ) << 6 ) |
                ( (u8)( m_pVDP->collision() ? 1 : 0 ) << 5 ) /*| 0x1f*/;

         m_pVDP->clearFrameIRQ();
         m_pVDP->clearLineIRQ();
         m_pVDP->clearCollision();

         if( m_pVDP->irqPending() )
         {
            m_pVDP->irqClear();
            m_pCPU->clearINT();
         }

         return( i );

         break;
      }

      case 0x05:
         return( (u8)0x04 );
         break;

      /* CURLINE */
      case 0x7e:
      case 0x7f:
         temp = m_pVDP->vcount();
         if( temp > 0xda )
         {
            return( (u8)( temp - 6 ) );
         } else
         {
            return( (u8)temp );
         }
         break;

      /* Joypad 1 */
      case 0xdc:
      case 0xc0:
         return( (u8)( 0xff & ( ( m_p1_keys[0] << 2 ) | ~0x04 ) &
                              ( ( m_p1_keys[1] << 3 ) | ~0x08 ) &
                              ( ( m_p1_keys[2] << 0 ) | ~0x01 ) &
                              ( ( m_p1_keys[3] << 1 ) | ~0x02 ) &
                              ( ( m_p1_keys[4] << 4 ) | ~0x10 ) &
                              ( ( m_p1_keys[5] << 5 ) | ~0x20 ) ) );
         break;

      /* Joypad 2 */
      case 0xdd:
      case 0xc1:
         return( m_countrydetect | 0x1f );
         break;

      /* GG Start button, localization */
      case 0x00:
         if( m_pVDP->isGG() )
         {
            return( ( ( m_p1_keys[6] & 1 ) << 7 ) |
                    ( m_jap ? 0x00 : 0x40 ) );
         } else
         {
            return( (u8)0xff );
         }
         break;

      default:
         return( (u8)0xff );
         break;
   }
}


void GGMS::z80_out( u8 loc, u8 d )
{
   switch( loc )
   {
      case 0xbd:
      case 0xbf:
         m_pVDP->writeControl( d );
         break;

      case 0xbe:
         m_pVDP->writeData( d );
         break;

      /* Sound */
      case 0x7e:
      case 0x7f:
         m_pSND->write( d );
         break;

      case 0x3e:  /* Memory control */
         if( m_bios_en )
         {
            m_memcontrol = d;

            if( !m_pVDP->isGG() )
            {
               m_pBanks = ( d & 0x08 ) ? m_banks_rom : m_banks_bios;
            }
         }
         break;

      case 0x3f:
         if( ( d & 0x0f ) == 0x05 )
         {
            m_countrydetect = ( d & 0x80 ) | ( ( d & 0x20 ) << 1 );
         }

         if( m_jap )
         {
            m_countrydetect ^= 0xc0;
         }
         break;
   }
}


u8 GGMS::z80_readMem( u16 loc ) const
{
   return( m_pPages[loc >> 10][loc & 0x3ff] );
}


void GGMS::z80_writeMem( u16 loc, u8 d )
{
   u16 a = (u16)loc & 0xffff;
   if( a >= 0xc000 )
   {
      /* WRAM */
      m_pRAM[a & 0x1fff] = d;

      if( a >= 0xfffc )
      {
         int i;
         int p = a & 0x03;

         d &= 0x1f;
/*			machine->banks[p] = d;
			GGMS_UpdateAllPages(machine);*/
         if( m_pBanks[p] != d )
         {
            m_pBanks[p] = d;

            if( p >= 1 )
            {
               for( i = (p - 1) * 16; i < ( ( p - 1 ) * 16 ) + 16; i++ )
               {
                  updatePage( i );
               }
            } else
            {
               for( i = 32; i < 48; i++ )
               {
                  updatePage( i );
               }
            }
         }
      } else
      if( a >= 0x8000 )
      {
         /* SRAM */
         if( (int)( m_pBanks[0] & 0x08 ) != 0 )
         {
            m_pSRAM[( a & 0x1fff ) | ( (u16)m_pBanks[0] & 0x04 ) << 12] = d;
            m_sram_changed = true;
         }
      }
   }
}


Z80 *GGMS::cpu() const
{
   return( m_pCPU );
}


GGVDP *GGMS::vdp() const
{
   return( m_pVDP );
}
