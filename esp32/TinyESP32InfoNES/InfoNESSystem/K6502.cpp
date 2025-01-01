#include "gbConfig.h"

/*===================================================================*/
/*                                                                   */
/*  K6502.cpp : 6502 Emulator                                        */
/*                                                                   */
/*  2000/5/10   InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include "gbConfig.h"
#include "gbGlobals.h"

#include "K6502.h"
#include "InfoNESSystem.h"

/*-------------------------------------------------------------------*/
/*  Operation Macros                                                 */
/*-------------------------------------------------------------------*/

// Clock Op.
#define CLK(a)   g_wPassedClocks += (a);

// Addressing Op.
// Address
// (Indirect,X)
#define AA_IX    K6502_ReadZpW( K6502_Read( PC++ ) + X )
// (Indirect),Y
#define AA_IY    K6502_ReadZpW( K6502_Read( PC++ ) ) + Y
// Zero Page
#define AA_ZP    K6502_Read( PC++ )
// Zero Page,X
#define AA_ZPX   (unsigned char)( K6502_Read( PC++ ) + X )
// Zero Page,Y
#define AA_ZPY   (unsigned char)( K6502_Read( PC++ ) + Y )
// Absolute
#define AA_ABS   ( K6502_Read( PC++ ) | (unsigned short int)K6502_Read( PC++ ) << 8 )
// Absolute2 ( PC-- )
#define AA_ABS2  ( K6502_Read( PC++ ) | (unsigned short int)K6502_Read( PC ) << 8 )
// Absolute,X
#define AA_ABSX  AA_ABS + X
// Absolute,Y
#define AA_ABSY  AA_ABS + Y

// Data
// (Indirect,X)
#define A_IX    K6502_Read( AA_IX )
// (Indirect),Y
#define A_IY    K6502_ReadIY()
// Zero Page
#define A_ZP    K6502_ReadZp( AA_ZP )
// Zero Page,X
#define A_ZPX   K6502_ReadZp( AA_ZPX )
// Zero Page,Y
#define A_ZPY   K6502_ReadZp( AA_ZPY )
// Absolute
#define A_ABS   K6502_Read( AA_ABS )
// Absolute,X
#define A_ABSX  K6502_ReadAbsX()
// Absolute,Y
#define A_ABSY  K6502_ReadAbsY()
// Immediate
#define A_IMM   K6502_Read( PC++ )

// Flag Op.
#define SETF(a)  F |= (a)
#define RSTF(a)  F &= ~(a)
#define TEST(a)  RSTF( FLAG_N | FLAG_Z ); SETF( g_byTestTable[ a ] )

// Load & Store Op.
#define STA(a)    K6502_Write( (a), A );
#define STX(a)    K6502_Write( (a), X );
#define STY(a)    K6502_Write( (a), Y );
#define LDA(a)    A = (a); TEST( A );
#define LDX(a)    X = (a); TEST( X );
#define LDY(a)    Y = (a); TEST( Y );

// Stack Op.
#define PUSH(a)   K6502_Write( BASE_STACK + SP--, (a) )
#define PUSHW(a)  PUSH( (a) >> 8 ); PUSH( (a) & 0xff )
#define POP(a)    a = K6502_Read( BASE_STACK + ++SP )
#define POPW(a)   POP(a); a |= ( K6502_Read( BASE_STACK + ++SP ) << 8 )

// Logical Op.
#define ORA(a)  A |= (a); TEST( A )
#define AND(a)  A &= (a); TEST( A )
#define EOR(a)  A ^= (a); TEST( A )
#define BIT(a)  byD0 = (a); RSTF( FLAG_N | FLAG_V | FLAG_Z ); SETF( ( byD0 & ( FLAG_N | FLAG_V ) ) | ( ( byD0 & A ) ? 0 : FLAG_Z ) );
#define CMP(a)  wD0 = (unsigned short int)A - (a); RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_byTestTable[ wD0 & 0xff ] | ( wD0 < 0x100 ? FLAG_C : 0 ) );
#define CPX(a)  wD0 = (unsigned short int)X - (a); RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_byTestTable[ wD0 & 0xff ] | ( wD0 < 0x100 ? FLAG_C : 0 ) );
#define CPY(a)  wD0 = (unsigned short int)Y - (a); RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_byTestTable[ wD0 & 0xff ] | ( wD0 < 0x100 ? FLAG_C : 0 ) );
  
// Math Op. (A D flag isn't being supported.)
#define ADC(a)  byD0 = (a); \
                wD0 = A + byD0 + ( F & FLAG_C ); \
                byD1 = (unsigned char)wD0; \
                RSTF( FLAG_N | FLAG_V | FLAG_Z | FLAG_C ); \
                SETF( g_byTestTable[ byD1 ] | ( ( ~( A ^ byD0 ) & ( A ^ byD1 ) & 0x80 ) ? FLAG_V : 0 ) | ( wD0 > 0xff ) ); \
                A = byD1;

#define SBC(a)  byD0 = (a); \
                wD0 = A - byD0 - ( ~F & FLAG_C ); \
                byD1 = (unsigned char)wD0; \
                RSTF( FLAG_N | FLAG_V | FLAG_Z | FLAG_C ); \
                SETF( g_byTestTable[ byD1 ] | ( ( ( A ^ byD0 ) & ( A ^ byD1 ) & 0x80 ) ? FLAG_V : 0 ) | ( wD0 < 0x100 ) ); \
                A = byD1;

#define DEC(a)  wA0 = a; byD0 = K6502_Read( wA0 ); --byD0; K6502_Write( wA0, byD0 ); TEST( byD0 )
#define INC(a)  wA0 = a; byD0 = K6502_Read( wA0 ); ++byD0; K6502_Write( wA0, byD0 ); TEST( byD0 )

// Shift Op.
#define ASLA    RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_ASLTable[ A ].byFlag ); A = g_ASLTable[ A ].byValue 
#define ASL(a)  RSTF( FLAG_N | FLAG_Z | FLAG_C ); wA0 = a; byD0 = K6502_Read( wA0 ); SETF( g_ASLTable[ byD0 ].byFlag ); K6502_Write( wA0, g_ASLTable[ byD0 ].byValue )
#define LSRA    RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_LSRTable[ A ].byFlag ); A = g_LSRTable[ A ].byValue 
#define LSR(a)  RSTF( FLAG_N | FLAG_Z | FLAG_C ); wA0 = a; byD0 = K6502_Read( wA0 ); SETF( g_LSRTable[ byD0 ].byFlag ); K6502_Write( wA0, g_LSRTable[ byD0 ].byValue ) 
#define ROLA    byD0 = F & FLAG_C; RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_ROLTable[ byD0 ][ A ].byFlag ); A = g_ROLTable[ byD0 ][ A ].byValue 
#define ROL(a)  byD1 = F & FLAG_C; RSTF( FLAG_N | FLAG_Z | FLAG_C ); wA0 = a; byD0 = K6502_Read( wA0 ); SETF( g_ROLTable[ byD1 ][ byD0 ].byFlag ); K6502_Write( wA0, g_ROLTable[ byD1 ][ byD0 ].byValue )
#define RORA    byD0 = F & FLAG_C; RSTF( FLAG_N | FLAG_Z | FLAG_C ); SETF( g_RORTable[ byD0 ][ A ].byFlag ); A = g_RORTable[ byD0 ][ A ].byValue 
#define ROR(a)  byD1 = F & FLAG_C; RSTF( FLAG_N | FLAG_Z | FLAG_C ); wA0 = a; byD0 = K6502_Read( wA0 ); SETF( g_RORTable[ byD1 ][ byD0 ].byFlag ); K6502_Write( wA0, g_RORTable[ byD1 ][ byD0 ].byValue )

// Jump Op.
#define JSR     wA0 = AA_ABS2; PUSHW( PC ); PC = wA0; 
#if 0
#define BRA(a)  if ( a ) { wA0 = PC; PC += (char)K6502_Read( PC ); CLK( 3 + ( ( wA0 & 0x0100 ) != ( PC & 0x0100 ) ) ); ++PC; } else { ++PC; CLK( 2 ); }
#else
#define BRA(a) { \
  if ( a ) \
  { \
    wA0 = PC; \
	byD0 = K6502_Read( PC ); \
	PC += ( ( byD0 & 0x80 ) ? ( 0xFF00 | (unsigned short int)byD0 ) : (unsigned short int)byD0 ); \
	CLK( 3 + ( ( wA0 & 0x0100 ) != ( PC & 0x0100 ) ) ); \
    ++PC; \
  } else { \
	++PC; \
	CLK( 2 ); \
  } \
}
#endif
#define JMP(a)  PC = a;

