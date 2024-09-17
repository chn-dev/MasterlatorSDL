/*******************************************************************************
 *  Copyright (c) 2024 Christian Nowak <chnowak@web.de>                        *
 *   This file is part of Masterlator.                                         *
 *                                                                             *
 *  Masterlator is free software: you can redistribute it and/or modify it     *
 *  under the terms of the GNU General Public License as published by the Free *
 *  Software Foundation, either version 3 of the License, or (at your option)  *
 *  any later version.                                                         *
 *                                                                             *
 *  Masterlator is distributed in the hope that it will be useful, but         *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 *  for more details.                                                          *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License along    *
 *  with Masterlator. If not, see <https://www.gnu.org/licenses/>.             *
 *******************************************************************************/


/* Z80_macros.h */

#ifndef __Z80_MACROS_H__
#define __Z80_MACROS_H__

#include "Z80.h"
#include "../defines.h"

#define BYTE2WORD(a)  ((u16)(s16)(s8)a)

//#define MASTERLATOR
/*#define NON_OO*/

#define cpu_getPC m_PC.aword
#define cpu_setPC(a) (m_PC.aword = a)

#define cpu_getPCh m_PC.abyte.high
#define cpu_getPCl m_PC.abyte.low

#define cpu_getSP m_SP.aword
#define cpu_setSP(a) (m_SP.aword = a)

#define cpu_getSPh m_SP.abyte.high
#define cpu_getSPl m_SP.abyte.low

#define cpu_getS m_SP.abyte.high
#define cpu_getP m_SP.abyte.low

#define cpu_setSPh(a) (m_SP.abyte.high = a)
#define cpu_setSPl(a) (m_SP.abyte.low = a)

#define cpu_getAF m_AF.aword
#define cpu_setAF(a) (m_AF.aword = a)

#define cpu_getA m_AF.abyte.high
#define cpu_setA(a) (m_AF.abyte.high = a)

#define cpu_getF m_AF.abyte.low
#define cpu_setF(a) (m_AF.abyte.low = a)

#define cpu_getBC m_BC.aword
#define cpu_setBC(a) (m_BC.aword = a)

#define cpu_getB m_BC.abyte.high
#define cpu_setB(a) (m_BC.abyte.high = a)

#define cpu_getC m_BC.abyte.low
#define cpu_setC(a) (m_BC.abyte.low = a)

#define cpu_getDE m_DE.aword
#define cpu_setDE(a) (m_DE.aword = a)

#define cpu_getD m_DE.abyte.high
#define cpu_setD(a) (m_DE.abyte.high = a)

#define cpu_getE m_DE.abyte.low
#define cpu_setE(a) (m_DE.abyte.low = a)

#define cpu_getHL m_HL.aword
#define cpu_setHL(a) (m_HL.aword = a)

#define cpu_getH m_HL.abyte.high
#define cpu_setH(a) (m_HL.abyte.high = a)

#define cpu_getL m_HL.abyte.low
#define cpu_setL(a) (m_HL.abyte.low = a)

#define cpu_getIX m_IX.aword
#define cpu_setIX(a) (m_IX.aword = a)

#define cpu_getIXh m_IX.abyte.high
#define cpu_setIXh(a) (m_IX.abyte.high = a)

#define cpu_getIXl m_IX.abyte.low
#define cpu_setIXl(a) (m_IX.abyte.low = a)

#define cpu_getIY m_IY.aword
#define cpu_setIY(a) (m_IY.aword = a)

#define cpu_getIYh m_IY.abyte.high
#define cpu_setIYh(a) (m_IY.abyte.high = a)

#define cpu_getIYl m_IY.abyte.low
#define cpu_setIYl(a) (m_IY.abyte.low = a)

#define cpu_getaf m_af.aword
#define cpu_setaf(a) (m_af.aword = a)

#define cpu_geta m_af.abyte.high
#define cpu_seta(a) (m_af.abyte.high = a)

#define cpu_getf m_af.abyte.low
#define cpu_setf(a) (m_af.abyte.low = a)

#define cpu_getbc m_bc.aword
#define cpu_setbc(a) (m_bc.aword = a)

