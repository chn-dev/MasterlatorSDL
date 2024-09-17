#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <set>
#include <map>
#include <EmulatorCore/GGMS.h>

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

   void loadSymbolsFile( std::string fname );

   GGMS *machine() const;

   virtual void z80_execStart( u16 loc );
   virtual void z80_execFinish( u16 loc );
   virtual bool z80_break();
   virtual std::string z80_locationToLabel( u16 loc );

private:
   bool isAtBreakpoint( u16 loc ) const;
   bool execSections( u8 *pScreenBuffer );

   bool m_Break;

   GGMS *m_pMachine;
   bool m_Activated;

   class Section;
   class SectionDisassembly;
   class SectionRegisters;
   class SectionMemoryDump;
   class SectionTiles;
   std::vector<Section *> m_Sections;
   std::vector<Section *> m_StaticSections;
   SectionDisassembly *sectionDisassembly() const;
   int m_CurrentSection;
};

#endif

