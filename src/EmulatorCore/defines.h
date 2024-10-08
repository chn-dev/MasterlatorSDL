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


/*----------------------------------------------------------------------------*/
/*!
\file defines.h
\author Christian Nowak <chnowak@web.de>
\brief Some basic type definitions
*/
/*----------------------------------------------------------------------------*/
/* defines.h */

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <SDL.h>
#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif
#endif

typedef union {
#ifdef LITTLE_ENDIAN
  struct {

    u8 low;
    u8 high;
  } abyte;
#else
  struct {

    u8 high;
    u8 low;
  } abyte;
#endif
  u16 aword;
} union_word;


typedef union {
#ifdef  LITTLE_ENDIAN
  struct {

    union_word low;
    union_word high;
  } aword;
#else
  struct {

    union_word high;
    union_word low;
  } aword;
#endif
  u32 adword;
} union_dword;


#ifndef BOOL
#define BOOL  int
#endif
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#endif