/*-------------------------------------------------------------------*/
/*  Global valiables                                                 */
/*-------------------------------------------------------------------*/

// 6502 Register
unsigned short int PC;
unsigned char SP;
unsigned char F;
unsigned char A;
unsigned char X;
unsigned char Y;

// The state of the IRQ pin
unsigned char IRQ_State;

// Wiring of the IRQ pin
unsigned char IRQ_Wiring;

// The state of the NMI pin
unsigned char NMI_State;

// Wiring of the NMI pin
unsigned char NMI_Wiring;

// The number of the clocks that it passed
unsigned short int g_wPassedClocks;

// A table for the test
unsigned char g_byTestTable[ 256 ];

// Value and Flag Data
struct value_table_tag
{
  unsigned char byValue;
  unsigned char byFlag;
};

// A table for ASL
struct value_table_tag g_ASLTable[ 256 ];

// A table for LSR
struct value_table_tag g_LSRTable[ 256 ];

// A table for ROL
struct value_table_tag g_ROLTable[ 2 ][ 256 ];

// A table for ROR
struct value_table_tag g_RORTable[ 2 ][ 256 ];

/*===================================================================*/
/*                                                                   */
/*                K6502_Init() : Initialize K6502                    */
/*                                                                   */
/*===================================================================*/
void K6502_Init()
{
/*
 *  Initialize K6502
 *
 *  You must call this function only once at first.
 */

  unsigned char idx;
  unsigned char idx2;

  // The establishment of the IRQ pin
  NMI_Wiring = NMI_State = 1;
  IRQ_Wiring = IRQ_State = 1;

  // Make a table for the test
  idx = 0;
  do
  {
    if ( idx == 0 )
      g_byTestTable[ 0 ] = FLAG_Z;
    else
    if ( idx > 127 )
      g_byTestTable[ idx ] = FLAG_N;
    else
      g_byTestTable[ idx ] = 0;

    ++idx;
  } while ( idx != 0 );

  // Make a table ASL
  idx = 0;
  do
  {
    g_ASLTable[ idx ].byValue = idx << 1;
    g_ASLTable[ idx ].byFlag = 0;

    if ( idx > 127 )
      g_ASLTable[ idx ].byFlag = FLAG_C;

    if ( g_ASLTable[ idx ].byValue == 0 )
      g_ASLTable[ idx ].byFlag |= FLAG_Z;
    else
    if ( g_ASLTable[ idx ].byValue & 0x80 )
      g_ASLTable[ idx ].byFlag |= FLAG_N;

    ++idx;
  } while ( idx != 0 );

  // Make a table LSR
  idx = 0;
  do
  {
    g_LSRTable[ idx ].byValue = idx >> 1;
    g_LSRTable[ idx ].byFlag = 0;

    if ( idx & 1 )
      g_LSRTable[ idx ].byFlag = FLAG_C;

    if ( g_LSRTable[ idx ].byValue == 0 )
      g_LSRTable[ idx ].byFlag |= FLAG_Z;

    ++idx;
  } while ( idx != 0 );

  // Make a table ROL
  for ( idx2 = 0; idx2 < 2; ++idx2 )
  {
    idx = 0;
    do
    {
      g_ROLTable[ idx2 ][ idx ].byValue = ( idx << 1 ) | idx2;
      g_ROLTable[ idx2 ][ idx ].byFlag = 0;

      if ( idx > 127 )
        g_ROLTable[ idx2 ][ idx ].byFlag = FLAG_C;

      if ( g_ROLTable[ idx2 ][ idx ].byValue == 0 )
        g_ROLTable[ idx2 ][ idx ].byFlag |= FLAG_Z;
      else
      if ( g_ROLTable[ idx2 ][ idx ].byValue & 0x80 )
        g_ROLTable[ idx2 ][ idx ].byFlag |= FLAG_N;

      ++idx;
    } while ( idx != 0 );
  }

  // Make a table ROR
  for ( idx2 = 0; idx2 < 2; ++idx2 )
  {
    idx = 0;
    do
    {
      g_RORTable[ idx2 ][ idx ].byValue = ( idx >> 1 ) | ( idx2 << 7 );
      g_RORTable[ idx2 ][ idx ].byFlag = 0;

      if ( idx & 1 )
        g_RORTable[ idx2 ][ idx ].byFlag = FLAG_C;

      if ( g_RORTable[ idx2 ][ idx ].byValue == 0 )
        g_RORTable[ idx2 ][ idx ].byFlag |= FLAG_Z;
      else
      if ( g_RORTable[ idx2 ][ idx ].byValue & 0x80 )
        g_RORTable[ idx2 ][ idx ].byFlag |= FLAG_N;

      ++idx;
    } while ( idx != 0 );
  }
}

/*===================================================================*/
/*                                                                   */
/*                K6502_Reset() : Reset a CPU                        */
/*                                                                   */
/*===================================================================*/
void K6502_Reset()
{
/*
 *  Reset a CPU
 *
 */

  // Reset Registers
  PC = K6502_ReadW( VECTOR_RESET );
  SP = 0xFF;
  A = X = Y = 0;
  F = FLAG_Z | FLAG_R | FLAG_I;

  // Set up the state of the Interrupt pin.
  NMI_State = NMI_Wiring;
  IRQ_State = IRQ_Wiring;

  // Reset Passed Clocks
  g_wPassedClocks = 0;
}

/*===================================================================*/
/*                                                                   */
/*    K6502_Set_Int_Wiring() : Set up wiring of the interrupt pin    */
/*                                                                   */
/*===================================================================*/
void K6502_Set_Int_Wiring( unsigned char byNMI_Wiring, unsigned char byIRQ_Wiring )
{
/*
 * Set up wiring of the interrupt pin
 *
 */

  NMI_Wiring = byNMI_Wiring;
  IRQ_Wiring = byIRQ_Wiring;
}

