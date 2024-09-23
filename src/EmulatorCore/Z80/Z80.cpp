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
\file Z80.cpp
\author Christian Nowak <chnowak@web.de>
\brief Implementation of the Z80 CPU
*/
/*----------------------------------------------------------------------------*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Z80.h"
#include "Tables.h"
#include "inctab.h"
#include "dectab.h"
#include "cptab.h"
#include "addtab.h"
#include "Z80_macros.h"
#include "Z80_Disasm.h"


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
*/
/*----------------------------------------------------------------------------*/
bool Z80::Instruction::operator==( const Z80::Instruction &other ) const
{
   if( address != other.address ||
       code != other.code )
      return( false );
   else
      return( true );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
*/
/*----------------------------------------------------------------------------*/
bool Z80::Instruction::operator!=( const Z80::Instruction &other ) const
{
   return( ! operator==( other ) );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Convert the Instruction to a string
*/
/*----------------------------------------------------------------------------*/
std::string Z80::Instruction::toString() const
{
   std::string sCode;
   char tst[256];

   sprintf( tst, "%04x - ", address );
   sCode.append( tst );

   std::string bytes;
   for( int i = 0; i < code.size(); i++ )
   {
      sprintf( tst, "%02x", code[i] );
      bytes.append( tst );
      if( i != code.size() - 1 )
         bytes.append( " " );
   }

   while( bytes.size() < 11 )
      bytes.append( " " );
   sCode.append( bytes );

   sCode.append( " - " );
   sCode.append( disassembly );

/*   std::string l = label;
   while( l.size() < 16 )
      l = l + " ";
   l = l.substr( 0, 16 );
   sCode = l + " " + sCode;*/

   return( sCode );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
Z80::MemoryInterface::~MemoryInterface()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
Z80::DebuggerInterface::~DebuggerInterface()
{
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Constructor
\param pInterface Pointer to the memory interface for reading/writing from/to memory 
and ports
*/
/*----------------------------------------------------------------------------*/
Z80::Z80( Z80::MemoryInterface *pInterface )
{
   reset();
   m_pMemInterface = pInterface;
   m_pDebuggerInterface = 0;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Destructor
*/
/*----------------------------------------------------------------------------*/
Z80::~Z80()
{
   reset();
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
This function is invoked when the CPU encounters an illegal instruction
*/
/*----------------------------------------------------------------------------*/
int Z80::illegalInstruction()
{
/*     printf ("Illegal opcode at %08x\n",GGMS_Addy2ROM((GGMS*)cpu_val, cpu_PC.aword)); */
   printf( "Illegal instruction: 0x" );

   for( int i = 0; i < 4; i++ )
   {
      printf( "%02x", read( cpu_getPC + i ) );
   }
   printf("\n");
   cpu_setPC( cpu_getPC + 1 );

   return( 4 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
*/
/*----------------------------------------------------------------------------*/
void Z80::reset()
{
   m_I = 0;
   m_R = 0;
   cpu_setAF( 0 );
   cpu_setaf( 0 );
   cpu_setBC( 0 );
   cpu_setbc( 0 );
   cpu_setDE( 0 );
   cpu_setde( 0 );
   cpu_setHL( 0 );
   cpu_sethl( 0 );
   cpu_setSP( 0 );
   cpu_setPC( 0 );
   cpu_setIX( 0 );
   cpu_setIY( 0 );
   cpu_IFF = 0;
   m_cyclecount = 0;
   m_eicount = -1;
   m_rdy = false;
   m_vec = 0;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Create a new Z80 object
\param pInterface Pointer to the memory interface
\return Pointer to the new Z80 object
*/
/*----------------------------------------------------------------------------*/
Z80 *Z80::create( Z80::MemoryInterface *pInterface )
{
   Z80 *pCpu = new Z80( pInterface );

   /*  Initialize registers */
   pCpu->reset();

   return( pCpu );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\param pDebuggerInterface Pointer to the debugger interface
*/
/*----------------------------------------------------------------------------*/
void Z80::setDebugger( Z80::DebuggerInterface *pDebuggerInterface )
{
   m_pDebuggerInterface = pDebuggerInterface;
}


/*  Decode instructions with the DDCB prefix */
/*
int Z80_Decode_DDCB ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,t2;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_ddcb.h"

  }
*/

/*  Decode instructions with the FDCB prefix */
/*
int Z80_Decode_FDCB ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,t2;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_fdcb.h"

  }
*/

/*  Decode instructions with the ED prefix */
/*
int Z80_Decode_ED ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,tt;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_ed.h"
  }
*/

/*  Decode instructions with the DD prefix */
/*
int Z80_Decode_DD ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,t2;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_dd.h"
  }
*/

/*  Decode instructions with the FD prefix */
/*
int Z80_Decode_FD ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,t2;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_fd.h"
  }
*/

/*  Decode instructions with the CB prefix */
/*
int Z80_Decode_CB ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t,t2;
    union_word  dw;

#define CYCLES(a) return a;
#include "decode_cb.h"
  }
*/

/*  Decode the next instruction at cpu_PC */
/*  and determine the corresponding function */
/*
int Z80_Decode ( Z80 * x )
  {
    Z80 * cpu = x;
    union_dword t, tt, t2;
    union_word  dw;

#define CYCLES(a) return a
#include "decode.h"
  }
*/


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Raise an interrupt
\param vector The 16bit interrupt vector
*/
/*----------------------------------------------------------------------------*/

void Z80::interrupt( u16 vector )
{
   if( vector == Z80_NMI )
   {
      cpu_IFF |= 0x20;   //  NMI
   } else
   {// if ( cpu_IFF&1 )
      cpu_IFF |= 0x10;   //  INT
   }
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Clear the interrupt
*/
/*----------------------------------------------------------------------------*/
void Z80::clearINT()
{
   cpu_IFF &= ~0x10;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute the interrupt
\return The number of CPU cycles that has been consumed
*/
/*----------------------------------------------------------------------------*/
int Z80::execInterrupt()
{
   /*  Any pending interrupts ? */
   if( cpu_IFF & 0x20 )
   {  /*  NMI */
      /*  Has the cpu been halted ? */
      if( cpu_IFF & 0x80 )
      {
         cpu_IFF &= ~0x80;
         cpu_setPC( cpu_getPC + 1 );
      }

      /*  ** Reset bit5 and bit1=bit0 */
      cpu_IFF = ( cpu_IFF & 0xdc ) | ( ( cpu_IFF & 0x01 ) << 1 );
/*         cpu_IFF &= ~0x01; */
      /*  ** Now restart at 0x66 */
      /*  Push PC onto the stack */
      write( cpu_getSP - 1, cpu_getPCh );
      write( cpu_getSP - 2, cpu_getPCl );
      cpu_setSP( cpu_getSP - 2 );
      /*  JMP to 0x0066 */
      cpu_setPC( 0x66 );

      m_cyclecount = 11;

      return( 11 );
   } else
   if( ( cpu_IFF & 0x10 ) && ( cpu_IFF & 1 ) )
   {  /*  INT */
      /*  Has the cpu been halted ? */
      if( cpu_IFF & 0x80 )
      {
         cpu_IFF &= ~0x80;
         cpu_setPC( cpu_getPC + 1 );
      }

      cpu_IFF &= ~0x10;
      cpu_IFF &= ~0x03; /*  Disable interrupts */
      /*  ** Now restart at 0x38 */
      /*  Push PC onto the stack */
      write( cpu_getSP - 1, cpu_getPCh );
      write( cpu_getSP - 2, cpu_getPCl );
      cpu_setSP( cpu_getSP - 2 );
      /*  JMP to 0x0038 */
      cpu_setPC( 0x38 );

      m_cyclecount = 11;

      return( 11 );
   }

   return( 0 );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return AF register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getAF() const
{
   return( m_AF );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the AF register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setAF( union_word v )
{
   m_AF = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return BC register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getBC() const
{
   return( m_BC );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the BC register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setBC( union_word v )
{
   m_BC = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return DE register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getDE() const
{
   return( m_DE );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the DE register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setDE( union_word v )
{
   m_DE = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return HL register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getHL() const
{
   return( m_HL );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the HL register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setHL( union_word v )
{
   m_HL = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return SP register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getSP() const
{
   return( m_SP );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the SP register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setSP( union_word v )
{
   m_SP = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return IX register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getIX() const
{
   return( m_IX );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the IX register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setIX( union_word v )
{
   m_IX = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return IY register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getIY() const
{
   return( m_IY );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the IY register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setIY( union_word v )
{
   m_IY = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return PC register value
*/
/*----------------------------------------------------------------------------*/
union_word Z80::getPC() const
{
   return( m_PC );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Set the PC register value
\param v
*/
/*----------------------------------------------------------------------------*/
void Z80::setPC( union_word v )
{
   m_PC = v;
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Do a disassembly
\param loc The 16bit address of the instruction to disassemble
\param beforeInstr Number of instructions before loc
\param afterInstr Number of instructions after loc
\return A vector of disassembled instructions
*/
/*----------------------------------------------------------------------------*/
std::vector<Z80::Z80::Instruction> Z80::disassemble( u16 loc, int beforeInstr, int afterInstr )
{
   std::vector<Z80::Instruction> result;
   int x = 0;
   do
   {
      result = disassemble( loc, beforeInstr + x );
      x++;
   } while( result.size() < beforeInstr );

   if( result.size() > beforeInstr )
   {
      result = std::vector<Z80::Instruction>( result.begin() + ( result.size() - beforeInstr ), result.end() );
   }

   for( int i = 0; i < afterInstr + 1; i++ )
   {
      u8 op[8];
      for( int j = 0; j < 8; j++ )
      {
         op[j] = m_pMemInterface->z80_readMem( loc + j );
      }

      char dest[256];
      int s = disassemble( loc, op, dest );
      Z80::Instruction instruction;
      instruction.address = loc;
      instruction.disassembly = dest;
      instruction.hasBreakpoint = false;
      for( int j = 0; j < s; j++ )
      {
         instruction.code.push_back( op[j] );
      }
      result.push_back( instruction );
      loc += s;
   }

   return( result );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Do a disassembly
\param loc The 16bit address of the instruction to disassemble
\param nInstructions The number of instructions to disassemble
\return A vector of disassembled instructions
*/
/*----------------------------------------------------------------------------*/
std::vector<Z80::Instruction> Z80::disassemble( u16 loc, int nInstructions )
{
   std::vector<std::vector<Z80::Instruction> > result;
   std::vector<Z80::Instruction> d;

   int ss = 4 * ( nInstructions + 2 );
   for( int i = 1; i < ss; i++ )
   {
//      int s = 0;
//      std::vector<Z80::Instruction> d = disassemble( loc - i, nInstructions, &s );
      std::vector<Z80::Instruction> d2 = disassembleFromToAddress( loc - i, loc );
      if( d2.size() > nInstructions + 4 )
         break;

      if( d2.size() >= nInstructions )
      {
         d2 = std::vector<Z80::Instruction>( d2.begin() + ( d2.size() - nInstructions ), d2.end() );
         result.push_back( d2 );
      }

//      if( loc - i + s == loc )
      {
//         result.push_back( d );
/*         for( int j = 0; j < d.size(); j++ )
            printf( "%s\n", d[j].toString().c_str() );*/
      }
   }

   int start = 0;
   if( result.size() > 1 )
   {
      for( int i = 0; i < nInstructions; i++ )
      {
         bool allEqual = true;
         for( int j = 0; j < result.size(); j++ )
         {
            if( result[0][i] != result[j][i] )
            {
               allEqual = false;
               break;
            }
         }

         if( allEqual )
         {
            start = i;
            break;
         }
      }

      d = std::vector<Z80::Instruction>( result[result.size() - 1].begin() + start, result[result.size() - 1].end() );
   } else
   {
      // TODO if result.size() == 0
     // d = result[0];
   }

   return( d );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Do a disassembly
\param startLoc The starting address of the disassembly
\param endLoc The ending address of the disassembly
\return A vector of disassembled instructions
*/
/*----------------------------------------------------------------------------*/
std::vector<Z80::Instruction> Z80::disassembleFromToAddress( u16 startLoc, u16 endLoc )
{
   std::vector<Z80::Instruction> result;

   u16 loc = startLoc;
   while( loc != endLoc )
   {
      Z80::Instruction instr = disassemble( loc );
      result.push_back( instr );
      u16 nextLoc = loc + (u16)instr.code.size();
      if( loc < endLoc && nextLoc > endLoc )
      {
         break;
      }
      loc += (u16)instr.code.size();
   }

   if( loc != endLoc )
   {
      result.clear();
   }

   return( result );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Disassemble a single instruction
\param loc The memory address of the instruction to disassemble
\return The disassembled instruction
*/
/*----------------------------------------------------------------------------*/
Z80::Instruction Z80::disassemble( u16 loc )
{
   char dest[256];
   u8 op[8];

   for( u16 j = 0; j < 8; j++ )
   {
      op[j] = m_pMemInterface->z80_readMem( loc + j );
   }

   int size = disassemble( loc, op, dest );

   Z80::Instruction instruction;
   instruction.disassembly = dest;
   instruction.address = loc;
   instruction.hasBreakpoint = false;
   for( int j = 0; j < size; j++ )
   {
      instruction.code.push_back( op[j] );
   }

   return( instruction );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Disassemble a number of instructions
\param loc The memory address of the first instruction to disassemble
\param nInstructions The number of instructions to disassemble
\param s If not null, this variable will be filled with the number of bytes that have 
 been disassembled
\return The disassembled instructions
*/
/*----------------------------------------------------------------------------*/
std::vector<Z80::Instruction> Z80::disassemble( u16 loc, int nInstructions, int *s )
{
   std::vector<Z80::Instruction> result;

   for( int i = 0; i < nInstructions; i++ )
   {
      Z80::Instruction instruction = disassemble( loc );;

      loc += (u16)instruction.code.size();
      if( s )
      {
         *s += (int)instruction.code.size();
      }
      result.push_back( instruction );
   }

   return( result );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Disassemble a single instruction
\param loc The memory address of the instruction to disassemble
\param op Pointer to the machine code of the instruction
\param dest Pointer to the string to write the disassembly to
\return Length of the instruction in bytes
*/
/*----------------------------------------------------------------------------*/
int Z80::disassemble( u16 loc, u8 *op, char *dest )
{
   const char *inst;
   unsigned int i, j, l, ofs = 0;

   switch( op[0] )
   {
      case 0xcb:
         inst = Z80_DisasmCB[op[1]];
         l = Z80_LengthCB[op[1]];
         break;

      case 0xdd:
         if( op[1] == 0xcb )
         {
            inst = Z80_DisasmDDCB[op[3]];
            l = Z80_LengthDDCB[op[3]];
         } else
         {
            inst = Z80_DisasmDD[op[1]];
            l = Z80_LengthDD[op[1]];

            if( !inst )
            {
               ofs = 1;
               inst = Z80_Disasm[op[1]];
               l = Z80_Length[op[1]]+1;
            }
         }
         break;

      case 0xed:
         inst = Z80_DisasmED[op[1]];
         l = Z80_LengthED[op[1]];
         break;

      case 0xfd:
         if( op[1] == 0xcb )
         {
            inst = Z80_DisasmFDCB[op[3]];
            l = Z80_LengthFDCB[op[3]];
         } else
         {
            inst = Z80_DisasmFD[op[1]];
            l = Z80_LengthFD[op[1]];

            if( !inst )
            {
               ofs = 1;
               inst = Z80_Disasm[op[1]];
               l = Z80_Length[op[1]]+1;
            }
         }
         break;

      default:
         inst = Z80_Disasm[op[0]];
         l = Z80_Length[op[0]];
         break;
   }

   if( !inst )
   {
      sprintf( dest, "DB %02Xh", op[0] );
      return( 1 );
   }


   int startVar = -1;
   for( i = j = 0; i < strlen( inst ); )
   {
      if( startVar < 0 )
      {
         if( inst[i] == '%' )
         {
            startVar = i;
            i++;
         } else
/*         if( ( ( inst[i] == '%' ) || ( inst[i] == '$' ) ) &&
               ( inst[i + 1] >= '0' ) && ( inst[i + 1] <= '9' ) )
         {
            char t[8];
            u8 r;
            u16 v;

            v = inst[i+1]-'0';
            r = op[v+ofs];

            if( inst[i] == '$' )
            {
               if( r & 0x80 )
               {
                  sprintf( t,"-%02X",(-r)&0xff );
               } else
               {
                  sprintf( t,"+%02X",r );
               }
            } else
            {
               sprintf( t, "%02X", r );
            }

            strcpy( &dest[j], t );
            j += strlen( t );
            i += 2;
         } else*/
         {
            dest[j++] = inst[i++];
         }
      } else
      {
         if( inst[i] == '%' )
         {
            int endVar = i;

            std::string name = std::string( inst ).substr( startVar + 1, endVar - startVar - 1 );
            startVar = endVar = -1;

            if( name.size() == 2 )
            {
               if( name[1] >= '0' && name[1] <= '9' )
               {
                  int param = name[1] - '0';

                  if( name[0] == 'p' )
                  {
                     // Pointer
                     u16 loc = ( op[param] & 0xff ) | ( ( op[param + 1] << 8 ) & 0xff00 );
                     std::string label = "";
                     if( m_pDebuggerInterface )
                     {
                        label = m_pDebuggerInterface->z80_locationToLabel( loc );
                     }

                     if( label.empty() )
                     {
                        char tst[8];
                        sprintf( tst, "%04Xh", loc );
                        for( int n = 0; n < strlen( tst ); n++ )
                        {
                           dest[j++] = tst[n];
                        }
                     } else
                     {
                        for( int n = 0; n < label.size(); n++ )
                        {
                           dest[j++] = label[n];
                        }
                     }
                  } else
                  if( name[0] == 'w' )
                  {
                     u16 word = ( op[param] & 0xff ) | ( ( op[param + 1] << 8 ) & 0xff00 );
                     char tst[8];
                     sprintf( tst, "%04X", word );
                     for( int n = 0; n < strlen( tst ); n++ )
                     {
                        dest[j++] = tst[n];
                     }
                  } else
                  if( name[0] == 'b' )
                  {
                     u8 byte = op[param] & 0xff;
                     char tst[8];
                     sprintf( tst, "%02X", byte );
                     for( int n = 0; n < strlen( tst ); n++ )
                     {
                        dest[j++] = tst[n];
                     }
                  } else
                  if( name[0] == 'r' )
                  {
                     u8 byte = op[param] & 0xff;
                     u16 r = (u16)(s16)(s8)byte;
                     u16 newloc = loc + r + l;
                     std::string label = "";
                     if( m_pDebuggerInterface )
                     {
                        label = m_pDebuggerInterface->z80_locationToLabel( newloc );
                     }
                     if( label.empty() )
                     {
                        char tst[8];
                        if( byte & 0x80 )
                        {
                           sprintf( tst, "-%02Xh",(-byte)&0xff );
                        } else
                        {
                           sprintf( tst, "+%02Xh", byte );
                        }

                        for( int n = 0; n < strlen( tst ); n++ )
                        {
                           dest[j++] = tst[n];
                        }
                     } else
                     {
                        for( int n = 0; n < label.size(); n++ )
                        {
                           dest[j++] = label[n];
                        }
                     }
                  }
               }
            }
         }

         i++;
      }
   }

   dest[j] = '\0';

   return( l );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Save the Z80's internal state to a block of memory
\param d Pointer to a block of memory
\return The number of bytes written to the block of memory
*/
/*----------------------------------------------------------------------------*/
int Z80::saveState( u8 *d )
{
   int s = 0;

   memcpy( &d[s], &m_AF, sizeof( m_AF ) ); s += sizeof( m_AF );
   memcpy( &d[s], &m_BC, sizeof( m_BC ) ); s += sizeof( m_BC );
   memcpy( &d[s], &m_DE, sizeof( m_DE ) ); s += sizeof( m_DE );
   memcpy( &d[s], &m_HL, sizeof( m_HL ) ); s += sizeof( m_HL );
   memcpy( &d[s], &m_SP, sizeof( m_SP ) ); s += sizeof( m_SP );
   memcpy( &d[s], &m_PC, sizeof( m_PC ) ); s += sizeof( m_PC );
   memcpy( &d[s], &m_IX, sizeof( m_IX ) ); s += sizeof( m_IX );
   memcpy( &d[s], &m_IY, sizeof( m_IY ) ); s += sizeof( m_IY );
   memcpy( &d[s], &m_af, sizeof( m_af ) ); s += sizeof( m_af );
   memcpy( &d[s], &m_bc, sizeof( m_bc ) ); s += sizeof( m_bc );
   memcpy( &d[s], &m_de, sizeof( m_de ) ); s += sizeof( m_de );
   memcpy( &d[s], &m_hl, sizeof( m_hl ) ); s += sizeof( m_hl );
   memcpy( &d[s], &m_I, sizeof( m_I ) ); s += sizeof( m_I );
   memcpy( &d[s], &m_R, sizeof( m_R ) ); s += sizeof( m_R );
   memcpy( &d[s], &m_IFF, sizeof( m_IFF ) ); s += sizeof( m_IFF );
   memcpy( &d[s], &m_eicount, sizeof( m_eicount ) ); s += sizeof( m_eicount );

   return( s );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Load the Z80's internal state from a block of memory
\param d Pointer to the block of memory
\return The number of bytes read from the block of memory
*/
/*----------------------------------------------------------------------------*/
int Z80::loadState( u8 *d )
{
   int s = 0;

   memcpy( &m_AF, &d[s], sizeof( m_AF ) ); s += sizeof( m_AF );
   memcpy( &m_BC, &d[s], sizeof( m_BC ) ); s += sizeof( m_BC );
   memcpy( &m_DE, &d[s], sizeof( m_DE ) ); s += sizeof( m_DE );
   memcpy( &m_HL, &d[s], sizeof( m_HL ) ); s += sizeof( m_HL );
   memcpy( &m_SP, &d[s], sizeof( m_SP ) ); s += sizeof( m_SP );
   memcpy( &m_PC, &d[s], sizeof( m_PC ) ); s += sizeof( m_PC );
   memcpy( &m_IX, &d[s], sizeof( m_IX ) ); s += sizeof( m_IX );
   memcpy( &m_IY, &d[s], sizeof( m_IY ) ); s += sizeof( m_IY );
   memcpy( &m_af, &d[s], sizeof( m_af ) ); s += sizeof( m_af );
   memcpy( &m_bc, &d[s], sizeof( m_bc ) ); s += sizeof( m_bc );
   memcpy( &m_de, &d[s], sizeof( m_de ) ); s += sizeof( m_de );
   memcpy( &m_hl, &d[s], sizeof( m_hl ) ); s += sizeof( m_hl );
   memcpy( &m_I, &d[s], sizeof( m_I ) ); s += sizeof( m_I );
   memcpy( &m_R, &d[s], sizeof( m_R ) ); s += sizeof( m_R );
   memcpy( &m_IFF, &d[s], sizeof( m_IFF ) ); s += sizeof( m_IFF );
   memcpy( &m_eicount, &d[s], sizeof( m_eicount ) ); s += sizeof( m_eicount );

   return( s );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Execute a single instruction
\return The number of clock cycles that were necessary to execute the instruction
*/
/*----------------------------------------------------------------------------*/
int Z80::step()
{
   int i = 0;

   if( m_pDebuggerInterface )
   {
      m_pDebuggerInterface->z80_execStart( m_PC.aword );
   }

   i += execInterrupt();

   if( ( cpu_IFF & 0x80 ) && ( m_eicount <= 0 ) )
      return( -1 );

   /*  Any pending interrupts ? */
   m_R++;

   if( m_eicount > 0 )
   {
      if( !--m_eicount )
      {
         cpu_IFF |= 0x03;
         m_eicount = -1;
         i += execInterrupt();
      }
   }

   if( cpu_IFF & 0x80 )
   {
      i += 4;
   } else
   {
      union_dword t, tt, t2;
      union_word  dw;

      /*         char dasm[16];
               u8 d[6];
               for( int i = 0; i < sizeof( d ); i++ )
                  d[i] = read(cpu_getPC + i);
               disassemble( &d[0], &dasm[0] );
               printf("PC=%04x [%02x %02x %02x %02x] AF=%04x BC=%04x DE=%04x HL=%04x SP=%04x IX=%04x IY=%04x - %s\n", cpu_getPC, d[0], d[1], d[2], d[3], cpu_getAF, cpu_getBC, cpu_getDE, cpu_getHL, cpu_getSP, cpu_getIX, cpu_getIY, &dasm[0]);*/
#define CYCLES(a) i += a
#include "decode.h"
   }

   if( m_pDebuggerInterface )
   {
      m_pDebuggerInterface->z80_execFinish( m_PC.aword );
   }

   return( i );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
Run the CPU for at least a specific number of cycles
\param c The number of cycles to run
\return The number of cycles that actually have been used to run the CPU
*/
/*----------------------------------------------------------------------------*/
int Z80::run( int c )
{ /*  Run at least c cycles */

   int i = 0;

   while( i < c )
   {
      int cycles = step();
      if( cycles < 0 )
         return( c );

      i += cycles;

      if( isAtBreakpoint() )
      {
         break;
      }
   }

   return( i );
}


/*----------------------------------------------------------------------------*/
/*! 2024-09-23
\return true if the PC is currently at a breakpoint
*/
/*----------------------------------------------------------------------------*/
bool Z80::isAtBreakpoint() const
{
   if( m_pDebuggerInterface )
   {
      return( m_pDebuggerInterface->z80_break() );
   } else
   {
      return( false );
   }
}
