#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <set>
#include <map>
#include "EmulatorCore/GGMS.h"

class Debugger : public Z80::DebuggerInterface
{
public:
   Debugger();
   Debugger( GGMS *pMachine );
   ~Debugger();

   void enable( bool en );
   bool isEnabled() const;
   bool doDebug( u8 *pScreenBuffer );
   bool singleInstructionStep( u8 *pScreenBuffer );

   virtual void z80_execStart( u16 loc );
   virtual void z80_execFinish( u16 loc );
   virtual bool z80_break();

private:
   void printMemoryDump( std::string title, int x, int y ) const;
   void printRegisters( int x, int y ) const;
   void printDisassembly( int x, int y ) const;

   std::set<GGMS::ReadPage> m_Breakpoints;
   bool m_Break;

   static void dumpRegister( char *pDest, const char *pName, union_word reg );
   void updateDisassembly();
   void updateDisassembly( int nInstructions, int cursorPos );
   GGMS *m_pMachine;
   bool m_Enabled;
   std::vector<Z80::Instruction> m_Disassembly;
   u16 m_CursorPosition;
};

#endif

