#include <fstream>

#include "Debugger.h"
#include "DebuggerSectionDisassembly.h"
#include "Font.h"
#include "ArchSDL.h"


#define NINSTRUCTIONS 23


Debugger::SectionDisassembly::SectionDisassembly( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height ) :
   Section( pDebugger, name, xp, yp, width, height ),
   m_CursorPosition( 0 ),
   m_NextInstrBreakIsSet( false ),
   m_NextInstrBreak( 0 ),
   m_CursorOffset( ( NINSTRUCTIONS - 1 ) / 2 )
{
}


void Debugger::SectionDisassembly::loadSymbolsFile( std::string fname )
{
   std::ifstream f;
   f.open( fname, std::ifstream::in );
   if( f.is_open() )
   {
      std::string line;
      std::string curSection = "";

      while( !f.eof() )
      {
         std::getline( f, line );

         size_t lp = line.find( "[" );
         size_t rp = line.rfind( "]" );
         if( ( lp != std::string::npos ) && ( rp != std::string::npos ) &&
               ( lp < rp ) )
         {
            curSection = line.substr( lp + 1, rp - lp - 1 );
         } else
         {
            if( curSection == "labels" )
            {
               lp = line.find_first_not_of( " \t\n\r" );
               if( lp == std::string::npos )
                  lp = 0;
               rp = line.find( ":", lp );
               if( rp == std::string::npos )
                  continue;

               std::string sBank = line.substr( lp, rp - lp );
               u8 bank = 0xff;
               try
               {
                  bank = std::stoi( sBank, nullptr, 16 );
               } catch( ... )
               {
                  continue;
               }

               lp = rp + 1;
               rp = line.find_first_not_of( "0123456789abcdefABCDEF", lp );
               if( rp == std::string::npos )
                  continue;
               std::string sOffset = line.substr( lp, rp - lp );
               u16 offset = 0xffff;
               try
               {
                  offset = std::stoi( sOffset, nullptr, 16 );
               } catch( ... )
               {
                  continue;
               }

               lp = rp + 1;
               rp = line.find_first_not_of( " \t\n\r", lp );
               if( rp == std::string::npos )
                  continue;
               std::string label = line.substr( rp );
               rp = label.find_first_of( " \t\r\n" );
               if( rp != std::string::npos )
                  label = label.substr( 0, rp - 1 );
               if( !label.empty() )
               {
                  if( label[0] != '*' )
                  {
                     GGMS::MemoryLocation memLoc = GGMS::MemoryLocation( GGMS::MemoryType_ROM, bank * 0x4000 + offset );
                     m_Labels[memLoc] = label;
                  }
               }
            }
         }
      }
      f.close();
   }
}


bool Debugger::SectionDisassembly::isAlwaysVisible()
{
   return( false );
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
   } else
   // Step over
   if( keyHasBeenPressed( SDLK_F10 ) )
   {
      u16 pc = debugger()->machine()->cpu()->getPC().aword;
      Z80::Instruction instr = debugger()->machine()->cpu()->disassemble( pc );
      if( instr.disassembly.rfind( "CALL", 0 ) == 0 )
      {
         u16 nextPC = pc + instr.code.size();
         m_NextInstrBreak = nextPC;
         m_NextInstrBreakIsSet = true;
         debugger()->activate( false );
      } else
      {
         screenBlank = singleInstructionStep( pScreenBuffer );
         if( !( isKeyPressed( SDLK_RSHIFT ) || isKeyPressed( SDLK_LSHIFT ) ) )
         {
            m_CursorPosition = debugger()->machine()->cpu()->getPC().aword;
         }
         updateDisassembly();
      }
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
         m_CursorPosition = getNextPC( m_CursorPosition, -1 );
/*         if( m_CursorOffset >= 5 )
            m_CursorOffset--;*/
         updateDisassembly();
      }

      if( keyHasBeenPressed( SDLK_DOWN ) )
      {
         m_CursorPosition = getNextPC( m_CursorPosition, 1 );
/*         if( NINSTRUCTIONS - m_CursorOffset >= 5 )
            m_CursorOffset++;*/
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


u16 Debugger::SectionDisassembly::getNextPC( u16 curPC, int step )
{
   int curOffset = -1;
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      if( m_Disassembly[i].label.size() == 0 &&
          m_Disassembly[i].address == curPC )
      {
         curOffset = i;
         break;
      }
   }

   if( curOffset < 0 )
      return( curPC );

   for( int j = curOffset + step; j >= 0 && j < m_Disassembly.size(); j += step )
   {
      if( m_Disassembly[j].label.size() == 0 )
      {
         return( m_Disassembly[j].address );
      }
   }

   return( curPC );
}


