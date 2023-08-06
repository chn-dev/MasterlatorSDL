  switch (read (cpu_getPC + 1)) {

    case 0x40:  M_BIT(0x01,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,B */
    case 0x41:  M_BIT(0x01,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,C */
    case 0x42:  M_BIT(0x01,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,D */
    case 0x43:  M_BIT(0x01,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,E */
    case 0x44:  M_BIT(0x01,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,H */
    case 0x45:  M_BIT(0x01,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,L */
    case 0x47:  M_BIT(0x01,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 0,A */

    case 0x48:  M_BIT(0x02,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,B */
    case 0x49:  M_BIT(0x02,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,C */
    case 0x4a:  M_BIT(0x02,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,D */
    case 0x4b:  M_BIT(0x02,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,E */
    case 0x4c:  M_BIT(0x02,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,H */
    case 0x4d:  M_BIT(0x02,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,L */
    case 0x4f:  M_BIT(0x02,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 1,A */

    case 0x50:  M_BIT(0x04,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,B */
    case 0x51:  M_BIT(0x04,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,C */
    case 0x52:  M_BIT(0x04,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,D */
    case 0x53:  M_BIT(0x04,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,E */
    case 0x54:  M_BIT(0x04,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,H */
    case 0x55:  M_BIT(0x04,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,L */
    case 0x57:  M_BIT(0x04,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 2,A */

    case 0x58:  M_BIT(0x08,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,B */
    case 0x59:  M_BIT(0x08,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,C */
    case 0x5a:  M_BIT(0x08,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,D */
    case 0x5b:  M_BIT(0x08,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,E */
    case 0x5c:  M_BIT(0x08,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,H */
    case 0x5d:  M_BIT(0x08,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,L */
    case 0x5f:  M_BIT(0x08,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 3,A */

    case 0x60:  M_BIT(0x10,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,B */
    case 0x61:  M_BIT(0x10,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,C */
    case 0x62:  M_BIT(0x10,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,D */
    case 0x63:  M_BIT(0x10,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,E */
    case 0x64:  M_BIT(0x10,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,H */
    case 0x65:  M_BIT(0x10,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,L */
    case 0x67:  M_BIT(0x10,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 4,A */

    case 0x68:  M_BIT(0x20,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,B */
    case 0x69:  M_BIT(0x20,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,C */
    case 0x6a:  M_BIT(0x20,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,D */
    case 0x6b:  M_BIT(0x20,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,E */
    case 0x6c:  M_BIT(0x20,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,H */
    case 0x6d:  M_BIT(0x20,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,L */
    case 0x6f:  M_BIT(0x20,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 5,A */

    case 0x70:  M_BIT(0x40,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,B */
    case 0x71:  M_BIT(0x40,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,C */
    case 0x72:  M_BIT(0x40,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,D */
    case 0x73:  M_BIT(0x40,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,E */
    case 0x74:  M_BIT(0x40,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,H */
    case 0x75:  M_BIT(0x40,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,L */
    case 0x77:  M_BIT(0x40,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 6,A */

    case 0x78:  M_BIT(0x80,cpu_getB);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,B */
    case 0x79:  M_BIT(0x80,cpu_getC);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,C */
    case 0x7a:  M_BIT(0x80,cpu_getD);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,D */
    case 0x7b:  M_BIT(0x80,cpu_getE);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,E */
    case 0x7c:  M_BIT(0x80,cpu_getH);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,H */
    case 0x7d:  M_BIT(0x80,cpu_getL);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,L */
    case 0x7f:  M_BIT(0x80,cpu_getA);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  BIT 7,A */

    case 0xc0:  cpu_setB(cpu_getB | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,B */
    case 0xc1:  cpu_setC(cpu_getC | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,C */
    case 0xc2:  cpu_setD(cpu_getD | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,D */
    case 0xc3:  cpu_setE(cpu_getE | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,E */
    case 0xc4:  cpu_setH(cpu_getH | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,H */
    case 0xc5:  cpu_setL(cpu_getL | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,L */
    case 0xc7:  cpu_setA(cpu_getA | 0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 0,A */

    case 0xc8:  cpu_setB(cpu_getB | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,B */
    case 0xc9:  cpu_setC(cpu_getC | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,C */
    case 0xca:  cpu_setD(cpu_getD | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,D */
    case 0xcb:  cpu_setE(cpu_getE | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,E */
    case 0xcc:  cpu_setH(cpu_getH | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,H */
    case 0xcd:  cpu_setL(cpu_getL | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,L */
    case 0xcf:  cpu_setA(cpu_getA | 0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 1,A */

    case 0xd0:  cpu_setB(cpu_getB | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,B */
    case 0xd1:  cpu_setC(cpu_getC | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,C */
    case 0xd2:  cpu_setD(cpu_getD | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,D */
    case 0xd3:  cpu_setE(cpu_getE | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,E */
    case 0xd4:  cpu_setH(cpu_getH | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,H */
    case 0xd5:  cpu_setL(cpu_getL | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,L */
    case 0xd7:  cpu_setA(cpu_getA | 0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 2,A */

    case 0xd8:  cpu_setB(cpu_getB | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,B */
    case 0xd9:  cpu_setC(cpu_getC | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,C */
    case 0xda:  cpu_setD(cpu_getD | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,D */
    case 0xdb:  cpu_setE(cpu_getE | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,E */
    case 0xdc:  cpu_setH(cpu_getH | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,H */
    case 0xdd:  cpu_setL(cpu_getL | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,L */
    case 0xdf:  cpu_setA(cpu_getA | 0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 3,A */

    case 0xe0:  cpu_setB(cpu_getB | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,B */
    case 0xe1:  cpu_setC(cpu_getC | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,C */
    case 0xe2:  cpu_setD(cpu_getD | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,D */
    case 0xe3:  cpu_setE(cpu_getE | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,E */
    case 0xe4:  cpu_setH(cpu_getH | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,H */
    case 0xe5:  cpu_setL(cpu_getL | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,L */
    case 0xe7:  cpu_setA(cpu_getA | 0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 4,A */

    case 0xe8:  cpu_setB(cpu_getB | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,B */
    case 0xe9:  cpu_setC(cpu_getC | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,C */
    case 0xea:  cpu_setD(cpu_getD | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,D */
    case 0xeb:  cpu_setE(cpu_getE | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,E */
    case 0xec:  cpu_setH(cpu_getH | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,H */
    case 0xed:  cpu_setL(cpu_getL | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,L */
    case 0xef:  cpu_setA(cpu_getA | 0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 5,A */

    case 0xf0:  cpu_setB(cpu_getB | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,B */
    case 0xf1:  cpu_setC(cpu_getC | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,C */
    case 0xf2:  cpu_setD(cpu_getD | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,D */
    case 0xf3:  cpu_setE(cpu_getE | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,E */
    case 0xf4:  cpu_setH(cpu_getH | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,H */
    case 0xf5:  cpu_setL(cpu_getL | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,L */
    case 0xf7:  cpu_setA(cpu_getA | 0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 6,A */

    case 0xf8:  cpu_setB(cpu_getB | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,B */
    case 0xf9:  cpu_setC(cpu_getC | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,C */
    case 0xfa:  cpu_setD(cpu_getD | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,D */
    case 0xfb:  cpu_setE(cpu_getE | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,E */
    case 0xfc:  cpu_setH(cpu_getH | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,H */
    case 0xfd:  cpu_setL(cpu_getL | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,L */
    case 0xff:  cpu_setA(cpu_getA | 0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break; /*  SET 7,A */


    case 0x80:  cpu_setB(cpu_getB & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,B */
    case 0x81:  cpu_setC(cpu_getC & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,C */
    case 0x82:  cpu_setD(cpu_getD & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,D */
    case 0x83:  cpu_setE(cpu_getE & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,E */
    case 0x84:  cpu_setH(cpu_getH & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,H */
    case 0x85:  cpu_setL(cpu_getL & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,L */
    case 0x87:  cpu_setA(cpu_getA & ~0x01);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 0,A */
    case 0x88:  cpu_setB(cpu_getB & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,B */
    case 0x89:  cpu_setC(cpu_getC & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,C */
    case 0x8a:  cpu_setD(cpu_getD & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,D */
    case 0x8b:  cpu_setE(cpu_getE & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,E */
    case 0x8c:  cpu_setH(cpu_getH & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,H */
    case 0x8d:  cpu_setL(cpu_getL & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,L */
    case 0x8f:  cpu_setA(cpu_getA & ~0x02);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 1,A */
    case 0x90:  cpu_setB(cpu_getB & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,B */
    case 0x91:  cpu_setC(cpu_getC & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,C */
    case 0x92:  cpu_setD(cpu_getD & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,D */
    case 0x93:  cpu_setE(cpu_getE & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,E */
    case 0x94:  cpu_setH(cpu_getH & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,H */
    case 0x95:  cpu_setL(cpu_getL & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,L */
    case 0x97:  cpu_setA(cpu_getA & ~0x04);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 2,A */
    case 0x98:  cpu_setB(cpu_getB & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,B */
    case 0x99:  cpu_setC(cpu_getC & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,C */
    case 0x9a:  cpu_setD(cpu_getD & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,D */
    case 0x9b:  cpu_setE(cpu_getE & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,E */
    case 0x9c:  cpu_setH(cpu_getH & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,H */
    case 0x9d:  cpu_setL(cpu_getL & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,L */
    case 0x9f:  cpu_setA(cpu_getA & ~0x08);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 3,A */
    case 0xa0:  cpu_setB(cpu_getB & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,B */
    case 0xa1:  cpu_setC(cpu_getC & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,C */
    case 0xa2:  cpu_setD(cpu_getD & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,D */
    case 0xa3:  cpu_setE(cpu_getE & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,E */
    case 0xa4:  cpu_setH(cpu_getH & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,H */
    case 0xa5:  cpu_setL(cpu_getL & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,L */
    case 0xa7:  cpu_setA(cpu_getA & ~0x10);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 4,A */
    case 0xa8:  cpu_setB(cpu_getB & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,B */
    case 0xa9:  cpu_setC(cpu_getC & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,C */
    case 0xaa:  cpu_setD(cpu_getD & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,D */
    case 0xab:  cpu_setE(cpu_getE & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,E */
    case 0xac:  cpu_setH(cpu_getH & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,H */
    case 0xad:  cpu_setL(cpu_getL & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,L */
    case 0xaf:  cpu_setA(cpu_getA & ~0x20);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 5,A */
    case 0xb0:  cpu_setB(cpu_getB & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,B */
    case 0xb1:  cpu_setC(cpu_getC & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,C */
    case 0xb2:  cpu_setD(cpu_getD & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,D */
    case 0xb3:  cpu_setE(cpu_getE & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,E */
    case 0xb4:  cpu_setH(cpu_getH & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,H */
    case 0xb5:  cpu_setL(cpu_getL & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,L */
    case 0xb7:  cpu_setA(cpu_getA & ~0x40);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 6,A */
    case 0xb8:  cpu_setB(cpu_getB & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,B */
    case 0xb9:  cpu_setC(cpu_getC & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,C */
    case 0xba:  cpu_setD(cpu_getD & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,D */
    case 0xbb:  cpu_setE(cpu_getE & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,E */
    case 0xbc:  cpu_setH(cpu_getH & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,H */
    case 0xbd:  cpu_setL(cpu_getL & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,L */
    case 0xbf:  cpu_setA(cpu_getA & ~0x80);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RES 7,A */

#define cpu_getXX   cpu_getHL
#define CYC  15
#define ILEN 2
#define OFS  0
#include "xx.h"
#undef cpu_getXX
#undef CYC
#undef ILEN
#undef OFS

    case 0x00:  M_RLC_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC B */
    case 0x01:  M_RLC_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC C */
    case 0x02:  M_RLC_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC D */
    case 0x03:  M_RLC_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC E */
    case 0x04:  M_RLC_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC H */
    case 0x05:  M_RLC_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC L */
    case 0x07:  M_RLC_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RLC A */

    case 0x08:  M_RRC_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC B */
    case 0x09:  M_RRC_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC C */
    case 0x0a:  M_RRC_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC D */
    case 0x0b:  M_RRC_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC E */
    case 0x0c:  M_RRC_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC H */
    case 0x0d:  M_RRC_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC L */
    case 0x0f:  M_RRC_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  RRC A */

    case 0x10:  M_RL_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL B */
    case 0x11:  M_RL_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL C */
    case 0x12:  M_RL_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL D */
    case 0x13:  M_RL_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL E */
    case 0x14:  M_RL_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL H */
    case 0x15:  M_RL_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL L */
    case 0x17:  M_RL_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RL A */

    case 0x18:  M_RR_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR B */
    case 0x19:  M_RR_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR C */
    case 0x1a:  M_RR_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR D */
    case 0x1b:  M_RR_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR E */
    case 0x1c:  M_RR_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR H */
    case 0x1d:  M_RR_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR L */
    case 0x1f:  M_RR_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;   /*  RR A */

    case 0x20:  M_SLA_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA B */
    case 0x21:  M_SLA_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA C */
    case 0x22:  M_SLA_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA D */
    case 0x23:  M_SLA_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA E */
    case 0x24:  M_SLA_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA H */
    case 0x25:  M_SLA_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA L */
    case 0x27:  M_SLA_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SLA A */

    case 0x28:  M_SRA_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA B */
    case 0x29:  M_SRA_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA C */
    case 0x2a:  M_SRA_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA D */
    case 0x2b:  M_SRA_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA E */
    case 0x2c:  M_SRA_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA H */
    case 0x2d:  M_SRA_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA L */
    case 0x2f:  M_SRA_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRA A */

    case 0x38:  M_SRL_REG(B);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL B */
    case 0x39:  M_SRL_REG(C);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL C */
    case 0x3a:  M_SRL_REG(D);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL D */
    case 0x3b:  M_SRL_REG(E);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL E */
    case 0x3c:  M_SRL_REG(H);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL H */
    case 0x3d:  M_SRL_REG(L);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL L */
    case 0x3f:  M_SRL_REG(A);cpu_setPC(cpu_getPC + 2);CYCLES(8);break;  /*  SRL A */

    default:
      CYCLES(illegalInstruction());
      break;
  }
