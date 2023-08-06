#include <SDL.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "getopts.h"

#include "EmulatorCore/GGMS.h"

#define TICKS_FOR_NEXT_FRAME (1000 / 60)

static void mixcallback( void *userdata, Uint8 *stream, int len )
{
   Uint16 tmpStream[len / 2];
   GGMS *pMachine = (GGMS *)userdata;
   pMachine->renderAudio( tmpStream, len / 2, 44100 );
   memcpy( stream, tmpStream, len );
}


SDL_GameController *findGameController()
{
   for( int i = 0; i < SDL_NumJoysticks(); i++ )
   {
      if( SDL_IsGameController( i ) )
      {
         return( SDL_GameControllerOpen( i ) );
      }
   }

   return( 0 );
}


int main( int argc, char *argv[] )
{
   std::string romFile;
   int xWidth = -1;
   int xHeight = -1;
   int width = -1;
   int height = -1;
   bool keepar = false;
   SDL_GameController *pGameController = 0;

   struct options opts[] =
   {
      { 1, (char *)"version",         (char *)"Show version info",                        (char *)"V", 0 },
      { 2, (char *)"rom",             (char *)"Load and start ROM file on startup",       (char *)"r", 1 },
      { 3, (char *)"width",           (char *)"Viewport width in pixels (default:auto)",  (char *)"w", 1 },
      { 4, (char *)"height",          (char *)"Viewport height in pixels (default:auto)", (char *)"h", 1 },
      { 5, (char *)"keepar",          (char *)"Keep aspect ratio",                        (char *)"k", 1 },
      { 0,  NULL,       NULL,                                 NULL, 0 }
   };

   int i = -1;
   char *args = (char *)calloc(1, 128);
   while( ( i = getopts( argc, argv, opts, args ) ) != 0 )
   {
      switch( i )
      {
         case 1: /* version */
            printf( "Masterlator v0.08b Copyright (c)2001, 2002, 2023 by Christian Nowak <chnowak@web.de>\n" );
            printf( "Project website: http:/www.chn-dev.net/\n" );
            break;

         case 2: /* rom */
            romFile = std::string( args );
            break;

         case 3: /* width */
         {
            std::string sWidth = std::string( args );
            sWidth = sWidth.erase( sWidth.find_last_not_of( " \n\r\t" ) + 1 );
            sWidth = sWidth.substr( sWidth.find_first_not_of( " \n\r\t" ) );
            if( sWidth.rfind( "x", 0 ) == 0 )
            {
               sWidth = sWidth.substr( 1 );
               try
               {
                  xWidth = std::stoi( sWidth );
               } catch( ... )
               {
                  xWidth = -1;
               }
            } else
            {
               try
               {
                  width = std::stoi( sWidth );
               } catch( ... )
               {
                  width = -1;
               }
            }
            break;
         }

         case 4: /* height */
         {
            std::string sHeight = std::string( args );
            sHeight = sHeight.erase( sHeight.find_last_not_of( " \n\r\t" ) + 1 );
            sHeight = sHeight.substr( sHeight.find_first_not_of( " \n\r\t" ) );
            if( sHeight.rfind( "x", 0 ) == 0 )
            {
               sHeight = sHeight.substr( 1 );
               try
               {
                  xHeight = std::stoi( sHeight );
               } catch( ... )
               {
                  xHeight = -1;
               }
            } else
            {
               try
               {
                  height = std::stoi( sHeight );
               } catch( ... )
               {
                  height = -1;
               }
            }
            break;
         }

         case 5: /* keep aspect ratio */
         {
            std::string keepar = std::string( args );
            break;
         }

         default:
            break;
      }
   }

   free( args );

   if( romFile.empty() )
   {
      fprintf( stderr, "Usage: -r romfile\n" );
      return( -1 );
   }

   if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) < 0 )
   {
      fprintf( stderr, "Error.\n" );
      return( -1 );
   }

   SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

   printf( "Loading ROM %s\n", romFile.c_str() );
   GGMS *pMachine = GGMS::create( romFile.c_str() );
   if( !pMachine )
   {
      fprintf( stderr, "Couldn't load ROM from %s\n", romFile.c_str() );
      return( -1 );
   }

   printf( "Screen Width: %d\n", pMachine->screenWidth() );
   printf( "Screen Height: %d\n", pMachine->screenHeight() );

   if( width < 0 )
   {
      if( xWidth > 0  )
      {
         width = pMachine->screenWidth() * xWidth;
      } else
      {
         width = pMachine->screenWidth();
      }
   }

   if( height < 0 )
   {
      if( xHeight > 0 )
      {
         height = pMachine->screenHeight() * xHeight;
      } else
      {
         height = pMachine->screenHeight();
      }
   }

   u8 *pScreenBuffer = new u8[pMachine->screenWidth() * pMachine->screenHeight()];

   SDL_Window *pWindow = 0;
   SDL_Renderer *pRenderer = 0;

   SDL_CreateWindowAndRenderer(
      width, height,
      SDL_WINDOW_SHOWN,
      &pWindow, &pRenderer );

   if( pWindow )
   {
      SDL_SetWindowTitle( pWindow, "Masterlator" );

      pGameController = findGameController();

      SDL_Surface *pSurface = SDL_GetWindowSurface( pWindow );
      SDL_Surface *pRenderSurface = SDL_CreateRGBSurface( 0, pMachine->screenWidth(), pMachine->screenHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 );

      SDL_AudioSpec desiredSpec;
      memset( &desiredSpec, 0, sizeof( SDL_AudioSpec ) );
      desiredSpec.freq = 44100;
      desiredSpec.format = AUDIO_S16;
      desiredSpec.channels = 1;
      desiredSpec.samples = 256;
      desiredSpec.callback = mixcallback;
      desiredSpec.userdata = pMachine;
      SDL_AudioSpec obtainedSpec;
      SDL_AudioDeviceID audioDeviceId = SDL_OpenAudioDevice( NULL, 0, &desiredSpec, &obtainedSpec, 0 );

      if( audioDeviceId > 0 )
      {
         SDL_PauseAudioDevice( audioDeviceId, 0 );
      }

      SDL_Event e;
      bool quit = false;
      while( !quit )
      {
         while( SDL_PollEvent( &e ) )
         {
            if( e.type == SDL_QUIT )
            {
               quit = true;
            } else
            if( e.type == SDL_CONTROLLERDEVICEADDED )
            {
               if( !pGameController )
               {
                  pGameController = SDL_GameControllerOpen( e.cdevice.which );
               }
            } else
            if( e.type == SDL_CONTROLLERDEVICEREMOVED )
            {
               if( pGameController && ( e.cdevice.which == SDL_JoystickInstanceID( SDL_GameControllerGetJoystick( pGameController ) ) ) )
               {
                  SDL_GameControllerClose( pGameController );
                  pGameController = findGameController();
               }
            } else
            if( e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP )
            {
               bool bPressed = ( e.type == SDL_CONTROLLERBUTTONDOWN );

               if( pGameController && ( e.cdevice.which == SDL_JoystickInstanceID( SDL_GameControllerGetJoystick( pGameController ) ) ) )
               {
                  switch( e.cbutton.button )
                  {
                     case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        pMachine->setP1UpButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        pMachine->setP1DownButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        pMachine->setP1LeftButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        pMachine->setP1RightButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_X:
                     case SDL_CONTROLLER_BUTTON_A:
                        pMachine->setP1AButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_Y:
                     case SDL_CONTROLLER_BUTTON_B:
                        pMachine->setP1BButton( bPressed );
                        break;
                     case SDL_CONTROLLER_BUTTON_START:
                        pMachine->setP1StartButton( bPressed );
                        break;
                  }
               }
            }
         }

         pMachine->renderFrame( pScreenBuffer, pMachine->screenWidth(), pMachine->screenHeight(), 0, 0 );

         SDL_LockSurface( pRenderSurface );
         for( int i = 0; i < pMachine->screenWidth() * pMachine->screenHeight(); i++ )
         {
            u8 c = pScreenBuffer[i];
            GGVDP::Color color = pMachine->getColor( c );
            ( (Uint32 *)pRenderSurface->pixels )[i] =
               0xff000000 |
               ( color.red << 16 ) |
               ( color.green << 8 ) |
               ( color.blue << 0 );

         }
         SDL_UnlockSurface( pRenderSurface );

         SDL_Texture *pTexture = SDL_CreateTextureFromSurface( pRenderer, pRenderSurface );
         SDL_RenderCopy( pRenderer, pTexture, 0, 0 );
         SDL_DestroyTexture( pTexture );
         SDL_RenderPresent( pRenderer );
      }

      if( audioDeviceId > 0 )
      {
         SDL_PauseAudioDevice( audioDeviceId, 1 );
         SDL_CloseAudioDevice( audioDeviceId );
      }

      SDL_DestroyRenderer( pRenderer );
      SDL_FreeSurface( pRenderSurface );
      SDL_FreeSurface( pSurface );
      SDL_DestroyWindow( pWindow );

      if( pGameController )
      {
         SDL_GameControllerClose( pGameController );
      }
   }

   delete pMachine;
   delete pScreenBuffer;

   SDL_Quit();

   return( 0 );
}

