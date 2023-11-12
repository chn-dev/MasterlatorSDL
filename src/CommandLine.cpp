#include <fstream>

#include "CommandLine.h"
#include "getopts.h"

int readCommandLine( int argc, char *argv[], Config *pConfig )
{
   struct options opts[] =
   {
      { 1,  (char *)"version",         (char *)"Show version info",                        (char *)"V",  0 },
      { 2,  (char *)"rom",             (char *)"Load and start ROM file on startup",       (char *)"r",  1 },
      { 3,  (char *)"width",           (char *)"Viewport width in pixels (default:auto)",  (char *)"dx", 1 },
      { 4,  (char *)"height",          (char *)"Viewport height in pixels (default:auto)", (char *)"dy", 1 },
      { 5,  (char *)"keepar",          (char *)"Keep aspect ratio",                        (char *)"k",  0 },
      { 6,  (char *)"fullscreen",      (char *)"Fullscreen",                               (char *)"f",  0 },
      { 7,  (char *)"maximized",       (char *)"Maximized Window",                         (char *)"m",  0 },
      { 8,  (char *)"startkey",        (char *)"Start Key",                                (char *)"sk", 1 },
      { 9,  (char *)"akey",            (char *)"A Key",                                    (char *)"ak", 1 },
      { 10, (char *)"bkey",            (char *)"B Key",                                    (char *)"bk", 1 },
      { 11, (char *)"upkey",           (char *)"Up Key",                                   (char *)"uk", 1 },
      { 12, (char *)"downkey",         (char *)"Down Key",                                 (char *)"dk", 1 },
      { 13, (char *)"leftkey",         (char *)"Left Key",                                 (char *)"lk", 1 },
      { 14, (char *)"rightkey",        (char *)"Right Key",                                (char *)"rk", 1 },
      { 15, (char *)"debug",           (char *)"Debugger",                                 (char *)"d",  0 },
      { 16, (char *)"sym",             (char *)"Symbols File",                             (char *)"s",  0 },
      { 0,   NULL,                     NULL,                                               NULL,         0 }
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
            pConfig->romFile = std::string( args );
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
                  pConfig->xWidth = std::stoi( sWidth );
               } catch( ... )
               {
                  pConfig->xWidth = -1;
               }
            } else
            {
               try
               {
                  pConfig->windowWidth = std::stoi( sWidth );
               } catch( ... )
               {
                  pConfig->windowWidth = -1;
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
                  pConfig->xHeight = std::stoi( sHeight );
               } catch( ... )
               {
                  pConfig->xHeight = -1;
               }
            } else
            {
               try
               {
                  pConfig->windowHeight = std::stoi( sHeight );
               } catch( ... )
               {
                  pConfig->windowHeight = -1;
               }
            }
            break;
         }

         case 5: /* keep aspect ratio */
         {
            pConfig->keepar = true;
            break;
         }

         case 6: /* Fullscreen */
         {
            pConfig->fullscreen = true;
            break;
         }

         case 7: /* Maximized */
         {
            pConfig->maximized = true;
            break;
         }

         case 8: /* Start Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->startKey = c;
            break;
         }

         case 9: /* A Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->aKey = c;
            break;
         }

         case 10: /* B Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->bKey = c;
            break;
         }

         case 11: /* Up Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->upKey = c;
            break;
         }

         case 12: /* Down Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->downKey = c;
            break;
         }

         case 13: /* Left Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->leftKey = c;
            break;
         }

         case 14: /* Right Key */
         {
            SDL_Keycode c = SDL_GetKeyFromName( args );
            if( c != 0 )
               pConfig->rightKey = c;
            break;
         }

         case 15: /* Debug */
         {
            pConfig->debug = true;
            break;
         }

         case 16: /* Symbols file */
         {
            pConfig->symbolsFile = std::string( args );
            break;
         }

         default:
            break;
      }
   }

   free( args );

   if( pConfig->romFile.empty() )
   {
      fprintf( stderr, "Usage: -r romfile\n" );
      return( -1 );
   }

   if( pConfig->debug && pConfig->symbolsFile.empty() )
   {
      std::string f = pConfig->romFile;
      size_t s = f.rfind( "." );
      if( s != std::string::npos )
      {
         f = f.substr( 0, s ) + ".sym";
         std::ifstream tst;
         tst.open( f, std::ifstream::in );
         if( tst.is_open() )
         {
            tst.close();
            pConfig->symbolsFile = f;
         }
      }
   }

   return( 0 );
}