#define cpu_getb m_bc.abyte.high
#define cpu_setb(a) (m_bc.abyte.high = a)

#define cpu_getc m_bc.abyte.low
#define cpu_setc(a) (m_bc.abyte.low = a)

#define cpu_getde m_de.aword
#define cpu_setde(a) (m_de.aword = a)

#define cpu_getd m_de.abyte.high
#define cpu_setd(a) (m_de.abyte.high = a)

#define cpu_gete m_de.abyte.low
#define cpu_sete(a) (m_de.abyte.low = a)

#define cpu_gethl m_hl.aword
#define cpu_sethl(a) (m_hl.aword = a)

#define cpu_geth m_hl.abyte.high
#define cpu_seth(a) (m_hl.abyte.high = a)

#define cpu_getl m_hl.abyte.low
#define cpu_setl(a) (m_hl.abyte.low = a)
#define cpu_IFF m_IFF

#define read(a)   (m_pMemInterface->z80_readMem(a))
#define write(a,b)  (m_pMemInterface->z80_writeMem(a,b))

#define read16(a) (read(a) | (read(a + 1) << 8))
#define write16(a,b)  write(a,(u8)(b));write(a + 1, (u8)(b >> 8));
#define in(a)   (m_pMemInterface->z80_in(a))
#define out(a,b)  (m_pMemInterface->z80_out(a,b))



#define M_PUSH(ah, al)\
    write(cpu_getSP-1, cpu_get##ah);\
    write(cpu_getSP-2, cpu_get##al);\
		cpu_setSP(cpu_getSP - 2);

#define M_POP(ah, al)\
    cpu_set##al(read(cpu_getSP+0));\
    cpu_set##ah(read(cpu_getSP+1));\
		cpu_setSP(cpu_getSP + 2);



#define M_READ_PC(c,d)\
    (read(cpu_getPC() + d))

#define M_READ_PC16(c,d)\
    (M_READ_PC(c,d) | (M_READ_PC(c,d + 1)<<8))

#define M_READ_IX_d(d)\
    (read(cpu_getIX() + BYTE2WORD(d)))

#define M_WRITE_IX_d(d,a)\
    (write(cpu_getIX() + BYTE2WORD(d),a)))

#define M_READ_IY_d(d)\
    (read(cpu_getIY() + BYTE2WORD(d)))

#define M_WRITE_IY_d(d,a)\
    (write(cpu_IY.aword+BYTE2WORD(d),a)))



#define PUSH_PC \
			write(cpu_getSP - 2, (u8)(cpu_getPC)); \
			write(cpu_getSP - 1, (u8)(cpu_getPC >> 8)); \
			cpu_setSP(cpu_getSP - 2);

/*
	    write(cpu_SP.aword-2, cpu_PC.abyte.low );\
      write(cpu_SP.aword-1, cpu_PC.abyte.high);\
      cpu_SP.aword-=2;
*/

#define POP_PC \
			cpu_setPC(read(cpu_getSP) | (read(cpu_getSP + 1) << 8)); \
			cpu_setSP(cpu_getSP + 2);

/*    	cpu_PC.abyte.low  = read(cpu_SP.aword++);\
      cpu_PC.abyte.high = read(cpu_SP.aword++);*/



/**** JP ****/
#define M_JP_nn   cpu_setPC(cpu_getPC + 1);\
      t.aword.low.abyte.low  = read(cpu_getPC);\
      t.aword.low.abyte.high = read(cpu_getPC + 1);\
      cpu_setPC(t.aword.low.aword);\
      CYCLES(10);

#define M_JP_NZ_nn  if ( cpu_getF & FLAG_Z ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_JP_nn; }
#define M_JP_Z_nn if ( cpu_getF & FLAG_Z ) { M_JP_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_JP_NC_nn  if ( cpu_getF & FLAG_C ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_JP_nn; }
#define M_JP_C_nn if ( cpu_getF & FLAG_C ) { M_JP_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_JP_PO_nn  if ( cpu_getF & FLAG_PV) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_JP_nn; }
#define M_JP_PE_nn  if ( cpu_getF & FLAG_PV) { M_JP_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_JP_P_nn if ( cpu_getF & FLAG_S ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_JP_nn; }
#define M_JP_N_nn if ( cpu_getF & FLAG_S ) { M_JP_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_JP_HL   cpu_setPC(cpu_getHL); CYCLES(4);


