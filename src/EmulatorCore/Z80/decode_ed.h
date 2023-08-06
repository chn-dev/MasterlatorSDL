  switch (read(cpu_getPC + 1)) {

    case 0x43:            /*  LD (nn),BC */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      write(dw.aword+0, cpu_getC);
      write(dw.aword+1, cpu_getB);
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x53:            /*  LD (nn),DE */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      write(dw.aword+0, cpu_getE);
      write(dw.aword+1, cpu_getD);
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x63:            /*  LD (nn),HL */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      write(dw.aword+0, cpu_getL);
      write(dw.aword+1, cpu_getH);
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x73:            /*  LD (nn),SP */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      write(dw.aword+0, cpu_getSPl);
      write(dw.aword+1, cpu_getSPh);
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x4b:            /*  LD BC,(nn) */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      cpu_setC(read(dw.aword+0));
      cpu_setB(read(dw.aword+1));
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x5b:            /*  LD DE,(nn) */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      cpu_setE(read(dw.aword+0));
      cpu_setD(read(dw.aword+1));
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x6b:            /*  LD HL,(nn) */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      cpu_setL(read(dw.aword+0));
      cpu_setH(read(dw.aword+1));
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;
    case 0x7b:            /*  LD SP,(nn) */
      dw.abyte.low  = read(cpu_getPC + 2);
      dw.abyte.high = read(cpu_getPC + 3);
      cpu_setSPl(read(dw.aword+0));
      cpu_setSPh(read(dw.aword+1));
      cpu_setPC(cpu_getPC + 4);CYCLES(20);break;

    case 0x47:  m_I = cpu_getA;cpu_setPC(cpu_getPC + 2);CYCLES(9);break;  /*  LD I,A */
    case 0x4f:  m_R = cpu_getA;cpu_setPC(cpu_getPC + 2);CYCLES(9);break;  /*  LD R,A */
    case 0x57:            /*  LD A,I */
      cpu_setA(m_I);
      cpu_setF((cpu_getF & FLAG_C) | SZTable[cpu_getA] | ((cpu_IFF&0x02)<<1));
      cpu_setPC(cpu_getPC + 2);CYCLES(9);break;
    case 0x5f:            /*  LD A,R */
      cpu_setA(m_R);
      cpu_setF((cpu_getF & FLAG_C) | SZTable[cpu_getA] | ((cpu_IFF&0x02)<<1));
      cpu_setPC(cpu_getPC + 2);CYCLES(9);break;

    case 0x40:            /*  IN B,(C) */
      cpu_setB(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getB]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x48:            /*  IN C,(C) */
      cpu_setC(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getC]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x50:            /*  IN D,(C) */
      cpu_setD(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getD]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x58:            /*  IN E,(C) */
      cpu_setE(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getE]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x60:            /*  IN H,(C) */
      cpu_setH(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getH]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x68:            /*  IN L,(C) */
      cpu_setL(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getL]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x70:            /*  IN (C) */
      dw.abyte.low=in(cpu_getC);
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[dw.abyte.low]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;
    case 0x78:            /*  IN A,(C) */
      cpu_setA(in(cpu_getC));
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getA]);
      cpu_setPC(cpu_getPC + 2);CYCLES(12);break;

    case 0xa2:  M_INI;cpu_setPC(cpu_getPC + 2);CYCLES(16);break; /*  INI */
    case 0xb2:            /*  INIR */
      M_INI;if(cpu_getF & FLAG_Z)
              {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
              {CYCLES(21);}
      break;

    case 0xaa:  M_IND;cpu_setPC(cpu_getPC + 2);CYCLES(16);break; /*  IND */
    case 0xba:            /*  INDR */
      M_IND;if(cpu_getF & FLAG_Z)
              {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
              {CYCLES(21);}
      break;

    case 0x41:  out(cpu_getC,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),B */
    case 0x49:  out(cpu_getC,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),C */
    case 0x51:  out(cpu_getC,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),D */
    case 0x59:  out(cpu_getC,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),E */
    case 0x61:  out(cpu_getC,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),H */
    case 0x69:  out(cpu_getC,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),L */
    case 0x79:  out(cpu_getC,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(12); break; /*  OUT (C),A */

    case 0xa3:  M_OUTI;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;  /*  OUTI */
    case 0xb3:            /*  OTIR */
      M_OUTI;if(cpu_getF & FLAG_Z)
                {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
                {CYCLES(21);}
      break;

    case 0xab:  M_OUTD;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;  /*  OUTD */
    case 0xbb:            /*  OTDR */
      M_OUTD;if(cpu_getF & FLAG_Z)
                {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
                {CYCLES(21);}
      break;


    case 0x4a:  M_ADC16(H, L, B, C);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  ADC HL,BC */
    case 0x5a:  M_ADC16(H, L, D, E);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  ADC HL,DE */
    case 0x6a:  M_ADC16(H, L, H, L);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  ADC HL,HL */
    case 0x7a:  M_ADC16(H, L, S, P);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  ADC HL.SP */

    case 0x42:  M_SBC16(H, L, B, C);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  SBC HL,BC */
    case 0x52:  M_SBC16(H, L, D, E);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  SBC HL,DE */
    case 0x62:  M_SBC16(H, L, H, L);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  SBC HL,HL */
    case 0x72:  M_SBC16(H, L, S, P);cpu_setPC(cpu_getPC + 2);CYCLES(15);break;  /*  SBC HL,SP */

    case 0xa0:  M_LDI;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;       /*  LDI */
    case 0xb0:                  /*  LDIR */
      M_LDI;if(cpu_getF & FLAG_PV)
              {CYCLES(21);} else
              {cpu_setPC(cpu_getPC + 2);CYCLES(16);}
      break;

    case 0xa8:  M_LDD;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;       /*  LDD */
    case 0xb8:                  /*  LDDR */
      M_LDD;if(cpu_getF & FLAG_PV)
              {CYCLES(21);} else
              {cpu_setPC(cpu_getPC + 2);CYCLES(16);}
      break;

    case 0xa1:  M_CPI;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;       /*  CPI */
    case 0xb1:                  /*  CPIR */
      M_CPI;if((cpu_getF&FLAG_Z) || (!(cpu_getF&FLAG_PV)))
              {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
              {CYCLES(21);}
      break;

    case 0xa9:  M_CPD;cpu_setPC(cpu_getPC + 2);CYCLES(16);break;       /*  CPD */
    case 0xb9:                  /*  CPDR */
      M_CPD;if((cpu_getF&FLAG_Z) || (!(cpu_getF&FLAG_PV)))
                {cpu_setPC(cpu_getPC + 2);CYCLES(16);} else
                {CYCLES(21);}
      break;

    case 0x4d:  POP_PC;CYCLES(14);break;           /*  RETI */

    case 0x45:                  /*  RETN */
      cpu_IFF = (cpu_IFF & 0xfe) | ((cpu_IFF&0x02)>>1);POP_PC;
      CYCLES(14);break;

    case 0x44:                  /*  NEG */
      dw.abyte.low=cpu_getA;cpu_setA(0);M_SUB(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;

    case 0x46:  cpu_IFF &= 0xf3;cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  IM0 */
    case 0x56:  cpu_IFF=(cpu_IFF&0xf3)|0x04;cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  IM1 */
    case 0x5e:  cpu_IFF=(cpu_IFF&0xf3)|0x08;cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  IM2 */

    case 0x6f:                  /*  RLD */
      dw.abyte.low  = read(cpu_getHL);
      dw.abyte.high = cpu_getA;
      cpu_setA((dw.abyte.high&0xf0)|((dw.abyte.low>>4)&0x0f));
      dw.abyte.low = (dw.abyte.low<<4) | (dw.abyte.high&0x0f);
      write(cpu_getHL,dw.abyte.low);
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getA]);
      cpu_setPC(cpu_getPC + 2);CYCLES(18);break;
    case 0x67:                  /*  RRD */
      dw.abyte.low  = read(cpu_getHL);
      dw.abyte.high = cpu_getA;
      cpu_setA((cpu_getA&0xf0)|(dw.abyte.low&0x0f));
      dw.abyte.low = ((dw.abyte.low>>4)&0x0f) | (dw.abyte.high<<4);
      write(cpu_getHL,dw.abyte.low);
      cpu_setF((cpu_getF & FLAG_C) | PSZTable[cpu_getA]);
      cpu_setPC(cpu_getPC + 2);CYCLES(18);break;

    default:
      CYCLES(illegalInstruction());
    break;
  }
