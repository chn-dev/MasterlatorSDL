#include "Debugger.h"
#include "DebuggerSectionDisassembly.h"
#include "DebuggerSectionRegisters.h"
#include "DebuggerSectionMemoryDump.h"
#include "DebuggerSectionTiles.h"

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
   m_CurrentSection( 1 )
{
   m_Sections.push_back( new SectionRegisters(   this, "Registers",   10, 10,  562, 70 ) );
   m_Sections.push_back( new SectionDisassembly( this, "Disassembly", 10, 90,  562, 240 ) );
   m_Sections.push_back( new SectionMemoryDump(  this, "Memory Dump", 10, 340, 562, 130 ) );

   m_Sections.push_back( new SectionTiles( this, "Tiles", 587, 212, 257, /*156*/258 ) );
}


Debugger::~Debugger()
{
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      Section *pSection = m_Sections[i];
      delete pSection;
   }

   m_Sections.clear();

   for( int i = 0; i < m_StaticSections.size(); i++ )
   {
      Section *pSection = m_StaticSections[i];
      delete pSection;
   }

   m_StaticSections.clear();
}


void Debugger::loadSymbolsFile( std::string fname )
{
   sectionDisassembly()->loadSymbolsFile( fname );
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


std::string Debugger::z80_locationToLabel( u16 loc )
{
   return( sectionDisassembly()->locationToLabel( loc ) );
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

/*   if( !isActivated() )
      return( false );*/

   Section *pActiveSection = 0;
   for( int i = 0; i < m_Sections.size(); i++ )
   {
      if( i == m_CurrentSection )
         pActiveSection = m_Sections[i];
      else
      {
         if( isActivated() || m_Sections[i]->isAlwaysVisible() )
         {
            screenBlank = screenBlank || m_Sections[i]->exec( pScreenBuffer, false );
         }
      }
   }

   if( pActiveSection )
   {
      if( isActivated() || pActiveSection->isAlwaysVisible() )
      {
         screenBlank = screenBlank || pActiveSection->exec( pScreenBuffer, true && isActivated() );
      }
   }

   return( screenBlank && isActivated() );
}


bool Debugger::doDebug( u8 *pScreenBuffer )
{
   bool screenBlank = execSections( pScreenBuffer );

   if( !isActivated() )
      return( false );

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


bool Debugger::singleInstructionStep( u8 *pScreenBuffer )
{
   return( m_pMachine->singleInstructionStep(
      pScreenBuffer,
      m_pMachine->screenWidth(), m_pMachine->screenHeight(),
      0, 0 ) );
}

