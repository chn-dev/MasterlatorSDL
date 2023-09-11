/* Z80.h */

#ifndef __Z80_H__
#define __Z80_H__

#include "../defines.h"
#include <string>
#include <vector>

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
      std::vector<u8> code;
      std::string disassembly;

      std::string toString() const;
      bool operator==( const Z80::Instruction &other ) const;
      bool operator!=( const Z80::Instruction &other ) const;
   };

   class Interface
   {
   public:
      virtual ~Interface() = 0;
      virtual u8 z80_in( u8 loc ) = 0;
      virtual void z80_out( u8 loc, u8 d ) = 0;
      virtual u8 z80_readMem( u16 loc ) const = 0;
      virtual void z80_writeMem( u16 loc, u8 d ) = 0;
      virtual void z80_exec( u16 loc ) = 0;
   };

   ~Z80();
   static Z80 *create( Z80::Interface *pInterface );
   void reset();
   void interrupt( u16 );
   void clearINT();
   int run( int );
   int step();

   int saveState( u8 *d );
   int loadState( u8 *d );
   std::vector<Z80::Instruction> disassemble( u16 loc, int beforeInstr, int afterInstr );

   union_word getAF() const;
   union_word getBC() const;
   union_word getDE() const;
   union_word getHL() const;
   union_word getSP() const;
   union_word getPC() const;
   union_word getIX() const;
   union_word getIY() const;

private:
   Z80( Z80::Interface *pInterface );
   std::vector<Z80::Instruction> disassemble( u16 loc, int nInstructions );
   std::vector<Z80::Instruction> disassemble( u16 loc, int nInstructions, int *s );
   int illegalInstruction();
   int execInterrupt();
   int disassemble( u8 *op, char *dest );

   union_word      m_AF;
   union_word      m_BC;
   union_word      m_DE;
   union_word      m_HL;
   union_word      m_SP;
   union_word      m_PC;
   union_word      m_IX;
   union_word      m_IY;
   union_word      m_af;
   union_word      m_bc;
   union_word      m_de;
   union_word      m_hl;
   u8              m_I;
   u8              m_R;
   u8              m_IFF;
   int             m_cyclecount;
   bool            m_rdy;   /* Finished executing an instruction ? */
   u16             m_vec;
   int             m_eicount;
   Z80::Interface *m_pInterface;
};

#endif