void Debugger::SectionDisassembly::printDisassembly( int x, int y ) const
{
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      if( m_Disassembly[i].label != "" )
      {
         print8x8( getScreen(), x, y + ( i * 10 ), 129, false, ( m_Disassembly[i].label + ":" ).c_str() );
      } else
      {
         GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( m_Disassembly[i].address );
         char tmp[256];
         sprintf( tmp, "%s", GGMS::MemoryLocation::toString( ml.memoryType() ).c_str() );
         std::string s = tmp;

         std::string lc;
         lc.append( m_Disassembly[i].hasBreakpoint ? "*" : " " );
         lc.append( debugger()->machine()->cpu()->getPC().aword == m_Disassembly[i].address ? " >" : "  " );

         print8x8( getScreen(), x, y + ( i * 10 ), 129,
            m_CursorPosition == m_Disassembly[i].address,
            lc + s + " - " + m_Disassembly[i].toString() );
      }
   }
}


bool Debugger::SectionDisassembly::isAtBreakpoint( u16 loc ) const
{
   GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( loc );
   if( m_NextInstrBreakIsSet )
   {
      if( m_NextInstrBreak == loc )
      {
         return( true );
      }
   }
   return( m_Breakpoints.find( ml ) != m_Breakpoints.end() );
}


void Debugger::SectionDisassembly::activate( bool ac )
{
   if( !isActivated() && ac )
   {
      m_CursorPosition = debugger()->machine()->cpu()->getPC().aword;
      m_NextInstrBreakIsSet = false;
      updateDisassembly();
   }

   Debugger::Section::activate( ac );
}


std::string Debugger::SectionDisassembly::locationToLabel( u16 loc )
{
   std::string label = "";
   GGMS::MemoryLocation memLoc = debugger()->machine()->addressToReadPage( loc );
   if( m_Labels.find( memLoc ) != m_Labels.end() )
   {
      label = m_Labels[memLoc];
   }

   return( label );
}


void Debugger::SectionDisassembly::updateDisassembly( int nInstructions, int cursorPos )
{
   /* cursorPos: at instr number, start counting at 0
      nInstructions: total number of instructions
      nInstrBeforeCursor = cursorPos - 1
      nInstrAfterCursor = nInstructions - cursorPos - 1 */
   m_Disassembly = debugger()->machine()->cpu()->disassemble( m_CursorPosition, cursorPos, nInstructions - cursorPos - 1 );

   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      GGMS::MemoryLocation ml = debugger()->machine()->addressToReadPage( m_Disassembly[i].address );
      if( m_Labels.find( ml ) != m_Labels.end() )
      {
         std::string label = m_Labels[ml];
         m_Disassembly[i].label = label;
      }

      if( m_Breakpoints.find( ml ) != m_Breakpoints.end() )
      {
         m_Disassembly[i].hasBreakpoint = true;
      }
   }

   int nLinesBeforeCursor = cursorPos;
   int nLinesAfterCursor = nInstructions - cursorPos - 1;
   int offsetAtCursor = -1;

   // ***********************
   std::vector<Z80::Instruction> disassembly;
   for( int i = 0; i < m_Disassembly.size(); i++ )
   {
      if( m_Disassembly[i].label.size() > 0 )
      {
         Z80::Instruction instr;
         instr.address = m_Disassembly[i].address;
         instr.hasBreakpoint = false;
         instr.label = m_Disassembly[i].label;
         disassembly.push_back( instr );
      }

      disassembly.push_back( m_Disassembly[i] );
      disassembly[disassembly.size() - 1].label = "";
      if( disassembly[disassembly.size() - 1].address == m_CursorPosition )
         offsetAtCursor = disassembly.size() - 1;
   }

   if( offsetAtCursor - cursorPos > 0 )
   {
      size_t s1 = disassembly.size();
      disassembly.erase( disassembly.begin(), disassembly.begin() + ( offsetAtCursor - cursorPos ) );
      size_t s2 = disassembly.size();
   }

   if( disassembly.size() > nInstructions )
   {
      disassembly.erase( disassembly.begin() + nInstructions, disassembly.end() );
   }

   m_Disassembly = disassembly;
}


void Debugger::SectionDisassembly::updateDisassembly()
{
    updateDisassembly( NINSTRUCTIONS, m_CursorOffset );
}

