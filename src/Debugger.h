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

   void activate(  bool ac );
   bool isActivated() const;
   bool doDebug( u8 *pScreenBuffer );
   bool singleInstructionStep( u8 *pScreenBuffer );

   GGMS *machine() const;

   virtual void z80_execStart( u16 loc );
   virtual void z80_execFinish( u16 loc );
   virtual bool z80_break();

private:
   void printMemoryDump( std::string title, int x, int y ) const;
   bool isAtBreakpoint( u16 loc ) const;
   bool execSections( u8 *pScreenBuffer );

   bool m_Break;

   GGMS *m_pMachine;
   bool m_Activated;

   class Section;
   class SectionDisassembly;
   class SectionRegisters;
   std::vector<Section *> m_Sections;
   SectionDisassembly *sectionDisassembly() const;
   int m_CurrentSection;
};

#endif

