/*===================================================================*/
/*                                                                   */
/*  InfoNES.h : NES Emulator for Win32, Linux(x86), Linux(PS2)       */
/*                                                                   */
/*  2000/05/14  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_H_INCLUDED
#define InfoNES_H_INCLUDED

 #include "gbConfig.h"
 #include "gbGlobals.h"
 

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

//JJ #include "InfoNESTypes.h"

/*-------------------------------------------------------------------*/
/*  NES resources                                                    */
/*-------------------------------------------------------------------*/

#define RAM_SIZE     0x2000
#define SRAM_SIZE    0x2000
#define PPURAM_SIZE  0x4000
#define SPRRAM_SIZE  256

/* RAM */
//JJ extern BYTE RAM[];

/* SRAM */
//JJ extern BYTE SRAM[];

/* ROM */
//JJ extern BYTE *ROM;

/* SRAM BANK ( 8Kb ) */
//JJ extern BYTE *SRAMBANK;

/* ROM BANK ( 8Kb * 4 ) */
//JJ extern BYTE *ROMBANK0;
//JJ extern BYTE *ROMBANK1;
//JJ extern BYTE *ROMBANK2;
//JJ extern BYTE *ROMBANK3;

/*-------------------------------------------------------------------*/
/*  PPU resources                                                    */
/*-------------------------------------------------------------------*/

/* PPU RAM */
//JJ extern BYTE PPURAM[];

/* VROM */
//JJ extern BYTE *VROM;

/* PPU BANK ( 1Kb * 16 ) */
//JJ extern BYTE *PPUBANK[];

#define NAME_TABLE0  8
#define NAME_TABLE1  9
#define NAME_TABLE2  10
#define NAME_TABLE3  11

#define NAME_TABLE_V_MASK 2
#define NAME_TABLE_H_MASK 1

/* Sprite RAM */
//JJ extern BYTE SPRRAM[];

#define SPR_Y    0
#define SPR_CHR  1
#define SPR_ATTR 2
#define SPR_X    3
#define SPR_ATTR_COLOR  0x3
#define SPR_ATTR_V_FLIP 0x80
#define SPR_ATTR_H_FLIP 0x40
#define SPR_ATTR_PRI    0x20

/* PPU Register */
//JJ extern BYTE PPU_R0;
//JJ extern BYTE PPU_R1;
//JJ extern BYTE PPU_R2;
//JJ extern BYTE PPU_R3;
//JJ extern BYTE PPU_R7;

//JJ extern BYTE PPU_Scr_V;
//JJ extern BYTE PPU_Scr_V_Next;
//JJ extern BYTE PPU_Scr_V_Byte;
//JJ extern BYTE PPU_Scr_V_Byte_Next;
//JJ extern BYTE PPU_Scr_V_Bit;
//JJ extern BYTE PPU_Scr_V_Bit_Next;

//JJ extern BYTE PPU_Scr_H;
//JJ extern BYTE PPU_Scr_H_Next;
//JJ extern BYTE PPU_Scr_H_Byte;
//JJ extern BYTE PPU_Scr_H_Byte_Next;
//JJ extern BYTE PPU_Scr_H_Bit;
//JJ extern BYTE PPU_Scr_H_Bit_Next;

//JJ extern BYTE PPU_Latch_Flag;
//JJ extern WORD PPU_Addr;
//JJ extern WORD PPU_Temp;
//JJ extern WORD PPU_Increment;

//JJ extern BYTE PPU_Latch_Flag;
//JJ extern BYTE PPU_UpDown_Clip;

#define R0_NMI_VB      0x80
#define R0_NMI_SP      0x40
#define R0_SP_SIZE     0x20
#define R0_BG_ADDR     0x10
#define R0_SP_ADDR     0x08
#define R0_INC_ADDR    0x04
#define R0_NAME_ADDR   0x03

#define R1_BACKCOLOR   0xe0
#define R1_SHOW_SP     0x10
#define R1_SHOW_SCR    0x08
#define R1_CLIP_SP     0x04
#define R1_CLIP_BG     0x02
#define R1_MONOCHROME  0x01

#define R2_IN_VBLANK   0x80
#define R2_HIT_SP      0x40
#define R2_MAX_SP      0x20
#define R2_WRITE_FLAG  0x10

#define SCAN_TOP_OFF_SCREEN     0
#define SCAN_ON_SCREEN          1
#define SCAN_BOTTOM_OFF_SCREEN  2
#define SCAN_UNKNOWN            3
#define SCAN_VBLANK             4

#define SCAN_TOP_OFF_SCREEN_START       0 
#define SCAN_ON_SCREEN_START            8
#define SCAN_BOTTOM_OFF_SCREEN_START  232
#define SCAN_UNKNOWN_START            240
#define SCAN_VBLANK_START             243
#define SCAN_VBLANK_END               262

#define STEP_PER_SCANLINE             113
#define STEP_PER_FRAME                29828

/* Develop Scroll Registers */
#define InfoNES_SetupScr() \
{ \
  /* V-Scroll Register */ \
  /* PPU_Scr_V_Byte_Next = ( BYTE )( ( PPU_Addr & 0x03e0 ) >> 5 ); */ \
  /* PPU_Scr_V_Bit_Next = ( BYTE )( ( PPU_Addr & 0x7000 ) >> 12 ); */ \
  /* H-Scroll Register */ \
  /* PPU_Scr_H_Byte_Next = ( BYTE )( PPU_Addr & 0x001f ); */ \
  /* NameTableBank */ \
  PPU_NameTableBank = NAME_TABLE0 + ( ( PPU_Addr & 0x0C00 ) >> 10 ); \
}

