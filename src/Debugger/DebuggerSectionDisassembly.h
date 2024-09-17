#ifndef DEBUGGERSECTIONDISASSEMBLY_H
#define DEBUGGERSECTIONDISASSEMBLY_H

#include "Debugger.h"
#include "DebuggerSection.h"
#include "EmulatorCore/GGMS.h"

class Debugger::SectionDisassembly : public Section
{
public:
   SectionDisassembly( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );

   virtual bool isAlwaysVisible();

   bool isAtBreakpoint( u16 loc ) const;

   void loadSymbolsFile( std::string fname );

   std::string locationToLabel( u16 loc );

private:
   void printDisassembly( int x, int y ) const;
   u16 getNextPC( u16 curPC, int step );

   std::set<GGMS::MemoryLocation> m_Breakpoints;
   u16 m_NextInstrBreak;
   bool m_NextInstrBreakIsSet;
   void updateDisassembly();
   void updateDisassembly( int nInstructions, int cursorPos );
   std::vector<Z80::Instruction> m_Disassembly;
   u16 m_CursorPosition;
   int m_CursorOffset;
   std::map<GGMS::MemoryLocation, std::string> m_Labels;
};

#endif

