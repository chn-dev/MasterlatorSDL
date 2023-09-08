#include <set>

#include "ArchSDL.h"
#include <SDL.h>

static SDL_GameController *pGameController = 0;
static SDL_Window *pWindow = 0;
static SDL_Renderer *pRenderer = 0;
static SDL_Surface *pRenderSurface = 0;
static SDL_AudioDeviceID audioDeviceId = -1;

static bool quit = false;
static std::set<SDL_Keycode> keyMap;


static void mixcallback( void *userdata, Uint8 *stream, int len )
{
//   Uint16 tmpStream[len / 2];
   GGMS *pMachine = (GGMS *)userdata;
//   pMachine->renderAudio( stream, len / 2, 44100 );
//   memcpy( stream, tmpStream, len );
}


static SDL_GameController *findGameController()
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


bool initSDL( GGMS *pMachine, const Config *pConfig )
{
   if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) < 0 )
   {
      return( false );
   }

   SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

   Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
   if( pConfig->maximized )
   {
      windowFlags |= SDL_WINDOW_MAXIMIZED;
   }

   if( pConfig->fullscreen )
   {
      windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
   }

   SDL_CreateWindowAndRenderer(
      pConfig->windowWidth, pConfig->windowHeight,
      windowFlags,
      &pWindow, &pRenderer );
   SDL_ShowCursor( !pConfig->fullscreen );

   if( !pWindow || !pRenderer )
   {
      return( false );
   }

   SDL_SetWindowTitle( pWindow, "Masterlator" );
   pGameController = findGameController();

   pRenderSurface = SDL_CreateRGBSurface( 0, pConfig->screenBufferWidth, pConfig->screenBufferHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 );

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
//      SDL_PauseAudioDevice( audioDeviceId, 0 );
   }

   return( true );
}


static void setKeyPressed( SDL_Keycode keyCode, bool pressed )
{
   keyMap.erase( keyCode );
   if( pressed )
      keyMap.insert( keyCode );
}


bool isKeyPressed( SDL_Keycode keyCode )
{
   return( keyMap.find( keyCode ) != keyMap.end() );
}


bool runSDL( GGMS *pMachine, const Config *pConfig, u8 *pScreenBuffer, Uint32 *pPalette )
{
   SDL_Event e;
   while( SDL_PollEvent( &e ) )
   {
      if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
      {
         bool keyDown = e.key.state == SDL_PRESSED;
         SDL_Keycode keyCode = e.key.keysym.sym;

         bool curDown = isKeyPressed( keyCode );
         setKeyPressed( keyCode, keyDown );

         if( keyDown != curDown )
         {
            if( keyDown && keyCode == SDLK_ESCAPE )
            {
               quit = true;
            }

            if( keyDown && keyCode == SDLK_RETURN )
            {
               if( isKeyPressed( SDLK_LALT ) )
               {
                  Uint32 f = SDL_GetWindowFlags( pWindow );
                  bool isFull = ( f & SDL_WINDOW_FULLSCREEN ) || ( f & SDL_WINDOW_FULLSCREEN_DESKTOP );
                  SDL_SetWindowFullscreen( pWindow, isFull ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP );
                  SDL_ShowCursor( isFull );
               }
            }

            if( keyDown && keyCode == SDLK_d )
            {
               u16 loc = pMachine->cpu()->getPC();
               printf( "PC = %04x\n", loc );
/*               for( int i = 0; i < 16; i++ )
               {
                  int s = 0;
                  std::string d = pMachine->cpu()->disassemble( loc, &s );
                  if( loc == pMachine->cpu()->getPC() )
                     printf( "*" );
                  else
                     printf( " " );

                  printf( "%04x - %s\n", loc, d.c_str() );
                  loc += s;
               }*/
               std::vector<Z80::Z80_Instruction> instr = pMachine->cpu()->disassemble( loc, 10, 10 );
               printf( "%d\n", instr.size() );
               for( int i = 0; i < instr.size(); i++ )
               {
                  printf( "%s\n", instr[i].toString().c_str() );
               }
               printf("\n");
            }

            if( keyCode == pConfig->aKey )
            {
               pMachine->setP1AButton( keyDown );
            }

            if( keyCode == pConfig->bKey )
            {
               pMachine->setP1BButton( keyDown );
            }
            if( keyCode == pConfig->startKey )
            {
               pMachine->setP1StartButton( keyDown );
            }

            if( keyCode == pConfig->upKey )
            {
               pMachine->setP1UpButton( keyDown );
            }

            if( keyCode == pConfig->downKey )
            {
               pMachine->setP1DownButton( keyDown );
            }

            if( keyCode == pConfig->leftKey )
            {
               pMachine->setP1LeftButton( keyDown );
            }

            if( keyCode == pConfig->rightKey )
            {
               pMachine->setP1RightButton( keyDown );
            }
         }
      } else
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

   SDL_Surface *pSurface = SDL_GetWindowSurface( pWindow );

   int surfaceWidth = pSurface->w;
   int surfaceHeight = pSurface->h;

   SDL_LockSurface( pRenderSurface );
   for( int i = 0; i < pConfig->screenBufferWidth * pConfig->screenBufferHeight; i++ )
   {
      ( (Uint32 *)pRenderSurface->pixels )[i] = pPalette[pScreenBuffer[i]];
   }
   SDL_UnlockSurface( pRenderSurface );

   SDL_Texture *pTexture = SDL_CreateTextureFromSurface( pRenderer, pRenderSurface );

   SDL_Rect srcRect;
   srcRect.x = 0;
   srcRect.y = 0;
   srcRect.w = pConfig->screenBufferWidth;
   srcRect.h = pConfig->screenBufferHeight;
   SDL_Rect dstRect;
   dstRect.x = 0;
   dstRect.y = 0;
   dstRect.w = surfaceWidth;
   dstRect.h = surfaceHeight;
   double ar = (double)srcRect.w / (double)srcRect.h;
   if( pConfig->keepar )
   {
      int w = dstRect.w;
      int h = dstRect.h;

      int w2 = ar * h;
      int h2 = w / ar;

      if( w2 > surfaceWidth )
      {
          h = h2;
      } else
      {
          w = w2;
      }

      dstRect.w = w;
      dstRect.h = h;
      dstRect.x = ( surfaceWidth - dstRect.w ) / 2;
      dstRect.y = ( surfaceHeight - dstRect.h ) / 2;
   }

   SDL_RenderFillRect( pRenderer, 0 );
   SDL_RenderCopy( pRenderer, pTexture, &srcRect, &dstRect );

   SDL_DestroyTexture( pTexture );
   SDL_RenderPresent( pRenderer );

   return( !quit );
}


void quitSDL()
{
   if( audioDeviceId > 0 )
   {
      SDL_PauseAudioDevice( audioDeviceId, 1 );
      SDL_CloseAudioDevice( audioDeviceId );
   }

   SDL_DestroyRenderer( pRenderer );
   SDL_FreeSurface( pRenderSurface );
   SDL_DestroyWindow( pWindow );

   if( pGameController )
   {
      SDL_GameControllerClose( pGameController );
   }

   SDL_Quit();
}
