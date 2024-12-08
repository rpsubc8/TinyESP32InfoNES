//===================================================================
//                                                                   
//  InfoNES.cpp : NES Emulator for Win32, Linux(x86), Linux(PS2)     
//                                                                   
//  2000/05/18  InfoNES Project ( based on pNesX )                   
//                                                                   
//===================================================================

//-------------------------------------------------------------------
// File List :
//
// [NES Hardware]
//   InfoNES.cpp
//   InfoNES.h
//   K6502_rw.h
//
// [Mapper function]
//   InfoNES_Mapper.cpp
//   InfoNES_Mapper.h
//
// [The function which depends on a system]
//   InfoNES_System_ooo.cpp (ooo is a system name. win, ...)
//   InfoNES_System.h
//
// [CPU]
//   K6502.cpp
//   K6502.h
//
// [Others]
//   InfoNES_Types.h
//
//--------------------------------------------------------------------

//-------------------------------------------------------------------
//  Include files                                                    
//-------------------------------------------------------------------

#include "gbConfig.h"
#include "gbGlobals.h"
#include "InfoNES.h"
#include "InfoNESSystem.h"
#include "InfoNESMapper.h"
#include "InfoNESpAPU.h"
#include "K6502.h"
#include <Arduino.h>

//#include <SDL/SDL.h>

//-------------------------------------------------------------------
//  NES resources                                                    
//-------------------------------------------------------------------

//RAM
unsigned char *RAM; //unsigned char RAM[ RAM_SIZE ];

//SRAM
unsigned char *SRAM; //unsigned char SRAM[ SRAM_SIZE ];

//ROM
unsigned char *ROM;

//SRAM BANK ( 8Kb )
unsigned char *SRAMBANK;

//ROM BANK ( 8Kb * 4 )
unsigned char *ROMBANK0;
unsigned char *ROMBANK1;
unsigned char *ROMBANK2;
unsigned char *ROMBANK3;

//-------------------------------------------------------------------
//  PPU resources                                                    
//-------------------------------------------------------------------

//PPU RAM
unsigned char *PPURAM; //unsigned char PPURAM[ PPURAM_SIZE ];

//VROM
unsigned char *VROM;

//PPU BANK ( 1Kb * 16 )
unsigned char *PPUBANK[ 16 ];

//Sprite RAM
unsigned char *SPRRAM; //unsigned char SPRRAM[ SPRRAM_SIZE ];

//PPU Register
unsigned char PPU_R0;
unsigned char PPU_R1;
unsigned char PPU_R2;
unsigned char PPU_R3;
unsigned char PPU_R7;

//Vertical scroll value
unsigned char PPU_Scr_V;
unsigned char PPU_Scr_V_Next;
unsigned char PPU_Scr_V_Byte;
unsigned char PPU_Scr_V_Byte_Next;
unsigned char PPU_Scr_V_Bit;
unsigned char PPU_Scr_V_Bit_Next;

//Horizontal scroll value
unsigned char PPU_Scr_H;
unsigned char PPU_Scr_H_Next;
unsigned char PPU_Scr_H_Byte;
unsigned char PPU_Scr_H_Byte_Next;
unsigned char PPU_Scr_H_Bit;
unsigned char PPU_Scr_H_Bit_Next;

//PPU Address
unsigned short int PPU_Addr;

//PPU Address
unsigned short int PPU_Temp;

//The increase value of the PPU Address
unsigned short int PPU_Increment;

//Current Scanline
unsigned short int PPU_Scanline;

//Scanline Table
unsigned char *PPU_ScanTable; //unsigned char PPU_ScanTable[ 263 ];

//Name Table Bank
unsigned char PPU_NameTableBank;

//BG Base Address
unsigned char *PPU_BG_Base;

//Sprite Base Address
unsigned char *PPU_SP_Base;

//Sprite Height
unsigned short int PPU_SP_Height;

//Sprite #0 Scanline Hit Position
int SpriteJustHit;

//VRAM Write Enable ( 0: Disable, 1: Enable )
unsigned char byVramWriteEnable;

//PPU Address and Scroll Latch Flag
unsigned char PPU_Latch_Flag;

//Up and Down Clipping Flag ( 0: non-clip, 1: clip )
unsigned char PPU_UpDown_Clip;

//Frame IRQ ( 0: Disabled, 1: Enabled )
unsigned char FrameIRQ_Enable;
unsigned short int FrameStep;

//-------------------------------------------------------------------
//  Display and Others resouces                                      
//-------------------------------------------------------------------

//Frame Skip
unsigned short int FrameSkip;
unsigned short int FrameCnt;

//Display Buffer
//#if 0
//unsigned short int DoubleFrame[ 2 ][ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
//unsigned short int *WorkFrame;
//unsigned short int WorkFrameIdx;
//#else
//unsigned short int WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
//#endif

#ifdef use_lib_WorkFrame8
 unsigned char *WorkFrame; //unsigned char WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
#else
 unsigned short int WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
#endif

//Character Buffer
unsigned char *ChrBuf; //unsigned char ChrBuf[ 256 * 2 * 8 * 8 ];

//Update flag for ChrBuf
unsigned char ChrBufUpdate;

//Palette Table
#ifdef use_lib_WorkFrame8
 unsigned char *PalTable; //unsigned char PalTable[32];
#else
 unsigned short int PalTable[32];
#endif 

