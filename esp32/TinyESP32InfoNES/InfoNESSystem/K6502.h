/*===================================================================*/
/*                                                                   */
/*  K6502.h : Header file for K6502                                  */
/*                                                                   */
/*  2000/05/29  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef K6502_H_INCLUDED
#define K6502_H_INCLUDED

#include "gbConfig.h"
#include "gbGlobals.h"

// Type definition
//#ifndef DWORD
//typedef unsigned long int DWORD; //JJ typedef unsigned long  DWORD;
//#endif

//#ifndef WORD
//typedef unsigned short int WORD; //JJ typedef unsigned short WORD;
//#endif

//#ifndef BYTE
//typedef unsigned char  BYTE;
//#endif

#ifndef NULL
#define NULL 0
#endif

/* 6502 Flags */
#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_R 0x20
#define FLAG_V 0x40
#define FLAG_N 0x80

/* Stack Address */
#define BASE_STACK   0x100

/* Interrupt Vectors */
#define VECTOR_NMI   0xfffa
#define VECTOR_RESET 0xfffc
#define VECTOR_IRQ   0xfffe

// NMI Request
#define NMI_REQ  NMI_State = 0;

// IRQ Request
#define IRQ_REQ  IRQ_State = 0;

// Emulator Operation
void K6502_Init();
void K6502_Reset();
void K6502_Set_Int_Wiring( unsigned char byNMI_Wiring, unsigned char byIRQ_Wiring );
void K6502_Step( register unsigned short int wClocks );

// I/O Operation (User definition)
static inline unsigned char K6502_Read( unsigned short int wAddr);
static inline unsigned short int K6502_ReadW( unsigned short int wAddr );
static inline unsigned short int K6502_ReadW2( unsigned short int wAddr );
static inline unsigned char K6502_ReadZp( unsigned char byAddr );
static inline unsigned short int K6502_ReadZpW( unsigned char byAddr );
static inline unsigned char K6502_ReadAbsX();
static inline unsigned char K6502_ReadAbsY();
static inline unsigned char K6502_ReadIY();

static inline void K6502_Write( unsigned short int wAddr, unsigned char byData );
static inline void K6502_WriteW( unsigned short int wAddr, unsigned short int wData );

// The state of the IRQ pin
//JJ extern BYTE IRQ_State;

// The state of the NMI pin
//JJ extern BYTE NMI_State;

// The number of the clocks that it passed
//JJ extern WORD g_wPassedClocks;

#endif //!K6502_H_INCLUDED


