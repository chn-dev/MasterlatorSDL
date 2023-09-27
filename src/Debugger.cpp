#include "Debugger.h"

#include "ArchSDL.h"
#include "Font.h"

#define NINSTRUCTIONS 23

Debugger::Section::Section( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   m_Activated( false ),
   m_pDebugger( pDebugger ),
   m_Name( name ),
   m_xPos( xp ),
   m_yPos( yp ),
   m_Width( width ),
   m_Height( height )
{
}


Debugger::Section::~Section()
{
}


std::string Debugger::Section::name() const
{
   return( m_Name );
}


Debugger *Debugger::Section::debugger() const
{
   return( m_pDebugger );
}


int Debugger::Section::getXPos() const
{
   return( m_xPos );
}


int Debugger::Section::getYPos() const
{
   return( m_yPos );
}


int Debugger::Section::getWidth() const
{
   return( m_Width );
}


int Debugger::Section::getHeight() const
{
   return( m_Height );
}


void Debugger::Section::activate( bool ac )
{
   if( m_Activated != ac )
   {
      m_Activated = ac;
   }
}


bool Debugger::Section::isActivated() const
{
   return( m_Activated );
}


bool Debugger::Section::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pDebugger->singleInstructionStep( pScreenBuffer ) );
}


bool Debugger::Section::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   print8x8( getScreen(), getXPos() + 2, getYPos() + 2, 130, isCurrentSection, m_Name );
   u8 c = isCurrentSection ? 130 : 131;

   for( int x = getXPos(); x < getXPos() + getWidth(); x++ )
   {
      getScreen()->pBuffer[( getYPos() * getScreen()->width ) + x] = c;
      getScreen()->pBuffer[( ( getYPos() + getHeight() ) * getScreen()->width ) + x] = c;
   }

   for( int y = getYPos(); y < getYPos() + getHeight(); y++ )
   {
      getScreen()->pBuffer[( y * getScreen()->width ) + getXPos() ] = c;
      getScreen()->pBuffer[( y * getScreen()->width ) + getXPos() + getWidth() ] = c;
   }
}


Debugger::SectionDisassembly::SectionDisassembly( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   Section( pDebugger, name, xp, yp, width, height ),
   m_CursorPosition( 0 )
{
   m_Breakpoints.insert( GGMS::MemoryLocation( GGMS::MemoryType_ROM, 0x340 ) );
}


bool Debugger::SectionDisassembly::exec( u8 *pScreenBuffer, bool isCurrentSection )
{
   Section::exec( pScreenBuffer, isCurrentSection );

   bool screenBlank = false;

   // Disassembly
   printDisassembly( getXPos() + 2, getYPos() + 2 + 10 );

   // Step into
   if( keyHasBeenPressed( SDLK_F11 ) )
   {
      screenBlank = singleInstructionStep( pScreenBuffer );
      if( !( isKeyPressed( SDLK_RSHIFT ) || isKeyPressed( SDLK_LSHIFT ) ) )
      {
         m_CursorPosition = debugger()->machine()->cpu()->getPC().aword;
      }
      updateDisassembly();
   }

   if( isCurrentSection )
   {
      // Toggle breakpoint
      if( keyHasBeenPressed( SDLK_SPACE ) )
      {
         GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( m_CursorPosition );
         if( m_Breakpoints.find( ml ) == m_Breakpoints.end() )
            m_Breakpoints.insert( ml );
         else
            m_Breakpoints.erase( ml );
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_HOME ) )
      {
         m_CursorPosition = debugger()->machine()->cpu()->getPC().aword;
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_UP ) )
      {
         for( int i = 0; i < m_Disassembly.size(); i++ )
         {
            if( m_CursorPosition == m_Disassembly[i].address )
            {
               m_CursorPosition = m_Disassembly[i - 1].address;
               break;
            }
         }
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_DOWN ) )
      {
         for( int i = 0; i < m_Disassembly.size(); i++ )
         {
            if( m_CursorPosition == m_Disassembly[i].address )
            {
               m_CursorPosition = m_Disassembly[i + 1].address;
               break;
            }
         }
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_PAGEUP ) )
      {
         m_CursorPosition = m_Disassembly[0].address;
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_PAGEDOWN ) )
      {
         m_CursorPosition = m_Disassembly[m_Disassembly.size() - 1].address;
         updateDisassembly();
      }
   }

   return( screenBlank );
}


void Debugger::SectionDisassembly::printDisassembly( int x, int y ) const
{
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( m_Disassembly[i].address );
      char tmp[256];
      sprintf( tmp, "%s:%06xh", GGMS::MemoryLocation::toString( ml.memoryType() ).c_str(), ml.offset() );
      std::string s = tmp;

      std::string lc;
      lc.append( m_Disassembly[i].hasBreakpoint ? "*" : " " );
      lc.append( debugger()->machine()->cpu()->getPC().aword == m_Disassembly[i].address ? " >" : "  " );

      print8x8( getScreen(), x, y + ( i * 10 ), 129,
         m_CursorPosition == m_Disassembly[i].address,
         lc + s + " - " + m_Disassembly[i].toString() );
   }
}


