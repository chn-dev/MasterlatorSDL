/* Z80.c */
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
/* #include "Z80_instructions.h" */


bool Z80::Instruction::operator==( const Z80::Instruction &other ) const
{
   if( address != other.address ||
       code != other.code )
      return( false );
   else
      return( true );
}


bool Z80::Instruction::operator!=( const Z80::Instruction &other ) const
{
   return( ! operator==( other ) );
}


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

   return( sCode );
}


Z80::Interface::~Interface()
{
}


Z80::Z80( Z80::Interface *pInterface )
{
   reset();
   m_pInterface = pInterface;
}


Z80::~Z80()
{
   reset();
}


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


Z80 *Z80::create( Z80::Interface *pInterface )
{
   Z80 *pCpu = new Z80( pInterface );

   /*  Initialize registers */
   pCpu->reset();

   return( pCpu );
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


/*  Create an interrupt */
void Z80::interrupt( u16 vector )
{
   if( vector == Z80_NMI )
   {
      cpu_IFF |= 0x20;   /*  NMI */
   } else
   {/* if ( cpu_IFF&1 ) */
      cpu_IFF |= 0x10;   /*  INT */
   }
}


void Z80::clearINT()
{
   cpu_IFF &= ~0x10;
}


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


union_word Z80::getAF() const
{
   return( m_AF );
}


union_word Z80::getBC() const
{
   return( m_BC );
}


union_word Z80::getDE() const
{
   return( m_DE );
}


union_word Z80::getHL() const
{
   return( m_HL );
}


union_word Z80::getSP() const
{
   return( m_SP );
}


union_word Z80::getIX() const
{
   return( m_IX );
}


union_word Z80::getIY() const
{
   return( m_IY );
}


union_word Z80::getPC() const
{
   return( m_PC );
}


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
         op[j] = m_pInterface->z80_readMem( loc + j );
      }

      char dest[256];
      int s = disassemble( op, dest );
      Z80::Instruction instruction;
      instruction.address = loc;
      instruction.disassembly = dest;
      for( int j = 0; j < s; j++ )
      {
         instruction.code.push_back( op[j] );
      }
      result.push_back( instruction );
      loc += s;
   }

   return( result );
}


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
      d = result[0];
   }

   return( d );
}


std::vector<Z80::Instruction> Z80::disassembleFromToAddress( u16 startLoc, u16 endLoc )
{
   std::vector<Z80::Instruction> result;

   u16 loc = startLoc;
   while( loc < endLoc )
   {
      Z80::Instruction instr = disassemble( loc );
      result.push_back( instr );
      loc += instr.code.size();
   }

   if( loc != endLoc )
   {
      result.clear();
   }

   return( result );
}


Z80::Instruction Z80::disassemble( u16 loc )
{
   char dest[256];
   u8 op[8];

   for( u16 j = 0; j < 8; j++ )
   {
      op[j] = m_pInterface->z80_readMem( loc + j );
   }

   int size = disassemble( op, dest );

   Z80::Instruction instruction;
   instruction.disassembly = dest;
   instruction.address = loc;
   for( int j = 0; j < size; j++ )
   {
      instruction.code.push_back( op[j] );
   }

   return( instruction );
}


std::vector<Z80::Instruction> Z80::disassemble( u16 loc, int nInstructions, int *s )
{
   std::vector<Z80::Instruction> result;

   for( int i = 0; i < nInstructions; i++ )
   {
      Z80::Instruction instruction = disassemble( loc );;

      loc += instruction.code.size();
      if( s )
      {
         *s += instruction.code.size();
      }
      result.push_back( instruction );
   }

   return( result );
}

int Z80::disassemble( u8 *op, char *dest )
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

   for( i = j = 0; i < strlen( inst ); )
   {
      if( ( ( inst[i] == '%' ) || ( inst[i] == '$' ) ) &&
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
      } else
      {
         dest[j++] = inst[i++];
      }
   }

   dest[j] = '\0';

   return( l );
}


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


int Z80::step()
{
   int i = 0;

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

      m_pInterface->z80_exec( m_PC.aword );

      /*         char dasm[16];
               u8 d[6];
               for( int i = 0; i < sizeof( d ); i++ )
                  d[i] = read(cpu_getPC + i);
               disassemble( &d[0], &dasm[0] );
               printf("PC=%04x [%02x %02x %02x %02x] AF=%04x BC=%04x DE=%04x HL=%04x SP=%04x IX=%04x IY=%04x - %s\n", cpu_getPC, d[0], d[1], d[2], d[3], cpu_getAF, cpu_getBC, cpu_getDE, cpu_getHL, cpu_getSP, cpu_getIX, cpu_getIY, &dasm[0]);*/
#define CYCLES(a) i += a
#include "decode.h"
   }

   return( i );
}


int Z80::run( int c )
{ /*  Run at least c cycles */

   int i = 0;

   while( i < c )
   {
      int cycles = step();
      if( cycles < 0 )
         return( c );

      i += cycles;
   }

   return( i );
}
