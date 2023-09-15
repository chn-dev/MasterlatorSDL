#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "EmulatorCore/GGMS.h"

class Debugger
{
public:
   Debugger();
   Debugger( GGMS *pMachine );
   ~Debugger();

   void enable( bool en );
   bool isEnabled() const;
   bool doDebug( u8 *pScreenBuffer );
   bool singleInstructionStep( u8 *pScreenBuffer );

private:
   static void dumpRegister( char *pDest, const char *pName, union_word reg );
   void updateDisassembly();
   void updateDisassembly( int nInstructions, int cursorPos );
   GGMS *m_pMachine;
   bool m_Enabled;
   std::vector<Z80::Instruction> m_Disassembly;
   u16 m_CursorPosition;
};

#endif

