/* Z80.h */

#ifndef __Z80_H__
#define __Z80_H__

#include "../defines.h"
#include <string>
#include <vector>
#include <map>

#define FLAG_C    0x01
#define FLAG_N    0x02
#define FLAG_PV   0x04
#define FLAG_3    0x08
#define FLAG_H    0x10
#define FLAG_5    0x20
#define FLAG_Z    0x40
#define FLAG_S    0x80


#define Z80_NMI   0x66

class Z80
{
public:
   class Instruction
   {
   public:
      u16 address;
      bool hasBreakpoint;
      std::string label;
      std::vector<u8> code;
      std::string disassembly;

      std::string toString() const;
      bool operator==( const Z80::Instruction &other ) const;
      bool operator!=( const Z80::Instruction &other ) const;
   };

   class MemoryInterface
   {
   public:
      virtual ~MemoryInterface() = 0;
      virtual u8 z80_in( u8 loc ) = 0;
      virtual void z80_out( u8 loc, u8 d ) = 0;
      virtual u8 z80_readMem( u16 loc ) const = 0;
      virtual void z80_writeMem( u16 loc, u8 d ) = 0;
   };

   class DebuggerInterface
   {
   public:
      virtual ~DebuggerInterface() = 0;
      virtual void z80_execStart( u16 loc ) = 0;
      virtual void z80_execFinish( u16 loc ) = 0;
      virtual bool z80_break() = 0;
      virtual std::string z80_locationToLabel( u16 loc ) = 0;
   };

   ~Z80();
   static Z80 *create( Z80::MemoryInterface *pInterface );

   void setDebugger( Z80::DebuggerInterface *pDebuggerInterface );
   bool isAtBreakpoint() const;

   void reset();
   void interrupt( u16 );
   void clearINT();
   int run( int );
   int step();

   int saveState( u8 *d );
   int loadState( u8 *d );
   std::vector<Z80::Instruction> disassemble( u16 loc, int beforeInstr, int afterInstr );
   Z80::Instruction disassemble( u16 loc );

   union_word getAF() const;
   void setAF( union_word v );
   union_word getBC() const;
   void setBC( union_word v );
   union_word getDE() const;
   void setDE( union_word v );
   union_word getHL() const;
   void setHL( union_word v );
   union_word getSP() const;
   void setSP( union_word v );
   union_word getPC() const;
   void setPC( union_word v );
   union_word getIX() const;
   void setIX( union_word v );
   union_word getIY() const;
   void setIY( union_word v );

private:
   Z80( Z80::MemoryInterface *pInterface );
   std::vector<Z80::Instruction> disassemble( u16 loc, int nInstructions );
   std::vector<Z80::Instruction> disassemble( u16 loc, int nInstructions, int *s );

   std::vector<Z80::Instruction> disassembleFromToAddress( u16 startLoc, u16 endLoc );

   int illegalInstruction();
   int execInterrupt();
   int disassemble( u16 loc, u8 *op, char *dest );

   union_word              m_AF;
   union_word              m_BC;
   union_word              m_DE;
   union_word              m_HL;
   union_word              m_SP;
   union_word              m_PC;
   union_word              m_IX;
   union_word              m_IY;
   union_word              m_af;
   union_word              m_bc;
   union_word              m_de;
   union_word              m_hl;
   u8                      m_I;
   u8                      m_R;
   u8                      m_IFF;
   int                     m_cyclecount;
   bool                    m_rdy;   /* Finished executing an instruction ? */
   u16                     m_vec;
   int                     m_eicount;
   Z80::MemoryInterface   *m_pMemInterface;
   Z80::DebuggerInterface *m_pDebuggerInterface;
};

#endif
