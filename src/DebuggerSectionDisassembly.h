#ifndef DEBUGGERSECTIONDISASSEMBLY_H
#define DEBUGGERSECTIONDISASSEMBLY_H

#include "Debugger.h"
#include "DebuggerSection.h"

class Debugger::SectionDisassembly : public Section
{
public:
   SectionDisassembly( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );

   bool isAtBreakpoint( u16 loc ) const;

private:
   void printDisassembly( int x, int y ) const;

   std::set<GGMS::MemoryLocation> m_Breakpoints;
   void updateDisassembly();
   void updateDisassembly( int nInstructions, int cursorPos );
   std::vector<Z80::Instruction> m_Disassembly;
   u16 m_CursorPosition;
};

#endif

