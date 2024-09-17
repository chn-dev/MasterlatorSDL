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


  switch (read(cpu_getPC)) {

        /**** LD dd,nn ****/
    case 0x01:  cpu_setBC(read16(cpu_getPC + 1));cpu_setPC(cpu_getPC + 3);CYCLES(10);break; /*  00000001 LD BC,nn */
    case 0x11:  cpu_setDE(read16(cpu_getPC + 1));cpu_setPC(cpu_getPC + 3);CYCLES(10);break; /*  00010001 LD DE,nn */
    case 0x21:  cpu_setHL(read16(cpu_getPC + 1));cpu_setPC(cpu_getPC + 3);CYCLES(10);break;  /*  00100001 LD HL,nn */
    case 0x31:  cpu_setSP(read16(cpu_getPC + 1));cpu_setPC(cpu_getPC + 3);CYCLES(10);break; /*  00110001 LD SP,nn */

    case 0x22: 
    dw.aword = read16(cpu_getPC + 1);write16(dw.aword,cpu_getHL);cpu_setPC(cpu_getPC + 3);CYCLES(16);break;  /*  00100010 LD (mm),HL */
    case 0x2a:  dw.aword = read16(cpu_getPC + 1);cpu_setHL(read16(dw.aword));cpu_setPC(cpu_getPC + 3);CYCLES(16);break; /*  00101010 LD HL,(mm) */

    case 0x02:  write(cpu_getBC,cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  00000010 LD (BC),A */
    case 0x12:  write(cpu_getDE,cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  00010010 LD (DE),A */
    case 0x32:  dw.aword = read16(cpu_getPC + 1);write(dw.aword,cpu_getA);cpu_setPC(cpu_getPC + 3);CYCLES(13);break; /*  00110010 LD (mm),A */

    case 0x0a:  cpu_setA(read(cpu_getBC));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  00001010 LD A,(BC) */
    case 0x1a:  cpu_setA(read(cpu_getDE));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  00011010 LD A,(DE) */
    case 0x3a:  dw.aword = read16(cpu_getPC + 1);cpu_setA(read(dw.aword));cpu_setPC(cpu_getPC + 3);CYCLES(13);break;  /*  00111010 LD A,(mm) */

    case 0x36:  write(cpu_getHL,read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(10);break;  /*  00110110 LD (HL),n */

    /**** LD R8,R8 ****/
    case 0x40:  cpu_setB(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000000 LD B,B */
    case 0x41:  cpu_setB(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000001 LD B,C */
    case 0x42:  cpu_setB(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000010 LD B,D */
    case 0x43:  cpu_setB(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000011 LD B,E */
    case 0x44:  cpu_setB(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000100 LD B,H */
    case 0x45:  cpu_setB(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000101 LD B,L */
    case 0x47:  cpu_setB(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01000111 LD B,A */

    case 0x48:  cpu_setC(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001000 LD C,B */
    case 0x49:  cpu_setC(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001001 LD C,C */
    case 0x4a:  cpu_setC(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001010 LD C,D */
    case 0x4b:  cpu_setC(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001011 LD C,E */
    case 0x4c:  cpu_setC(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001100 LD C,H */
    case 0x4d:  cpu_setC(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001101 LD C,L */
    case 0x4f:  cpu_setC(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01001111 LD C,A */

    case 0x50:  cpu_setD(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010000 LD D,B */
    case 0x51:  cpu_setD(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010001 LD D,C */
    case 0x52:  cpu_setD(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010010 LD D,D */
    case 0x53:  cpu_setD(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010011 LD D,E */
    case 0x54:  cpu_setD(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010100 LD D,H */
    case 0x55:  cpu_setD(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010101 LD D,L */
    case 0x57:  cpu_setD(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01010111 LD D,A */

    case 0x58:  cpu_setE(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011000 LD E,B */
    case 0x59:  cpu_setE(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011001 LD E,C */
    case 0x5a:  cpu_setE(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011010 LD E,D */
    case 0x5b:  cpu_setE(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011011 LD E,E */
    case 0x5c:  cpu_setE(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011100 LD E,H */
    case 0x5d:  cpu_setE(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011101 LD E,L */
    case 0x5f:  cpu_setE(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01011111 LD E,A */

    case 0x60:  cpu_setH(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100000 LD H,B */
    case 0x61:  cpu_setH(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100001 LD H,C */
    case 0x62:  cpu_setH(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100010 LD H,D */
    case 0x63:  cpu_setH(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100011 LD H,E */
    case 0x64:  cpu_setH(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100100 LD H,H */
    case 0x65:  cpu_setH(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100101 LD H,L */
    case 0x67:  cpu_setH(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01100111 LD H,A */

    case 0x68:  cpu_setL(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101000 LD L,B */
    case 0x69:  cpu_setL(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101001 LD L,C */
    case 0x6a:  cpu_setL(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101010 LD L,D */
    case 0x6b:  cpu_setL(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101011 LD L,E */
    case 0x6c:  cpu_setL(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101100 LD L,H */
    case 0x6d:  cpu_setL(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101101 LD L,L */
    case 0x6f:  cpu_setL(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01101111 LD L,A */

    case 0x78:  cpu_setA(cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111000 LD A,B */
    case 0x79:  cpu_setA(cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111001 LD A,C */
    case 0x7a:  cpu_setA(cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111010 LD A,D */
    case 0x7b:  cpu_setA(cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111011 LD A,E */
    case 0x7c:  cpu_setA(cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111100 LD A,H */
    case 0x7d:  cpu_setA(cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111101 LD A,L */
    case 0x7f:  cpu_setA(cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  01111111 LD A,A */


    /**** LD R8,n ****/
    case 0x06:  cpu_setB(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00000110 LD B,N */
    case 0x0e:  cpu_setC(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00001110 LD C,N */
    case 0x16:  cpu_setD(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00010110 LD D,N */
    case 0x1e:  cpu_setE(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00011110 LD E,N */
    case 0x26:  cpu_setH(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00100110 LD H,N */
    case 0x2e:  cpu_setL(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00101110 LD L,N */
    case 0x3e:  cpu_setA(read(cpu_getPC + 1));cpu_setPC(cpu_getPC + 2);CYCLES(7);break;   /*  00111110 LD A,N */


    /**** LD R8,(HL) ****/
    case 0x46:  cpu_setB(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01000110 LD B,(HL) */
    case 0x4e:  cpu_setC(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01001110 LD C,(HL) */
    case 0x56:  cpu_setD(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01010110 LD D,(HL) */
    case 0x5e:  cpu_setE(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01011110 LD E,(HL) */
    case 0x66:  cpu_setH(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01100110 LD H,(HL) */
    case 0x6e:  cpu_setL(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01101110 LD L,(HL) */
    case 0x7e:  cpu_setA(read(cpu_getHL));cpu_setPC(cpu_getPC + 1);CYCLES(7);break;  /*  01111110 LD A,(HL) */


    /**** LD (HL),R8 ****/
    case 0x70:  write(cpu_getHL,cpu_getB); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110000 LD (HL),B */
    case 0x71:  write(cpu_getHL,cpu_getC); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110001 LD (HL),C */
    case 0x72:  write(cpu_getHL,cpu_getD); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110010 LD (HL),D */
    case 0x73:  write(cpu_getHL,cpu_getE); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110011 LD (HL),E */
    case 0x74:  write(cpu_getHL,cpu_getH); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110100 LD (HL),H */
    case 0x75:  write(cpu_getHL,cpu_getL); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110101 LD (HL),L */
    case 0x77:  write(cpu_getHL,cpu_getA); cpu_setPC(cpu_getPC + 1);CYCLES(7);break; /*  01110111 LD (HL),A */


    /**** PUSH qq ****/
    case 0xc5:  M_PUSH(B, C);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  11000101 PUSH BC */
    case 0xd5:  M_PUSH(D, E);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  11010101 PUSH DE */
    case 0xe5:  M_PUSH(H, L);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  11100101 PUSH HL */
    case 0xf5:  M_PUSH(A, F);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  11110101 PUSH AF */

    /**** POP qq ****/
    case 0xc1:  M_POP(B, C);cpu_setPC(cpu_getPC + 1);CYCLES(10);break;    /*  11000001 POP BC */
    case 0xd1:  M_POP(D, E);cpu_setPC(cpu_getPC + 1);CYCLES(10);break;    /*  11010001 POP DE */
    case 0xe1:  M_POP(H, L);cpu_setPC(cpu_getPC + 1);CYCLES(10);break;    /*  11100001 POP HL */
    case 0xf1:  M_POP(A, F);cpu_setPC(cpu_getPC + 1);CYCLES(10);break;    /*  11110001 POP AF */

    /**** JP ****/
    case 0xc3:  M_JP_nn;break;    /*  11000011 JP nn */
    case 0xc2:  M_JP_NZ_nn;break; /*  11000010 JP NZ,nn */
    case 0xca:  M_JP_Z_nn;break;  /*  11001010 JP  Z,nn */
    case 0xd2:  M_JP_NC_nn;break; /*  11010010 JP NC,nn */
    case 0xda:  M_JP_C_nn;break;  /*  11011010 JP  C,nn */
    case 0xe2:  M_JP_PO_nn;break; /*  11100010 JP PO,nn */
    case 0xea:  M_JP_PE_nn;break; /*  11101010 JP PE,nn */
    case 0xf2:  M_JP_P_nn;break;  /*  11110010 JP  P,nn */
    case 0xfa:  M_JP_N_nn;break;  /*  11111010 JP  N,nn */
    case 0xe9:  M_JP_HL;break;    /*  11101001 JP (HL) */


    /**** JR ****/
    case 0x18:  M_JR_e;break;   /*  00011000 JR e */
    case 0x20:  M_JR_NZ_e;break;  /*  00100000 JR NZ,e */
    case 0x28:  M_JR_Z_e;break;   /*  00101000 JR  Z,e */
    case 0x30:  M_JR_NC_e;break;  /*  00110000 JR NC,e */
    case 0x38:  M_JR_C_e;break;   /*  00111000 JR  C,e */


    case 0xf9:  cpu_setSP(cpu_getHL);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  11111001 LD SP,HL */

    case 0xcd:  M_CALL_nn;break;  /*  11001101 CALL nn */

    case 0xc4:  M_CALL_NZ_nn;break; /*  11000100 CALL NZ,nn */
    case 0xcc:  M_CALL_Z_nn;break;  /*  11001100 CALL  Z,nn */
    case 0xd4:  M_CALL_NC_nn;break; /*  11010100 CALL NC,nn */
    case 0xdc:  M_CALL_C_nn;break;  /*  11011100 CALL  C,nn */
    case 0xe4:  M_CALL_PO_nn;break; /*  11100100 CALL PO,nn */
    case 0xec:  M_CALL_PE_nn;break; /*  11101100 CALL PE,nn */
    case 0xf4:  M_CALL_P_nn;break;  /*  11110100 CALL  P,nn */
    case 0xfc:  M_CALL_N_nn;break;  /*  11111100 CALL  N,nn */

    case 0xc9:  M_RET;break;    /*  11001001 RET */

    case 0xc0:  M_RET_NZ;break;   /*  11000000 RET NZ */
    case 0xc8:  M_RET_Z;break;    /*  11001000 RET  Z */
    case 0xd0:  M_RET_NC;break;   /*  11010000 RET NC */
    case 0xd8:  M_RET_C;break;    /*  11011000 RET  C */
    case 0xe0:  M_RET_PO;break;   /*  11100000 RET PO */
    case 0xe8:  M_RET_PE;break;   /*  11101000 RET PE */
    case 0xf0:  M_RET_P;break;    /*  11110000 RET  P */
    case 0xf8:  M_RET_N;break;    /*  11111000 RET  N */

    case 0x10:  M_DJNZ_e;break;   /*  00010000 DJNZ e */

    case 0xdb:  cpu_setA(in(read(cpu_getPC + 1)));cpu_setPC(cpu_getPC + 2);CYCLES(11);break;   /*  11011011 IN A,(n) */
    case 0xd3:  out(read(cpu_getPC + 1),cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(11);break; /*  11010011 OUT (n),a */

    case 0x80:  M_ADD(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,B */
    case 0x81:  M_ADD(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,C */
    case 0x82:  M_ADD(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,D */
    case 0x83:  M_ADD(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,E */
    case 0x84:  M_ADD(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,H */
    case 0x85:  M_ADD(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,L */
    case 0x87:  M_ADD(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADD A,A */

    case 0x88:  M_ADC(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,B */
    case 0x89:  M_ADC(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,C */
    case 0x8a:  M_ADC(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,D */
    case 0x8b:  M_ADC(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,E */
    case 0x8c:  M_ADC(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,H */
    case 0x8d:  M_ADC(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A;L */
    case 0x8f:  M_ADC(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  ADC A,A */

    case 0x90:  M_SUB(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,B */
    case 0x91:  M_SUB(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,C */
    case 0x92:  M_SUB(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,D */
    case 0x93:  M_SUB(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,E */
    case 0x94:  M_SUB(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,H */
    case 0x95:  M_SUB(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,L */
    case 0x97:  M_SUB(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SUB A,A */

    case 0x98:  M_SBC(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,B */
    case 0x99:  M_SBC(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,C */
    case 0x9a:  M_SBC(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,D */
    case 0x9b:  M_SBC(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,E */
    case 0x9c:  M_SBC(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,H */
    case 0x9d:  M_SBC(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,L */
    case 0x9f:  M_SBC(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  SBC A,A */


    case 0xc6:  /*  ADD A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_ADD(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xce:  /*  ADC A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_ADC(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xd6:  /*  SUB A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_SUB(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xde:  /*  SBC A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_SBC(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;

    case 0x86:  /*  ADD A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_ADD(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;
    case 0x8e:  /*  ADC A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_ADC(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;
    case 0x96:  /*  SUB A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_SUB(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;
    case 0x9e:  /*  SBC A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_SBC(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;

    case 0xb8:  M_CP(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,B */
    case 0xb9:  M_CP(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,C */
    case 0xba:  M_CP(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,D */
    case 0xbb:  M_CP(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,E */
    case 0xbc:  M_CP(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,H */
    case 0xbd:  M_CP(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,L */
    case 0xbf:  M_CP(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  CP A,A */

    case 0xfe:  /*  CP A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_CP(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xbe:  /*  CP A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_CP(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;

    case 0xa0:  M_AND(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,B */
    case 0xa1:  M_AND(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,C */
    case 0xa2:  M_AND(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,D */
    case 0xa3:  M_AND(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,E */
    case 0xa4:  M_AND(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,H */
    case 0xa5:  M_AND(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,L */
    case 0xa7:  M_AND(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  AND A,A */

    case 0xe6:  /*  AND A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_AND(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xa6:  /*  AND A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_AND(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;


    case 0xb0:  M_OR(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,B */
    case 0xb1:  M_OR(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,C */
    case 0xb2:  M_OR(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,D */
    case 0xb3:  M_OR(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,E */
    case 0xb4:  M_OR(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,H */
    case 0xb5:  M_OR(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,L */
    case 0xb7:  M_OR(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;  /*  OR A,A */

    case 0xf6:  /*  OR A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_OR(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xb6:  /*  OR A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_OR(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;


    case 0xa8:  M_XOR(cpu_getB);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,B */
    case 0xa9:  M_XOR(cpu_getC);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,C */
    case 0xaa:  M_XOR(cpu_getD);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,D */
    case 0xab:  M_XOR(cpu_getE);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,E */
    case 0xac:  M_XOR(cpu_getH);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,H */
    case 0xad:  M_XOR(cpu_getL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,L */
    case 0xaf:  M_XOR(cpu_getA);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  XOR A,A */

    case 0xee:  /*  XOR A,n */
      dw.abyte.low = read(cpu_getPC + 1);
      M_XOR(dw.abyte.low);cpu_setPC(cpu_getPC + 2);CYCLES(7);break;
    case 0xae:  /*  XOR A,(HL) */
      dw.abyte.low = read(cpu_getHL);
      M_XOR(dw.abyte.low);cpu_setPC(cpu_getPC + 1);CYCLES(7);break;

    case 0x07:  /*  RLCA */
      dw.abyte.low = (cpu_getA & 0x80) >> 7;
      cpu_setA((cpu_getA << 1) | dw.abyte.low);
      cpu_setF((cpu_getF & ~(FLAG_H|FLAG_N|FLAG_C)) | dw.abyte.low);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;
    case 0x17:  /*  RLA */
      dw.abyte.low = (cpu_getA & 0x80) >> 7;
      cpu_setA((cpu_getA << 1) | (cpu_getF & FLAG_C));
      cpu_setF((cpu_getF & ~(FLAG_H|FLAG_N|FLAG_C)) | dw.abyte.low);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;
    case 0x0f:  /*  RRCA */
      dw.abyte.low = cpu_getA & FLAG_C;
      cpu_setA((cpu_getA >> 1) | (dw.abyte.low << 7));
      cpu_setF((cpu_getF & ~(FLAG_H|FLAG_N|FLAG_C)) | dw.abyte.low);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;
    case 0x1f:  /*  RRA */
      dw.abyte.low = cpu_getA & FLAG_C;
      cpu_setA((cpu_getA >> 1) | ((cpu_getF & FLAG_C) << 7));
      cpu_setF((cpu_getF & ~(FLAG_H|FLAG_N|FLAG_C)) | dw.abyte.low);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;

    case 0x09:  M_ADD16(H, L, B, C);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  ADD HL,BC */
    case 0x19:  M_ADD16(H, L, D, E);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  ADD HL,DE */
    case 0x29:  M_ADD16(H, L, H, L);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  ADD HL,HL */
    case 0x39:  M_ADD16(H, L, S, P);cpu_setPC(cpu_getPC + 1);CYCLES(11);break; /*  ADD HL,SP */

    case 0x03:  cpu_setBC(cpu_getBC + 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  INC BC */
    case 0x13:  cpu_setDE(cpu_getDE + 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  INC DE */
    case 0x23:  cpu_setHL(cpu_getHL + 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  INC HL */
    case 0x33:  cpu_setSP(cpu_getSP + 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  INC SP */

    case 0x0b:  cpu_setBC(cpu_getBC - 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  DEC BC */
    case 0x1b:  cpu_setDE(cpu_getDE - 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  DEC DE */
    case 0x2b:  cpu_setHL(cpu_getHL - 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  DEC HL */
    case 0x3b:  cpu_setSP(cpu_getSP - 1);cpu_setPC(cpu_getPC + 1);CYCLES(6);break; /*  DEC SP */

    case 0x04:  M_INC_REG(B);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC B */
    case 0x0c:  M_INC_REG(C);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC C */
    case 0x14:  M_INC_REG(D);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC D */
    case 0x1c:  M_INC_REG(E);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC E */
    case 0x24:  M_INC_REG(H);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC H */
    case 0x2c:  M_INC_REG(L);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC L */
    case 0x3c:  M_INC_REG(A);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  INC A */
    case 0x34:  dw.abyte.low = read(cpu_getHL);   /*  INC (HL) */
                M_INC(dw.abyte.low);
                write(cpu_getHL, dw.abyte.low);
                cpu_setPC(cpu_getPC + 1);CYCLES(11);break;

    case 0x05:  M_DEC_REG(B);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC B */
    case 0x0d:  M_DEC_REG(C);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC C */
    case 0x15:  M_DEC_REG(D);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC D */
    case 0x1d:  M_DEC_REG(E);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC E */
    case 0x25:  M_DEC_REG(H);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC H */
    case 0x2d:  M_DEC_REG(L);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC L */
    case 0x3d:  M_DEC_REG(A);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  DEC A */
    case 0x35:  dw.abyte.low = read(cpu_getHL);   /*  DEC (HL) */
                M_DEC(dw.abyte.low);
                write(cpu_getHL, dw.abyte.low);
                cpu_setPC(cpu_getPC + 1);CYCLES(11);break;

    case 0xeb:  M_EX(DE,HL);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  EX DE,HL */
    case 0x08:  M_EX(AF,af);cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  EX AF,AF' */
    case 0xd9:  M_EX(BC,bc);M_EX(DE,de);M_EX(HL,hl);cpu_setPC(cpu_getPC + 1);CYCLES(4);break; /*  EXX */
    case 0xe3:  dw.abyte.low  = read(cpu_getSP);    /*  EX (SP),HL */
                dw.abyte.high = read(cpu_getSP + 1);
                write(cpu_getSP, cpu_getL);
                write(cpu_getSP + 1, cpu_getH);
                cpu_setHL(dw.aword);
                cpu_setPC(cpu_getPC + 1);CYCLES(19);break;

    case 0xc7:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x00);CYCLES(11);break; /*  RST 00h */
    case 0xcf:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x08);CYCLES(11);break; /*  RST 08h */
    case 0xd7:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x10);CYCLES(11);break; /*  RST 10h */
    case 0xdf:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x18);CYCLES(11);break; /*  RST 18h */
    case 0xe7:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x20);CYCLES(11);break; /*  RST 20h */
    case 0xef:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x28);CYCLES(11);break; /*  RST 28h */
    case 0xf7:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x30);CYCLES(11);break; /*  RST 30h */
    case 0xff:  cpu_setPC(cpu_getPC + 1);PUSH_PC;cpu_setPC(0x38);CYCLES(11);break; /*  RST 38h */

    case 0x2f:  cpu_setA(~cpu_getA);     /*  CPL */
                cpu_setF((cpu_getF&~(FLAG_3|FLAG_5)) |
                          FLAG_H|FLAG_N |
                          (cpu_getA&(FLAG_3|FLAG_5)));
                cpu_setPC(cpu_getPC + 1);CYCLES(4);break;

    case 0x3f:  /*  CCF */
      cpu_setF((cpu_getF&~(FLAG_N|FLAG_H|FLAG_3|FLAG_5)) |
             ((cpu_getF&FLAG_C)<<4) |
             (cpu_getA&(FLAG_3|FLAG_5)));
      cpu_setF(cpu_getF ^ FLAG_C);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;
    case 0x37:  /*  SCF */
      cpu_setF((cpu_getF & ~(FLAG_N|FLAG_H|FLAG_3|FLAG_5)) |
             (cpu_getA&(FLAG_3|FLAG_5)) |
             FLAG_C);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;

    case 0x00:  cpu_setPC(cpu_getPC + 1);CYCLES(4);break;       /*  NOP */

    case 0xf3:  cpu_IFF &= 0xfc;cpu_setPC(cpu_getPC + 1);CYCLES(4);break;    /*  DI */
    case 0xfb:  cpu_IFF |= 0x40;m_eicount=2;cpu_setPC(cpu_getPC + 1);CYCLES(4);break;   /*  EI */
    case 0x76:  cpu_IFF |= 0x80;CYCLES(4);break;    /*  HALT */

    case 0x27:                /*  DAA */
      dw.aword = (cpu_getA | (((cpu_getF&0x03)|((cpu_getF&0x10)>>2))<<8))<<1;
      cpu_setA(DAATable[dw.aword]);
      cpu_setF(DAATable[dw.aword+1]);
      cpu_setPC(cpu_getPC + 1);CYCLES(4);break;

    case 0xed:
#include "decode_ed.h"
      break;

    case 0xdd:
#include "decode_dd.h"
      break;

    case 0xfd:
#include "decode_fd.h"
      break;

    case 0xcb:
#include "decode_cb.h"
      break;

    default:
      CYCLES(illegalInstruction());
      break;
  }