/**** JR ****/
#define JR_e    cpu_setPC(cpu_getPC + BYTE2WORD(read(cpu_getPC + 1))+2);
#define M_JR_e    JR_e; CYCLES(12);
#define M_JR_C_e  if ( cpu_getF & FLAG_C ) { M_JR_e; } else { cpu_setPC(cpu_getPC + 2); CYCLES(7); }
#define M_JR_NC_e if ( cpu_getF & FLAG_C ) { cpu_setPC(cpu_getPC + 2); CYCLES(7); } else { M_JR_e; }
#define M_JR_Z_e  if ( cpu_getF & FLAG_Z ) { M_JR_e; } else { cpu_setPC(cpu_getPC + 2); CYCLES(7); }
#define M_JR_NZ_e if ( cpu_getF & FLAG_Z ) { cpu_setPC(cpu_getPC + 2); CYCLES(7); } else { M_JR_e; }


#define M_CALL_nn t.aword.low.abyte.low  = read(cpu_getPC + 1);\
      t.aword.low.abyte.high = read(cpu_getPC + 2);\
      cpu_setPC(cpu_getPC + 3);PUSH_PC;\
      cpu_setPC(t.aword.low.aword);\
      CYCLES(17);

#define M_CALL_NZ_nn  if ( cpu_getF & FLAG_Z ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_CALL_nn; }
#define M_CALL_Z_nn if ( cpu_getF & FLAG_Z ) { M_CALL_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_CALL_NC_nn  if ( cpu_getF & FLAG_C ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_CALL_nn; }
#define M_CALL_C_nn if ( cpu_getF & FLAG_C ) { M_CALL_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }
#define M_CALL_PO_nn  if ( cpu_getF & FLAG_PV) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_CALL_nn; }
#define M_CALL_PE_nn  if ( cpu_getF & FLAG_PV) { M_CALL_nn; } else { cpu_setPC(cpu_getPC +3); CYCLES(10); }
#define M_CALL_P_nn if ( cpu_getF & FLAG_S ) { cpu_setPC(cpu_getPC + 3); CYCLES(10); } else { M_CALL_nn; }
#define M_CALL_N_nn if ( cpu_getF & FLAG_S ) { M_CALL_nn; } else { cpu_setPC(cpu_getPC + 3); CYCLES(10); }

#define M_RET   POP_PC; CYCLES(10);
#define M_RET_NZ  if ( cpu_getF & FLAG_Z ) { cpu_setPC(cpu_getPC + 1); CYCLES(5); } else { POP_PC; CYCLES(11); }
#define M_RET_Z   if ( cpu_getF & FLAG_Z ) { POP_PC; CYCLES(11); } else { cpu_setPC(cpu_getPC + 1); CYCLES(5); }
#define M_RET_NC  if ( cpu_getF & FLAG_C ) { cpu_setPC(cpu_getPC + 1); CYCLES(5); } else { POP_PC; CYCLES(11); }
#define M_RET_C   if ( cpu_getF & FLAG_C ) { POP_PC; CYCLES(11); } else { cpu_setPC(cpu_getPC + 1); CYCLES(5); }
#define M_RET_PO  if ( cpu_getF & FLAG_PV) { cpu_setPC(cpu_getPC + 1); CYCLES(5); } else { POP_PC; CYCLES(11); }
#define M_RET_PE  if ( cpu_getF & FLAG_PV) { POP_PC; CYCLES(11); } else { cpu_setPC(cpu_getPC + 1); CYCLES(5); }
#define M_RET_P   if ( cpu_getF & FLAG_S ) { cpu_setPC(cpu_getPC + 1); CYCLES(5); } else { POP_PC; CYCLES(11); }
#define M_RET_N   if ( cpu_getF & FLAG_S ) { POP_PC; CYCLES(11); } else { cpu_setPC(cpu_getPC + 1); CYCLES(5); }

