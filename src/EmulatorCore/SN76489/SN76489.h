/* sn76489.h */
#ifndef __SN76489_H__
#define __SN76489_H__

#include "../defines.h"

class SN76489
{
public:
   ~SN76489();
   static SN76489 *create();

   void reset();
   void cycle( int sampleFreq, u16 *pDest, int len );
   void write( u8 );
   
   int saveState( u8 *d );
   int loadState( u8 *d );

private:
   SN76489();
   void calcVoice( int sampleFreq, int bufLen, int voice, u16 *buf, bool overwrite );
   u32 sqWidth( int channel, int sampleFreq );

   u16   m_chanfreq[4];
   u16   m_chanvol[4];
   u32   m_sqofs[4];     /* 16.16bit fixed point */
   bool  m_pos[3];      /* TRUE = +, FALSE = - */
   void *m_pBuf[4];     /* Pointer to 4 buffers for the sound data */
   u8    m_lastchan;
   u16   m_noiseofs;
   bool  m_chan3f;      /* Use channel 2 freq as the noise freq ? */
   bool  m_FB;
};

#endif
