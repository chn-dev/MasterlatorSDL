#include <SDL.h>
#include "Font.h"
#include "CommandLine.h"
#include "ArchSDL.h"
#include "Debugger.h"

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

   u8 *pScreenBuffer = new u8[pMachine->screenWidth() * pMachine->screenHeight()];

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
   getScreen()->pal[130] = 0xffff8080;
   getScreen()->pal[131] = 0xff804040;

   printf( "Screen Width: %d\n", pMachine->screenWidth() );
   printf( "Screen Height: %d\n", pMachine->screenHeight() );
   printf( "Keep aspect ratio: %d\n", config.keepar );
   printf( "Maximized: %d\n", config.maximized );
   printf( "Fullscreen: %d\n", config.fullscreen );
   printf( "Window width = %d\n", config.windowWidth );
   printf( "Window height = %d\n", config.windowHeight );

   Debugger *pDebugger = new Debugger( pMachine );
   pMachine->cpu()->setDebugger( pDebugger );
   if( config.debug )
   {
      if( !config.symbolsFile.empty() )
      {
         pDebugger->loadSymbolsFile( config.symbolsFile );
      }
      pDebugger->activate( true );
   }

   do
   {
      bool screenBlank = false;

      if( !pDebugger->isActivated() )
      {
         //while( !singleInstructionStep( pMachine ) );
         pMachine->renderFrame( pScreenBuffer, pMachine->screenWidth(), pMachine->screenHeight(), 0, 0 );
         if( pMachine->isAtBreakpoint() )
         {
            pDebugger->activate( true );
         } else
         {
            screenBlank = true;
         }
      }

      if( config.debug )
      {
         if( keyHasBeenPressed( SDLK_d ) && !pDebugger->isActivated() )
         {
            pDebugger->activate( true );
         }

         if( keyHasBeenPressed( SDLK_F5 ) && pDebugger->isActivated() )
         {
            pDebugger->activate( false );
         }

         if( pDebugger->isActivated() )
         {
            screenBlank = pDebugger->doDebug( pScreenBuffer );
         } else
         {
            pDebugger->doDebug( pScreenBuffer );
         }
      }

      // Copy machine screen buffer to main application buffer
      int xo = 0;
      int yo = 0;
      if( config.debug )
      {
         xo = ( ( 256 - pMachine->screenWidth() ) / 2 ) + ( getScreen()->width - 256 ) - 10;
         yo = ( ( 192 - pMachine->screenHeight() ) / 2 ) + 10;
      }

      // Copy pixel data
      for( int y = 0; y < pMachine->screenHeight(); y++ )
      {
         for( int x = 0; x < pMachine->screenWidth(); x++ )
         {
            int srcOfs = x + ( y * pMachine->screenWidth() );
            int border = 10;
            int destOfs =
               getScreen()->width * ( y + yo ) +
               x + xo;

            getScreen()->pBuffer[destOfs] = pScreenBuffer[srcOfs];
            if( screenBlank )
               pScreenBuffer[srcOfs] = 0;
         }
      }

      // Draw frame around the screen output
      if( config.debug )
      {
         for( int x = xo; x < xo + pMachine->screenWidth(); x++ )
         {
            getScreen()->pBuffer[( ( yo - 1 ) * getScreen()->width ) + x] = 129;
            getScreen()->pBuffer[( ( yo + pMachine->screenHeight() ) * getScreen()->width ) + x] = 129;
         }
         for( int y = yo - 1; y < yo + pMachine->screenHeight() + 1; y++ )
         {
            getScreen()->pBuffer[( y * getScreen()->width ) + xo - 1] = 129;
            getScreen()->pBuffer[( y * getScreen()->width ) + xo + pMachine->screenWidth()] = 129;
         }
      }

      // Update the palette
      for( int i = 0; i < 32; i++ )
      {
         GGVDP::Color c = pMachine->getColor( i );
         getScreen()->pal[i] = 0xff000000 |
                              ( c.red << 16 ) |
                              ( c.green << 8 ) |
                              ( c.blue << 0 );
      }
   } while( runSDL( pMachine, &config, 128 ) );

   quitSDL();

   delete pDebugger;
   delete pMachine;
   delete pScreenBuffer;

   return( 0 );
}
