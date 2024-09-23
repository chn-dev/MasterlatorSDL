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
\file SN76489.cpp
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the VDP (Video Display Processor)
*/
/*----------------------------------------------------------------------------*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SN76489.h"
#include "SN76489Noise.h"

static u16 SN76489_Volumes[16] =
{
  65535,  /*  0 dB */
  52057,  /* -2 dB */
  41350,  /* -4 dB */
  32846,  /* -6 dB */
  26090,  /* -8 dB */
  20724,  /* -10dB */
  16462,  /* -12dB */
  13076,  /* -14dB */
  10387,  /* -16dB */
   8250,  /* -18dB */
   6554,  /* -20dB */
   5206,  /* -22dB */
   4135,  /* -24dB */
   3285,  /* -26dB */
   2609,  /* -28dB */
      0   /* Off */
};


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Constructor
*/
/*----------------------------------------------------------------------------*/
SN76489::SN76489()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
SN76489::~SN76489()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Create a new SN76489 object
\return Pointer to the new SN76489 object
*/
/*----------------------------------------------------------------------------*/
SN76489 *SN76489::create()
{
   SN76489 *pSnd = new SN76489();

   pSnd->reset();
   pSnd->m_lastchan = 0;
   
   return( pSnd );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
*/
/*----------------------------------------------------------------------------*/
void SN76489::reset()
{
   m_sqofs[0] = 0;
   m_sqofs[1] = 0;
   m_sqofs[2] = 0;
   m_sqofs[3] = 0;
   m_chanvol[0] = 15;
   m_chanvol[1] = 15;
   m_chanvol[2] = 15;
   m_chanvol[3] = 15;
   m_pos[0] = true;
   m_pos[1] = true;
   m_pos[2] = true;
   m_noiseofs = 0;
   m_FB = false;
   
   m_chanfreq[0] = 0xffff;
   m_chanfreq[1] = 0xffff;
   m_chanfreq[2] = 0xffff;
   m_chanfreq[3] = 0xffff;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Generate sound data for a specific voice
\param sampleFreq The sampling frequency in Hz
\param bufLen The output buffer length in samples
\param buf The output buffer (assuming unsigned 16bit)
\param overwrite true if the output samples should overwrite the existing samples in the 
 buffer or false if the should be added
*/
/*----------------------------------------------------------------------------*/
void SN76489::calcVoice( int sampleFreq, int bufLen, int voice, u16 *buf, bool overwrite )
{
   s16 pos = ( ( ( m_pos[voice] ? -32767 : 32767 ) * (s32)SN76489_Volumes[m_chanvol[voice]] ) >> 16 ) >> 2;
   u32 sqwidth = sqWidth( voice, sampleFreq );
   bool p = m_pos[voice];

   for( int i = 0; i < bufLen; i++ )
   {
      if( overwrite )
         *buf++ = pos;
      else
         *buf++ += pos;

      m_sqofs[voice] += 0x00010000;
      
      if( m_sqofs[voice] >= sqwidth )
      {
         m_sqofs[voice] -= sqwidth;
         pos = -pos;
         p = !p;
         m_pos[voice] = !m_pos[voice];
      }
   }
   
   m_pos[voice] = p;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute the sound processor for a specific number of sound samples and generate the 
sound data
\param sampleFreq The sampling frequency in Hz
\param pDest The output buffer (assuming unsigned 16bit)
\param len The output buffer length in samples
*/
/*----------------------------------------------------------------------------*/
void SN76489::cycle( int sampleFreq, u16 *pDest, int len )
{
   for ( int c = 0; c < 3; c++ )
   {
      calcVoice( sampleFreq, len, c, pDest, c == 0 );
   }

   u32 sqw = sqWidth( 3, sampleFreq );
   for( int i = 0; i < len ; i++ )
   {
      u8 d;
      u32 w;

      if( !m_FB)
      {
         w = sqw << 4;
         d = m_noiseofs & 1;
      } else
      {
         w = sqw;
         d = SN76489_Noise[m_noiseofs];
      }

      *pDest++ += ( ( ( ( d ? 32767 : -32767 ) * (s32)SN76489_Volumes[m_chanvol[3]]) >> 16 ) >> 2 );
      m_sqofs[3] += 0x00010000;

      if( m_sqofs[3] >= w )
      {
         m_sqofs[3] -= w;
         m_noiseofs++;

         if( m_noiseofs > 32766 )
         {
            m_noiseofs = 0;
         }
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Retrieve the square width value for calculating the square wave form
\param channel The channel number
\param sampleFreq The sampling frequency in Hz
\return The sqare width value
*/
/*----------------------------------------------------------------------------*/
u32 SN76489::sqWidth( int channel, int sampleFreq )
{
   channel &= 3;
   u32 result = 0;

   if( channel != 3 )
   {
      u32 t = (u32)sampleFreq * (u32)m_chanfreq[channel];
      result  = ( t / 125000 ) << 16;
      result |= (u32)( (double)( t % 125000) * (double)( 65535.0 / 125000.0 ) );
      result >>= 1;
   } else
   {
      if( m_chan3f )
      {
         u32 t = (u32)sampleFreq * (u32)m_chanfreq[2];
         result  = ( t / 125000 ) << 16;
         result |= (u32)( (double)( t % 125000) * (double)( 65535.0 / 125000.0 ) );
         result >>= 1;
      } else
      {
         u16 chanfreq = m_chanfreq[3];
         if( chanfreq == 0 )
            chanfreq = 0xffff;

         u32 freq = (u32)sampleFreq;
         result = (u32)( ( freq / chanfreq ) << 16 ) |
                  (u32)( (double)( freq % chanfreq ) *
                         (double)( 65535.0 / (double)chanfreq ) );
      }
   }
   
   return( result );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Write a byte to the SN76489's data register
\param d The byte to be written
*/
/*----------------------------------------------------------------------------*/
void SN76489::write( u8 d )
{
   if( ( d & 0x90 ) == 0x90 )
   {
      m_chanvol[( d >> 5 ) & 3] = d & 0x0f;
   } else
   if( ( d & 0x90 ) == 0x80 )
   {
      u8 c = ( d >> 5 ) & 3;

      if( d == 3 )
         return;

      m_lastchan = c;

      if( c != 3 )
      {
         m_chanfreq[c] = ( m_chanfreq[c] & ~0x000f ) | ( d & 0x0f );
      }
   } else
   if( ( d & 0x80 ) == 0x00 )
   {
      u8 c = m_lastchan;

      if( c != 3 )
      {
         m_chanfreq[c] = ( m_chanfreq[c] & ~0xfff0 ) | ( ( d & 0x3f ) << 4 );
      }
   }

   if( ( d & 0xf0 ) == 0xe0 )
   {
      if( d & 0x04 )
      {
         m_FB = true;
      } else
      {
         m_FB = false;
      }

      switch( d & 0x03 )
      {
         case 0: /* 7800Hz */
            m_chan3f = false;
            m_chanfreq[3] = 7800;
            break;

         case 1: /* 3900Hz */
            m_chan3f = false;
            m_chanfreq[3] = 3900;
            break;

         case 2: /* 1950Hz */
            m_chan3f = false;
            m_chanfreq[3] = 1950;
            break;

         case 3: /* Channel 2 */
            m_chan3f = true;
            break;
      }
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Save the SN76489's internal state to a block of memory
\param d Pointer to a block of memory
\return The number of bytes written to the block of memory
*/
/*----------------------------------------------------------------------------*/
int SN76489::saveState( u8 *d )
{
   int s = 0;

   memcpy( &d[s], m_chanfreq, sizeof( m_chanfreq ) ); s += sizeof( m_chanfreq );
   memcpy( &d[s], m_chanvol, sizeof( m_chanvol ) ); s += sizeof( m_chanvol );
   memcpy( &d[s], m_sqofs, sizeof( m_sqofs ) ); s += sizeof( m_sqofs );
   memcpy( &d[s], m_pos, sizeof( m_pos ) ); s += sizeof( m_pos );
   memcpy( &d[s], &m_lastchan, sizeof( m_lastchan ) ); s += sizeof( m_lastchan );
   memcpy( &d[s], &m_noiseofs, sizeof( m_noiseofs ) ); s += sizeof( m_noiseofs );
   memcpy( &d[s], &m_chan3f, sizeof( m_chan3f ) ); s += sizeof( m_chan3f );
   memcpy( &d[s], &m_FB, sizeof( m_FB ) ); s += sizeof( m_FB );
   
   return( s );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Load the SN76489's internal state from a block of memory
\param d Pointer to the block of memory
\return The number of bytes read from the block of memory
*/
/*----------------------------------------------------------------------------*/
int SN76489::loadState( u8 *d )
{
   int s = 0;

   memcpy( m_chanfreq, &d[s], sizeof( m_chanfreq ) ); s += sizeof( m_chanfreq );
   memcpy( m_chanvol, &d[s], sizeof( m_chanvol ) ); s += sizeof( m_chanvol );
   memcpy( m_sqofs, &d[s], sizeof( m_sqofs ) ); s += sizeof( m_sqofs );
   memcpy( m_pos, &d[s], sizeof( m_pos ) ); s += sizeof( m_pos );
   memcpy( &m_lastchan, &d[s], sizeof( m_lastchan ) ); s += sizeof( m_lastchan );
   memcpy( &m_noiseofs, &d[s], sizeof( m_noiseofs ) ); s += sizeof( m_noiseofs );
   memcpy( &m_chan3f, &d[s], sizeof( m_chan3f ) ); s += sizeof( m_chan3f );
   memcpy( &m_FB, &d[s], sizeof( m_FB ) ); s += sizeof( m_FB );
   
   return( s );
}
