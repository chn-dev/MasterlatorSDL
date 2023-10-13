#ifndef DEBUGGERSECTION_H
#define DEBUGGERSECTION_H

#include "Debugger.h"

class Debugger::Section
{
public:
   Section( Debugger *pDebugger, std::string name, int xp, int yp, int width, int height );
   ~Section();

   virtual bool exec( u8 *pScreenBuffer, bool isCurrentSection );
   virtual void activate( bool ac );
   bool isActivated() const;

   virtual bool isAlwaysVisible() = 0;

   bool singleInstructionStep( u8 *pScreenBuffer );

   int getXPos() const;
   int getYPos() const;
   int getWidth() const;
   int getHeight() const;
   std::string name() const;

protected:
   Debugger *debugger() const;
   std::string m_Name;

private:
   bool m_Activated;
   Debugger *m_pDebugger;
   int m_xPos;
   int m_yPos;
   int m_Width;
   int m_Height;
};

#endif

