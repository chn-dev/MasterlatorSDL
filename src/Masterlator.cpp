#include <SDL.h>
#include "Font.h"
#include "CommandLine.h"
#include "ArchSDL.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "EmulatorCore/GGMS.h"

#define FRAME_W (256+8)
#define FRAME_H (192+8)

#define TICKS_FOR_NEXT_FRAME (1000 / 60)


std::vector<Z80::Instruction> disassembly;
u8 *pScreenBuffer;


void dumpRegister( char *pDest, const char *pName, union_word reg )
{
   sprintf( pDest, "%s = %04xh = '%c%c' = %5du = %6di",
            pName,
            reg,
            isprint( reg.abyte.high ) ? reg.abyte.high : ' ',
            isprint( reg.abyte.low ) ? reg.abyte.low : ' ',
            reg.aword,
            (s16)reg.aword );
}


void updateDisassembly( GGMS *pMachine, int nInstructions, int cursorPos )
{
   u16 loc = pMachine->cpu()->getPC().aword;
   disassembly = pMachine->cpu()->disassemble( loc, cursorPos, nInstructions - cursorPos - 1 );
}


bool singleInstructionStep( GGMS *pMachine, Config *pConfig )
{
   return( pMachine->singleInstructionStep(
      pScreenBuffer,
      pMachine->screenWidth(), pMachine->screenHeight(),
      0, 0 ) );
}