/* Current Scanline */
//JJ extern WORD PPU_Scanline;

/* Scanline Table */
//JJ extern BYTE PPU_ScanTable[];

/* Name Table Bank */
//JJ extern BYTE PPU_NameTableBank;

/* BG Base Address */
//JJ extern BYTE *PPU_BG_Base;

/* Sprite Base Address */
//JJ extern BYTE *PPU_SP_Base;

/* Sprite Height */
//JJ extern WORD PPU_SP_Height;

/* NES display size */
#define NES_DISP_WIDTH      256
#define NES_DISP_HEIGHT     240

/* VRAM Write Enable ( 0: Disable, 1: Enable ) */
//JJ extern BYTE byVramWriteEnable;

/* Frame IRQ ( 0: Disabled, 1: Enabled )*/
//JJ extern BYTE FrameIRQ_Enable;
//JJ extern WORD FrameStep;

/*-------------------------------------------------------------------*/
/*  Display and Others resouces                                      */
/*-------------------------------------------------------------------*/

/* Frame Skip */
//JJ extern WORD FrameSkip;
//JJ extern WORD FrameCnt;
//JJ extern WORD FrameWait;

//JJ #if 0
//JJ extern WORD DoubleFrame[ 2 ][ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
//JJ extern WORD *WorkFrame;
//JJ extern WORD WorkFrameIdx;
//JJ #else
//JJ //JJ extern WORD WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
//JJ #endif

//JJ extern BYTE ChrBuf[];

//JJ extern BYTE ChrBufUpdate;

//JJ extern WORD PalTable[];

/*-------------------------------------------------------------------*/
/*  APU and Pad resources                                            */
/*-------------------------------------------------------------------*/

//JJ extern BYTE APU_Reg[];
//JJ extern int APU_Mute;

//JJ extern DWORD PAD1_Latch;
//JJ extern DWORD PAD2_Latch;
//JJ extern DWORD PAD_System;
//JJ extern DWORD PAD1_Bit;
//JJ extern DWORD PAD2_Bit;

#define PAD_SYS_QUIT   1
#define PAD_SYS_OK     2
#define PAD_SYS_CANCEL 4
#define PAD_SYS_UP     8
#define PAD_SYS_DOWN   0x10
#define PAD_SYS_LEFT   0x20
#define PAD_SYS_RIGHT  0x40

#define PAD_PUSH(a,b)  ( ( (a) & (b) ) != 0 )

/*-------------------------------------------------------------------*/
/*  Mapper Function                                                  */
/*-------------------------------------------------------------------*/

/* Initialize Mapper */
//JJ extern void (*MapperInit)();
/* Write to Mapper */
//JJ extern void (*MapperWrite)( WORD wAddr, BYTE byData );
/* Write to SRAM */
//JJ extern void (*MapperSram)( WORD wAddr, BYTE byData );
/* Write to APU */
//JJ extern void (*MapperApu)( WORD wAddr, BYTE byData );
/* Read from Apu */
//JJ extern BYTE (*MapperReadApu)( WORD wAddr );
/* Callback at VSync */
//JJ extern void (*MapperVSync)();
/* Callback at HSync */
//JJ extern void (*MapperHSync)();
/* Callback at PPU read/write */
//JJ extern void (*MapperPPU)( WORD wAddr );
/* Callback at Rendering Screen 1:BG, 0:Sprite */
//JJ extern void (*MapperRenderScreen)( BYTE byMode );

/*-------------------------------------------------------------------*/
/*  ROM information                                                  */
/*-------------------------------------------------------------------*/

/* .nes File Header */
struct NesHeader_tag
{
  unsigned char byID[ 4 ];
  unsigned char byRomSize;
  unsigned char byVRomSize;
  unsigned char byInfo1;
  unsigned char byInfo2;
  unsigned char byReserve[ 8 ];
};

/* .nes File Header */
//JJ extern struct NesHeader_tag NesHeader;

/* Mapper No. */
//JJ extern BYTE MapperNo;

/* Other */
//JJ extern BYTE ROM_Mirroring;
//JJ extern BYTE ROM_SRAM;
//JJ extern BYTE ROM_Trainer;
//JJ extern BYTE ROM_FourScr;

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Initialize InfoNES */
void InfoNES_Init();

/* Completion treatment */
void InfoNES_Fin();

/* Load a cassette */
int InfoNES_Load( const char *pszFileName );

/* Reset InfoNES */
int InfoNES_Reset();

/* Initialize PPU */
void InfoNES_SetupPPU();

/* Set up a Mirroring of Name Table */
void InfoNES_Mirroring( int nType );

/* The main loop of InfoNES */ 
void InfoNES_Main();

/* The loop of emulation */
void InfoNES_Cycle();

/* A function in H-Sync */
int InfoNES_HSync();

/* Render a scanline */
void InfoNES_DrawLine();

/* Get a position of scanline hits sprite #0 */
void InfoNES_GetSprHitY();

/* Develop character data */
void InfoNES_SetupChr();

#ifdef use_lib_sound_fabgl
 void jj_snd_pop(void);
#endif 

void SDL_sonido_poll_play(void);

#endif /* !InfoNES_H_INCLUDED */