//Table for Mirroring
unsigned char PPU_MirrorTable[][ 4 ] =
{
  { NAME_TABLE0, NAME_TABLE0, NAME_TABLE1, NAME_TABLE1 },
  { NAME_TABLE0, NAME_TABLE1, NAME_TABLE0, NAME_TABLE1 },
  { NAME_TABLE1, NAME_TABLE1, NAME_TABLE1, NAME_TABLE1 },
  { NAME_TABLE0, NAME_TABLE0, NAME_TABLE0, NAME_TABLE0 },
  { NAME_TABLE0, NAME_TABLE1, NAME_TABLE2, NAME_TABLE3 },
  { NAME_TABLE0, NAME_TABLE0, NAME_TABLE0, NAME_TABLE1 }
};

//-------------------------------------------------------------------
//  APU and Pad resources                                            
//-------------------------------------------------------------------

//APU Register
unsigned char APU_Reg[ 0x18 ];

//APU Mute ( 0:OFF, 1:ON )
int APU_Mute = 0;

//Pad data
unsigned int PAD1_Latch; //DWORD PAD1_Latch;
unsigned int PAD2_Latch; //DWORD PAD2_Latch;
unsigned int PAD_System; //DWORD PAD_System;
unsigned int PAD1_Bit; //DWORD PAD1_Bit;
unsigned int PAD2_Bit; //DWORD PAD2_Bit;

//-------------------------------------------------------------------
//  Mapper Function                                                  
//-------------------------------------------------------------------

//Initialize Mapper
void (*MapperInit)();
//Write to Mapper
void (*MapperWrite)( unsigned short int wAddr, unsigned char byData );
//Write to SRAM
void (*MapperSram)( unsigned short int wAddr, unsigned char byData );
//Write to Apu
void (*MapperApu)( unsigned short int wAddr, unsigned char byData );
//Read from Apu
unsigned char (*MapperReadApu)( unsigned short int wAddr );
//Callback at VSync
void (*MapperVSync)();
//Callback at HSync
void (*MapperHSync)();
//Callback at PPU read/write
void (*MapperPPU)( unsigned short int wAddr );
//Callback at Rendering Screen 1:BG, 0:Sprite
void (*MapperRenderScreen)( unsigned char byMode );

//-------------------------------------------------------------------
//  ROM information                                                  
//-------------------------------------------------------------------

//.nes File Header
struct NesHeader_tag NesHeader;

//Mapper Number
unsigned char MapperNo;

//Mirroring 0:Horizontal 1:Vertical
unsigned char ROM_Mirroring;
//It has SRAM
unsigned char ROM_SRAM;
//It has Trainer
unsigned char ROM_Trainer;
//Four screen VRAM
unsigned char ROM_FourScr;

//===================================================================
//                                                                   
//                InfoNES_Init() : Initialize InfoNES                
//                                                                   
//===================================================================
void InfoNES_Init()
{
//  Initialize InfoNES
//
//  Remarks
//   Initialize K6502 and Scanline Table.

  int nIdx;

  // Initialize 6502
  K6502_Init();

  // Initialize Scanline Table
  for ( nIdx = 0; nIdx < 263; ++nIdx )
  {
    if ( nIdx < SCAN_ON_SCREEN_START )
      PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
    else
    if ( nIdx < SCAN_BOTTOM_OFF_SCREEN_START )
      PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
    else
    if ( nIdx < SCAN_UNKNOWN_START )
      PPU_ScanTable[ nIdx ] = SCAN_ON_SCREEN;
    else
    if ( nIdx < SCAN_VBLANK_START )
      PPU_ScanTable[ nIdx ] = SCAN_UNKNOWN;
    else
      PPU_ScanTable[ nIdx ] = SCAN_VBLANK;
  }
}

//===================================================================
//                                                                   
//                InfoNES_Fin() : Completion treatment               
//                                                                   
//===================================================================
void InfoNES_Fin()
{
//  Completion treatment
//
//  Remarks
//    Release resources
 
  // Finalize pAPU
  InfoNES_pAPUDone();

  // Release a memory for ROM
  InfoNES_ReleaseRom();
}

//===================================================================
//                                                                   
//                  InfoNES_Load() : Load a cassette                 
//                                                                   
//===================================================================
int InfoNES_Load( const char *pszFileName )
{
//  Load a cassette
//
//  Parameters
//    const char *pszFileName            (Read)
//      File name of ROM image
//
//  Return values
//     0 : It was finished normally.
//    -1 : An error occurred.
//
//  Remarks
//    Read a ROM image in the memory. 
//    Reset InfoNES.
 

  // Release a memory for ROM
  InfoNES_ReleaseRom();

  // Read a ROM image in the memory
  if ( InfoNES_ReadRom( pszFileName ) < 0 )
    return -1;

  // Reset InfoNES
  if ( InfoNES_Reset() < 0 )
    return -1;

  // Successful
  return 0;
}

