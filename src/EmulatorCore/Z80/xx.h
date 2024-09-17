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


    case 0x46:      /* BIT 0,(XX+d) */
      M_BIT(0x01,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x4e:      /* BIT 1,(XX+d) */
      M_BIT(0x02,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x56:      /* BIT 2,(XX+d) */
      M_BIT(0x04,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x5e:      /* BIT 3,(XX+d) */
      M_BIT(0x08,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x66:      /* BIT 4,(XX+d) */
      M_BIT(0x10,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x6e:      /* BIT 5,(XX+d) */
      M_BIT(0x20,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x76:      /* BIT 6,(XX+d) */
      M_BIT(0x40,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;
    case 0x7e:      /* BIT 7,(XX+d) */
      M_BIT(0x80,read(cpu_getXX + OFS));cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC-3);break;

    case 0xc6:      /* SET 0,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x01));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xce:      /* SET 1,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x02));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xd6:      /* SET 2,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x04));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xde:      /* SET 3,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x08));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xe6:      /* SET 4,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x10));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xee:      /* SET 5,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x20));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xf6:      /* SET 6,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x40));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xfe:      /* SET 7,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)|0x80));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;

    case 0x86:      /* RES 0,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x01)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x8e:      /* RES 1,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x02)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x96:      /* RES 2,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x04)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x9e:      /* RES 3,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x08)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xa6:      /* RES 4,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x10)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xae:      /* RES 5,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x20)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xb6:      /* RES 6,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x40)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0xbe:      /* RES 7,(XX+d) */
      dw.aword = cpu_getXX + OFS;
      write(dw.aword,(u8)(read(dw.aword)&(~0x80)));
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;

    case 0x06:      /* RLC (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_RLC(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x16:      /* RL (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_RL(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x0e:      /* RRC (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_RRC(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x1e:      /* RR (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_RR(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x26:      /* SLA (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_SLA(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x2e:      /* SRA (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_SRA(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x36:			/* SLL (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_SLL(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
    case 0x3e:      /* SRL (XX+d) */
      dw.aword = cpu_getXX + OFS;
      t2.aword.low.abyte.low = read(dw.aword);
      M_SRL(t2.aword.low.abyte.low);
      write(dw.aword,t2.aword.low.abyte.low);
      cpu_setPC(cpu_getPC + ILEN);CYCLES(CYC);break;