/*===================================================================*/
/*                                                                   */
/*  K6502_Step() :                                                   */
/*          Only the specified number of the clocks execute Op.      */
/*                                                                   */
/*===================================================================*/
void K6502_Step( unsigned short int wClocks )
{
/*
 *  Only the specified number of the clocks execute Op.
 *
 *  Parameters
 *    WORD wClocks              (Read)
 *      The number of the clocks
 */

  unsigned char byCode;

  unsigned short int wA0;
  unsigned char byD0;
  unsigned char byD1;
  unsigned short int wD0;

  // Dispose of it if there is an interrupt requirement
  if ( NMI_State != NMI_Wiring )
  {
    // NMI Interrupt
    NMI_State = NMI_Wiring;
    CLK( 7 );

    PUSHW( PC );
    PUSH( F & ~FLAG_B );

    RSTF( FLAG_D );
    SETF( FLAG_I );

    PC = K6502_ReadW( VECTOR_NMI );
  }
  else
  if ( IRQ_State != IRQ_Wiring )
  {
    // IRQ Interrupt
    // Execute IRQ if an I flag isn't being set
    if ( !( F & FLAG_I ) )
    {
      IRQ_State = IRQ_Wiring;
      CLK( 7 );

      PUSHW( PC );
      PUSH( F & ~FLAG_B );

      RSTF( FLAG_D );
      SETF( FLAG_I );
    
      PC = K6502_ReadW( VECTOR_IRQ );
    }
  }

  // It has a loop until a constant clock passes
  while ( g_wPassedClocks < wClocks )
  {
    // Read an instruction
    byCode = K6502_Read( PC++ );

    // Execute an instruction.
    switch ( byCode )
    {
      case 0x00:  // BRK
        ++PC; PUSHW( PC ); SETF( FLAG_B ); PUSH( F ); SETF( FLAG_I ); RSTF( FLAG_D ); PC = K6502_ReadW( VECTOR_IRQ ); CLK( 7 );
        break;

      case 0x01:  // ORA (Zpg,X)
        ORA( A_IX ); CLK( 6 );
        break;

      case 0x05:  // ORA Zpg
        ORA( A_ZP ); CLK( 3 );
        break;

      case 0x06:  // ASL Zpg
        ASL( AA_ZP ); CLK( 5 );
        break;

      case 0x08:  // PHP
        SETF( FLAG_B ); PUSH( F ); CLK( 3 );
        break;

      case 0x09:  // ORA #Oper
        ORA( A_IMM ); CLK( 2 );
        break;

      case 0x0A:  // ASL A
        ASLA; CLK( 2 );
        break;

      case 0x0D:  // ORA Abs
        ORA( A_ABS ); CLK( 4 );
        break;

      case 0x0e:  // ASL Abs 
        ASL( AA_ABS ); CLK( 6 );
        break;

      case 0x10: // BPL Oper
        BRA( !( F & FLAG_N ) );
        break;

      case 0x11: // ORA (Zpg),Y
        ORA( A_IY ); CLK( 5 );
        break;

      case 0x15: // ORA Zpg,X
        ORA( A_ZPX ); CLK( 4 );
        break;

      case 0x16: // ASL Zpg,X
        ASL( AA_ZPX ); CLK( 6 );
        break;

      case 0x18: // CLC
        RSTF( FLAG_C ); CLK( 2 );
        break;

      case 0x19: // ORA Abs,Y
        ORA( A_ABSY ); CLK( 4 );
        break;

      case 0x1D: // ORA Abs,X
        ORA( A_ABSX ); CLK( 4 );
        break;

      case 0x1E: // ASL Abs,X
        ASL( AA_ABSX ); CLK( 7 );
        break;

      case 0x20: // JSR Abs
        JSR; CLK( 6 );
        break;

      case 0x21: // AND (Zpg,X)
        AND( A_IX ); CLK( 6 );
        break;

      case 0x24: // BIT Zpg
        BIT( A_ZP ); CLK( 3 );
        break;

      case 0x25: // AND Zpg
        AND( A_ZP ); CLK( 3 );
        break;

      case 0x26: // ROL Zpg
        ROL( AA_ZP ); CLK( 5 );
        break;

      case 0x28: // PLP
        POP( F ); SETF( FLAG_R ); CLK( 4 );
        break;

      case 0x29: // AND #Oper
        AND( A_IMM ); CLK( 2 );
        break;

      case 0x2A: // ROL A
        ROLA; CLK( 2 );
        break;

      case 0x2C: // BIT Abs
        BIT( A_ABS ); CLK( 4 );
        break;

      case 0x2D: // AND Abs 
        AND( A_ABS ); CLK( 4 );
        break;

      case 0x2E: // ROL Abs
        ROL( AA_ABS ); CLK( 6 );
        break;

      case 0x30: // BMI Oper 
        BRA( F & FLAG_N );
        break;

      case 0x31: // AND (Zpg),Y
        AND( A_IY ); CLK( 5 );
        break;

      case 0x35: // AND Zpg,X
        AND( A_ZPX ); CLK( 4 );
        break;

      case 0x36: // ROL Zpg,X
        ROL( AA_ZPX ); CLK( 6 );
        break;

      case 0x38: // SEC
        SETF( FLAG_C ); CLK( 2 );
        break;

      case 0x39: // AND Abs,Y
        AND( A_ABSY ); CLK( 4 );
        break;

      case 0x3D: // AND Abs,X
        AND( A_ABSX ); CLK( 4 );
        break;

      case 0x3E: // ROL Abs,X
        ROL( AA_ABSX ); CLK( 7 );
        break;

      case 0x40: // RTI
        POP( F ); SETF( FLAG_R ); POPW( PC ); CLK( 6 );
        break;

      case 0x41: // EOR (Zpg,X)
        EOR( A_IX ); CLK( 6 );
        break;

      case 0x45: // EOR Zpg
        EOR( A_ZP ); CLK( 3 );
        break;

      case 0x46: // LSR Zpg
        LSR( AA_ZP ); CLK( 5 );
        break;

      case 0x48: // PHA
        PUSH( A ); CLK( 3 );
        break;

      case 0x49: // EOR #Oper
        EOR( A_IMM ); CLK( 2 );
        break;

      case 0x4A: // LSR A
        LSRA; CLK( 2 );
        break;

      case 0x4C: // JMP Abs
        JMP( AA_ABS ); CLK( 3 );
        break;

      case 0x4D: // EOR Abs
        EOR( A_ABS ); CLK( 4 );
        break;

      case 0x4E: // LSR Abs
        LSR( AA_ABS ); CLK( 6 );
        break;

      case 0x50: // BVC
        BRA( !( F & FLAG_V ) );
        break;

      case 0x51: // EOR (Zpg),Y
        EOR( A_IY ); CLK( 5 );
        break;

      case 0x55: // EOR Zpg,X
        EOR( A_ZPX ); CLK( 4 );
        break;

      case 0x56: // LSR Zpg,X
        LSR( AA_ZPX ); CLK( 6 );
        break;

      case 0x58: // CLI
        byD0 = F;
        RSTF( FLAG_I ); CLK( 2 );
        if ( ( byD0 & FLAG_I ) && IRQ_State != IRQ_Wiring )  
        {
          IRQ_State = IRQ_Wiring;          
          CLK( 7 );

          PUSHW( PC );
          PUSH( F & ~FLAG_B );

          RSTF( FLAG_D );
          SETF( FLAG_I );
    
          PC = K6502_ReadW( VECTOR_IRQ );
        }
        break;

      case 0x59: // EOR Abs,Y
        EOR( A_ABSY ); CLK( 4 );
        break;

      case 0x5D: // EOR Abs,X
        EOR( A_ABSX ); CLK( 4 );
        break;

      case 0x5E: // LSR Abs,X
        LSR( AA_ABSX ); CLK( 7 );
        break;

      case 0x60: // RTS
        POPW( PC ); ++PC; CLK( 6 );
        break;

      case 0x61: // ADC (Zpg,X)
        ADC( A_IX ); CLK( 6 );
        break;

      case 0x65: // ADC Zpg
        ADC( A_ZP ); CLK( 3 );
        break;

      case 0x66: // ROR Zpg
        ROR( AA_ZP ); CLK( 5 );
        break;

      case 0x68: // PLA
        POP( A ); TEST( A ); CLK( 4 );
        break;

      case 0x69: // ADC #Oper
        ADC( A_IMM ); CLK( 2 );
        break;

      case 0x6A: // ROR A
        RORA; CLK( 2 );
        break;

      case 0x6C: // JMP (Abs)
        JMP( K6502_ReadW2( AA_ABS ) ); CLK( 5 );
        break;

      case 0x6D: // ADC Abs
        ADC( A_ABS ); CLK( 4 );
        break;

      case 0x6E: // ROR Abs
        ROR( AA_ABS ); CLK( 6 );
        break;

      case 0x70: // BVS
        BRA( F & FLAG_V );
        break;

      case 0x71: // ADC (Zpg),Y
        ADC( A_IY ); CLK( 5 );
        break;

      case 0x75: // ADC Zpg,X
        ADC( A_ZPX ); CLK( 4 );
        break;

      case 0x76: // ROR Zpg,X
        ROR( AA_ZPX ); CLK( 6 );
        break;

      case 0x78: // SEI
        SETF( FLAG_I ); CLK( 2 );
        break;

      case 0x79: // ADC Abs,Y
        ADC( A_ABSY ); CLK( 4 );
        break;

      case 0x7D: // ADC Abs,X
        ADC( A_ABSX ); CLK( 4 );
        break;

      case 0x7E: // ROR Abs,X
        ROR( AA_ABSX ); CLK( 7 );
        break;

      case 0x81: // STA (Zpg,X)
        STA( AA_IX ); CLK( 6 );
        break;
      
      case 0x84: // STY Zpg
        STY( AA_ZP ); CLK( 3 );
        break;

      case 0x85: // STA Zpg
        STA( AA_ZP ); CLK( 3 );
        break;

      case 0x86: // STX Zpg
        STX( AA_ZP ); CLK( 3 );
        break;

      case 0x88: // DEY
        --Y; TEST( Y ); CLK( 2 );
        break;

      case 0x8A: // TXA
        A = X; TEST( A ); CLK( 2 );
        break;

      case 0x8C: // STY Abs
        STY( AA_ABS ); CLK( 4 );
        break;

      case 0x8D: // STA Abs
        STA( AA_ABS ); CLK( 4 );
        break;

      case 0x8E: // STX Abs
        STX( AA_ABS ); CLK( 4 );
        break;

      case 0x90: // BCC
        BRA( !( F & FLAG_C ) );
        break;

      case 0x91: // STA (Zpg),Y
        STA( AA_IY ); CLK( 6 );
        break;

      case 0x94: // STY Zpg,X
        STY( AA_ZPX ); CLK( 4 );
        break;

      case 0x95: // STA Zpg,X
        STA( AA_ZPX ); CLK( 4 );
        break;

      case 0x96: // STX Zpg,Y
        STX( AA_ZPY ); CLK( 4 );
        break;

      case 0x98: // TYA
        A = Y; TEST( A ); CLK( 2 );
        break;

      case 0x99: // STA Abs,Y
        STA( AA_ABSY ); CLK( 5 );
        break;

      case 0x9A: // TXS
        SP = X; CLK( 2 );
        break;

      case 0x9D: // STA Abs,X
        STA( AA_ABSX ); CLK( 5 );
        break;

      case 0xA0: // LDY #Oper
        LDY( A_IMM ); CLK( 2 );
        break;

      case 0xA1: // LDA (Zpg,X)
        LDA( A_IX ); CLK( 6 );
        break;

      case 0xA2: // LDX #Oper
        LDX( A_IMM ); CLK( 2 );
        break;

      case 0xA4: // LDY Zpg
        LDY( A_ZP ); CLK( 3 );
        break;

      case 0xA5: // LDA Zpg
        LDA( A_ZP ); CLK( 3 );
        break;

      case 0xA6: // LDX Zpg
        LDX( A_ZP ); CLK( 3 );
        break;

      case 0xA8: // TAY
        Y = A; TEST( A ); CLK( 2 );
        break;

      case 0xA9: // LDA #Oper
        LDA( A_IMM ); CLK( 2 );
        break;

      case 0xAA: // TAX
        X = A; TEST( A ); CLK( 2 );
        break;

      case 0xAC: // LDY Abs
        LDY( A_ABS ); CLK( 4 );
        break;

      case 0xAD: // LDA Abs
        LDA( A_ABS ); CLK( 4 );
        break;

      case 0xAE: // LDX Abs
        LDX( A_ABS ); CLK( 4 );
        break;

      case 0xB0: // BCS
        BRA( F & FLAG_C );
        break;

      case 0xB1: // LDA (Zpg),Y
        LDA( A_IY ); CLK( 5 );
        break;

      case 0xB4: // LDY Zpg,X
        LDY( A_ZPX ); CLK( 4 );
        break;

      case 0xB5: // LDA Zpg,X
        LDA( A_ZPX ); CLK( 4 );
        break;

      case 0xB6: // LDX Zpg,Y
        LDX( A_ZPY ); CLK( 4 );
        break;

      case 0xB8: // CLV
        RSTF( FLAG_V ); CLK( 2 );
        break;

      case 0xB9: // LDA Abs,Y
        LDA( A_ABSY ); CLK( 4 );
        break;

      case 0xBA: // TSX
        X = SP; TEST( X ); CLK( 2 );
        break;

      case 0xBC: // LDY Abs,X
        LDY( A_ABSX ); CLK( 4 );
        break;

      case 0xBD: // LDA Abs,X
        LDA( A_ABSX ); CLK( 4 );
        break;

      case 0xBE: // LDX Abs,Y
        LDX( A_ABSY ); CLK( 4 );
        break;

      case 0xC0: // CPY #Oper
        CPY( A_IMM ); CLK( 2 );
        break;

      case 0xC1: // CMP (Zpg,X)
        CMP( A_IX ); CLK( 6 );
        break;

      case 0xC4: // CPY Zpg
        CPY( A_ZP ); CLK( 3 );
        break;

      case 0xC5: // CMP Zpg
        CMP( A_ZP ); CLK( 3 );
        break;

      case 0xC6: // DEC Zpg
        DEC( AA_ZP ); CLK( 5 );
        break;

      case 0xC8: // INY
        ++Y; TEST( Y ); CLK( 2 );
        break;

      case 0xC9: // CMP #Oper
        CMP( A_IMM ); CLK( 2 );
        break;

      case 0xCA: // DEX
        --X; TEST( X ); CLK( 2 );
        break;

      case 0xCC: // CPY Abs
        CPY( A_ABS ); CLK( 4 );
        break;

      case 0xCD: // CMP Abs
        CMP( A_ABS ); CLK( 4 );
        break;

      case 0xCE: // DEC Abs
        DEC( AA_ABS ); CLK( 6 );
        break;

      case 0xD0: // BNE
        BRA( !( F & FLAG_Z ) );
        break;

      case 0xD1: // CMP (Zpg),Y
        CMP( A_IY ); CLK( 5 );
        break;

      case 0xD5: // CMP Zpg,X
        CMP( A_ZPX ); CLK( 4 );
        break;

      case 0xD6: // DEC Zpg,X
        DEC( AA_ZPX ); CLK( 6 );
        break;

      case 0xD8: // CLD
        RSTF( FLAG_D ); CLK( 2 );
        break;

      case 0xD9: // CMP Abs,Y
        CMP( A_ABSY ); CLK( 4 );
        break;

      case 0xDD: // CMP Abs,X
        CMP( A_ABSX ); CLK( 4 );
        break;

      case 0xDE: // DEC Abs,X
        DEC( AA_ABSX ); CLK( 7 );
        break;

      case 0xE0: // CPX #Oper
        CPX( A_IMM ); CLK( 2 );
        break;

      case 0xE1: // SBC (Zpg,X)
        SBC( A_IX ); CLK( 6 );
        break;

      case 0xE4: // CPX Zpg
        CPX( A_ZP ); CLK( 3 );
        break;

      case 0xE5: // SBC Zpg
        SBC( A_ZP ); CLK( 3 );
        break;

      case 0xE6: // INC Zpg
        INC( AA_ZP ); CLK( 5 );
        break;

      case 0xE8: // INX
        ++X; TEST( X ); CLK( 2 );
        break;

      case 0xE9: // SBC #Oper
        SBC( A_IMM ); CLK( 2 );
        break;

      case 0xEA: // NOP
        CLK( 2 );
        break;

      case 0xEC: // CPX Abs
        CPX( A_ABS ); CLK( 4 );
        break;

      case 0xED: // SBC Abs
        SBC( A_ABS ); CLK( 4 );
        break;

      case 0xEE: // INC Abs
        INC( AA_ABS ); CLK( 6 );
        break;

      case 0xF0: // BEQ
        BRA( F & FLAG_Z );
        break;

      case 0xF1: // SBC (Zpg),Y
        SBC( A_IY ); CLK( 5 );
        break;

      case 0xF5: // SBC Zpg,X
        SBC( A_ZPX ); CLK( 4 );
        break;

      case 0xF6: // INC Zpg,X
        INC( AA_ZPX ); CLK( 6 );
        break;

      case 0xF8: // SED
        SETF( FLAG_D ); CLK( 2 );
        break;

      case 0xF9: // SBC Abs,Y
        SBC( A_ABSY ); CLK( 4 );
        break;

      case 0xFD: // SBC Abs,X
        SBC( A_ABSX ); CLK( 4 );
        break;

      case 0xFE: // INC Abs,X
        INC( AA_ABSX ); CLK( 7 );
        break;

      /*-----------------------------------------------------------*/
      /*  Unlisted Instructions ( thanks to virtualnes )           */
      /*-----------------------------------------------------------*/

			case	0x1A: // NOP (Unofficial)
			case	0x3A: // NOP (Unofficial)
			case	0x5A: // NOP (Unofficial)
			case	0x7A: // NOP (Unofficial)
			case	0xDA: // NOP (Unofficial)
			case	0xFA: // NOP (Unofficial)
				CLK( 2 );
				break;

			case	0x80: // DOP (CYCLES 2)
			case	0x82: // DOP (CYCLES 2)
			case	0x89: // DOP (CYCLES 2)
			case	0xC2: // DOP (CYCLES 2)
			case	0xE2: // DOP (CYCLES 2)
				PC++;
				CLK( 2 );
				break;

			case	0x04: // DOP (CYCLES 3)
			case	0x44: // DOP (CYCLES 3)
			case	0x64: // DOP (CYCLES 3)
				PC++;
				CLK( 3 );
				break;

			case	0x14: // DOP (CYCLES 4)
			case	0x34: // DOP (CYCLES 4)
			case	0x54: // DOP (CYCLES 4)
			case	0x74: // DOP (CYCLES 4)
			case	0xD4: // DOP (CYCLES 4)
			case	0xF4: // DOP (CYCLES 4)
        PC++; 
        CLK( 4 );
        break;

			case	0x0C: // TOP
			case	0x1C: // TOP
			case	0x3C: // TOP
			case	0x5C: // TOP
			case	0x7C: // TOP
			case	0xDC: // TOP
			case	0xFC: // TOP
				PC+=2;
				CLK( 4 );
				break;

      default:   // Unknown Instruction
        CLK( 2 );
#if 0
        InfoNES_MessageBox( "0x%02x is unknown instruction.\n", byCode ) ;
#endif
        break;
        
    }  /* end of switch ( byCode ) */

  }  /* end of while ... */

  // Correct the number of the clocks
  g_wPassedClocks -= wClocks;
}

