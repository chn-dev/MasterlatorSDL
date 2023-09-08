#include <SDL.h>
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
      fprintf( stderr, "Error.\n" );
      return( -1 );
   }

   u8 *pScreenBuffer = new u8[config.screenBufferWidth * config.screenBufferHeight];
   Uint32 pal[256];

   printf( "Screen Width: %d\n", pMachine->screenWidth() );
   printf( "Screen Height: %d\n", pMachine->screenHeight() );
   printf( "Keep aspect ratio: %d\n", config.keepar );
   printf( "Maximized: %d\n", config.maximized );
   printf( "Fullscreen: %d\n", config.fullscreen );
   printf( "Window width = %d\n", config.windowWidth );
   printf( "Window height = %d\n", config.windowHeight );

   do
   {
      pMachine->renderFrame(
         pScreenBuffer,
         config.screenBufferWidth, config.screenBufferHeight,
         config.debug ? ( config.screenBufferWidth - FRAME_W ) + ( FRAME_W - pMachine->screenWidth() ) / 2 : 0,
         config.debug ? ( FRAME_H - pMachine->screenHeight() ) / 2 : 0 );

      if( config.debug )
      {

      }

      for( int i = 0; i < 32; i++ )
      {
         GGVDP::Color c = pMachine->getColor( i );
         pal[i] = 0xff000000 |
               ( c.red << 16 ) |
               ( c.green << 8 ) |
               ( c.blue << 0 );
      }
   } while( runSDL( pMachine, &config, pScreenBuffer, pal ) );

   quitSDL();

   delete pMachine;
   delete pScreenBuffer;

   return( 0 );
}