//===================================================================
//                                                                   
//                 InfoNES_Reset() : Reset InfoNES                   
//                                                                   
//===================================================================
int InfoNES_Reset()
{
/*
 *  Reset InfoNES
 *
 *  Return values
 *     0 : Normally
 *    -1 : Non support mapper
 *
 *  Remarks
 *    Initialize Resources, PPU and Mapper.
 *    Reset CPU.
 */

  int nIdx;

  /*-------------------------------------------------------------------*/
  /*  Get information on the cassette                                  */
  /*-------------------------------------------------------------------*/

  // Get Mapper Number
  MapperNo = NesHeader.byInfo1 >> 4;

  // Check bit counts of Mapper No.
  for ( nIdx = 4; nIdx < 8 && NesHeader.byReserve[ nIdx ] == 0; ++nIdx )
    ;

  if ( nIdx == 8 )
  {
    // Mapper Number is 8bits
    MapperNo |= ( NesHeader.byInfo2 & 0xf0 );
  }

  // Get information on the ROM
  ROM_Mirroring = NesHeader.byInfo1 & 1;
  ROM_SRAM = NesHeader.byInfo1 & 2;
  ROM_Trainer = NesHeader.byInfo1 & 4;
  ROM_FourScr = NesHeader.byInfo1 & 8;

  /*-------------------------------------------------------------------*/
  /*  Initialize resources                                             */
  /*-------------------------------------------------------------------*/

  // Clear RAM
  InfoNES_MemorySet( RAM, 0, sizeof RAM );

  // Reset frame skip and frame count
  FrameSkip = 0;
  FrameCnt = 0;

#if 0
  // Reset work frame
  WorkFrame = DoubleFrame[ 0 ];
  WorkFrameIdx = 0;
#endif

  // Reset update flag of ChrBuf
  ChrBufUpdate = 0xff;

  // Reset palette table
  InfoNES_MemorySet( PalTable, 0, sizeof PalTable );

  // Reset APU register
  InfoNES_MemorySet( APU_Reg, 0, sizeof APU_Reg );

  // Reset joypad
  PAD1_Latch = PAD2_Latch = PAD_System = 0;
  PAD1_Bit = PAD2_Bit = 0;

  /*-------------------------------------------------------------------*/
  /*  Initialize PPU                                                   */
  /*-------------------------------------------------------------------*/

  InfoNES_SetupPPU();

  /*-------------------------------------------------------------------*/
  /*  Initialize pAPU                                                  */
  /*-------------------------------------------------------------------*/

  InfoNES_pAPUInit();

  /*-------------------------------------------------------------------*/
  /*  Initialize Mapper                                                */
  /*-------------------------------------------------------------------*/

  // Get Mapper Table Index
  for ( nIdx = 0; MapperTable[ nIdx ].nMapperNo != -1; ++nIdx )
  {
    if ( MapperTable[ nIdx ].nMapperNo == MapperNo )
      break;
  }

  if ( MapperTable[ nIdx ].nMapperNo == -1 )
  {
    // Non support mapper
    InfoNES_MessageBox( "Mapper #%d is unsupported.\n", MapperNo );
    return -1;
  }

  // Set up a mapper initialization function
  MapperTable[ nIdx ].pMapperInit();

  /*-------------------------------------------------------------------*/
  /*  Reset CPU                                                        */
  /*-------------------------------------------------------------------*/

  K6502_Reset();

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*                InfoNES_SetupPPU() : Initialize PPU                */
/*                                                                   */
/*===================================================================*/
void InfoNES_SetupPPU()
{
/*
 *  Initialize PPU
 *
 */
  int nPage;

  // Clear PPU and Sprite Memory
  InfoNES_MemorySet( PPURAM, 0, sizeof PPURAM );
  InfoNES_MemorySet( SPRRAM, 0, sizeof SPRRAM );

  // Reset PPU Register
  PPU_R0 = PPU_R1 = PPU_R2 = PPU_R3 = PPU_R7 = 0;

  // Reset latch flag
  PPU_Latch_Flag = 0;

  // Reset up and down clipping flag
  PPU_UpDown_Clip = 0;

  FrameStep = 0;
  FrameIRQ_Enable = 0;

  // Reset Scroll values
  PPU_Scr_V = PPU_Scr_V_Next = PPU_Scr_V_Byte = PPU_Scr_V_Byte_Next = PPU_Scr_V_Bit = PPU_Scr_V_Bit_Next = 0;
  PPU_Scr_H = PPU_Scr_H_Next = PPU_Scr_H_Byte = PPU_Scr_H_Byte_Next = PPU_Scr_H_Bit = PPU_Scr_H_Bit_Next = 0;

  // Reset PPU address
  PPU_Addr = 0;
  PPU_Temp = 0;

  // Reset scanline
  PPU_Scanline = 0;

  // Reset hit position of sprite #0 
  SpriteJustHit = 0;

  // Reset information on PPU_R0
  PPU_Increment = 1;
  PPU_NameTableBank = NAME_TABLE0;
  PPU_BG_Base = ChrBuf;
  PPU_SP_Base = ChrBuf + 256 * 64;
  PPU_SP_Height = 8;

  // Reset PPU banks
  for ( nPage = 0; nPage < 16; ++nPage )
    PPUBANK[ nPage ] = &PPURAM[ nPage * 0x400 ];

  /* Mirroring of Name Table */
  InfoNES_Mirroring( ROM_Mirroring );

  /* Reset VRAM Write Enable */
  byVramWriteEnable = ( NesHeader.byVRomSize == 0 ) ? 1 : 0;
}

/*===================================================================*/
/*                                                                   */
/*       InfoNES_Mirroring() : Set up a Mirroring of Name Table      */
/*                                                                   */
/*===================================================================*/
void InfoNES_Mirroring( int nType )
{
/*
 *  Set up a Mirroring of Name Table
 *
 *  Parameters
 *    int nType          (Read)
 *      Mirroring Type
 *        0 : Horizontal
 *        1 : Vertical
 *        2 : One Screen 0x2400
 *        3 : One Screen 0x2000
 *        4 : Four Screen
 *        5 : Special for Mapper #233
 */

  PPUBANK[ NAME_TABLE0 ] = &PPURAM[ PPU_MirrorTable[ nType ][ 0 ] * 0x400 ];
  PPUBANK[ NAME_TABLE1 ] = &PPURAM[ PPU_MirrorTable[ nType ][ 1 ] * 0x400 ];
  PPUBANK[ NAME_TABLE2 ] = &PPURAM[ PPU_MirrorTable[ nType ][ 2 ] * 0x400 ];
  PPUBANK[ NAME_TABLE3 ] = &PPURAM[ PPU_MirrorTable[ nType ][ 3 ] * 0x400 ];
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_Main() : The main loop of InfoNES            */
/*                                                                   */
/*===================================================================*/
void InfoNES_Main()
{
/*
 *  The main loop of InfoNES
 *
 */

  // Initialize InfoNES
  InfoNES_Init();

  // Main loop
  while ( 1 )
  {
    /*-------------------------------------------------------------------*/
    /*  To the menu screen                                               */
    /*-------------------------------------------------------------------*/
    if ( InfoNES_Menu() == -1 )
      break;  // Quit
    
    /*-------------------------------------------------------------------*/
    /*  Start a NES emulation                                            */
    /*-------------------------------------------------------------------*/
    InfoNES_Cycle();
  }

  // Completion treatment
  InfoNES_Fin();
}


unsigned int gb_1segundo_antes=0;
unsigned int gb_1segundo_cur=0;
void ShowPFS()
{
 gb_1segundo_cur= millis(); //SDL_GetTicks();
 if ((gb_1segundo_cur-gb_1segundo_antes)>=999)
 {
  Serial.printf("fps:%d\n",gb_fps_cur); //fflush(stdout);

  gb_1segundo_antes= gb_1segundo_cur;
  gb_fps_cur=0;
 }
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_Cycle() : The loop of emulation              */
/*                                                                   */
/*===================================================================*/
void InfoNES_Cycle()
{
/*
 *  The loop of emulation
 *
 */

#if 0
  // Set the PPU adress to the buffered value
  if ( ( PPU_R1 & R1_SHOW_SP ) || ( PPU_R1 & R1_SHOW_SCR ) )
		PPU_Addr = PPU_Temp;
#endif

  // Emulation loop
  for (;;)
  {    
    ShowPFS();   
       
    int nStep;

    // Set a flag if a scanning line is a hit in the sprite #0
    if ( SpriteJustHit == PPU_Scanline &&
      PPU_ScanTable[ PPU_Scanline ] == SCAN_ON_SCREEN )
    {
      // # of Steps to execute before sprite #0 hit
      nStep = SPRRAM[ SPR_X ] * STEP_PER_SCANLINE / NES_DISP_WIDTH;

      // Execute instructions
      K6502_Step( nStep );

      // Set a sprite hit flag
      if ( ( PPU_R1 & R1_SHOW_SP ) && ( PPU_R1 & R1_SHOW_SCR ) )
        PPU_R2 |= R2_HIT_SP;

      // NMI is required if there is necessity
      if ( ( PPU_R0 & R0_NMI_SP ) && ( PPU_R1 & R1_SHOW_SP ) )
        NMI_REQ;

      // Execute instructions
      K6502_Step( STEP_PER_SCANLINE - nStep );
    }
    else
    {
      // Execute instructions
      K6502_Step( STEP_PER_SCANLINE );
    }

    // Frame IRQ in H-Sync
    FrameStep += STEP_PER_SCANLINE;
    if ( FrameStep > STEP_PER_FRAME && FrameIRQ_Enable )
    {
      FrameStep %= STEP_PER_FRAME;
      IRQ_REQ;
      APU_Reg[ 0x4015 ] |= 0x40;
    }

    // A mapper function in H-Sync
    MapperHSync();
    
    // A function in H-Sync
    if ( InfoNES_HSync() == -1 )
      return;  // To the menu screen

    // HSYNC Wait
    InfoNES_Wait();
  }
}

#ifdef use_lib_esp32_dac
/*
 void SDL_sonido_poll()
 {
  unsigned int id= PPU_Scanline;
  
  for (unsigned char c=0;c<4;c++)
  {
   gb_cola_vol_mixer[id][c]= gb_vol_now[c];
   gb_cola_max_ch[id][c]= gb_max_ch_now[c];
  }
 }
 */
#endif

/*===================================================================*/
/*                                                                   */
/*              InfoNES_HSync() : A function in H-Sync               */
/*                                                                   */
/*===================================================================*/
int InfoNES_HSync()
{
//  A function in H-Sync
//
//  Return values
//    0 : Normally
//   -1 : Exit an emulation
 

  //-------------------------------------------------------------------
  //  Render a scanline                                                
  //-------------------------------------------------------------------
  if ( FrameCnt == 0 &&
       PPU_ScanTable[ PPU_Scanline ] == SCAN_ON_SCREEN )
  {
    #ifdef use_lib_frame_skip
     if ((gb_fps_cur & 0x01)==0)
     {
      InfoNES_DrawLine();
     }
    #else
     InfoNES_DrawLine();
    #endif 
    #ifdef use_lib_esp32_dac
     //SDL_sonido_poll();//Poll sonido    
    #endif 
  }

  //-------------------------------------------------------------------
  //  Set new scroll values                                            
  //-------------------------------------------------------------------
  PPU_Scr_V      = PPU_Scr_V_Next;
  PPU_Scr_V_Byte = PPU_Scr_V_Byte_Next;
  PPU_Scr_V_Bit  = PPU_Scr_V_Bit_Next;

  PPU_Scr_H      = PPU_Scr_H_Next;
  PPU_Scr_H_Byte = PPU_Scr_H_Byte_Next;
  PPU_Scr_H_Bit  = PPU_Scr_H_Bit_Next;

  /*-------------------------------------------------------------------*/
  /*  Next Scanline                                                    */
  /*-------------------------------------------------------------------*/
  PPU_Scanline = ( PPU_Scanline == SCAN_VBLANK_END ) ? 0 : PPU_Scanline + 1;

  /*-------------------------------------------------------------------*/
  /*  Operation in the specific scanning line                          */
  /*-------------------------------------------------------------------*/
  switch ( PPU_Scanline )
  {
    case SCAN_TOP_OFF_SCREEN:
      // Reset a PPU status
      PPU_R2 = 0;

      // Set up a character data
      if ( NesHeader.byVRomSize == 0 && FrameCnt == 0 )
        InfoNES_SetupChr();

      // Get position of sprite #0
      InfoNES_GetSprHitY();
      break;

    case SCAN_UNKNOWN_START:
      if ( FrameCnt == 0 )
      {
        // Transfer the contents of work frame on the screen
        InfoNES_LoadFrame();
        
#if 0
        // Switching of the double buffer
        WorkFrameIdx = 1 - WorkFrameIdx;
        WorkFrame = DoubleFrame[ WorkFrameIdx ];
#endif
      }
      break;

    case SCAN_VBLANK_START:
      // FrameCnt + 1
      FrameCnt = ( FrameCnt >= FrameSkip ) ? 0 : FrameCnt + 1;

      // Set a V-Blank flag
      PPU_R2 = R2_IN_VBLANK;

      // Reset latch flag
      PPU_Latch_Flag = 0;

      // pAPU Sound function in V-Sync
      if ( !APU_Mute )
        InfoNES_pAPUVsync();

      // A mapper function in V-Sync
      MapperVSync();

      // Get the condition of the joypad
      InfoNES_PadState( &PAD1_Latch, &PAD2_Latch, &PAD_System );
      
      // NMI on V-Blank
      if ( PPU_R0 & R0_NMI_VB )
        NMI_REQ;

      // Exit an emulation if a QUIT button is pushed
      if ( PAD_PUSH( PAD_System, PAD_SYS_QUIT ) )
        return -1;  // Exit an emulation      
      
      break;
  }

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*              InfoNES_DrawLine() : Render a scanline               */
/*                                                                   */
/*===================================================================*/
void InfoNES_DrawLine()
{
 //Render a scanline  

  int nX;
  int nY;
  int nY4;
  int nYBit;
  #ifdef use_lib_WorkFrame8
   unsigned char *pPalTbl;
  #else
   unsigned short int *pPalTbl;
  #endif
  unsigned char *pAttrBase;
  #ifdef use_lib_WorkFrame8
   unsigned char *pPoint;
  #else
   unsigned short int *pPoint;
  #endif 
  int nNameTable;
  unsigned char *pbyNameTable;
  unsigned char *pbyChrData;
  unsigned char *pSPRRAM;
  int nAttr;
  int nSprCnt;
  int nIdx;
  int nSprData;
  unsigned char bySprCol;
  unsigned char pSprBuf[ NES_DISP_WIDTH + 7 ];

  //-------------------------------------------------------------------
  // Render Background
  //-------------------------------------------------------------------

  //MMC5 VROM switch
  MapperRenderScreen( 1 );

  // Pointer to the render position
  pPoint = &WorkFrame[ PPU_Scanline * NES_DISP_WIDTH ];

  // Clear a scanline if screen is off
  if ( !( PPU_R1 & R1_SHOW_SCR ) )
  {
    //#ifdef use_lib_palette_ttgo
    // InfoNES_MemorySet( pPoint, 0x3F, NES_DISP_WIDTH << 1 ); //Borrar linea el 0 es gris
    //#else
     InfoNES_MemorySet( pPoint, 0, NES_DISP_WIDTH << 1 );
    //#endif 
  }
  else
  {
    nNameTable = PPU_NameTableBank;

    nY = PPU_Scr_V_Byte + ( PPU_Scanline >> 3 );

    nYBit = PPU_Scr_V_Bit + ( PPU_Scanline & 7 );

    if ( nYBit > 7 )
    {
      ++nY;
      nYBit &= 7;
    }
    nYBit <<= 3;

    if ( nY > 29 )
    {
      // Next NameTable (An up-down direction)
      nNameTable ^= NAME_TABLE_V_MASK;
      nY -= 30;
    }

    nX = PPU_Scr_H_Byte;

    nY4 = ( ( nY & 2 ) << 1 );

    /*-------------------------------------------------------------------*/
    /*  Rendering of the block of the left end                           */
    /*-------------------------------------------------------------------*/

    pbyNameTable = PPUBANK[ nNameTable ] + (nY<<5) + nX; //JJ pbyNameTable = PPUBANK[ nNameTable ] + nY * 32 + nX;
    pbyChrData = PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
    pAttrBase = PPUBANK[ nNameTable ] + 0x3c0 + (( nY>>2)<<3); //JJ pAttrBase = PPUBANK[ nNameTable ] + 0x3c0 + ( nY / 4 ) * 8;
    //#ifdef use_lib_WorkFrame8
    // pPalTbl =  (unsigned char *)&PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
    //#else    
     pPalTbl =  &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
    //#endif 

    for ( nIdx = PPU_Scr_H_Bit; nIdx < 8; ++nIdx )
    {
     *( pPoint++ ) = pPalTbl[ pbyChrData[ nIdx ] ];     
    }

    // Callback at PPU read/write
    MapperPPU( PATTBL( pbyChrData ) );

    ++nX;
    ++pbyNameTable;

    /*-------------------------------------------------------------------*/
    /*  Rendering of the left table                                      */
    /*-------------------------------------------------------------------*/

    for ( ; nX < 32; ++nX )
    {
      pbyChrData = PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
      //#ifdef use_lib_WorkFrame8
      // pPalTbl = (unsigned char *)&PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
      //#else
       pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
      //#endif 

      pPoint[ 0 ] = pPalTbl[ pbyChrData[ 0 ] ]; 
      pPoint[ 1 ] = pPalTbl[ pbyChrData[ 1 ] ];
      pPoint[ 2 ] = pPalTbl[ pbyChrData[ 2 ] ];
      pPoint[ 3 ] = pPalTbl[ pbyChrData[ 3 ] ];
      pPoint[ 4 ] = pPalTbl[ pbyChrData[ 4 ] ];
      pPoint[ 5 ] = pPalTbl[ pbyChrData[ 5 ] ];
      pPoint[ 6 ] = pPalTbl[ pbyChrData[ 6 ] ];
      pPoint[ 7 ] = pPalTbl[ pbyChrData[ 7 ] ];
      pPoint += 8;

      // Callback at PPU read/write
      MapperPPU( PATTBL( pbyChrData ) );

      ++pbyNameTable;
    }

    // Holizontal Mirror
    nNameTable ^= NAME_TABLE_H_MASK;

    pbyNameTable = PPUBANK[ nNameTable ] + nY * 32;
    pAttrBase = PPUBANK[ nNameTable ] + 0x3c0 + ( nY / 4 ) * 8;

    //-------------------------------------------------------------------
    //  Rendering of the right table
    //-------------------------------------------------------------------

    for ( nX = 0; nX < PPU_Scr_H_Byte; ++nX )
    {
      pbyChrData = PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
      //#ifdef use_lib_WorkFrame8
      // pPalTbl = (unsigned char*)&PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
      //#else
       pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
      //#endif 

      pPoint[ 0 ] = pPalTbl[ pbyChrData[ 0 ] ]; 
      pPoint[ 1 ] = pPalTbl[ pbyChrData[ 1 ] ];
      pPoint[ 2 ] = pPalTbl[ pbyChrData[ 2 ] ];
      pPoint[ 3 ] = pPalTbl[ pbyChrData[ 3 ] ];
      pPoint[ 4 ] = pPalTbl[ pbyChrData[ 4 ] ];
      pPoint[ 5 ] = pPalTbl[ pbyChrData[ 5 ] ];
      pPoint[ 6 ] = pPalTbl[ pbyChrData[ 6 ] ];
      pPoint[ 7 ] = pPalTbl[ pbyChrData[ 7 ] ];
      pPoint += 8;

      // Callback at PPU read/write
      MapperPPU( PATTBL( pbyChrData ) );

      ++pbyNameTable;
    }

    //-------------------------------------------------------------------
    //  Rendering of the block of the right end
    //-------------------------------------------------------------------

    pbyChrData = PPU_BG_Base + ( *pbyNameTable << 6 ) + nYBit;
    //#ifdef use_lib_WorkFrame8
    // pPalTbl = (unsigned char*)&PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
    //#else
     pPalTbl = &PalTable[ ( ( ( pAttrBase[ nX >> 2 ] >> ( ( nX & 2 ) + nY4 ) ) & 3 ) << 2 ) ];
    //#endif 
    for ( nIdx = 0; nIdx < PPU_Scr_H_Bit; ++nIdx )
    {
      pPoint[ nIdx ] = pPalTbl[ pbyChrData[ nIdx ] ];
    }

    // Callback at PPU read/write
    MapperPPU( PATTBL( pbyChrData ) );

    //-------------------------------------------------------------------
    //  Backgroud Clipping
    //-------------------------------------------------------------------
    if ( !( PPU_R1 & R1_CLIP_BG ) )
    {
      #ifdef use_lib_WorkFrame8
       unsigned char *pPointTop;
      #else
       unsigned short int *pPointTop;
      #endif 

      pPointTop = &WorkFrame[ PPU_Scanline * NES_DISP_WIDTH ];
      InfoNES_MemorySet( pPointTop, 0, 8 << 1 );
    }

    //-------------------------------------------------------------------
    //  Clear a scanline if up and down clipping flag is set
    //-------------------------------------------------------------------
    if ( PPU_UpDown_Clip && 
       ( SCAN_ON_SCREEN_START > PPU_Scanline || PPU_Scanline > SCAN_BOTTOM_OFF_SCREEN_START ) )
    {
      #ifdef use_lib_WorkFrame8
       unsigned char *pPointTop;
      #else
       unsigned short int *pPointTop;
      #endif 

      pPointTop = &WorkFrame[ PPU_Scanline * NES_DISP_WIDTH ];
      InfoNES_MemorySet( pPointTop, 0, NES_DISP_WIDTH << 1 );
    }  
  }

  //-------------------------------------------------------------------
  //  Render a sprite
  //-------------------------------------------------------------------

  //MMC5 VROM switch
  MapperRenderScreen( 0 );

  if ( PPU_R1 & R1_SHOW_SP )
  {
    // Reset Scanline Sprite Count
    PPU_R2 &= ~R2_MAX_SP;

    // Reset sprite buffer
    InfoNES_MemorySet( pSprBuf, 0, sizeof pSprBuf );

    // Render a sprite to the sprite buffer
    nSprCnt = 0;
    for ( pSPRRAM = SPRRAM + ( 63 << 2 ); pSPRRAM >= SPRRAM; pSPRRAM -= 4 )
    {
      nY = pSPRRAM[ SPR_Y ] + 1;
      if ( nY > PPU_Scanline || nY + PPU_SP_Height <= PPU_Scanline )
        continue;  // Next sprite

     //-------------------------------------------------------------------
     //  A sprite in scanning line
     //-------------------------------------------------------------------

      // Holizontal Sprite Count +1
      ++nSprCnt;
      
      nAttr = pSPRRAM[ SPR_ATTR ];
      nYBit = PPU_Scanline - nY;
      nYBit = ( nAttr & SPR_ATTR_V_FLIP ) ? ( PPU_SP_Height - nYBit - 1 ) << 3 : nYBit << 3;

      if ( PPU_R0 & R0_SP_SIZE )
      {
        // Sprite size 8x16
        if ( pSPRRAM[ SPR_CHR ] & 1 )
        {
          pbyChrData = ChrBuf + 16384 + ( ( pSPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit; //JJ pbyChrData = ChrBuf + 256 * 64 + ( ( pSPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit;
        }
        else
        {
          pbyChrData = ChrBuf + ( ( pSPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit;
        }
      }
      else
      {
        // Sprite size 8x8
        pbyChrData = PPU_SP_Base + ( pSPRRAM[ SPR_CHR ] << 6 ) + nYBit;
      }

      nAttr ^= SPR_ATTR_PRI;
      bySprCol = ( nAttr & ( SPR_ATTR_COLOR | SPR_ATTR_PRI ) ) << 2;
      nX = pSPRRAM[ SPR_X ];

      if ( nAttr & SPR_ATTR_H_FLIP )
      {
        // Horizontal flip
        if ( pbyChrData[ 7 ] )
          pSprBuf[ nX ]     = bySprCol | pbyChrData[ 7 ];
        if ( pbyChrData[ 6 ] )
          pSprBuf[ nX + 1 ] = bySprCol | pbyChrData[ 6 ];
        if ( pbyChrData[ 5 ] )
          pSprBuf[ nX + 2 ] = bySprCol | pbyChrData[ 5 ];
        if ( pbyChrData[ 4 ] )
          pSprBuf[ nX + 3 ] = bySprCol | pbyChrData[ 4 ];
        if ( pbyChrData[ 3 ] )
          pSprBuf[ nX + 4 ] = bySprCol | pbyChrData[ 3 ];
        if ( pbyChrData[ 2 ] )
          pSprBuf[ nX + 5 ] = bySprCol | pbyChrData[ 2 ];
        if ( pbyChrData[ 1 ] )
          pSprBuf[ nX + 6 ] = bySprCol | pbyChrData[ 1 ];
        if ( pbyChrData[ 0 ] )
          pSprBuf[ nX + 7 ] = bySprCol | pbyChrData[ 0 ];
      }
      else
      {
        // Non flip
        if ( pbyChrData[ 0 ] )
          pSprBuf[ nX ]     = bySprCol | pbyChrData[ 0 ];
        if ( pbyChrData[ 1 ] )
          pSprBuf[ nX + 1 ] = bySprCol | pbyChrData[ 1 ];
        if ( pbyChrData[ 2 ] )
          pSprBuf[ nX + 2 ] = bySprCol | pbyChrData[ 2 ];
        if ( pbyChrData[ 3 ] )
          pSprBuf[ nX + 3 ] = bySprCol | pbyChrData[ 3 ];
        if ( pbyChrData[ 4 ] )
          pSprBuf[ nX + 4 ] = bySprCol | pbyChrData[ 4 ];
        if ( pbyChrData[ 5 ] )
          pSprBuf[ nX + 5 ] = bySprCol | pbyChrData[ 5 ];
        if ( pbyChrData[ 6 ] )
          pSprBuf[ nX + 6 ] = bySprCol | pbyChrData[ 6 ];
        if ( pbyChrData[ 7 ] )
          pSprBuf[ nX + 7 ] = bySprCol | pbyChrData[ 7 ];
      }
    }

    // Rendering sprite
    pPoint -= ( NES_DISP_WIDTH - PPU_Scr_H_Bit );
    for ( nX = 0; nX < NES_DISP_WIDTH; ++nX )
    {
      nSprData = pSprBuf[ nX ];
      if ( nSprData  && ( nSprData & 0x80 || pPoint[ nX ] & 0x8000 ) )
      {
        pPoint[ nX ] = PalTable[ ( nSprData & 0xf ) + 0x10 ];
      }
    }

    //-------------------------------------------------------------------
    //  Sprite Clipping
    //-------------------------------------------------------------------
    if ( !( PPU_R1 & R1_CLIP_SP ) )
    {
      #ifdef use_lib_WorkFrame8
       unsigned char *pPointTop;
      #else
       unsigned short int *pPointTop;
      #endif

      pPointTop = &WorkFrame[ PPU_Scanline * NES_DISP_WIDTH ];
      InfoNES_MemorySet( pPointTop, 0, 8 << 1 );
    }

    if ( nSprCnt >= 8 )
      PPU_R2 |= R2_MAX_SP;  // Set a flag of maximum sprites on scanline
  }
}

//===================================================================
//                                                                   
// InfoNES_GetSprHitY() : Get a position of scanline hits sprite #0  
//                                                                   
//===================================================================
void InfoNES_GetSprHitY()
{
//Get a position of scanline hits sprite #0

  int nYBit;
  unsigned int *pdwChrData;
  int nOff;

  if ( SPRRAM[ SPR_ATTR ] & SPR_ATTR_V_FLIP )
  {
    // Vertical flip
    nYBit = ( PPU_SP_Height - 1 ) << 3;
    nOff = -2;
  }
  else
  {
    // Non flip
    nYBit = 0;
    nOff = 2;
  }

  if ( PPU_R0 & R0_SP_SIZE )
  {
    // Sprite size 8x16
    if ( SPRRAM[ SPR_CHR ] & 1 )
    {
      pdwChrData = (unsigned int *)( ChrBuf + 256 * 64 + ( ( SPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit );
    }
    else
    {
      pdwChrData = (unsigned int * )( ChrBuf + ( ( SPRRAM[ SPR_CHR ] & 0xfe ) << 6 ) + nYBit );
    } 
  }
  else
  {
    // Sprite size 8x8
    pdwChrData = (unsigned int *)( PPU_SP_Base + ( SPRRAM[ SPR_CHR ] << 6 ) + nYBit );
  }

  if ( ( SPRRAM[ SPR_Y ] + 1 <= SCAN_UNKNOWN_START ) && ( SPRRAM[SPR_Y] > 0 ) )
	{
		for ( int nLine = 0; nLine < PPU_SP_Height; nLine++ )
		{
			if ( pdwChrData[ 0 ] | pdwChrData[ 1 ] )
			{
        // Scanline hits sprite #0
				SpriteJustHit = SPRRAM[SPR_Y] + 1 + nLine;
				nLine = SCAN_VBLANK_END;
			}
			pdwChrData += nOff;
		}
  } else {
    // Scanline didn't hit sprite #0
		SpriteJustHit = SCAN_UNKNOWN_START + 1;
  }
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_SetupChr() : Develop character data            */
/*                                                                   */
/*===================================================================*/
void InfoNES_SetupChr()
{
/*
 *  Develop character data
 *
 */

  unsigned char *pbyBGData;
  unsigned char byData1;
  unsigned char byData2;
  int nIdx;
  int nY;
  int nOff;
  static unsigned char *pbyPrevBank[ 8 ];
  int nBank;

  for ( nBank = 0; nBank < 8; ++nBank )
  {
    if ( pbyPrevBank[ nBank ] == PPUBANK[ nBank ] && !( ( ChrBufUpdate >> nBank ) & 1 ) )
      continue;  // Next bank

    /*-------------------------------------------------------------------*/
    /*  An address is different from the last time                       */
    /*    or                                                             */
    /*  An update flag is being set                                      */
    /*-------------------------------------------------------------------*/

    for ( nIdx = 0; nIdx < 64; ++nIdx )
    {
      nOff = ( nBank << 12 ) + ( nIdx << 6 );

      for ( nY = 0; nY < 8; ++nY )
      {
        pbyBGData = PPUBANK[ nBank ] + ( nIdx << 4 ) + nY;

        byData1 = ( ( pbyBGData[ 0 ] >> 1 ) & 0x55 ) | ( pbyBGData[ 8 ] & 0xAA );
        byData2 = ( pbyBGData[ 0 ] & 0x55 ) | ( ( pbyBGData[ 8 ] << 1 ) & 0xAA );

        ChrBuf[ nOff ]     = ( byData1 >> 6 ) & 3;
        ChrBuf[ nOff + 1 ] = ( byData2 >> 6 ) & 3;
        ChrBuf[ nOff + 2 ] = ( byData1 >> 4 ) & 3;
        ChrBuf[ nOff + 3 ] = ( byData2 >> 4 ) & 3;
        ChrBuf[ nOff + 4 ] = ( byData1 >> 2 ) & 3;
        ChrBuf[ nOff + 5 ] = ( byData2 >> 2 ) & 3;
        ChrBuf[ nOff + 6 ] = byData1 & 3;
        ChrBuf[ nOff + 7 ] = byData2 & 3;

        nOff += 8;
      }
    }
    // Keep this address
    pbyPrevBank[ nBank ] = PPUBANK[ nBank ];
  }

  // Reset update flag
  ChrBufUpdate = 0;
}