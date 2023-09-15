#include "Debugger.h"

#include "ArchSDL.h"
#include "Font.h"

#define NINSTRUCTIONS 31

Debugger::Debugger() :
   m_pMachine( 0 ),
   m_Enabled( false ),
   m_CursorPosition( 0 )
{
}


Debugger::Debugger( GGMS *pMachine ) :
   m_pMachine( pMachine ),
   m_Enabled( false ),
   m_CursorPosition( 0 )
{
}


Debugger::~Debugger()
{
}


void Debugger::enable( bool en )
{
   if( m_Enabled == en )
      return;

   m_CursorPosition = m_pMachine->cpu()->getPC().aword;
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
   bool screenBlank = false;
   union_word af = m_pMachine->cpu()->getAF();
   u8 a = af.abyte.high;
   u8 f = af.abyte.low;
   char tmp[256];
   char flags[256];
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
   print8x8( getScreen(), 10, 10, 129, false, tmp );

   sprintf( tmp, "F  = %02xh = %s", f, flags );
   print8x8( getScreen(), 320, 10, 129, false, tmp );

   sprintf( tmp, "PC = %04xh", m_pMachine->cpu()->getPC().aword );
   print8x8( getScreen(), 320, 20, 129, false, tmp );

   sprintf( tmp, "SP = %04xh", m_pMachine->cpu()->getSP().aword );
   print8x8( getScreen(), 320, 30, 129, false, tmp );

   dumpRegister( tmp, "BC", m_pMachine->cpu()->getBC() );
   print8x8( getScreen(), 10, 20, 129, false, tmp );

   dumpRegister( tmp, "DE", m_pMachine->cpu()->getDE() );
   print8x8( getScreen(), 10, 30, 129, false, tmp );

   dumpRegister( tmp, "HL", m_pMachine->cpu()->getHL() );
   print8x8( getScreen(), 10, 40, 129, false, tmp );

   dumpRegister( tmp, "IX", m_pMachine->cpu()->getIX() );
   print8x8( getScreen(), 10, 50, 129, false, tmp );

   dumpRegister( tmp, "IY", m_pMachine->cpu()->getIY() );
   print8x8( getScreen(), 10, 60, 129, false, tmp );

   sprintf( tmp, "vcount = %d", m_pMachine->vdp()->vcount() );
   print8x8( getScreen(), 10, 70, 129, false, tmp );

   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::ReadPage rp = m_pMachine->addressToReadPage( m_Disassembly[i].address );
      char tmp[256];
      sprintf( tmp, "%s:%06xh", GGMS::ReadPage::toString( rp.pageType() ).c_str(), rp.offset() );
      std::string s = tmp;

      print8x8( getScreen(), 10, 100 + ( i * 10 ), 129,
         m_CursorPosition == m_Disassembly[i].address,
         ( m_pMachine->cpu()->getPC().aword == m_Disassembly[i].address ? " >" : "  " ) + s + " - " + m_Disassembly[i].toString() );
   }

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
            printf( "%d %d\n", m_Disassembly.size(), i );
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
            printf( "%d %d\n", m_Disassembly.size(), i );
            break;
         }
      }
      updateDisassembly();
      printf( "DOWN\n" );
   }

   return( screenBlank );
}


void Debugger::updateDisassembly( int nInstructions, int cursorPos )
{
   m_Disassembly = m_pMachine->cpu()->disassemble( m_CursorPosition, cursorPos, nInstructions - cursorPos - 1 );
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
      reg,
      isprint( reg.abyte.high ) ? reg.abyte.high : ' ',
      isprint( reg.abyte.low ) ? reg.abyte.low : ' ',
      reg.aword,
      (s16)reg.aword );
}
