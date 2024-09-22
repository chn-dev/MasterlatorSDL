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
\file Font.h
\author Christian Nowak <chnowak@web.de>
\brief Functions for getting game information from the ROM's crc32 checksum
*/
/*----------------------------------------------------------------------------*/
#ifndef __CRC32_H__
#define __CRC32_H__

#include "defines.h"

u32 calccrc32(u8 *, u32, u32);
const char *SMS_GetGameFromChecksum(u32);
const char *GG_GetGameFromChecksum(u32);
const char *SMS_GetGame(u8 *, u32);
const char *GG_GetGame(u8 *, u32);

#endif
