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


class Debugger::Section
{
public:
   Section( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );
   ~Section();

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );
   bool isActivated() const;

   bool singleInstructionStep( u8 *pScreenBuffer );

   int getXPos() const;
   int getYPos() const;
   int getWidth() const;
   int getHeight() const;
   std::string name() const;

protected:
   Debugger *debugger() const;

private:
   bool m_Activated;
   Debugger *m_pDebugger;
   std::string m_Name;
   int m_xPos;
   int m_yPos;
   int m_Width;
   int m_Height;
};


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
