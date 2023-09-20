#include "Debugger.h"

#include "ArchSDL.h"
#include "Font.h"

#define NINSTRUCTIONS 21

Debugger::Debugger() :
   m_pMachine( 0 ),
   m_Enabled( false ),
   m_CursorPosition( 0 ),
   m_Break( false )
{
}


Debugger::Debugger( GGMS *pMachine ) :
   m_pMachine( pMachine ),
   m_Enabled( false ),
   m_CursorPosition( 0 ),
   m_Break( false )
{
   m_Breakpoints.insert( GGMS::ReadPage( GGMS::PageType_ROM, 0x340 ) );
}


Debugger::~Debugger()
{
}


void Debugger::z80_execStart( u16 loc )
{
}


void Debugger::z80_execFinish( u16 loc )
{
   if( isEnabled() )
      return;

   m_Break = m_Breakpoints.find( m_pMachine->addressToReadPage( loc ) ) != m_Breakpoints.end();
}


bool Debugger::z80_break()
{
   return( m_Break );
}


void Debugger::enable( bool en )
{
   if( m_Enabled == en )
      return;

   m_CursorPosition = m_pMachine->cpu()->getPC().aword;
//   m_CursorPosition = 0x340;
   if( !m_Enabled && en )
   {
      updateDisassembly();
   }

   m_Enabled = en;
}


bool Debugger::isEnabled() const
{
   return( m_Enabled );
}


bool Debugger::doDebug( u8 *pScreenBuffer )
{
   if( !isEnabled() )
      return( false );

   bool screenBlank = false;

   // Registers
   printRegisters( 10, 10 );

   // Disassembly
   printDisassembly( 0, 82 );

   printMemoryDump( "Memory Dump", 10, 300 );
   printMemoryDump( "Memory Dump", 300, 300 );
   printMemoryDump( "Memory Dump", 600, 300 );

   // Toggle breakpoint
   if( keyHasBeenPressed( SDLK_SPACE ) )
   {
      GGMS::ReadPage rp = m_pMachine->addressToReadPage( m_CursorPosition );
      if( m_Breakpoints.find( rp ) == m_Breakpoints.end() )
         m_Breakpoints.insert( rp );
      else
         m_Breakpoints.erase( rp );
      updateDisassembly();
   } else
   // Step into
   if( keyHasBeenPressed( SDLK_F11 ) )
   {
      screenBlank = singleInstructionStep( pScreenBuffer );
      updateDisassembly();
   } else
   if( keyHasBeenPressed( SDLK_UP ) )
   {
      for( int i = 0; i < m_Disassembly.size(); i++ )
      {
         if( m_CursorPosition == m_Disassembly[i].address )
         {
            m_CursorPosition = m_Disassembly[i - 1].address;
            printf( "%ld %d\n", m_Disassembly.size(), i );
            break;
         }
      }
      updateDisassembly();
      printf( "UP\n" );
   } else
   if( keyHasBeenPressed( SDLK_DOWN ) )
   {
      for( int i = 0; i < m_Disassembly.size(); i++ )
      {
         if( m_CursorPosition == m_Disassembly[i].address )
         {
            m_CursorPosition = m_Disassembly[i + 1].address;
            printf( "%ld %d\n", m_Disassembly.size(), i );
            break;
         }
      }
      updateDisassembly();
      printf( "DOWN\n" );
   } else
   if( keyHasBeenPressed( SDLK_PAGEUP ) )
   {
      m_CursorPosition = m_Disassembly[0].address;
      updateDisassembly();
      printf( "PG UP\n" );
   } else
   if( keyHasBeenPressed( SDLK_PAGEDOWN ) )
   {
      m_CursorPosition = m_Disassembly[m_Disassembly.size() - 1].address;
      updateDisassembly();
      printf( "PG DOWN\n" );
   }

   return( screenBlank );
}


