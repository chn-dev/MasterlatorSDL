#ifndef DEBUGGERSECTIONMEMORYDUMP_H
#define DEBUGGERSECTIONMEMORYDUMP_H

#include "Debugger.h"
#include "DebuggerSection.h"
#include "DebuggerSectionRegisters.h"

class Debugger::SectionMemoryDump : public Section
{
public:
   SectionMemoryDump( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

   virtual bool isAlwaysVisible();

private:
   void printMemoryDump( std::string title, int x, int y );
   u16 getRegisterValue16( SectionRegisters::Z80Register reg ) const;

   const std::vector<std::pair<SectionRegisters::Z80Register, std::string> > m_Regs
   {
      { SectionRegisters::Z80Register_None, "" },
      { SectionRegisters::Z80Register_BC, "BC" },
      { SectionRegisters::Z80Register_DE, "DE" },
      { SectionRegisters::Z80Register_HL, "HL" },
      { SectionRegisters::Z80Register_IX, "IX" },
      { SectionRegisters::Z80Register_IY, "IY" },
      { SectionRegisters::Z80Register_SP, "SP" },
      { SectionRegisters::Z80Register_PC, "PC" }
   };

   u16 m_Location;
   u16 m_Offset;
   int m_n;
};

#endif