int main( int argc, char *argv[] )
{
   Config config;
   if( readCommandLine( argc, argv, &config ) < 0 )
   {
      return( - 1 );
   }

   printf( "Loading ROM %s\n", config.romFile.c_str() );
   GGMS *pMachine = GGMS::create( config.romFile.c_str(), config.debug );
   if( !pMachine )
   {
      fprintf( stderr, "Couldn't load ROM from %s\n", config.romFile.c_str() );
      return( -1 );
   }

   pScreenBuffer = new u8[pMachine->screenWidth() * pMachine->screenHeight()];

   config.screenBufferWidth = pMachine->screenWidth();
   config.screenBufferHeight = pMachine->screenHeight();

   if( config.debug )
   {
      config.screenBufferWidth = 854;
      config.screenBufferHeight = 480;
   }

   if( config.windowWidth < 0 )
   {
      if( config.xWidth > 0  )
      {
         config.windowWidth = config.screenBufferWidth * config.xWidth;
      } else
      {
         config.windowWidth = config.screenBufferWidth;
      }
   }

   if( config.windowHeight < 0 )
   {
      if( config.xHeight > 0 )
      {
         config.windowHeight = config.screenBufferHeight * config.xHeight;
      } else
      {
         config.windowHeight = config.screenBufferHeight;
      }
   }

   if( config.fullscreen )
   {
      config.maximized = false;
   }

   if( !initSDL( pMachine, &config ) )
   {
      delete pMachine;
      delete pScreenBuffer;
      fprintf( stderr, "Error.\n" );
      return( -1 );
   }

   getScreen()->pal[128] = 0xff000000;
   getScreen()->pal[129] = 0xffffffff;

   printf( "Screen Width: %d\n", pMachine->screenWidth() );
   printf( "Screen Height: %d\n", pMachine->screenHeight() );
   printf( "Keep aspect ratio: %d\n", config.keepar );
   printf( "Maximized: %d\n", config.maximized );
   printf( "Fullscreen: %d\n", config.fullscreen );
   printf( "Window width = %d\n", config.windowWidth );
   printf( "Window height = %d\n", config.windowHeight );

   bool debugMode = false;

   do
   {
      bool screenBlank = false;

      if( !debugMode )
      {
         while( !singleInstructionStep( pMachine, &config ) );
         screenBlank = true;
      }

      if( config.debug )
      {
         if( keyHasBeenPressed( SDLK_d ) && !debugMode )
         {
            printf( "DEBUG %04x\n", pMachine->cpu()->getPC() );
            debugMode = true;
            updateDisassembly( pMachine, 31, 15 );
         }

         if( keyHasBeenPressed( SDLK_c ) && debugMode )
         {
            printf( "CONTINUE\n" );
            debugMode = false;
         }

         if( debugMode )
         {
            union_word af = pMachine->cpu()->getAF();
            u8 a = af.abyte.high;
            u8 f = af.abyte.low;
            char tmp[256];
            char flags[256];
            sprintf( flags, "%c%c%c%c%c%c%c%c",
               f & FLAG_S ? 'S' : '-',
               f & FLAG_Z ? 'Z' : '-',
               f & FLAG_5 ? '5' : '-',
               f & FLAG_H ? 'H' : '-',
               f & FLAG_3 ? '3' : '-',
               f & FLAG_PV ? 'V' : '-',
               f & FLAG_N ? 'N' : '-',
               f & FLAG_C ? 'C' : '-' );
            sprintf( tmp, "A  =   %02xh =  '%c' = %5du = %6di", 
               a, isprint( a ) ? a : ' ', a, (int)(s8)a );
            print8x8( getScreen(), 10, 10, 129, false, tmp );

            sprintf( tmp, "F  = %02xh = %s", f, flags );
            print8x8( getScreen(), 320, 10, 129, false, tmp );

            sprintf( tmp, "PC = %04xh", pMachine->cpu()->getPC().aword );
            print8x8( getScreen(), 320, 20, 129, false, tmp );

            sprintf( tmp, "SP = %04xh", pMachine->cpu()->getSP().aword );
            print8x8( getScreen(), 320, 30, 129, false, tmp );

            dumpRegister( tmp, "BC", pMachine->cpu()->getBC() );
            print8x8( getScreen(), 10, 20, 129, false, tmp );

            dumpRegister( tmp, "DE", pMachine->cpu()->getDE() );
            print8x8( getScreen(), 10, 30, 129, false, tmp );

            dumpRegister( tmp, "HL", pMachine->cpu()->getHL() );
            print8x8( getScreen(), 10, 40, 129, false, tmp );

            dumpRegister( tmp, "IX", pMachine->cpu()->getIX() );
            print8x8( getScreen(), 10, 50, 129, false, tmp );

            dumpRegister( tmp, "IY", pMachine->cpu()->getIY() );
            print8x8( getScreen(), 10, 60, 129, false, tmp );

            sprintf( tmp, "vcount = %d", pMachine->vdp()->vcount() );
            print8x8( getScreen(), 10, 70, 129, false, tmp );

            for( int i = 0; i < disassembly.size(); i++ )
            {
               print8x8( getScreen(), 10, 100 + ( i * 10 ), 129, pMachine->cpu()->getPC().aword == disassembly[i].address, disassembly[i].toString() );
            }

            if( keyHasBeenPressed( SDLK_F11 ) )
            {
               printf("1\n");
               screenBlank = singleInstructionStep( pMachine, &config );
               printf( "2\n" );
               updateDisassembly( pMachine, 31, 15 );
               printf( "3\n" );
            }
         }
      }

      for( int y = 0; y < pMachine->screenHeight(); y++ )
      {
         for( int x = 0; x < pMachine->screenWidth(); x++ )
         {
            int srcOfs = x + ( y * pMachine->screenWidth() );
            int destOfs = getScreen()->width * ( y + 10 ) +
               x + ( getScreen()->width - 256 - 10 );
            getScreen()->pBuffer[destOfs] = pScreenBuffer[srcOfs];
            if( screenBlank )
               pScreenBuffer[srcOfs] = 0;
         }
      }

      for( int i = 0; i < 32; i++ )
      {
         GGVDP::Color c = pMachine->getColor( i );
         getScreen()->pal[i] = 0xff000000 |
                              ( c.red << 16 ) |
                              ( c.green << 8 ) |
                              ( c.blue << 0 );
      }

      if( debugMode )
      {

      }
   } while( runSDL( pMachine, &config ) );

   quitSDL();

   delete pMachine;
   delete pScreenBuffer;

   return( 0 );
}