// Addressing Op.
// Data
// Absolute,X
static inline unsigned char K6502_ReadAbsX(){ unsigned short int wA0, wA1; wA0 = AA_ABS; wA1 = wA0 + X; CLK( ( wA0 & 0x0100 ) != ( wA1 & 0x0100 ) ); return K6502_Read( wA1 ); };
// Absolute,Y
static inline unsigned char K6502_ReadAbsY(){ unsigned short int wA0, wA1; wA0 = AA_ABS; wA1 = wA0 + Y; CLK( ( wA0 & 0x0100 ) != ( wA1 & 0x0100 ) ); return K6502_Read( wA1 ); };
// (Indirect),Y
static inline unsigned char K6502_ReadIY(){ unsigned short int wA0, wA1; wA0 = K6502_ReadZpW( K6502_Read( PC++ ) ); wA1 = wA0 + Y; CLK( ( wA0 & 0x0100 ) != ( wA1 & 0x0100 ) ); return K6502_Read( wA1 ); };

/*===================================================================*/
/*                                                                   */
/*                  6502 Reading/Writing Operation                   */
/*                                                                   */
/*===================================================================*/
#include "K6502rw.h"







#ifdef use_lib_esp32_dac
 //Noise Frequency Lookup Table NTSC                                     
 unsigned int ApuNoiseFreqNTSC[16]=
 {
     4,    8,   16,   32,   64,   96,  128,  160,
   202,  254,  380,  508,  762, 1016, 2034, 4068
 };

 //Noise Frequency Lookup Table PAL
 unsigned int ApuNoiseFreqPAL[16]=
 {
     4,    8,   14,   30,   60,   88,  118,   148,
   188,  236,  354,  472,  708,  944, 1890,  3778
 };

