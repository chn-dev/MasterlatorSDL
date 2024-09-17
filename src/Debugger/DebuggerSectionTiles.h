#ifndef DEBUGGERSECTIONTILES_H
#define DEBUGGERSECTIONTILES_H

#include "Debugger.h"
#include "DebuggerSection.h"

class Debugger::SectionTiles : public Section
{
public:
   SectionTiles( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );

   virtual bool isAlwaysVisible();

private:
   void printSprites();
   void paintTiles();
   void paintTile( int x, int y, int tile );

   int m_n;
};


#endif

