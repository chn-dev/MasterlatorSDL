  switch (read(cpu_getPC + 1)) {

    /* NEWINSTR */
    case 0x06:	cpu_setB  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD B,nn */
    case 0x0e:	cpu_setC  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD C,nn */
    case 0x16:	cpu_setD  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD D,nn */
    case 0x1e:	cpu_setE  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD E,nn */
    case 0x26:	cpu_setIYh(read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD IYl,nn */
    case 0x2e:	cpu_setIYl(read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD IYh,nn */
    case 0x3e:	cpu_setA  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD A,nn */

    case 0x21:  cpu_setIYl(read(cpu_getPC + 2));        /*  LD IY,nn */
                cpu_setIYh(read(cpu_getPC + 3));
                cpu_setPC(cpu_getPC + 4);CYCLES(14);break;

    case 0x22:  dw.abyte.low  = read(cpu_getPC + 2);    /*  LD (nn),IY */
                dw.abyte.high = read(cpu_getPC + 3);
                write(dw.aword+0,cpu_getIYl);
                write(dw.aword+1,cpu_getIYh);
                cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x2a:  dw.abyte.low  = read(cpu_getPC + 2);    /*  LD IY,(nn) */
                dw.abyte.high = read(cpu_getPC + 3);
                cpu_setIYl(read(dw.aword+0));
                cpu_setIYh(read(dw.aword+1));
                cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x36:                                          /*  LD (IY+d),n */
                write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), read(cpu_getPC + 3));
                cpu_setPC(cpu_getPC + 4);CYCLES(19);break;

    case 0x46:                                          /*  LD B,(IY+d) */
                cpu_setB(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
                cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x4e:                /*  LD C,(IY+d) */
                cpu_setC(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
                cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x56:                /*  LD D,(IY+d) */
      cpu_setD(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x5e:                /*  LD E,(IY+d) */
      cpu_setE(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x66:                /*  LD H,(IY+d) */
      cpu_setH(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x6e:                /*  LD L,(IY+d) */
      cpu_setL(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x7e:                /*  LD A,(IY+d) */
      cpu_setA(read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

    case 0x70:                /*  LD (IY+d),B */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getB);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x71:                /*  LD (IY+d),C */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getC);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x72:                /*  LD (IY+d),D */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getD);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x73:                /*  LD (IY+d),E */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getE);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x74:                /*  LD (IY+d),H */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getH);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x75:                    /*  LD (IY+d),L */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getL);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x77:                /*  LD (IY+d),A */
      write(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)), cpu_getA);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

    case 0xe1:  M_POP(IYh, IYl);cpu_setPC(cpu_getPC + 2);CYCLES(14);break;   /*  POP IY */
    case 0xe5:  M_PUSH(IYh, IYl);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  PUSH IY */
    case 0xe9:  cpu_setPC(cpu_getIY);CYCLES(8);break;   /*  JP IY */

    case 0xf9:  cpu_setSP(cpu_getIY);cpu_setPC(cpu_getPC + 2);CYCLES(10);break; /*  LD SP,IY */

    case 0x09:  M_ADD16(I, Y, B, C);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IY,BC */
    case 0x19:  M_ADD16(I, Y, D, E);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IY,DE */
    case 0x29:  M_ADD16(I, Y, I, Y);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IY,IY */
    case 0x39:  M_ADD16(I, Y, S, P);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IY,SP */

    case 0x23:  cpu_setIY(cpu_getIY + 1);cpu_setPC(cpu_getPC + 2);CYCLES(10);break;   /*  INC IY */
    case 0x2b:  cpu_setIY(cpu_getIY - 1);cpu_setPC(cpu_getPC + 2);CYCLES(10);break;   /*  DEC IY */

    case 0x86:                /*  ADD A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_ADD(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x8e:                /*  ADC A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_ADC(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x96:                /*  SUB A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_SUB(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x9e:                /*  SBC A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_SBC(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0xa6:                /*  AND A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_AND(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0xb6:                /*  OR A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_OR(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0xae:                /*  XOR A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_XOR(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0xbe:                /*  CP A,(IY+d) */
      dw.abyte.low = read(cpu_getIY + BYTE2WORD(read(cpu_getPC + 2)));
      M_CP(dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

    case 0x34:                /*  INC (IY+d) */
      t2.aword.low.aword = cpu_getIY + BYTE2WORD(read(cpu_getPC + 2));
      dw.abyte.low = read(t2.aword.low.aword);
      M_INC(dw.abyte.low);
      write(t2.aword.low.aword, dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(23);break;

    case 0x35:                /*  DEC (IY+d) */
      t2.aword.low.aword = cpu_getIY + BYTE2WORD(read(cpu_getPC + 2));
      dw.abyte.low = read(t2.aword.low.aword);
      M_DEC(dw.abyte.low);
      write(t2.aword.low.aword, dw.abyte.low);
      cpu_setPC(cpu_getPC + 3);CYCLES(23);break;

    case 0xe3:                /*  EX (SP),IY */
      dw.abyte.low  = read(cpu_getSP + 0);
      dw.abyte.high = read(cpu_getSP + 1);
      write(cpu_getSP + 0,cpu_getIYl);
      write(cpu_getSP + 1,cpu_getIYh);
      cpu_setIY(dw.aword);
      cpu_setPC(cpu_getPC + 2);CYCLES(23);break;

    case 0x60: cpu_setIYh(cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,B */
    case 0x61: cpu_setIYh(cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,C */
    case 0x62: cpu_setIYh(cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,D */
    case 0x63: cpu_setIYh(cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,E */
    case 0x64: cpu_setIYh(cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,H */
    case 0x65: cpu_setIYh(cpu_getL); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,L */
    case 0x67: cpu_setIYh(cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYh,A */

    case 0x68: cpu_setIYl(cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,B */
    case 0x69: cpu_setIYl(cpu_getC); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,C */
    case 0x6a: cpu_setIYl(cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,D */
    case 0x6b: cpu_setIYl(cpu_getE); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,E */
    case 0x6c: cpu_setIYl(cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,H */
    case 0x6d: cpu_setIYl(cpu_getL); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,L */
    case 0x6f: cpu_setIYl(cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IYl,A */

    case 0x45: cpu_setB(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD B,IYl */
    case 0x54: cpu_setD(cpu_getIYh); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD D,IYh */
    case 0x55: cpu_setD(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;  /*  LD D,IYl */
    case 0x5d: cpu_setE(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD E,IYl */

    case 0x4d: cpu_setC(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD C,IYl */

    case 0x7c: cpu_setA(cpu_getIYh); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD A,IYh */
    case 0x7d: cpu_setA(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD A,IYl */

    case 0x02: write(cpu_getBC,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(11);break;  /*  LD (BC),A */
    case 0x0a: cpu_setA(read(cpu_getBC));cpu_setPC(cpu_getPC + 2);CYCLES(11);break; /*  LD A,(BC) */

    case 0x85: M_ADD(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  ADD IYl */
    case 0x8d: M_ADC(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  ADC IYl */
    case 0x9c: M_SBC(cpu_getIYh); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;      /*  SBC IYh */
    case 0xad: M_XOR(cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  XOR IYl */
    case 0xb5: M_OR (cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  OR  IYl */
    case 0xbd: M_CP (cpu_getIYl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  CP  IYl */

    case 0x2d: dw.abyte.low = cpu_getIYl; M_DEC(dw.abyte.low); cpu_setIYl(dw.abyte.low); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;      /*  DEC IYl */

    case 0xeb:  M_EX(DE,IY);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;    /*  EX DE,IY */

    case 0xcb:
#include "decode_fdcb.h"
      break;

    default:
      CYCLES(illegalInstruction());
      break;
  }