bool Debugger::SectionDisassembly::isAtBreakpoint( u16 loc ) const
{
   return( m_Breakpoints.find( debugger()->machine()->addressToReadPage( loc ) ) != m_Breakpoints.end() );
}


void Debugger::SectionDisassembly::activate( bool ac )
{
   if( !isActivated() && ac )
   {
      m_CursorPosition = debugger()->machine()->cpu()->getPC().aword;
      updateDisassembly();
   }

   Debugger::Section::activate( ac );
}


void Debugger::SectionDisassembly::updateDisassembly( int nInstructions, int cursorPos )
{
   m_Disassembly = debugger()->machine()->cpu()->disassemble( m_CursorPosition, cursorPos, nInstructions - cursorPos - 1 );
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( m_Disassembly[i].address );
      if( m_Breakpoints.find( ml ) != m_Breakpoints.end() )
      {
         m_Disassembly[i].hasBreakpoint = true;
      }
   }
}


void Debugger::SectionDisassembly::updateDisassembly()
{
    updateDisassembly( NINSTRUCTIONS, ( NINSTRUCTIONS - 1 ) / 2 );
}



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



Debugger::Debugger() :
   m_pMachine( 0 ),
   m_Activated( false ),
   m_Break( false ),
   m_CurrentSection( -1 )
{
}


Debugger::Debugger( GGMS *pMachine ) :
   m_pMachine( pMachine ),
   m_Activated( false ),
   m_Break( false ),
   m_CurrentSection( 0 )
{
   m_Sections.push_back( new SectionDisassembly( this, "Disassembly", 10, 80, 480, 240 ) );
   m_Sections.push_back( new SectionRegisters(   this, "Registers",   10, 10, 480, 70 ) );
}


Debugger::~Debugger()
{
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      Section *pSection = m_Sections[i];
      delete pSection;
   }

   m_Sections.clear();
}


GGMS *Debugger::machine() const
{
   return( m_pMachine );
}


void Debugger::z80_execStart( u16 loc )
{
}


void Debugger::z80_execFinish( u16 loc )
{
   if( isActivated() )
      return;

   m_Break = isAtBreakpoint( loc );
}


bool Debugger::isAtBreakpoint( u16 loc ) const
{
   SectionDisassembly *pSecDis = sectionDisassembly();
   if( !pSecDis )
      return( false );
   return( pSecDis->isAtBreakpoint( loc ) );
}


Debugger::SectionDisassembly *Debugger::sectionDisassembly() const
{
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      SectionDisassembly *pSecDis = dynamic_cast<SectionDisassembly *>( m_Sections[i] );
      if( pSecDis )
         return( pSecDis );
   }

   return( 0 );
}


bool Debugger::z80_break()
{
   return( m_Break );
}


void Debugger::activate( bool ac )
{
   if( m_Activated == ac )
      return;

   for( int i = 0; i < m_Sections.size(); i++ )
   {
      m_Sections[i]->activate( ac );
   }

   m_Activated = ac;
}


bool Debugger::isActivated() const
{
   return( m_Activated );
}


bool Debugger::execSections( u8 *pScreenBuffer )
{
   bool screenBlank = false;
   Section *pActiveSection = 0;
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      if( i == m_CurrentSection )
         pActiveSection = m_Sections[i];
      else
         screenBlank = screenBlank || m_Sections[i]->exec( pScreenBuffer, false );
   }

   if( pActiveSection )
   {
      screenBlank = screenBlank || pActiveSection->exec( pScreenBuffer, true );
   }

   return( screenBlank );
}


bool Debugger::doDebug( u8 *pScreenBuffer )
{
   if( !isActivated() )
      return( false );

   bool screenBlank = execSections( pScreenBuffer );

   if( keyHasBeenPressed( SDLK_TAB ) )
   {
      if( isKeyPressed( SDLK_RSHIFT ) || isKeyPressed( SDLK_LSHIFT ) )
      {
         m_CurrentSection--;
         if( m_CurrentSection < 0 )
            m_CurrentSection = m_Sections.size() - 1;
      } else
      {
         m_CurrentSection++;
         if( m_CurrentSection >= m_Sections.size() )
            m_CurrentSection = 0;
      }
   }

//   printMemoryDump( "Memory Dump", 10, 300 );
//   printMemoryDump( "Memory Dump", 300, 300 );
//   printMemoryDump( "Memory Dump", 600, 300 );

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


bool Debugger::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pMachine->singleInstructionStep(
      pScreenBuffer,
      m_pMachine->screenWidth(), m_pMachine->screenHeight(),
      0, 0 ) );
}
