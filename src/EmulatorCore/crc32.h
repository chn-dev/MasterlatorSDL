#ifndef __CRC32_H__
#define __CRC32_H__

#include "defines.h"

u32 calccrc32(u8 *, u32, u32);
const char *SMS_GetGameFromChecksum(u32);
const char *GG_GetGameFromChecksum(u32);
const char *SMS_GetGame(u8 *, u32);
const char *GG_GetGame(u8 *, u32);

#endif