void Debugger::printMemoryDump( std::string title, int x, int y ) const
{
   u16 loc = 0xd000;
   int bytesPerLine = 8;
   char tmp[256];

   for( int line = 0; line < 16; line++ )
   {
      std::string l;

      sprintf( tmp, "%04x: ", loc );
      l = tmp;

      for( int i = 0; i < bytesPerLine; i++ )
      {
         if( i > 0 )
            l.append( " " );

         sprintf( tmp, "%02x", m_pMachine->z80_readMem( loc ) );
         l.append( tmp );
         loc++;
      }

      print8x8( getScreen(), x, y + ( 10 * line ), 129, false, l );
   }
}

void Debugger::printRegisters( int x, int y ) const
{
   union_word af = m_pMachine->cpu()->getAF();
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
   print8x8( getScreen(), x, y, 129, false, tmp );

   sprintf( tmp, "F  = %02xh = %s", f, flags );
   print8x8( getScreen(), x + 310, y, 129, false, tmp );

   sprintf( tmp, "PC = %04xh", m_pMachine->cpu()->getPC().aword );
   print8x8( getScreen(), x + 310, y + 10, 129, false, tmp );

   sprintf( tmp, "SP = %04xh", m_pMachine->cpu()->getSP().aword );
   print8x8( getScreen(), x + 310, y + 20, 129, false, tmp );

   dumpRegister( tmp, "BC", m_pMachine->cpu()->getBC() );
   print8x8( getScreen(), x, y + 10, 129, false, tmp );

   dumpRegister( tmp, "DE", m_pMachine->cpu()->getDE() );
   print8x8( getScreen(), x, y + 20, 129, false, tmp );

   dumpRegister( tmp, "HL", m_pMachine->cpu()->getHL() );
   print8x8( getScreen(), x, y + 30, 129, false, tmp );

   dumpRegister( tmp, "IX", m_pMachine->cpu()->getIX() );
   print8x8( getScreen(), x, y + 40, 129, false, tmp );

   dumpRegister( tmp, "IY", m_pMachine->cpu()->getIY() );
   print8x8( getScreen(), x, y + 50, 129, false, tmp );

   sprintf( tmp, "vcount = %d", m_pMachine->vdp()->vcount() );
   print8x8( getScreen(), x, y + 60, 129, false, tmp );
}


void Debugger::printDisassembly( int x, int y ) const
{
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::ReadPage rp = m_pMachine->addressToReadPage( m_Disassembly[i].address );
      char tmp[256];
      sprintf( tmp, "%s:%06xh", GGMS::ReadPage::toString( rp.pageType() ).c_str(), rp.offset() );
      std::string s = tmp;

      std::string lc;
      lc.append( m_Disassembly[i].hasBreakpoint ? "*" : " " );
      lc.append( m_pMachine->cpu()->getPC().aword == m_Disassembly[i].address ? " >" : "  " );

      print8x8( getScreen(), x, y + ( i * 10 ), 129,
         m_CursorPosition == m_Disassembly[i].address,
         lc + s + " - " + m_Disassembly[i].toString() );
   }
}



void Debugger::updateDisassembly( int nInstructions, int cursorPos )
{
   m_Disassembly = m_pMachine->cpu()->disassemble( m_CursorPosition, cursorPos, nInstructions - cursorPos - 1 );
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::ReadPage rp = m_pMachine->addressToReadPage( m_Disassembly[i].address );
      if( m_Breakpoints.find( rp ) != m_Breakpoints.end() )
      {
         m_Disassembly[i].hasBreakpoint = true;
      }
   }
}


void Debugger::updateDisassembly()
{
    updateDisassembly( NINSTRUCTIONS, ( NINSTRUCTIONS - 1 ) / 2 );
}


bool Debugger::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pMachine->singleInstructionStep(
      pScreenBuffer,
      m_pMachine->screenWidth(), m_pMachine->screenHeight(),
      0, 0 ) );
}


void Debugger::dumpRegister( char *pDest, const char *pName, union_word reg )
{
   sprintf( pDest, "%s = %04xh = '%c%c' = %5du = %6di",
      pName,
      reg.aword,
      isprint( reg.abyte.high ) ? reg.abyte.high : ' ',
      isprint( reg.abyte.low ) ? reg.abyte.low : ' ',
      reg.aword,
      (s16)reg.aword );
}
