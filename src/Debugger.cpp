#include "Debugger.h"
#include "DebuggerSectionDisassembly.h"
#include "DebuggerSectionRegisters.h"

#include "ArchSDL.h"
#include "Font.h"


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
   m_Sections.push_back( new SectionDisassembly( this, "Disassembly", 10, 90, 480, 240 ) );
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
