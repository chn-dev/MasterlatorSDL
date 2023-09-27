#include "Debugger.h"
#include "DebuggerSectionDisassembly.h"
#include "Font.h"
#include "ArchSDL.h"


#define NINSTRUCTIONS 23


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