#define M_DJNZ_e  cpu_setB(cpu_getB - 1); if (cpu_getB==0) { cpu_setPC(cpu_getPC + 2); CYCLES(8); } else { JR_e; CYCLES(13); }


#define M_IN_A_n  cpu_AF.abyte.high = in(read(cpu_PC.aword+1)); cpu_PC.aword+=2; CYCLES(11);
#define M_OUT_n_A out(read(cpu_PC.aword+1),cpu_AF.abyte.high); cpu_PC.aword+=2; CYCLES(11);

#define M_ADD16(ah, al, bh, bl)  t.adword = (u32)cpu_get##ah##al + (u32)cpu_get##bh##bl;\
      cpu_setF((cpu_getF & ~(FLAG_H|FLAG_N|FLAG_C|FLAG_3|FLAG_5)) |\
             (t.aword.high.abyte.low & FLAG_C) |\
             ((t.aword.low.aword^cpu_get##ah##al^cpu_get##bh##bl)&0x1000?FLAG_H:0)|\
             (t.aword.low.abyte.high&(FLAG_3|FLAG_5)));\
      cpu_set##ah##al(t.aword.low.aword);

#define M_ADC16(ah, al, bh, bl)  tt.adword = (u32)cpu_get##bh##bl + (u32)(cpu_getF & FLAG_C);\
      t.adword = (u32)cpu_get##ah##al + tt.adword;\
      cpu_setF((t.aword.low.abyte.high & FLAG_S) |\
             (t.aword.low.aword==0?FLAG_Z:0) |\
             (t.aword.high.abyte.low & FLAG_C) |\
             (((t.aword.low.abyte.high^cpu_get##ah^cpu_get##bh) & 0x01)?FLAG_H:0)|\
             ((~(cpu_get##ah##al^cpu_get##bh##bl)&(cpu_get##bh##bl^t.aword.low.aword)&0x8000)?FLAG_PV:0)|\
             (t.aword.low.abyte.high&(FLAG_3|FLAG_5)));\
      cpu_set##ah##al(t.aword.low.aword);

#define M_SBC16(ah, al, bh, bl)  tt.adword = (u32)cpu_get##bh##bl + (u32)(cpu_getF & FLAG_C);\
      t.adword = (u32)cpu_get##ah##al - tt.adword;\
      cpu_setF((t.aword.low.abyte.high & FLAG_S) |\
             (t.aword.low.aword==0?FLAG_Z:0) |\
             (t.aword.high.abyte.low & FLAG_C) |\
             (((t.aword.low.aword^cpu_get##ah##al^cpu_get##bh##bl) & 0x0100)?FLAG_H:0)|\
             (((cpu_get##ah##al^cpu_get##bh##bl)&(cpu_get##ah##al^t.aword.low.aword)&0x8000)?FLAG_PV:0)|\
             (t.aword.low.abyte.high&(FLAG_3|FLAG_5)));\
      cpu_set##ah##al(t.aword.low.aword);

#define M_INC(a)  a++;\
      cpu_setF((cpu_getF & FLAG_C) | inctab[a]);

#define M_DEC(a)  a--;\
      cpu_setF((cpu_getF & FLAG_C) | dectab[a]);

#define M_INC_REG(a)  cpu_set##a(cpu_get##a + 1);\
      cpu_setF((cpu_getF & FLAG_C) | inctab[cpu_get##a]);

#define M_DEC_REG(a)  cpu_set##a(cpu_get##a - 1);\
      cpu_setF((cpu_getF & FLAG_C) | dectab[cpu_get##a]);


#define M_ADD(b) \
	cpu_setF(addtab[0][cpu_getA][b]); \
	cpu_setA(cpu_getA + b);

#define M_ADC(b) \
	t.aword.low.aword = cpu_getF & FLAG_C; \
	cpu_setF(addtab[t.aword.low.aword][cpu_getA][b]); \
	cpu_setA(cpu_getA + b + t.aword.low.aword);

#define M_SBC(b) \
	t.aword.low.aword = cpu_getF & FLAG_C; \
	cpu_setF(cptab[t.aword.low.aword][cpu_getA][b]); \
	cpu_setA(cpu_getA - (b + t.aword.low.aword));

#define M_SUB(b) cpu_setF(cptab[0][cpu_getA][b]); \
                 cpu_setA(cpu_getA - b);

#define M_CP(b) cpu_setF(cptab[0][cpu_getA][b]);

/*
#define M_INC(a)  a++;\
      cpu_AF.abyte.low = (cpu_AF.abyte.low & FLAG_C) |\
             SZTable[a] |\
             ((a&0x0f)?0:FLAG_H) |\
             (a==0x80?FLAG_PV:0) |\
             (a&(FLAG_3|FLAG_5));

#define M_DEC(a)  a--;\
      cpu_AF.abyte.low = (cpu_AF.abyte.low & FLAG_C) |\
             SZTable[a] |\
             ((a&0x0f)==0x0f?FLAG_H:0) |\
             (a==0x7f?FLAG_PV:0) |\
             FLAG_N |\
             (a&(FLAG_3|FLAG_5));

#define M_ADD(b)  t.aword.low.aword = (u16)cpu_AF.abyte.high + (u16)b; \
      cpu_AF.abyte.low = SZTable[t.aword.low.abyte.low] | \
             ((t.aword.low.abyte.low^cpu_AF.abyte.high^b)&FLAG_H) | \
             ((~(cpu_AF.abyte.high^b)&(b^t.aword.low.abyte.low)&0x80)?FLAG_PV:0) | \
             t.aword.low.abyte.high | \
             (t.aword.low.abyte.low & (FLAG_3|FLAG_5)); \
      cpu_AF.abyte.high = t.aword.low.abyte.low;

#define M_ADC(b)  dw.aword = (u16)b + (u16)(cpu_AF.abyte.low&FLAG_C); \
      t.aword.low.aword = (u16)cpu_AF.abyte.high + dw.aword; \
      cpu_AF.abyte.low = SZTable[t.aword.low.abyte.low] | \
             ((t.aword.low.abyte.low^cpu_AF.abyte.high^b)&FLAG_H) | \
             ((~(cpu_AF.abyte.high^b)&(b^t.aword.low.abyte.low)&0x80)?FLAG_PV:0) | \
             t.aword.low.abyte.high | \
             (t.aword.low.abyte.low&(FLAG_3|FLAG_5)); \
      cpu_AF.abyte.high = t.aword.low.abyte.low;

#define M_SBC(b)  dw.aword = (u16)b + (u16)(cpu_AF.abyte.low&FLAG_C);\
      t.aword.low.aword = (u16)cpu_AF.abyte.high - dw.aword;\
      cpu_AF.abyte.low = SZTable[t.aword.low.abyte.low] | \
             ((t.aword.low.abyte.low^cpu_AF.abyte.high^b)&FLAG_H) | \
             (((cpu_AF.abyte.high^b)&(cpu_AF.abyte.high^t.aword.low.abyte.low)&0x80)?FLAG_PV:0) | \
             -t.aword.low.abyte.high | \
             FLAG_N | \
             (dw.abyte.low&(FLAG_3|FLAG_5)); \
      cpu_AF.abyte.high = t.aword.low.abyte.low;

#define M_SUB(b)  t.aword.low.aword = (u16)cpu_AF.abyte.high - (u16)b;\
      cpu_AF.abyte.low = SZTable[t.aword.low.abyte.low] | \
             ((t.aword.low.abyte.low^cpu_AF.abyte.high^b)&FLAG_H) | \
             (((cpu_AF.abyte.high^b)&(cpu_AF.abyte.high^t.aword.low.abyte.low)&0x80)>>5) | \
             -t.aword.low.abyte.high | \
             FLAG_N | \
             (t.aword.low.abyte.low&(FLAG_3|FLAG_5));\
      cpu_AF.abyte.high = t.aword.low.abyte.low;

#define M_CP(b) t.aword.low.aword = (u16)cpu_AF.abyte.high - (u16)b;\
      cpu_AF.abyte.low = SZTable[t.aword.low.abyte.low] | \
             ((t.aword.low.abyte.low^cpu_AF.abyte.high^b)&FLAG_H) | \
             (((cpu_AF.abyte.high^b)&(cpu_AF.abyte.high^t.aword.low.abyte.low)&0x80)?FLAG_PV:0) | \
             -t.aword.low.abyte.high | \
             FLAG_N | \
             (cpu_AF.abyte.high & (FLAG_3|FLAG_5));
*/

#define M_AND(b)  cpu_setA(cpu_getA & b);\
      cpu_setF(PSZTable[cpu_getA] |  /* P,S,Z */\
             FLAG_H);        /* H */

#define M_OR(b)   cpu_setA(cpu_getA | b);\
      cpu_setF(PSZTable[cpu_getA]);   /* P,S,Z */

#define M_XOR(b)  cpu_setA(cpu_getA ^ b);\
      cpu_setF(PSZTable[cpu_getA]);   /* P,S,Z */


#define M_EX(a,b) t.aword.low.aword = cpu_get##a;\
      cpu_set##a(cpu_get##b);\
      cpu_set##b(t.aword.low.aword);


#define M_BIT(a,b)  cpu_setF((cpu_getF & FLAG_C) | PSZTable[(a)&(b)] | FLAG_H);

#define M_RLC(a)  t.aword.low.abyte.low = (a & 0x80)>>7;\
      a = (a << 1) | t.aword.low.abyte.low;\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_RL(a)   t.aword.low.abyte.low = (a&0x80)>>7;\
      a = (a<<1) | (cpu_getF & FLAG_C);\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_RRC(a)  t.aword.low.abyte.low = a & FLAG_C;\
      a = (a>>1) | (t.aword.low.abyte.low<<7);\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_RR(a)   t.aword.low.abyte.low = a & FLAG_C;\
      a = (a>>1) | ((cpu_getF & FLAG_C)<<7);\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_SLA(a)  t.aword.low.abyte.low = (a&0x80)>>7;\
      a <<= 1;\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_SRA(a)  t.aword.low.abyte.low = a & FLAG_C;\
      a = (a>>1) | (a&0x80);\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_SRL(a)  t.aword.low.abyte.low = a & FLAG_C;\
      a >>= 1;\
      cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_SLL(a)	t.aword.low.abyte.low = (a&0x80)>>7;\
			a = (a << 1) | 1;\
			cpu_setF(PSZTable[a] | t.aword.low.abyte.low);

#define M_RLC_REG(a)  t.aword.low.abyte.low = (cpu_get##a & 0x80)>>7;\
      cpu_set##a((cpu_get##a << 1) | t.aword.low.abyte.low);\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_RL_REG(a)   t.aword.low.abyte.low = (cpu_get##a & 0x80)>>7;\
      cpu_set##a((cpu_get##a <<1) | (cpu_getF & FLAG_C));\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_RRC_REG(a)  t.aword.low.abyte.low = cpu_get##a & FLAG_C;\
      cpu_set##a((cpu_get##a>>1) | (t.aword.low.abyte.low<<7));\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_RR_REG(a)   t.aword.low.abyte.low = cpu_get##a & FLAG_C;\
      cpu_set##a((cpu_get##a>>1) | ((cpu_getF & FLAG_C)<<7));\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_SLA_REG(a)  t.aword.low.abyte.low = (cpu_get##a&0x80)>>7;\
      cpu_set##a(cpu_get##a << 1);\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_SRA_REG(a)  t.aword.low.abyte.low = cpu_get##a & FLAG_C;\
      cpu_set##a((cpu_get##a>>1) | (cpu_get##a&0x80));\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);

#define M_SRL_REG(a)  t.aword.low.abyte.low = cpu_get##a & FLAG_C;\
      cpu_set##a(cpu_get##a >> 1);\
      cpu_setF(PSZTable[cpu_get##a] | t.aword.low.abyte.low);


#define M_INI   dw.abyte.low = in(cpu_getC);\
      write(cpu_getHL,dw.abyte.low);\
      cpu_setB(cpu_getB - 1);\
      cpu_setHL(cpu_getHL + 1);\
      cpu_setF((cpu_getB==0?FLAG_Z:0)|\
             (cpu_getB&(FLAG_3|FLAG_5|FLAG_S))|\
             ((dw.abyte.low>>6)&FLAG_N)|\
             (((u16)(cpu_getC+1)+(u16)dw.abyte.low)&0xff00?(FLAG_C|FLAG_H):0));

#define M_IND   dw.abyte.low = in(cpu_getC);\
      write(cpu_getHL,dw.abyte.low);\
      cpu_setB(cpu_getB - 1);\
      cpu_setHL(cpu_getHL - 1);\
      cpu_setF((cpu_getB==0?FLAG_Z:0)|\
             (cpu_getB&(FLAG_3|FLAG_5|FLAG_S))|\
             ((dw.abyte.low>>6)&FLAG_N)|\
             (((u16)(cpu_getC-1)+(u16)dw.abyte.low)&0xff00?(FLAG_C|FLAG_H):0));

#define M_OUTI    dw.abyte.low = read(cpu_getHL);\
      out(cpu_getC, dw.abyte.low);\
      cpu_setB(cpu_getB - 1);\
      cpu_setHL(cpu_getHL + 1);\
      cpu_setF((cpu_getB==0?FLAG_Z:0)|\
             (cpu_getB&(FLAG_3|FLAG_5|FLAG_S))|\
             ((dw.abyte.low>>6)&FLAG_N)|\
             (((u16)(cpu_getC+1)+(u16)dw.abyte.low)&0xff00?(FLAG_C|FLAG_H):0));

#define M_OUTD    dw.abyte.low = read(cpu_getHL);\
      out(cpu_getC, dw.abyte.low);\
      cpu_setB(cpu_getB - 1);\
      cpu_setHL(cpu_getHL - 1);\
      cpu_setF((cpu_getB==0?FLAG_Z:0)|\
             (cpu_getB&(FLAG_3|FLAG_5|FLAG_S))|\
             ((dw.abyte.low>>6)&FLAG_N)|\
             (((u16)(cpu_getC-1)+(u16)dw.abyte.low)&0xff00?(FLAG_C|FLAG_H):0));

#define M_LDI   write(cpu_getDE,read(cpu_getHL));\
      cpu_setDE(cpu_getDE + 1);cpu_setHL(cpu_getHL + 1);cpu_setBC(cpu_getBC - 1);\
      cpu_setF((cpu_getF & ~(FLAG_N|FLAG_H|FLAG_PV)) | (cpu_getBC==0?0:FLAG_PV));

#define M_LDD   write(cpu_getDE,read(cpu_getHL));\
      cpu_setDE(cpu_getDE - 1);cpu_setHL(cpu_getHL - 1);cpu_setBC(cpu_getBC - 1);\
      cpu_setF((cpu_getF & ~(FLAG_N|FLAG_H|FLAG_PV)) | (cpu_getBC==0?0:FLAG_PV));

#define M_CPI   dw.abyte.low = read(cpu_getHL);\
      t.aword.low.aword = cpu_getA - dw.abyte.low;\
      cpu_setHL(cpu_getHL + 1);cpu_setBC(cpu_getBC - 1);\
      cpu_setF((cpu_getF & FLAG_C) |\
             SZTable[t.aword.low.abyte.low] |\
               ((t.aword.low.abyte.low^cpu_getA^dw.abyte.low)&FLAG_H) |\
             (cpu_getBC==0?0:FLAG_PV) |\
             FLAG_N);

#define M_CPD   dw.abyte.low = read(cpu_getHL);\
      t.aword.low.aword = cpu_getA - dw.abyte.low;\
      cpu_setHL(cpu_getHL - 1);cpu_setBC(cpu_getBC - 1);\
      cpu_setF((cpu_getF & FLAG_C) |\
             SZTable[t.aword.low.abyte.low] |\
               ((t.aword.low.abyte.low^cpu_getA^dw.abyte.low)&FLAG_H) |\
             (cpu_getBC==0?0:FLAG_PV) |\
             FLAG_N);

#endif /* __Z80_MACROS_H__ */