void jj_recalcula_freq_dutty(unsigned char idCanal,unsigned int auxFrec, unsigned int dutty)
{
 unsigned int durPos=0;
 unsigned int durNeg=0;
 
 switch(dutty)
 {
  case 0: //12.5 percent
   durPos= SAMPLE_RATE/auxFrec/8;
   durNeg= (SAMPLE_RATE/auxFrec/2) + (SAMPLE_RATE/auxFrec/8);
   break;
 
  case 1: //25 percent
   durPos= SAMPLE_RATE/auxFrec/4;
   durNeg= (SAMPLE_RATE/auxFrec/2) + (SAMPLE_RATE/auxFrec/4);
   break; 
   
  case 2: //50 percent
   durPos= SAMPLE_RATE/auxFrec/2;
   durNeg= durPos;
   break; 
   
  case 3: //25 percent negative 
   durPos= (SAMPLE_RATE/auxFrec/2) + (SAMPLE_RATE/auxFrec/4);
   durNeg= SAMPLE_RATE/auxFrec/4;
   break;
 }
  
 //gb_max_cont_pos_ch[idCanal]= durPos; //Antes era asi
 //gb_max_cont_neg_ch[idCanal]= durNeg; //Antes era asi
 
 if (idCanal==2)
 {//Triangulo  
  //gb_triangle_inc_ch= (durPos==0)? 1: (durPos/15); //Cada cuanto sube 1 //Antes era asi
  //printf("Triangle:frec:%d durpos:%d inc:%d\n",auxFrec, durPos, gb_triangle_inc_ch); fflush(stdout);
 } 
 
 //LATCH
 switch (idCanal) 
 {
  case 0: case 1: 
   gb_latch_freq_pulse[idCanal]= auxFrec;
   gb_latch_pos_max_pulse[idCanal]= durPos;
   gb_latch_pos_max_pulse[idCanal]= durNeg;
   break;
   
  case 2:         
   gb_latch_freq_triangle= auxFrec;
   gb_latch_max_triangle= durPos;
   gb_latch_inc_triangle= (durPos==0)? 1: (durPos/15); //gb_latch_inc_triangle= gb_triangle_inc_ch; //Antes era asi
   break;       

  case 3: //Ruido
   gb_latch_pos_max_pulse[3]= durPos;
   gb_latch_neg_max_pulse[3]= durNeg; //Seria el mismo 50 50 por ciento
   //printf("RUIDO!!! p:%d n:%d\n",durPos,durNeg); fflush(stdout);   
 }
}

//********************************************************************
void jj_proc_canal_vol(unsigned char idCanal, unsigned char data)
{//DDLC NNNN   Duty, loop envelope/disable length counter, constant volume, envelope period/volume
 unsigned char env, auxloop, auxDuty, auxDur, auxC; //Canal 0 y 1 ondas cuadradas

 env= ((data>>4)&0x01);
 auxloop= ((data>>5)&0x01);
 auxDuty= ((data>>6)&0x03);
 auxC= ((data>>4)&0x01);
  
 //Duty 	Output waveform
 //0 	0 1 0 0 0 0 0 0 (12.5%)
 //1 	0 1 1 0 0 0 0 0 (25%)
 //2 	0 1 1 1 1 0 0 0 (50%)
 //3 	1 0 0 1 1 1 1 1 (25% negated) 
 
 //gbVolMixer_now[0]= (data & 0x0F)>>3;
 //gbVolMixer_now[0]= ((data & 0x0F) == 0)?0:1;
 //gbVol_canal_now[0]= ((data & 0x0F) == 0)?0:1;
 
 
 //gbVol_canal_now[idCanal]= (data & 0x0F);
 gbDutty_canal_now[idCanal]= auxDuty;
 
 if (auxC==0)
 {//Envolvente
  gb_latch_vol_pulse[idCanal]= 15; //gbVol_canal_now[idCanal]= 15; //Antes era asi
  if (idCanal==0) {gbPulso0Env=1;}
  if (idCanal==1) {gbPulso1Env=1;}
 }
 else
 {
  gb_latch_vol_pulse[idCanal]= (data & 0x0F); //gbVol_canal_now[idCanal]= (data & 0x0F); //Antes era asi
  if (idCanal==0) {gbPulso0Env=0;}
  if (idCanal==1) {gbPulso1Env=0;}
 }
 
 
/*
 //gb_latch_vol_pulse[idCanal]= (data & 0x0F); //Antes era asi
 if (auxC==0)
 {//Envolvente
  gb_latch_vol_pulse[idCanal]= 15; //gbVol_canal_now[idCanal]= 15; //Antes era asi
  if (idCanal==0) {gbPulso0Env=1;}
  if (idCanal==1) {gbPulso1Env=1;}
 }
 else
 {
  gb_latch_vol_pulse[idCanal]= (data & 0x0F); //gbVol_canal_now[idCanal]= (data & 0x0F); //Antes era asi
  if (idCanal==0) {gbPulso0Env=0;}
  if (idCanal==1) {gbPulso1Env=0;}
 } 
 */
  
// #ifdef use_lib_debug_apu_memwrite
//  printf("Cuadrado:%d vol:%d vol:%d env:%d loop:%d duty:%02X C:%d",idCanal,data,gbVol_canal_now[idCanal],env,auxloop,auxDuty, auxC);
//  printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
//  fflush(stdout);
// #endif
  
 //return RAM[address];
}

