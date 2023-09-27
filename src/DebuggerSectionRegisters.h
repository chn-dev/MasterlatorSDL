#ifndef DEBUGGERSECTIONREGISTERS_H
#define DEBUGGERSECTIONREGISTERS_H

#include "Debugger.h"
#include "DebuggerSection.h"

class Debugger::SectionRegisters : public Section
{
public:
   enum Z80Register
   {
      Z80Register_A,
      Z80Register_BC,
      Z80Register_DE,
      Z80Register_HL,
      Z80Register_IX,
      Z80Register_IY,
      Z80Register_F,
      Z80Register_PC,
      Z80Register_SP
   };

   SectionRegisters( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

private:
   static void dumpRegister( char *pDest, const char *pName, union_word reg );
   void printRegisters( int x, int y, bool isCurrentSection ) const;

   Z80Register m_CurrentRegister;
   int m_CurrentDigit;
   std::map<SDL_Keycode, std::map<Z80Register, Z80Register> > m_RegKeyMap;
};

#endif

