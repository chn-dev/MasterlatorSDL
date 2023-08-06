  switch (read(cpu_getPC + 1)) {
/*         case 0x03: cpu_BC.aword++;cpu_setPC(cpu_getPC + 2);CYCLES(6);break; INC BC */
/*         case 0x13: cpu_DE.aword++;cpu_PC.aword++;CYCLES(6);break; INC DE */
/*         case 0x23: cpu_HL.aword++;cpu_PC.aword++;CYCLES(6);break; INC HL */
/*         case 0x33: cpu_SP.aword++;cpu_PC.aword++;CYCLES(6);break; INC SP */

    /* NEWINSTR */
    case 0x06:	cpu_setB  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD B,nn */
    case 0x0e:	cpu_setC  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD C,nn */
    case 0x16:	cpu_setD  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD D,nn */
    case 0x1e:	cpu_setE  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD E,nn */
    case 0x26:	cpu_setIXh(read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD IXl,nn */
    case 0x2e:	cpu_setIXl(read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD IXh,nn */
    case 0x3e:	cpu_setA  (read(cpu_getPC + 2));cpu_setPC(cpu_getPC + 3);CYCLES(11);break; /* LD A,nn */

    case 0xc7:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x00);CYCLES(11+3);break;  /*  RST 00h */
    case 0xcf:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x08);CYCLES(11+3);break;  /*  RST 08h */
    case 0xd7:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x10);CYCLES(11+3);break;  /*  RST 10h */
    case 0xdf:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x18);CYCLES(11+3);break;  /*  RST 18h */
    case 0xe7:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x20);CYCLES(11+3);break;  /*  RST 20h */
    case 0xef:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x28);CYCLES(11+3);break;  /*  RST 28h */
    case 0xf7:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x30);CYCLES(11+3);break;  /*  RST 30h */
    case 0xff:  cpu_setPC(cpu_getPC + 2);PUSH_PC;cpu_setPC(0x38);CYCLES(11+3);break;  /*  RST 38h */


    case 0x21:  cpu_setIXl(read(cpu_getPC + 2)); /*  LD IX,nn */
                cpu_setIXh(read(cpu_getPC + 3));
                cpu_setPC(cpu_getPC + 4);CYCLES(14);break;

    case 0x22:  dw.abyte.low  = read(cpu_getPC + 2);    /*  LD (nn),IX */
                dw.abyte.high = read(cpu_getPC + 3);
                write(dw.aword+0,cpu_getIXl);
                write(dw.aword+1,cpu_getIXh);
                cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x2a:  dw.abyte.low  = read(cpu_getPC + 2);    /*  LD IX,(nn) */
                dw.abyte.high = read(cpu_getPC + 3);
                cpu_setIXl(read(dw.aword+0));
                cpu_setIXh(read(dw.aword+1));
                cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x36:                /*  LD (IX+d),n */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), read(cpu_getPC + 3));
      cpu_setPC(cpu_getPC + 4);CYCLES(19);break;

    case 0x46:                /*  LD B,(IX+d) */
      cpu_setB(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x4e:                /*  LD C,(IX+d) */
      cpu_setC(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x56:                /*  LD D,(IX+d) */
      cpu_setD(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x5e:                /*  LD E,(IX+d) */
      cpu_setE(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x66:                /*  LD H,(IX+d) */
      cpu_setH(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x6e:                /*  LD L,(IX+d) */
      cpu_setL(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x7e:                /*  LD A,(IX+d) */
      cpu_setA(read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2))));
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

    case 0x70:                /*  LD (IX+d),B */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getB);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x71:                /*  LD (IX+d),C */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getC);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x72:                /*  LD (IX+d),D */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getD);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x73:                /*  LD (IX+d),E */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getE);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x74:                /*  LD (IX+d),H */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getH);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x75:                    /*  LD (IX+d),L */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getL);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
    case 0x77:                /*  LD (IX+d),A */
      write(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)), cpu_getA);
      cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

    case 0xe1:  M_POP(IXh, IXl);cpu_setPC(cpu_getPC + 2);CYCLES(14);break;   /*  POP IX */
    case 0xe5:  M_PUSH(IXh, IXl);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  PUSH IX */
    case 0xe9:  cpu_setPC(cpu_getIX);CYCLES(8);break;   /*  JP IX */

    case 0xf9:  cpu_setSP(cpu_getIX);cpu_setPC(cpu_getPC + 2);CYCLES(10);break; /*  LD SP,IX */

    case 0x09:  M_ADD16(I, X, B, C);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IX,BC */
    case 0x19:  M_ADD16(I, X, D, E);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IX,DE */
    case 0x29:  M_ADD16(I, X, I, X);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IX,IX */
    case 0x39:  M_ADD16(I, X, S, P);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;    /*  ADD IX,SP */

    case 0x23:  cpu_setIX(cpu_getIX + 1);cpu_setPC(cpu_getPC + 2);CYCLES(10);break;   /*  INC IX */
    case 0x2b:  cpu_setIX(cpu_getIX - 1);cpu_setPC(cpu_getPC + 2);CYCLES(10);break;   /*  DEC IX */

  case 0x86:                /*  ADD A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_ADD(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0x8e:                /*  ADC A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_ADC(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0x96:                /*  SUB A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_SUB(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0x9e:                /*  SBC A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_SBC(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0xa6:                /*  AND A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_AND(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0xb6:                /*  OR A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_OR(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0xae:                /*  XOR A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_XOR(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;
  case 0xbe:                /*  CP A,(IX+d) */
    dw.abyte.low = read(cpu_getIX + BYTE2WORD(read(cpu_getPC + 2)));
    M_CP(dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(19);break;

  case 0x34:                /*  INC (IX+d) */
    t2.aword.low.aword = cpu_getIX + BYTE2WORD(read(cpu_getPC + 2));
    dw.abyte.low = read(t2.aword.low.aword);
    M_INC(dw.abyte.low);
    write(t2.aword.low.aword, dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(23);break;

  case 0x35:                /*  DEC (IX+d) */
    t2.aword.low.aword = cpu_getIX + BYTE2WORD(read(cpu_getPC + 2));
    dw.abyte.low = read(t2.aword.low.aword);
    M_DEC(dw.abyte.low);
    write(t2.aword.low.aword, dw.abyte.low);
    cpu_setPC(cpu_getPC + 3);CYCLES(23);break;

  case 0xe3:                /*  EX (SP),IX */
    dw.abyte.low  = read(cpu_getSP+0);
    dw.abyte.high = read(cpu_getSP+1);
    write(cpu_getSP+0,cpu_getIXl);
    write(cpu_getSP+1,cpu_getIXh);
    cpu_setIX(dw.aword);
    cpu_setPC(cpu_getPC + 2);CYCLES(23);break;

    case 0x24: dw.abyte.low = cpu_getIXh; M_INC(dw.abyte.low); cpu_setIXh(dw.abyte.low); cpu_setPC(cpu_getPC + 2);CYCLES(11);break; /*  INC IXh */

    case 0x45: cpu_setB(cpu_getIXl);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;    /*  LD B,IXl */
    case 0x4d: cpu_setC(cpu_getIXl);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;    /*  LD C,IXl */

    case 0x54: cpu_setD(cpu_getIXh);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD D,IXh */
    case 0x5d: cpu_setE(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD E,IXl */
    case 0x60: cpu_setIXh(cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXh,B */
    case 0x62: cpu_setIXh(cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXh,D */
    case 0x67: cpu_setIXh(cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXh,A */
    case 0x68: cpu_setIXl(cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXl,B */
    case 0x6b: cpu_setIXl(cpu_getE); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXl,E */
    case 0x6f: cpu_setIXl(cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD IXl,A */

    case 0x7c: cpu_setA(cpu_getIXh);cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD A,IXh */
    case 0x55: cpu_setD(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD D,IXl */
    case 0x7d: cpu_setA(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break; /*  LD A,IXl */

    case 0x85: M_ADD(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  ADD IXl */
    case 0x8c: M_ADC(cpu_getIXh); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  ADC IXh */
    case 0x8d: M_ADC(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  ADC IXl */
    case 0x95: M_SUB(cpu_getIXl); cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  SUB IXl */

    case 0xad: M_XOR(cpu_getIXl);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;    /*  XOR IXl */
    case 0xb5: M_OR(cpu_getIXl);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  OR A,IXl */
    case 0xbd: M_CP(cpu_getIXl);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;     /*  CP IXl */

    case 0xeb:  M_EX(DE,IX);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;    /*  EX DE,IX */

    case 0xcb:
#include "decode_ddcb.h"
      break;

    default:
      CYCLES(illegalInstruction());
    break;
  }