//********************************************************************************
void jj_proc_canal_freq(unsigned char idCanal, unsigned char isHigh, unsigned char data)
{//Freq low high canal 0 y 1 cuadrado
 //TTTT TTTT    Low  0x4002
 //LLLL LTTT    High 0x4003
 unsigned int aux,auxDur,auxFrec;
 
 if (isHigh==0)
 {
  if (idCanal==0)
  {
   gb_frec_canal1_low = data;
  }
  else
  {
   gb_frec_canal2_low = data;
  }
 }
 else
 {
  if (idCanal==0)
  {
   gb_frec_canal1_high = (data & 0x07);
   auxDur= ((data>>3)&&0x1F);
  }
  else
  {
   gb_frec_canal2_high = (data & 0x07);
   auxDur= ((data>>3)&&0x1F);      
  }
  //printf("Canal%d: data:%d dur:%d\n",idCanal, data, auxDur); fflush(stdout);
 }
  
 aux= (idCanal==0)? gb_frec_canal1_high: gb_frec_canal2_high;
 aux= aux<<8;
 aux= (idCanal==0)? (aux | gb_frec_canal1_low) : (aux | gb_frec_canal2_low);
 if (gb_use_video_mode_pal==1)
 {
  auxFrec= (unsigned int)(1.662607 * 1000000 / (16*(aux+1)));  
 }
 else
 {
  auxFrec= (unsigned int)(1.789773 * 1000000 / (16*(aux+1)));  
 }

 //gb_max_cont_ch[0]= SAMPLE_RATE/auxFrec/2;
 if (auxFrec!=0)
 {
  jj_recalcula_freq_dutty(idCanal, auxFrec, gbDutty_canal_now[idCanal]);      
 }
 
 
 //Envolvente
 if (gbPulso0Env==1)
 {//Reseteo la envolvente
  //if (gbVol_canal_now[0]==0)
  {
   gb_latch_vol_pulse[0]= 15; //gbVol_canal_now[0]= 15; //Antes era asi
  }
 }
 else
 {
  //gbVol_canal_now[0]= gb_latch_vol_pulse[0];
 }
 
 if (gbPulso1Env==1)
 {//Reseteo la envolvente
  //if (gbVol_canal_now[1]==0)
  {
    gb_latch_vol_pulse[1]= 15; //gbVol_canal_now[1]= 15; //Antes era asi
  }
 }  
 else
 {
  //gbVol_canal_now[1]= gb_latch_vol_pulse[1];      
 }
   
   
 //printf("Cuadrado:%d: data:%d dr:%d fq:%d",idCanal, data, auxDur,auxFrec);
 //printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
 //fflush(stdout);   
     
 //return RAM[address];
}

//******************************************************************
void jj_proc_triangle_vol(unsigned char data)
{
 //NO HACEMOS NADA
 //Se puede silenciar canal triangulo escbiendo 0x80 en 0x4008
  gb_latch_vol_triangle= (data & 0x0F); //gbVol_canal_now[2]= (data & 0x0F); //Antes era asi
 if (data==0x80)
 {
  gb_latch_vol_triangle= 0; //gbVol_canal_now[2]= 0; //Antes era asi
 }  
 

 //LATCH
 ////gb_latch_vol_triangle= (data & 0x0F);  //Antes era asi
 //if (data==0x80) //Antes era asi
 //{//Antes era asi
 // gb_latch_vol_triangle= 0;//Antes era asi
 //}//Antes era asi
               
 //gbVolMixer_now[2]= data & 0x0F;
 //gbVolMixer_now[2]= ((data & 0x01) == 0x01)? 2: 0;
 //gbVolMixer_now[2]= ((data & 0x01) == 0x01)? 1: 0;
 //gbVol_canal_now[2]= ((data & 0x01) == 0x01)? 15: 0;
 //gbVolMixer_now[2]= ((data & 0x0F) == 0)?0:15;
  
 #ifdef use_lib_debug_apu_memwrite
  printf("Triangulo vol:%d vol:%d\n",data,gbVol_canal_now[2]);
  printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
  fflush(stdout);
 #endif  
  
 //return RAM[address]; 
}

//******************************************************************
void jj_proc_triangle_freq(unsigned char isHigh,unsigned char data)
{
 unsigned int aux,auxDur,auxFrec;
      
 if (isHigh==0)
 {
  gb_frec_canal3_low = data;
 }
 else
 {
  gb_frec_canal3_high = data & 0x07;
 }

 aux= gb_frec_canal3_high;
 aux= aux<<8;
 aux= aux | gb_frec_canal3_low;

 //printf("aux:%d %d\n",aux,(aux-1)); fflush(stdout);
  
 if (gb_use_video_mode_pal==1)
 {
  //auxFrec= (auxFrec==0)?1:(unsigned int)(1.662607 * 1000000 / (16*(aux+1)));
  //revisar division 0
  if (aux==0)
   auxFrec=0;
  else
   auxFrec= (unsigned int)(1.662607 * 1000000 / (32*(aux-1)));  
   //auxFrec= (auxFrec==0)?1:(unsigned int)(1.662607 * 1000000 / (32*(aux-1)));  
 }
 else
 {
  //auxFrec= (auxFrec==0)?1:(unsigned int)(1.789773 * 1000000 / (16*(aux+1)));  
  if (aux==0)
   auxFrec=0;
  else
   auxFrec= (unsigned int)(1.789773 * 1000000 / (32*(aux-1)));  
   //auxFrec= (auxFrec==0)?1:(unsigned int)(1.789773 * 1000000 / (32*(aux-1)));  
 }
 //gb_max_cont_ch[2]= SAMPLE_RATE/auxFrec/2;    
 if (auxFrec!=0)
 {
  jj_recalcula_freq_dutty(2, auxFrec, 2); //50 percent  
 }
 //gbVol_canal_now[2]= ((auxFrec==0) || (aux<2))?0: 15;
 if (auxFrec==0)
 {//Silencio solo si la frecuencia es 0
  gb_latch_vol_triangle= 0; //gbVol_canal_now[2]= 0; //Antes era asi
 }
  
 //return RAM[address];
}


//*****************************************************************
void jj_proc_noise_vol(unsigned char data)
{//--LC VVVV  0x400C
 unsigned char auxloop, auxC; //Canal 0 y 1 ondas cuadradas

 //env= ((data>>4)&0x01);
 auxloop= ((data>>5)&0x01);
 //auxDuty= ((data>>6)&0x03);
 auxC= ((data>>4)&0x01);
    
 if (auxC==0)
 {//Envolvente
  gb_latch_vol_noise= 15; //gbVol_canal_now[3]= 15; //Antes era asi
  gbRuidoEnv=1;    
 }
 else
 {
  gb_latch_vol_noise= (data & 0x0F); //gbVol_canal_now[3]= (data & 0x0F); //Antes era asi
  gbRuidoEnv=0;
 }
 //gb_latch_vol_pulse[idCanal]= (data & 0x0F);     
 
 //printf("Noise vol:%d %d cte:%d",data,gbVol_canal_now[3],auxC);
 //printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
 //fflush(stdout);
}

//******************************************************************
void jj_proc_noise_freq(unsigned char data)
{//ruido registro 400E
 unsigned int idFreq,auxFreqId,auxFrec,auxFrecFinal;
      
 idFreq= (data & 0x0F); 
 auxFreqId= (gb_use_video_mode_pal==1)? ApuNoiseFreqPAL[idFreq]: ApuNoiseFreqNTSC[idFreq];
 auxFrec  = (39375000/44) / auxFreqId;
 //No periodico dividir 32767
 //Periodico dividir 93
 
 auxFrecFinal= auxFrec/93;
 auxFrecFinal= auxFrecFinal/4;
 if (auxFrecFinal!=0)
 {
  jj_recalcula_freq_dutty(3, auxFrecFinal, 2); //50 percent  
 }
 //gbVol_canal_now[2]= ((auxFrec==0) || (aux<2))?0: 15;
 if (auxFrecFinal==0)
 {//Silencio solo si la frecuencia es 0
  gb_latch_vol_noise= 0; //gbVol_canal_now[3]= 0; //Antes era asi
 }
 else
 {
  if (gbRuidoEnv==1)
  {//Reseteo la envolvente
   gb_latch_vol_noise= 15; //gbVol_canal_now[3]= 15;  //Antes era asi
  }
 }
 
 
 //printf("RUIDO:data:%d id:%d freqId:%d freq:%d Noperiod:%d Period:%d",data,idFreq,auxFreqId,auxFrecFinal, (auxFrec/32767), (auxFrec/93) );
 //printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
 //fflush(stdout);
}


//******************************************************************
void jj_proc_dmc_rate(unsigned char data)
{//IL--.RRRR
 //Rate   $0   $1   $2   $3   $4   $5   $6   $7   $8   $9   $A   $B   $C   $D   $E   $F
 //      ------------------------------------------------------------------------------
 //NTSC  428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54
 //PAL   398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98,  78,  66,  50
 unsigned char gb_rate_ntsc[16]={
  428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54
 };

 unsigned char gb_rate_pal[16]={
  398, 354, 316, 298, 276, 236, 210, 198, 176, 148, 132, 118,  98,  78,  66,  50
 }; 

 unsigned int auxRate,auxId;

 auxId= (data & 0x0F);
 auxRate= (gb_use_video_mode_pal==1)? (unsigned int)(1.662607 * 1000000/gb_rate_pal[auxId]) : (unsigned int)(1.789773 * 1000000/gb_rate_ntsc[auxId]);
 
 gb_dmc_rate= auxRate;
 
// printf("DMC rate:data:%d id:%d rate:%d\n",data,auxId,auxRate); fflush(stdout);
}

//******************************************************************
void jj_proc_dmc_load(unsigned char data)
{
// printf("DMC load:data:%d\n",data); fflush(stdout);
}

