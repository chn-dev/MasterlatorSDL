  switch (read(cpu_getPC + 3)) {

#define cpu_getXX   cpu_getIY
#define CYC  23
#define ILEN 4
#define OFS  BYTE2WORD(read(cpu_getPC + 2))
#include "xx.h"
#undef cpu_getXX
#undef CYC
#undef ILEN
#undef OFS

    default:
      CYCLES(illegalInstruction());
      break;
  }