//******************************************************************
void jj_proc_dmc_addr(unsigned char data)
{
 unsigned int sampleAddr=  (unsigned int)0xC000 + (((unsigned int)data) * 64);
 
 gb_dmc_addr= sampleAddr;
// printf("DMC addr:data:%d addr:%08X\n",data,sampleAddr); fflush(stdout);
}

//******************************************************************
void DumpDMC(unsigned int addr,unsigned int len)
{
/*  
 unsigned char aux;
 int valorDeltaSigma=0;
 unsigned char cont16=0;
 unsigned char aByte;
 unsigned char bit;
 //int auxValor;
 
 printf("\nDump DMC BEGIN addr:%08X len:%d\n",addr,len);
 for (unsigned int i=0;i<len;i++)
 {
  aux= K6502_Read(addr); //ROM[addr];
  
  //for (int b=7;b>=0;b--)
  for (int b=0;b<7;b++)  
  {
   bit= ((aux>>b)&0x01);
   
   if (bit==1){ valorDeltaSigma++; } //DeltaSigma
   else { valorDeltaSigma--; }   
   
   
   
   //DeltaSigma BEGIN
   //aByte= (valor>0) ? valor: (-valor+127); //DeltaSigma   
   //aByte= valor+64; //DeltaSigma
   aByte= (valorDeltaSigma>0)? valorDeltaSigma : (255-valorDeltaSigma);
   //DeltaSigma END
   
   //Modo bit ahorro BEGIN
   //aByte= (bit==0)? 250: 10; //valor bit
   //Modo bit ahorro END
  
   printf("0x%02X,",aByte);
   //printf("(%d)%d,",bit,valorDeltaSigma); //DeltaSigma
   cont16++;
   if(cont16>31)
   {
    cont16=0;
    printf("\n");
   }
  }

  addr++;
 }
 
 printf("\nDump DMC END\n");
 fflush(stdout);
 */
}

//******************************************************************
void jj_proc_dmc_len(unsigned char data)
{
 unsigned int longitud= ((unsigned int)(data)* 16)+1;
 unsigned int auxResample= (gb_dmc_rate>SAMPLE_RATE)? gb_dmc_rate/SAMPLE_RATE : SAMPLE_RATE/gb_dmc_rate;
//  printf("DMC len data:%d lon:%d SAMPLERATE:%d rate:%d resample:%d\n",data,longitud,SAMPLE_RATE,gb_dmc_rate,auxResample); fflush(stdout);
 unsigned int addr= 0xC000+gb_dmc_addr;
 unsigned int cont16=0;
  
 unsigned int contSample=0;
 int valorDeltaSigma=0;
  
  
  //DumpDMC(addr,longitud);
  
  unsigned char contPeque=0;
  for (unsigned int i=0;i<longitud;i++)
  {
   unsigned char aux= K6502_Read(addr); //ROM[addr];
   
   //for (int b=7;b>=0;b--)
   for (int b=0;b<7;b++)
   {
    unsigned char bit= ((aux>>b)&0x01);
    //unsigned char signo= (bit==0)? 250: 10;
    
    if (bit==1){ valorDeltaSigma++; } //DeltaSigma
    else { valorDeltaSigma--; }
    
    if (gb_dmc_rate>SAMPLE_RATE)
    {
      if (gb_use_dmc_deltaSigma==1)
      {
       gb_dmc_sample[contSample]= valorDeltaSigma; //Modo DeltaSigma
      }      
      else
      {
       gb_dmc_sample[contSample]= bit; //Ahorra 7 bits modo pulsos
      }      
            
      contPeque++;
      if (contPeque>2)
      {
        contSample++;
        contPeque=0;
      }        
    }
    else
    {
     for (unsigned int resa=0; resa<=(auxResample+0); resa++)
     {
      if (gb_use_dmc_deltaSigma==1)
      {
       gb_dmc_sample[contSample]= valorDeltaSigma; //Modo DeltaSigma
      }
      else
      {
       gb_dmc_sample[contSample]= bit; //Ahorra 7 bits modo pulsos
      }
          
      contSample++;
     }
    }
   }
   addr++;
  }
  
  //Resampleo
  
  
  
  gb_dmc_sample_len= contSample;
  gb_dmc_sample_cur= 0;
  //gbVolMixer_now[4]= 1; //activamos mixer No hace falta
  gb_latch_vol_dmc=15; //gbVol_canal_now[4]= 15; //Ativamos canal Volumen //Antes era asi
  
  
  /*
  printf("\nSAMPLE BEGIN\n");    
  for (unsigned int i=0;i<longitud;i++)
  {
   unsigned char aux= K6502_Read(addr); //ROM[addr];
   //for (int b=6;b>=0;b--)
   for (int b=0;b<7;b++)
   {
    unsigned char bit= ((aux>>b)&0x01);
    unsigned char signo= (bit==0)? 250: 10; 
    printf("0x%02X,",signo);
   }   
   //printf("0x%02X,",aux);
      
   cont16++;
   if(cont16>2){ printf("\n"); cont16=0; }
   //if(cont16>15){ printf("\n"); cont16=0; }
   
   addr++;
  }
  printf("\nSAMPLE END\n");
  */
  
 // fflush(stdout);     
}

//******************************************************************
void jj_proc_mix(unsigned char data)
{//MIXER
 #ifdef use_lib_debug_apu_memwrite
  printf("MIXER:%02X\n",data);
  fflush(stdout);
 #endif 
 //gbVolMixer_now[0]= ((data & 0x01) != 0x01) ? 0 : 1; //Antes era asi   
 //gbVolMixer_now[1]= ((data & 0x02) != 0x02) ? 0 : 1; //Antes era asi 
 //gbVolMixer_now[2]= ((data & 0x04) != 0x04) ? 0 : 1; //Antes era asi
 //gbVolMixer_now[3]= ((data & 0x08) != 0x08) ? 0 : 1; //Antes era asi
 //gbVolMixer_now[4]= ((data & 0x10) != 0x10) ? 0 : 1; //Antes era asi
 //return RAM[address];
 
// printf("MIX:%d%d%d%d%d",gbVolMixer_now[4],gbVolMixer_now[3],gbVolMixer_now[2],gbVolMixer_now[1],gbVolMixer_now[0]);  
// printf (" l:%d f:%d\n",PPU_Scanline,gb_fps_cur);
// fflush(stdout);


 //LATCH
 gb_latch_vol_mix[0]= ((data & 0x01) != 0x01) ? 0 : 1;     
 gb_latch_vol_mix[1]= ((data & 0x02) != 0x02) ? 0 : 1; 
 gb_latch_vol_mix[2]= ((data & 0x04) != 0x04) ? 0 : 1;
 gb_latch_vol_mix[3]= ((data & 0x08) != 0x08) ? 0 : 1;
 gb_latch_vol_mix[4]= ((data & 0x10) != 0x10) ? 0 : 1;
}



void jj_apu_memwrite(unsigned int address, unsigned char data)
{

 switch (address)
 {
  case 0x4000: jj_proc_canal_vol(0,data); break; //Dutty, loop, env, vol canal 0 cuadrado (canal 1) DDLC NNNN
//  case 0x4001: printf("0x4001\n"); fflush(stdout); break;
  case 0x4002: jj_proc_canal_freq(0,0,data); break; //freq low canal 0 (canal1)  TTTT TTTT
  case 0x4003: jj_proc_canal_freq(0,1,data); break; //freq high canal 0 (canal1) LLLL LTTT 
       
  case 0x4004: jj_proc_canal_vol(1,data); break; //Dutty, loop, env, vol canal 1 cuadrado (canal 2) DDLC NNNN
//  case 0x4005: printf("0x4005\n"); fflush(stdout); break;
  case 0x4006: jj_proc_canal_freq(1,0,data); break; //freq low canal 1 (canal2)  TTTT TTTT
  case 0x4007: jj_proc_canal_freq(1,1,data); break; //freq high canal 1 (canal2) LLLL LTTT 
  
  case 0x4008: jj_proc_triangle_vol(data); break;    //C---.----
  case 0x400A: jj_proc_triangle_freq(0,data); break; //LLLL.LLLL
  case 0x400B: jj_proc_triangle_freq(1,data); break; //llll.lHHH
  
  case 0x400C: jj_proc_noise_vol(data); break; //--LC VVVV --, loop, env, vol canal 2 (canal 3) ruido
  case 0x400E: jj_proc_noise_freq(data); break; //L--- PPPP 	Loop noise (L), noise period (P) 
  case 0x400F: break; //LLLL L--- 	Length counter load (L) 
  
  //case 0x4010: jj_proc_dmc_rate(data); break; //IL--.RRRR
  //case 0x4011: jj_proc_dmc_load(data); break; //-DDD.DDDD
  //case 0x4012: jj_proc_dmc_addr(data); break; //AAAA.AAAA
  //case 0x4013: jj_proc_dmc_len(data); break; //LLLL.LLLL
  
  case 0x4015: jj_proc_mix(data); break;
 }


/*

 //if ((address>=0x4000)&&(address<=0x4015))
 //{
  //printf("SND:%08X d:%02X\n",address,data); fflush(stdout);
 //}
 //return; 
     
     
 unsigned int aux,auxFrec;
 //memory[address] = data;
 //Canal 1
 if (address == 0x4000)
 {
  //gbVolMixer_now[0]= (data & 0x0F)>>3;
  //gbVolMixer_now[0]= ((data & 0x0F) == 0)?0:1;
  gbVol_canal_now[0]= ((data & 0x0F) == 0)?0:1;
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal1 vol:%d vol:%d\n",data,gbVol_canal_now[0]); fflush(stdout);
  #endif
  
  //return RAM[address];
  return;
 }
 
 if (address == 0x4002)
 {
  gb_frec_canal1_low = data;
  //printf("1 LOW:%d\n",gb_frec_canal1_low); fflush(stdout);
  //return RAM[address];
  
  aux= gb_frec_canal1_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal1_low;
  auxFrec= (1789773/ (16*(aux+1)));
  gbFrecMixer_now[0]= auxFrec;

  gb_max_cont_ch[0]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;

  return;
 }

 if (address == 0x4003)
 {
  gb_frec_canal1_high = (data & 0x07);
  //printf("1 HIGH:%d\n",gb_frec_canal1_high); fflush(stdout);
  aux= gb_frec_canal1_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal1_low;
  //auxFrec= (unsigned int)((1.789773 * 1000000) / (16*(aux+1)));
  auxFrec= (1789773/ (16*(aux+1)));

  gbFrecMixer_now[0]= auxFrec;
  //if (auxFrec>1000)
  //{
  // auxFrec= 1000;
  //}

  //gbFrecMixer_now[0]= (unsigned short int)auxFrec;
  gb_max_cont_ch[0]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
    
  //gb_max_ch_now[0]= SAMPLE_RATE/auxFrec/2;
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal1 Dato:%d Frec:%d cont:%d\n",aux,auxFrec,gb_max_cont_ch[0]);
   fflush(stdout);
  #endif 
  return;
 } 
  
  
  
 //Canal2
 if (address == 0x4004)
 {
  //gbVolMixer_now[1]= (data & 0x0F)>>3;
  //gbVolMixer_now[1]= ((data & 0x0F) == 0)?0:1;
  gbVol_canal_now[1]= ((data & 0x0F) == 0)?0:1;
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal2 vol:%d vol:%d\n",data,gbVol_canal_now[1]); fflush(stdout);
  #endif
    
  //return RAM[address];
  return;
 }

 if (address == 0x4006)
 {
  gb_frec_canal2_low = data;

  aux= gb_frec_canal2_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal2_low;
  auxFrec= (1789773/ (16*(aux+1)));
  gbFrecMixer_now[1]= auxFrec;  

  gb_max_cont_ch[1]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
  //return RAM[address];
  return;
 }

 if (address == 0x4007)
 {             
  gb_frec_canal2_high = data & 0x07;
  aux= gb_frec_canal2_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal2_low;
  //auxFrec= (unsigned int)((1.789773 * 1000000) / (16*(aux+1)));  
  auxFrec= (1789773/ (16*(aux+1)));

  gbFrecMixer_now[1]= auxFrec;
  //if (auxFrec>1000){
  // auxFrec= 1000;
  //}  
  
  //gbFrecMixer_now[1]= (unsigned short int)auxFrec;
  gb_max_cont_ch[1]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
  
  //gb_vol_now[1]= (unsigned short int)auxFrec;
  //gb_max_ch_now[1]= SAMPLE_RATE/auxFrec/2;    
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal2 Dato:%d Frec:%d cont:%d\n",aux,auxFrec,gb_max_cont_ch[1]);
   fflush(stdout);
  #endif 
  //return RAM[address];
  return;
 } 
 
 
 //Canal3 Triangulo
 if (address == 0x4008)
 {
  //gbVolMixer_now[2]= data & 0x0F;
  //gbVolMixer_now[2]= ((data & 0x01) == 0x01)? 2: 0;
  //gbVolMixer_now[2]= ((data & 0x01) == 0x01)? 1: 0;
  gbVol_canal_now[2]= ((data & 0x01) == 0x01)? 1: 0;
  //gbVolMixer_now[2]= ((data & 0x0F) == 0)?0:15;
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal3 vol:%d vol:%d\n",data,gbVol_canal_now[2]); fflush(stdout);
  #endif  
  
  //return RAM[address];
  return;
 }
 
 if (address == 0x400A)
 {
  gb_frec_canal3_low = data;

  aux= gb_frec_canal3_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal3_low;
  auxFrec= (1789773/ (16*(aux+1)));
  gbFrecMixer_now[2]= auxFrec;

  gb_max_cont_ch[2]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
  //return RAM[address];
  return;
 }
 
 if (address == 0x400B)
 {
  gb_frec_canal3_high = data & 0x07;
  aux= gb_frec_canal3_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal3_low;
  //auxFrec= (unsigned int)((1.789773 * 1000000) / (16*(aux+1)));  
  auxFrec= (1789773/ (16*(aux+1)));

  gbFrecMixer_now[2]= auxFrec;
  //if (auxFrec>1000){
  // auxFrec= 1000;
  //}  
  
  //gbFrecMixer_now[2]= (unsigned short int)auxFrec;
  gb_max_cont_ch[2]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
  //gbVolMixer_now[2]= (gbFrecMixer_now[2] < 100)? 0: 2;
  
  //gb_vol_now[2]= (unsigned short int)auxFrec;
  //sgb_max_ch_now[2]= SAMPLE_RATE/auxFrec/2;
  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal3 Dato:%d Frec:%d cont:%d\n",aux,auxFrec,gb_max_cont_ch[2]);
   fflush(stdout);
  #endif 
//  return RAM[address];
  return;
 } 
 
 //RUIDO
 if (address == 0x400C)
 {//volumen ruido
  gbVol_canal_now[3]= ((data & 0x01) == 0x01)? 1: 0;  
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal4 vol:%d vol:%d\n",data,gbVol_canal_now[3]); fflush(stdout);
  #endif
  return;
 }
 
 if (address == 0x400E)
 {
  gb_frec_canal4_low = data;

  aux= gb_frec_canal4_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal4_low;
  auxFrec= (1789773/ (16*(aux+1)));
  gbFrecMixer_now[3]= auxFrec;    

  gb_max_cont_ch[3]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;

  return;             
 }
 
 if (address == 0x400F)
 {
  gb_frec_canal4_high = data & 0x07;
  aux= gb_frec_canal4_high;
  aux= aux<<8;
  aux= aux | gb_frec_canal4_low;
  //auxFrec= (unsigned int)(1.789773 * 1000000 / (16*(aux+1)));  
  auxFrec= (1789773/ (16*(aux+1)));
  gbFrecMixer_now[3]= auxFrec;

  gb_max_cont_ch[3]= (auxFrec==0)? 1 : SAMPLE_RATE/auxFrec/2;
  #ifdef use_lib_debug_apu_memwrite
   printf("Canal4 Dato:%d Frec:%d cont:%d\n",aux,auxFrec,gb_max_cont_ch[3]);
   fflush(stdout);
  #endif 
  return;
 }
 
 
 if (address == 0x4015)
 {//MIXER
  #ifdef use_lib_debug_apu_memwrite
   printf("MIXER:%02X\n",data);
   fflush(stdout);
  #endif 
  gbVolMixer_now[0]= ((data & 0x01) != 0x01) ? 0 : 1;     
  gbVolMixer_now[1]= ((data & 0x02) != 0x02) ? 0 : 1; 
  gbVolMixer_now[2]= ((data & 0x04) != 0x04) ? 0 : 1;
  gbVolMixer_now[3]= ((data & 0x08) != 0x08) ? 0 : 1;
  //return RAM[address];
  
  //printf ("PPU_Scanline:%d\n",PPU_Scanline);fflush(stdout);
  return;
 }        
  
//  return RAM[address];  
 */

}

#endif


