#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"


//InfoNES_Mapper_107 begin
/*===================================================================*/
/*                                                                   */
/*             Mapper 107 : Magic Dragon Mapper                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 107                                            */
/*-------------------------------------------------------------------*/
void Map107_Init()
{
  /* Initialize Mapper */
  MapperInit = Map107_Init;

  /* Write to Mapper */
  MapperWrite = Map107_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 107 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map107_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( (((byData>>1)&0x03)<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE( ( ((byData&0x07)<<3) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ( ((byData&0x07)<<3) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ( ((byData&0x07)<<3) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ( ((byData&0x07)<<3) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ( ((byData&0x07)<<3) + 4 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( ((byData&0x07)<<3) + 5 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ( ((byData&0x07)<<3) + 6 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( ((byData&0x07)<<3) + 7 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_107 end












//InfoNES_Mapper_108 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 108                             */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 108                                            */
/*-------------------------------------------------------------------*/
void Map108_Init()
{
  /* Initialize Mapper */
  MapperInit = Map108_Init;

  /* Write to Mapper */
  MapperWrite = Map108_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0xC % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( 0xD % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( 0xE % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( 0xF % ( NesHeader.byRomSize << 1 ) );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 108 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map108_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
//InfoNES_Mapper_108 end











//InfoNES_Mapper_109 begin
/*===================================================================*/
/*                                                                   */
/*          Mapper 109 : SACHEN The Great Wall SA-019                */
/*                                                                   */
/*===================================================================*/

unsigned char	Map109_Reg;
unsigned char	Map109_Chr0, Map109_Chr1, Map109_Chr2, Map109_Chr3;
unsigned char	Map109_Chrmode0, Map109_Chrmode1;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 109                                            */
/*-------------------------------------------------------------------*/
void Map109_Init()
{
  /* Initialize Mapper */
  MapperInit = Map109_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map109_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Initialize Registers */
  Map109_Reg = 0;
  Map109_Chr0 = 0;
  Map109_Chr1 = 0;
  Map109_Chr2 = 0;
  Map109_Chr3 = 0;
  Map109_Chrmode0 = 0;
  Map109_Chrmode1 = 0;

  /* Set PPU Banks */
  Map109_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 109 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map109_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map109_Reg = byData;
    break;
  case	0x4101:
    switch( Map109_Reg ) {
    case 0:
      Map109_Chr0 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 1:
      Map109_Chr1 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 2:
      Map109_Chr2 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 3:
      Map109_Chr3 = byData;
      Map109_Set_PPU_Banks();
      break;
    case 4:
      Map109_Chrmode0 = byData & 0x01;
      Map109_Set_PPU_Banks();
      break;
    case 5:
      ROMBANK0 = ROMPAGE( ( ( byData & 0x07 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( byData & 0x07 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ( ( byData & 0x07 ) + 2 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( ( byData & 0x07 ) + 3 ) % ( NesHeader.byRomSize << 1 ) );
      break;
    case 6:
      Map109_Chrmode1 = byData & 0x07;
      Map109_Set_PPU_Banks();
      break;
    case 7:
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 109 Set Bank PPU Function                                 */
/*-------------------------------------------------------------------*/
void Map109_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE((Map109_Chr0) % (NesHeader.byVRomSize<<3) );
    PPUBANK[ 1 ] = VROMPAGE((Map109_Chr1|((Map109_Chrmode1<<3)&0x8)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((Map109_Chr2|((Map109_Chrmode1<<2)&0x8)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((Map109_Chr3|((Map109_Chrmode1<<1)&0x8)|(Map109_Chrmode0*0x10)) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((NesHeader.byVRomSize<<3)-4);
    PPUBANK[ 5 ] = VROMPAGE((NesHeader.byVRomSize<<3)-3);
    PPUBANK[ 6 ] = VROMPAGE((NesHeader.byVRomSize<<3)-2);
    PPUBANK[ 7 ] = VROMPAGE((NesHeader.byVRomSize<<3)-1);
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_109 end










//InfoNES_Mapper_110 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 110                               */
/*                                                                   */
/*===================================================================*/

unsigned char	Map110_Reg0, Map110_Reg1;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 110                                            */
/*-------------------------------------------------------------------*/
void Map110_Init()
{
  /* Initialize Mapper */
  MapperInit = Map110_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map110_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize Registers */
  Map110_Reg0 = 0;
  Map110_Reg1 = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 110 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map110_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map110_Reg1 = byData & 0x07;
    break;
  case	0x4101:
    switch( Map110_Reg1 ) {
    case	5:
      ROMBANK0 = ROMPAGE( ((byData << 2) + 0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ((byData << 2) + 1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ((byData << 2) + 2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ((byData << 2) + 3) % ( NesHeader.byRomSize << 1 ) );
      break;

    case	0:
      Map110_Reg0 = byData & 0x01;
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	2:
      Map110_Reg0 = byData;
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	4:
      Map110_Reg0 = Map110_Reg0 | (byData<<1);
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    case	6:
      Map110_Reg0 = Map110_Reg0 | (byData<<2);
      PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }
}
//InfoNES_Mapper_110 end









//InfoNES_Mapper_112 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 112 (Pirates)                       */
/*                                                                   */
/*===================================================================*/

unsigned char  Map112_Regs[8];
unsigned int Map112_Prg0,Map112_Prg1;
unsigned int Map112_Chr01,Map112_Chr23,Map112_Chr4,Map112_Chr5,Map112_Chr6,Map112_Chr7;

unsigned char  Map112_IRQ_Enable;  /* IRQs enabled */
unsigned char  Map112_IRQ_Cnt;     /* IRQ scanline counter, decreasing */
unsigned char  Map112_IRQ_Latch;   /* IRQ scanline counter latch */

#define Map112_Chr_Swap()  ( Map112_Regs[0] & 0x80 )
#define Map112_Prg_Swap()  ( Map112_Regs[0] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 112                                            */
/*-------------------------------------------------------------------*/
void Map112_Init()
{
  /* Initialize Mapper */
  MapperInit = Map112_Init;

  /* Write to Mapper */
  MapperWrite = Map112_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map112_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* clear registers FIRST!!! */
  for(int i = 0; i < 8; i++) 
  {
    Map112_Regs[i] = 0x00;
  }

  /* set CPU bank pointers */
  Map112_Prg0 = 0;
  Map112_Prg1 = 1;
  Map112_Set_CPU_Banks();

  /* set VROM banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map112_Chr01 = 0;
    Map112_Chr23 = 2;
    Map112_Chr4  = 4;
    Map112_Chr5  = 5;
    Map112_Chr6  = 6;
    Map112_Chr7  = 7;
    Map112_Set_PPU_Banks();
  }
  else
  {
    Map112_Chr01 = Map112_Chr23 = Map112_Chr4 = Map112_Chr5 = Map112_Chr6 = Map112_Chr7 = 0;
  }

  Map112_IRQ_Enable = 0;
  Map112_IRQ_Cnt = 0;
  Map112_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map112_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  /* Set ROM Banks */
  switch( wAddr & 0xE001 )
  {
    case 0x8000:
      Map112_Regs[0] = byData;
      Map112_Set_PPU_Banks();
      Map112_Set_CPU_Banks();
      break;

    case 0xA000:
      Map112_Regs[1] = byData;
      dwBankNum = Map112_Regs[1];
      switch ( Map112_Regs[0] & 0x07 )
      {
        case 0x02:
	  if ( NesHeader.byVRomSize > 0 )
	  {
	    dwBankNum &= 0xfe;
	    Map112_Chr01 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x03:
	  if ( NesHeader.byVRomSize > 0 )
	  {
	    dwBankNum &= 0xfe;
	    Map112_Chr23 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x04:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr4 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x05:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr5 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x06:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr6 = dwBankNum;
	    Map112_Set_PPU_Banks();
          }
	  break;

        case 0x07:
	  if ( NesHeader.byVRomSize > 0 )
          {
	    Map112_Chr7 = dwBankNum;
	    Map112_Set_PPU_Banks();
	  }
	  break;

        case 0x00:
	  Map112_Prg0 = dwBankNum;
	  Map112_Set_CPU_Banks();
	  break;

        case 0x01:
	  Map112_Prg1 = dwBankNum;
	  Map112_Set_CPU_Banks();
	  break;
      }
      break;

    case 0x8001:
      Map112_Regs[2] = byData;
      if ( !ROM_FourScr )
      {
	if ( byData & 0x01 )
        {
	  InfoNES_Mirroring( 1 );   /* Vertical */
	}
	else
        {
	  InfoNES_Mirroring( 0 );   /* Horizontal */
	}
      }
      break;

    case 0xA001:
      Map112_Regs[3] = byData;
      break;

    case 0xC000:
      Map112_Regs[4] = byData;
      Map112_IRQ_Cnt = Map112_Regs[4];
      break;

    case 0xC001:
      Map112_Regs[5] = byData;
      Map112_IRQ_Latch = Map112_Regs[5];
      break;

    case 0xE000:
      Map112_Regs[6] = byData;
      Map112_IRQ_Enable = 0;

      if ( byData )
      {
	InfoNES_Mirroring( 0 );   /* Horizontal */
      }
      else
      {
	InfoNES_Mirroring( 1 );   /* Vertical */
      }
      break;

    case 0xE001:
      Map112_Regs[7] = byData;
      Map112_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map112_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map112_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map112_IRQ_Cnt-- ) )
        {
          Map112_IRQ_Cnt = Map112_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map112_Set_CPU_Banks()
{
  /* Set ROM Banks */
  if ( Map112_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map112_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map112_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
  else
  {
    ROMBANK0 = ROMPAGE( Map112_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map112_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 112 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map112_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map112_Chr_Swap() )
    {
      PPUBANK[ 0 ] = VROMPAGE( Map112_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map112_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map112_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map112_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map112_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map112_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map112_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map112_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } 
    else 
    {
      PPUBANK[ 0 ] = VROMPAGE( ( Map112_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map112_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map112_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map112_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map112_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map112_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map112_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map112_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_112 end












//InfoNES_Mapper_113 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 113 (PC-Sachen/Hacker)                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 113                                            */
/*-------------------------------------------------------------------*/
void Map113_Init()
{
  /* Initialize Mapper */
  MapperInit = Map113_Init;

  /* Write to Mapper */
  MapperWrite = Map113_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map113_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map113_Apu( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x4100:
    case 0x4111:
    case 0x4120:
    case 0x4900:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 113 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map113_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank, byChrBank;

  switch ( wAddr )
  {
    case 0x8008:
    case 0x8009:
      byPrgBank = byData >> 3;

      if ( ( NesHeader.byRomSize << 1 ) <= 8 && ( NesHeader.byVRomSize << 3 ) == 128 )
      {
        byChrBank = ( ( byData >> 3 ) & 0x08 ) + ( byData & 0x07 );
      } else {
        byChrBank = byData & 0x07;
      }

      /* Set ROM Banks */
      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
      PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
      PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
      PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
      PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
      
      InfoNES_SetupChr();
      break;
  }
}
//InfoNES_Mapper_113 end










//InfoNES_Mapper_114 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 114 (PC-SuperGames)                  */
/*                                                                   */
/*===================================================================*/

unsigned char  Map114_Regs[ 8 ];
unsigned int Map114_Prg0, Map114_Prg1;
unsigned int Map114_Chr01, Map114_Chr23;
unsigned int Map114_Chr4, Map114_Chr5, Map114_Chr6, Map114_Chr7;

unsigned char Map114_IRQ_Enable;
unsigned char Map114_IRQ_Cnt;
unsigned char Map114_IRQ_Latch;

#define Map114_Chr_Swap()    ( Map114_Regs[ 0 ] & 0x80 )
#define Map114_Prg_Swap()    ( Map114_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 114                                            */
/*-------------------------------------------------------------------*/
void Map114_Init()
{
  /* Initialize Mapper */
  MapperInit = Map114_Init;

  /* Write to Mapper */
  MapperWrite = Map114_Write;

  /* Write to SRAM */
  MapperSram = Map114_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map114_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++)
  {
    Map114_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map114_Prg0 = 0;
  Map114_Prg1 = 1;
  Map114_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map114_Chr01 = 0;
    Map114_Chr23 = 2;
    Map114_Chr4  = 4;
    Map114_Chr5  = 5;
    Map114_Chr6  = 6;
    Map114_Chr7  = 7;
    Map114_Set_PPU_Banks();
  } else {
    Map114_Chr01 = Map114_Chr23 = 0;
    Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map114_IRQ_Enable = 0;
  Map114_IRQ_Cnt = 0;
  Map114_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map114_Sram( unsigned short int wAddr, unsigned char byData )
{
  if ( ( wAddr == 0x6000 ) && ( byData == 0x00 ) )
  {
    /* Initialize State Registers */
    for ( int nPage = 0; nPage < 8; nPage++)
    {
      Map114_Regs[ nPage ] = 0x00;
    }

    /* Set ROM Banks */
    Map114_Prg0 = 0;
    Map114_Prg1 = 1;
    Map114_Set_CPU_Banks();

    /* Set PPU Banks */
    if ( NesHeader.byVRomSize > 0 )
    { 
      Map114_Chr01 = 0;
      Map114_Chr23 = 2;
      Map114_Chr4  = 4;
      Map114_Chr5  = 5;
      Map114_Chr6  = 6;
      Map114_Chr7  = 7;
      Map114_Set_PPU_Banks();
    } else {
      Map114_Chr01 = Map114_Chr23 = 0;
      Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
    }

    /* Initialize IRQ Registers */
    Map114_IRQ_Enable = 0;
    Map114_IRQ_Cnt = 0;
    Map114_IRQ_Latch = 0;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map114_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map114_Regs[ 0 ] = byData;
      Map114_Set_PPU_Banks();
      Map114_Set_CPU_Banks();
      break;

    case 0x8001:
      Map114_Regs[ 1 ] = byData;
      dwBankNum = Map114_Regs[ 1 ];

      switch ( Map114_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr01 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr23 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr4 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr5 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr6 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map114_Chr7 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map114_Prg0 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;

        case 0x07:
          Map114_Prg1 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map114_Regs[ 2 ] = byData;

      if ( !ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map114_Regs[ 3 ] = byData;

      if ( byData & 0x80 )
      {
        /* Enable Save RAM $6000-$7fff */
      } else {
        /* Disable Save RAM $6000-$7fff */
      }
      break;

    case 0xc000:
      Map114_Regs[ 4 ] = byData;
      Map114_IRQ_Cnt = Map114_Regs[ 4 ];
      break;

    case 0xc001:
      Map114_Regs[ 5 ] = byData;
      Map114_IRQ_Latch = Map114_Regs[ 5 ];
      break;

    case 0xe000:
      Map114_Regs[ 6 ] = byData;
      Map114_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map114_Regs[ 7 ] = byData;
      Map114_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map114_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map114_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map114_IRQ_Cnt-- ) )
        {
          Map114_IRQ_Cnt = Map114_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_CPU_Banks()
{
  if ( Map114_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map114_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map114_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map114_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map114_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map114_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map114_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map114_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map114_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map114_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map114_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map114_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map114_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map114_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_114 end








//InfoNES_Mapper_115 begin
/*===================================================================*/
/*                                                                   */
/*   Mapper 115 : CartSaint : Yuu Yuu Hakusho Final JusticePao(?)    */
/*                                                                   */
/*===================================================================*/

unsigned char	Map115_Reg[8];
unsigned char	Map115_Prg0, Map115_Prg1, Map115_Prg2, Map115_Prg3;
unsigned char	Map115_Prg0L, Map115_Prg1L;
unsigned char	Map115_Chr0, Map115_Chr1, Map115_Chr2, Map115_Chr3;
unsigned char    Map115_Chr4, Map115_Chr5, Map115_Chr6, Map115_Chr7;

unsigned char	Map115_IRQ_Enable;
unsigned char	Map115_IRQ_Counter;
unsigned char	Map115_IRQ_Latch;

unsigned char	Map115_ExPrgSwitch;
unsigned char	Map115_ExChrSwitch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 115                                            */
/*-------------------------------------------------------------------*/
void Map115_Init()
{
  /* Initialize Mapper */
  MapperInit = Map115_Init;

  /* Write to Mapper */
  MapperWrite = Map115_Write;

  /* Write to SRAM */
  MapperSram = Map115_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map115_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map115_Reg[i] = 0x00;
  }

  Map115_Prg0 = Map115_Prg0L = 0;
  Map115_Prg1 = Map115_Prg1L = 1;
  Map115_Prg2 = ( NesHeader.byRomSize << 1 ) - 2;
  Map115_Prg3 = ( NesHeader.byRomSize << 1 ) - 1;

  Map115_ExPrgSwitch = 0;
  Map115_ExChrSwitch = 0;

  /* Set ROM Banks */
  Map115_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    Map115_Chr0 = 0;
    Map115_Chr1 = 1;
    Map115_Chr2 = 2;
    Map115_Chr3 = 3;
    Map115_Chr4 = 4;
    Map115_Chr5 = 5;
    Map115_Chr6 = 6;
    Map115_Chr7 = 7;
    Map115_Set_PPU_Banks();
  } else {
    Map115_Chr0 = Map115_Chr2 = Map115_Chr4 = Map115_Chr5 = Map115_Chr6 = Map115_Chr7 = 0;
    Map115_Chr1 = Map115_Chr3 = 1;
  }

  Map115_IRQ_Enable = 0;	/* Disable */
  Map115_IRQ_Counter = 0;
  Map115_IRQ_Latch = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map115_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map115_Reg[0] = byData;
    Map115_Set_CPU_Banks();
    Map115_Set_PPU_Banks();
    break;
  case	0x8001:
    Map115_Reg[1] = byData;
    switch( Map115_Reg[0] & 0x07 ) {
    case	0x00:
      Map115_Chr0 = byData & 0xFE;
      Map115_Chr1 = Map115_Chr0+1;
      Map115_Set_PPU_Banks();
      break;
    case	0x01:
      Map115_Chr2 = byData & 0xFE;
      Map115_Chr3 = Map115_Chr2+1;
      Map115_Set_PPU_Banks();
      break;
    case	0x02:
      Map115_Chr4 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x03:
      Map115_Chr5 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x04:
      Map115_Chr6 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x05:
      Map115_Chr7 = byData;
      Map115_Set_PPU_Banks();
      break;
    case	0x06:
      Map115_Prg0 = Map115_Prg0L = byData;
      Map115_Set_CPU_Banks();
      break;
    case	0x07:
      Map115_Prg1 = Map115_Prg1L = byData;
      Map115_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map115_Reg[2] = byData;
    if ( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map115_Reg[3] = byData;
    break;
  case	0xC000:
    Map115_Reg[4] = byData;
    Map115_IRQ_Counter = byData;
    Map115_IRQ_Enable = 0xFF;
    break;
  case	0xC001:
    Map115_Reg[5] = byData;
    Map115_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map115_Reg[6] = byData;
    Map115_IRQ_Enable = 0;
    break;
  case	0xE001:
    Map115_Reg[7] = byData;
    Map115_IRQ_Enable = 0xFF;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map115_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr ) {
  case	0x6000:
    Map115_ExPrgSwitch = byData;
    Map115_Set_CPU_Banks();
    break;
  case	0x6001:
    Map115_ExChrSwitch = byData&0x1;
    Map115_Set_PPU_Banks();
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map115_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map115_IRQ_Enable ) {
	if( !(Map115_IRQ_Counter--) ) {
	  Map115_IRQ_Counter = Map115_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map115_Set_CPU_Banks()
{
  if( Map115_ExPrgSwitch & 0x80 ) {
    Map115_Prg0 = ((Map115_ExPrgSwitch<<1)&0x1e);
    Map115_Prg1 = Map115_Prg0+1;

    ROMBANK0 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( Map115_Prg0+2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( Map115_Prg1+2 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    Map115_Prg0 = Map115_Prg0L;
    Map115_Prg1 = Map115_Prg1L;
    if( Map115_Reg[0] & 0x40 ) {
      ROMBANK0 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
      ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
    } else {
      ROMBANK0 = ROMPAGE( Map115_Prg0 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( Map115_Prg1 % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
      ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 115 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map115_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    if( Map115_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr7) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr3) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map115_ExChrSwitch<<8)+Map115_Chr7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_115 end











//InfoNES_Mapper_116 begin
/*===================================================================*/
/*                                                                   */
/*   Mapper 116 : CartSaint : Yuu Yuu AV Kyoretsuden                 */
/*                                                                   */
/*===================================================================*/

unsigned char	Map116_Reg[8];
unsigned char	Map116_Prg0, Map116_Prg1, Map116_Prg2, Map116_Prg3;
unsigned char	Map116_Prg0L, Map116_Prg1L;
unsigned char	Map116_Chr0, Map116_Chr1, Map116_Chr2, Map116_Chr3;
unsigned char    Map116_Chr4, Map116_Chr5, Map116_Chr6, Map116_Chr7;

unsigned char	Map116_IRQ_Enable;
unsigned char	Map116_IRQ_Counter;
unsigned char	Map116_IRQ_Latch;

unsigned char	Map116_ExPrgSwitch;
unsigned char	Map116_ExChrSwitch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 116                                            */
/*-------------------------------------------------------------------*/
void Map116_Init()
{
  /* Initialize Mapper */
  MapperInit = Map116_Init;

  /* Write to Mapper */
  MapperWrite = Map116_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map116_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map116_Reg[i] = 0x00;
  }

  Map116_Prg0 = Map116_Prg0L = 0;
  Map116_Prg1 = Map116_Prg1L = 1;
  Map116_Prg2 = ( NesHeader.byRomSize << 1 ) - 2;
  Map116_Prg3 = ( NesHeader.byRomSize << 1 ) - 1;

  Map116_ExPrgSwitch = 0;
  Map116_ExChrSwitch = 0;

  /* Set ROM Banks */
  Map116_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    Map116_Chr0 = 0;
    Map116_Chr1 = 1;
    Map116_Chr2 = 2;
    Map116_Chr3 = 3;
    Map116_Chr4 = 4;
    Map116_Chr5 = 5;
    Map116_Chr6 = 6;
    Map116_Chr7 = 7;
    Map116_Set_PPU_Banks();
  } else {
    Map116_Chr0 = Map116_Chr2 = Map116_Chr4 = Map116_Chr5 = Map116_Chr6 = Map116_Chr7 = 0;
    Map116_Chr1 = Map116_Chr3 = 1;
  }

  Map116_IRQ_Enable = 0;	/* Disable */
  Map116_IRQ_Counter = 0;
  Map116_IRQ_Latch = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map116_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map116_Reg[0] = byData;
    Map116_Set_CPU_Banks();
    Map116_Set_PPU_Banks();
    break;
  case	0x8001:
    Map116_Reg[1] = byData;
    switch( Map116_Reg[0] & 0x07 ) {
    case	0x00:
      Map116_Chr0 = byData & 0xFE;
      Map116_Chr1 = Map116_Chr0+1;
      Map116_Set_PPU_Banks();
      break;
    case	0x01:
      Map116_Chr2 = byData & 0xFE;
      Map116_Chr3 = Map116_Chr2+1;
      Map116_Set_PPU_Banks();
      break;
    case	0x02:
      Map116_Chr4 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x03:
      Map116_Chr5 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x04:
      Map116_Chr6 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x05:
      Map116_Chr7 = byData;
      Map116_Set_PPU_Banks();
      break;
    case	0x06:
      Map116_Prg0 = byData;
      Map116_Set_CPU_Banks();
      break;
    case	0x07:
      Map116_Prg1 = byData;
      Map116_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map116_Reg[2] = byData;
    if ( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map116_Reg[3] = byData;
    break;
  case	0xC000:
    Map116_Reg[4] = byData;
    Map116_IRQ_Counter = byData;
    Map116_IRQ_Enable = 0xFF;
    break;
  case	0xC001:
    Map116_Reg[5] = byData;
    Map116_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map116_Reg[6] = byData;
    Map116_IRQ_Enable = 0;
    break;
  case	0xE001:
    Map116_Reg[7] = byData;
    Map116_IRQ_Enable = 0xFF;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map116_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map116_IRQ_Enable ) {
	if( !(Map116_IRQ_Counter--) ) {
	  Map116_IRQ_Counter = Map116_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map116_Set_CPU_Banks()
{
  if( Map116_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
    ROMBANK1 = ROMPAGE( Map116_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map116_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
  } else {
    ROMBANK0 = ROMPAGE( Map116_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map116_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 2 );
    ROMBANK3 = ROMPAGE( ( NesHeader.byRomSize << 1 ) - 1 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 116 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map116_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    if( Map116_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr7) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr3) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE( ((Map116_ExChrSwitch<<8)+Map116_Chr7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_116 end









//InfoNES_Mapper_117 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 117 (PC-Future)                         */
/*                                                                   */
/*===================================================================*/

unsigned char Map117_IRQ_Line;
unsigned char Map117_IRQ_Enable1;
unsigned char Map117_IRQ_Enable2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 117                                            */
/*-------------------------------------------------------------------*/
void Map117_Init()
{
  /* Initialize Mapper */
  MapperInit = Map117_Init;

  /* Write to Mapper */
  MapperWrite = Map117_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map117_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map117_IRQ_Line = 0;
  Map117_IRQ_Enable1 = 0;
  Map117_IRQ_Enable2 = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map117_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
  
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    /* Set PPU Banks */
    case 0xA000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA004:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA005:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA006:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xA007:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc002:
    case 0xc003:
      Map117_IRQ_Enable1 = Map117_IRQ_Line = byData;
      break;

    case 0xe000:
      Map117_IRQ_Enable2 = byData & 0x01;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 117 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map117_HSync()
{
  if ( Map117_IRQ_Enable1 && Map117_IRQ_Enable2 )
  {
    if ( Map117_IRQ_Line == PPU_Scanline )
    {
      Map117_IRQ_Enable1 = 0x00;
      IRQ_REQ;
    }
  }
}
//InfoNES_Mapper_117 end










//InfoNES_Mapper_118 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 118 (Others)                         */
/*                                                                   */
/*===================================================================*/

unsigned char  Map118_Regs[ 8 ];
unsigned int Map118_Prg0, Map118_Prg1;
unsigned int Map118_Chr0, Map118_Chr1, Map118_Chr2, Map118_Chr3;
unsigned int Map118_Chr4, Map118_Chr5, Map118_Chr6, Map118_Chr7;

unsigned char Map118_IRQ_Enable;
unsigned char Map118_IRQ_Cnt;
unsigned char Map118_IRQ_Latch;

#define Map118_Chr_Swap()    ( Map118_Regs[ 0 ] & 0x80 )
#define Map118_Prg_Swap()    ( Map118_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 118                                            */
/*-------------------------------------------------------------------*/
void Map118_Init()
{
  /* Initialize Mapper */
  MapperInit = Map118_Init;

  /* Write to Mapper */
  MapperWrite = Map118_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map118_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++)
  {
    Map118_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map118_Prg0 = 0;
  Map118_Prg1 = 1;
  Map118_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map118_Chr0 = 0;
    Map118_Chr1 = 1;
    Map118_Chr2 = 2;
    Map118_Chr3 = 3;
    Map118_Chr4 = 4;
    Map118_Chr5 = 5;
    Map118_Chr6 = 6;
    Map118_Chr7 = 7;
    Map118_Set_PPU_Banks();
  } else {
    Map118_Chr0 = Map118_Chr1 = Map118_Chr2 = Map118_Chr3 = 0;
    Map118_Chr4 = Map118_Chr5 = Map118_Chr6 = Map118_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map118_IRQ_Enable = 0;
  Map118_IRQ_Cnt = 0;
  Map118_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map118_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map118_Regs[ 0 ] = byData;
      Map118_Set_PPU_Banks();
      Map118_Set_CPU_Banks();
      break;

    case 0x8001:
      Map118_Regs[ 1 ] = byData;
      dwBankNum = Map118_Regs[ 1 ];

      /* Name Table Mirroring */
      if ( ( Map118_Regs[ 0 ] & 0x07 ) < 6 )
      {
        if ( byData & 0x80 )
        {
          InfoNES_Mirroring( 3 );
        } else {
          InfoNES_Mirroring( 2 );
        }
      }

      switch ( Map118_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map118_Chr0 = dwBankNum;
            Map118_Chr1 = dwBankNum + 1;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map118_Chr2 = dwBankNum;
            Map118_Chr3 = dwBankNum + 1;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr4 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr5 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr6 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map118_Chr7 = dwBankNum;
            Map118_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map118_Prg0 = dwBankNum;
          Map118_Set_CPU_Banks();
          break;

        case 0x07:
          Map118_Prg1 = dwBankNum;
          Map118_Set_CPU_Banks();
          break;
      }
      break;

    case 0xc000:
      Map118_Regs[ 4 ] = byData;
      Map118_IRQ_Cnt = Map118_Regs[ 4 ];
      break;

    case 0xc001:
      Map118_Regs[ 5 ] = byData;
      Map118_IRQ_Latch = Map118_Regs[ 5 ];
      break;

    case 0xe000:
      Map118_Regs[ 6 ] = byData;
      Map118_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map118_Regs[ 7 ] = byData;
      Map118_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map118_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map118_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map118_IRQ_Cnt-- ) )
        {
          Map118_IRQ_Cnt = Map118_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map118_Set_CPU_Banks()
{
  if ( Map118_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map118_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map118_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map118_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map118_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 118 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map118_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map118_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map118_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map118_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map118_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map118_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map118_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map118_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map118_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map118_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( Map118_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map118_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map118_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map118_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map118_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map118_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map118_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map118_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_118 end











//InfoNES_Mapper_119 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 119 : TQ-ROM                           */
/*                                                                   */
/*===================================================================*/

unsigned char	Map119_Reg[8];
unsigned char	Map119_Prg0, Map119_Prg1;
unsigned char	Map119_Chr01, Map119_Chr23, Map119_Chr4, Map119_Chr5, Map119_Chr6, Map119_Chr7;
unsigned char	Map119_WeSram;

unsigned char	Map119_IRQ_Enable;
unsigned char	Map119_IRQ_Counter;
unsigned char	Map119_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 119                                            */
/*-------------------------------------------------------------------*/
void Map119_Init()
{
  /* Initialize Mapper */
  MapperInit = Map119_Init;

  /* Write to Mapper */
  MapperWrite = Map119_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map119_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */ 
  for( int i = 0; i < 8; i++ ) {
    Map119_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map119_Prg0 = 0;
  Map119_Prg1 = 1;
  Map119_Set_CPU_Banks();

  /* Set PPU Banks */
  Map119_Chr01 = 0;
  Map119_Chr23 = 2;
  Map119_Chr4  = 4;
  Map119_Chr5  = 5;
  Map119_Chr6  = 6;
  Map119_Chr7  = 7;
  Map119_Set_PPU_Banks();

  Map119_WeSram  = 0;		// Disable
  Map119_IRQ_Enable = 0;	// Disable
  Map119_IRQ_Counter = 0;
  Map119_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map119_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map119_Reg[0] = byData;
    Map119_Set_CPU_Banks();
    Map119_Set_PPU_Banks();
    break;
  case	0x8001:
    Map119_Reg[1] = byData;
    
    switch( Map119_Reg[0] & 0x07 ) {
    case	0x00:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr01 = byData & 0xFE;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x01:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr23 = byData & 0xFE;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x02:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr4 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x03:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr5 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x04:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr6 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x05:
      if( NesHeader.byVRomSize > 0 ) {
	Map119_Chr7 = byData;
	Map119_Set_PPU_Banks();
      }
      break;
    case	0x06:
      Map119_Prg0 = byData;
      Map119_Set_CPU_Banks();
      break;
    case	0x07:
      Map119_Prg1 = byData;
      Map119_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map119_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    Map119_Reg[3] = byData;
    break;
  case	0xC000:
    Map119_Reg[4] = byData;
    Map119_IRQ_Counter = byData;
    break;
  case	0xC001:
    Map119_Reg[5] = byData;
    Map119_IRQ_Latch = byData;
    break;
  case	0xE000:
    Map119_Reg[6] = byData;
    Map119_IRQ_Enable = 0;
    Map119_IRQ_Counter = Map119_IRQ_Latch;
    break;
  case	0xE001:
    Map119_Reg[7] = byData;
    Map119_IRQ_Enable = 1;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map119_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map119_IRQ_Enable ) {
	if( !(Map119_IRQ_Counter--) ) {
	  Map119_IRQ_Counter = Map119_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map119_Set_CPU_Banks()
{
  if( Map119_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map119_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map119_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map119_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map119_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 119 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map119_Set_PPU_Banks()
{
  if( Map119_Reg[0]&0x80 ) {
    if(Map119_Chr4&0x40)	PPUBANK[ 0 ] = CRAMPAGE(Map119_Chr4&0x07);
    else			PPUBANK[ 0 ] = VROMPAGE(Map119_Chr4 % (NesHeader.byRomSize<<1));
    if(Map119_Chr5&0x40)	PPUBANK[ 1 ] = CRAMPAGE(Map119_Chr5&0x07);
    else			PPUBANK[ 1 ] = VROMPAGE(Map119_Chr5 % (NesHeader.byRomSize<<1));
    if(Map119_Chr6&0x40)	PPUBANK[ 2 ] = CRAMPAGE(Map119_Chr6&0x07);
    else			PPUBANK[ 2 ] = VROMPAGE(Map119_Chr6 % (NesHeader.byRomSize<<1));
    if(Map119_Chr7&0x40)	PPUBANK[ 3 ] = CRAMPAGE(Map119_Chr7&0x07);
    else			PPUBANK[ 3 ] = VROMPAGE(Map119_Chr7 % (NesHeader.byRomSize<<1));
    
    if((Map119_Chr01+0)&0x40)   PPUBANK[ 4 ] = CRAMPAGE((Map119_Chr01+0)&0x07);
    else		        PPUBANK[ 4 ] = VROMPAGE((Map119_Chr01+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr01+1)&0x40)   PPUBANK[ 5 ] = CRAMPAGE((Map119_Chr01+1)&0x07);
    else		     	PPUBANK[ 5 ] = VROMPAGE((Map119_Chr01+1) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+0)&0x40)   PPUBANK[ 6 ] = CRAMPAGE((Map119_Chr23+0)&0x07);
    else			PPUBANK[ 6 ] = VROMPAGE((Map119_Chr23+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+1)&0x40)   PPUBANK[ 7 ] = CRAMPAGE((Map119_Chr23+1)&0x07);
    else			PPUBANK[ 7 ] = VROMPAGE((Map119_Chr23+1) % (NesHeader.byVRomSize<<3));
  } else {
    if((Map119_Chr01+0)&0x40)   PPUBANK[ 0 ] = CRAMPAGE((Map119_Chr01+0)&0x07);
    else			PPUBANK[ 0 ] = VROMPAGE((Map119_Chr01+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr01+1)&0x40)   PPUBANK[ 1 ] = CRAMPAGE((Map119_Chr01+1)&0x07);
    else			PPUBANK[ 1 ] = VROMPAGE((Map119_Chr01+1) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+0)&0x40)   PPUBANK[ 2 ] = CRAMPAGE((Map119_Chr23+0)&0x07);
    else		     	PPUBANK[ 2 ] = VROMPAGE((Map119_Chr23+0) % (NesHeader.byVRomSize<<3));
    if((Map119_Chr23+1)&0x40)   PPUBANK[ 3 ] = CRAMPAGE((Map119_Chr23+1)&0x07);
    else		     	PPUBANK[ 3 ] = VROMPAGE((Map119_Chr23+1) % (NesHeader.byVRomSize<<3));
    
    if(Map119_Chr4&0x40)	PPUBANK[ 4 ] = CRAMPAGE(Map119_Chr4&0x07);
    else			PPUBANK[ 4 ] = VROMPAGE(Map119_Chr4 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr5&0x40)	PPUBANK[ 5 ] = CRAMPAGE(Map119_Chr5&0x07);
    else			PPUBANK[ 5 ] = VROMPAGE(Map119_Chr5 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr6&0x40)	PPUBANK[ 6 ] = CRAMPAGE(Map119_Chr6&0x07);
    else			PPUBANK[ 6 ] = VROMPAGE(Map119_Chr6 % (NesHeader.byVRomSize<<3));
    if(Map119_Chr7&0x40)	PPUBANK[ 7 ] = CRAMPAGE(Map119_Chr7&0x07);
    else			PPUBANK[ 7 ] = VROMPAGE(Map119_Chr7 % (NesHeader.byVRomSize<<3));
  }
  InfoNES_SetupChr();
}
//InfoNES_Mapper_119 end










//InfoNES_Mapper_120 begin
//InfoNES_Mapper_120 end







//InfoNES_Mapper_122 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 122 (Sunsoft)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 122                                            */
/*-------------------------------------------------------------------*/
void Map122_Init()
{
  /* Initialize Mapper */
  MapperInit = Map122_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map122_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 122 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map122_Sram( unsigned short int wAddr, unsigned char byData )
{
  if ( wAddr == 0x6000 )
  {
    unsigned char byChrBank0 = byData & 0x07;
    unsigned char byChrBank1 = ( byData & 0x70 ) >> 4;

    byChrBank0 = ( byChrBank0 << 2 ) % ( NesHeader.byVRomSize << 3 );
    byChrBank1 = ( byChrBank1 << 2 ) % ( NesHeader.byVRomSize << 3 );

    PPUBANK[ 0 ] = VROMPAGE( byChrBank0 + 0 );
    PPUBANK[ 1 ] = VROMPAGE( byChrBank0 + 1 );
    PPUBANK[ 2 ] = VROMPAGE( byChrBank0 + 2 );
    PPUBANK[ 3 ] = VROMPAGE( byChrBank0 + 3 );
    PPUBANK[ 4 ] = VROMPAGE( byChrBank1 + 0 );
    PPUBANK[ 5 ] = VROMPAGE( byChrBank1 + 1 );
    PPUBANK[ 6 ] = VROMPAGE( byChrBank1 + 2 );
    PPUBANK[ 7 ] = VROMPAGE( byChrBank1 + 3 );
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_122 end









//InfoNES_Mapper_133 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 133 : SACHEN CHEN                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 133                                            */
/*-------------------------------------------------------------------*/
void Map133_Init()
{
  /* Initialize Mapper */
  MapperInit = Map133_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map133_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 133 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map133_Apu( unsigned short int wAddr, unsigned char byData )
{
  if ( wAddr == 0x4120 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE( ((byData&0x04) + 0 ) % (NesHeader.byRomSize << 1) );
    ROMBANK1 = ROMPAGE( ((byData&0x04) + 1 ) % (NesHeader.byRomSize << 1) );
    ROMBANK2 = ROMPAGE( ((byData&0x04) + 2 ) % (NesHeader.byRomSize << 1) );
    ROMBANK3 = ROMPAGE( ((byData&0x04) + 3 ) % (NesHeader.byRomSize << 1) );
  
    /* Set PPU Banks */
    PPUBANK[ 0 ] = VROMPAGE( (((byData&0x03)<<3) + 0) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 1 ] = VROMPAGE( (((byData&0x03)<<3) + 1) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 2 ] = VROMPAGE( (((byData&0x03)<<3) + 2) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 3 ] = VROMPAGE( (((byData&0x03)<<3) + 3) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 4 ] = VROMPAGE( (((byData&0x03)<<3) + 4) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 5 ] = VROMPAGE( (((byData&0x03)<<3) + 5) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 6 ] = VROMPAGE( (((byData&0x03)<<3) + 6) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 7 ] = VROMPAGE( (((byData&0x03)<<3) + 7) % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
  }
  //Map133_Wram[ wAddr & 0x1fff ] = byData;
}
//InfoNES_Mapper_133 end












//InfoNES_Mapper_134 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 134                               */
/*                                                                   */
/*===================================================================*/

unsigned char    Map134_Cmd, Map134_Prg, Map134_Chr;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 134                                            */
/*-------------------------------------------------------------------*/
void Map134_Init()
{
  /* Initialize Mapper */
  MapperInit = Map134_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map134_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 134 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map134_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0x4101 ) {
  case	0x4100:
    Map134_Cmd = byData & 0x07;
    break;
  case	0x4101:
    switch( Map134_Cmd ) {
    case 0:	
      Map134_Prg = 0;
      Map134_Chr = 3;
      break;
    case 4:
      Map134_Chr &= 0x3;
      Map134_Chr |= (byData & 0x07) << 2;
      break;
    case 5:
      Map134_Prg = byData & 0x07;
      break;
    case 6:
      Map134_Chr &= 0x1C;
      Map134_Chr |= byData & 0x3;
      break;
    case 7:
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ((Map134_Prg<<2) + 0 ) % (NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( ((Map134_Prg<<2) + 1 ) % (NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( ((Map134_Prg<<2) + 2 ) % (NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( ((Map134_Prg<<2) + 3 ) % (NesHeader.byRomSize << 1) );
  
  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ((Map134_Chr<<3) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( ((Map134_Chr<<3) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( ((Map134_Chr<<3) + 2) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( ((Map134_Chr<<3) + 3) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( ((Map134_Chr<<3) + 4) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( ((Map134_Chr<<3) + 5) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( ((Map134_Chr<<3) + 6) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( ((Map134_Chr<<3) + 7) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();

  //Map134_Wram[ wAddr & 0x1fff ] = byData;
}
//InfoNES_Mapper_134 end










//InfoNES_Mapper_135 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 135 : SACHEN CHEN                        */
/*                                                                   */
/*===================================================================*/

unsigned char    Map135_Cmd;
unsigned char	Map135_Chr0l, Map135_Chr1l, Map135_Chr0h, Map135_Chr1h, Map135_Chrch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 135                                            */
/*-------------------------------------------------------------------*/
void Map135_Init()
{
  /* Initialize Mapper */
  MapperInit = Map135_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map135_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  Map135_Cmd = 0;
  Map135_Chr0l = Map135_Chr1l = Map135_Chr0h = Map135_Chr1h = Map135_Chrch = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map135_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map135_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0x4101 ) {
  case	0x4100:
    Map135_Cmd = byData & 0x07;
    break;
  case	0x4101:
    switch( Map135_Cmd ) {
    case	0:
      Map135_Chr0l = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	1:
      Map135_Chr0h = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	2:
      Map135_Chr1l = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	3:
      Map135_Chr1h = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	4:
      Map135_Chrch = byData & 0x07;
      Map135_Set_PPU_Banks();
      break;
    case	5:
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( (((byData%0x07)<<2) + 0 ) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (((byData%0x07)<<2) + 1 ) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData%0x07)<<2) + 2 ) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData%0x07)<<2) + 3 ) % (NesHeader.byRomSize << 1) );
      break;
    case	6:
      break;
    case	7:
      switch( (byData>>1)&0x03 ) {
      case	0: InfoNES_Mirroring( 2 ); break;
      case	1: InfoNES_Mirroring( 0  ); break;
      case	2: InfoNES_Mirroring( 1  ); break;
      case	3: InfoNES_Mirroring( 2 ); break;
      }
      break;
    }
    break;
  }
  //Map135_Wram[ wAddr & 0x1fff ] = byData;
}

/*-------------------------------------------------------------------*/
/*  Mapper 135 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map135_Set_PPU_Banks()
{
  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( (((0|(Map135_Chr0l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( (((1|(Map135_Chr0h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( (((0|(Map135_Chr1l<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( (((1|(Map135_Chr1h<<1)|(Map135_Chrch<<4))<<1) + 1) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_135 end












//InfoNES_Mapper_140 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 140                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 140                                            */
/*-------------------------------------------------------------------*/
void Map140_Init()
{
  /* Initialize Mapper */
  MapperInit = Map0_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map140_Sram;

  /* Write to APU */
  MapperApu = Map140_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 ) {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 140 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map140_Sram( unsigned short int wAddr, unsigned char byData )
{
  Map140_Apu( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 140 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map140_Apu( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( (((byData&0xF0)>>2) + 0 ) % (NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( (((byData%0xF0)>>2) + 1 ) % (NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( (((byData%0xF0)>>2) + 2 ) % (NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( (((byData%0xF0)>>2) + 3 ) % (NesHeader.byRomSize << 1) );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( (((byData&0x0F)<<3) + 0) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 1 ] = VROMPAGE( (((byData&0x0F)<<3) + 1) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 2 ] = VROMPAGE( (((byData&0x0F)<<3) + 2) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 3 ] = VROMPAGE( (((byData&0x0F)<<3) + 3) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 4 ] = VROMPAGE( (((byData&0x0F)<<3) + 4) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 5 ] = VROMPAGE( (((byData&0x0F)<<3) + 5) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 6 ] = VROMPAGE( (((byData&0x0F)<<3) + 6) % (NesHeader.byVRomSize << 3) );
  PPUBANK[ 7 ] = VROMPAGE( (((byData&0x0F)<<3) + 7) % (NesHeader.byVRomSize << 3) );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_140 end












//InfoNES_Mapper_151 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 151 (VSUnisystem)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 151                                            */
/*-------------------------------------------------------------------*/
void Map151_Init()
{
  /* Initialize Mapper */
  MapperInit = Map151_Init;

  /* Write to Mapper */
  MapperWrite = Map151_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 151 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map151_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  switch( wAddr & 0xF000 )
  {
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xA000:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xC000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xE000:
      PPUBANK[ 0 ] = VROMPAGE( ( byData*4+0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( byData*4+1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( byData*4+2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( byData*4+3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xF000:
      PPUBANK[ 4 ] = VROMPAGE( ( byData*4+0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( byData*4+1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( byData*4+2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( byData*4+3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  }
}
//InfoNES_Mapper_151 end









//InfoNES_Mapper_160 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 160 (Pirates)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map160_IRQ_Enable;
unsigned char Map160_IRQ_Cnt;
unsigned char Map160_IRQ_Latch;
unsigned char Map160_Refresh_Type;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 160                                            */
/*-------------------------------------------------------------------*/
void Map160_Init()
{
  /* Initialize Mapper */
  MapperInit = Map160_Init;

  /* Write to Mapper */
  MapperWrite = Map160_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map160_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Initialize IRQ Registers */
  Map160_IRQ_Enable = 0;
  Map160_IRQ_Cnt = 0;
  Map160_IRQ_Latch = 0;
  Map160_Refresh_Type = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map160_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8002:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9000:
      if ( byData == 0x2b )
      {
        Map160_Refresh_Type = 1;      /* Title */
      }
      else if ( byData == 0xa8 )
      {
        Map160_Refresh_Type = 2;      /* Passwd */
      }
      else if ( byData == 0x1f )
      {
        Map160_Refresh_Type = 3;      /* Game Over */
      }
      else if ( byData == 0x7c )
      {
        Map160_Refresh_Type = 4;      /* Continue */
      }
      else if ( byData == 0x18 )
      {
        Map160_Refresh_Type = 5;      /* Roulette */
      }
      else if ( byData == 0x60 )
      {
        Map160_Refresh_Type = 6;      /* Congratulation */
      }     
      else
      {
        Map160_Refresh_Type = 0;   
      }

      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9002:
      if ( Map160_Refresh_Type == 2 && byData != 0xe8 )
      {
        Map160_Refresh_Type = 0;     /* Not Passwd */
      }
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9004:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  
    case 0x9005:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9006:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x9007:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;      

    case 0xc000:
      Map160_IRQ_Cnt = Map160_IRQ_Latch;
      Map160_IRQ_Enable = Map160_IRQ_Latch;
      break;

    case 0xc001:
      Map160_IRQ_Latch = byData;
      break;

    case 0xc002:
      Map160_IRQ_Enable = 0;
      break;

    case 0xc003:
      Map160_IRQ_Cnt = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 160 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map160_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( PPU_Scanline == 0 || PPU_Scanline == 239 )
  {
    if ( Map160_Refresh_Type == 1 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x58 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x59 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x5a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x5b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 2 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x78 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x79 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 3 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x70 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x71 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x72 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x73 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 6 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x5c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x5d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 64 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x6c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x6d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x6e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x6f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 128 )
  {
    if ( Map160_Refresh_Type == 4 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x68 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x69 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x6a % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x6b % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else if ( Map160_Refresh_Type == 5 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x74 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x75 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x76 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x77 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( PPU_Scanline == 160 )
  {
    if ( Map160_Refresh_Type == 6 )
    {
      PPUBANK[ 0 ] = VROMPAGE( 0x60 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 0x61 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 0x5e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 0x5f % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 0x7c % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 0x7d % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 0x7e % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 0x7f % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if ( Map160_IRQ_Enable )
  {
    if ( Map160_IRQ_Cnt == 0xff )
    {
      IRQ_REQ;
      Map160_IRQ_Cnt = 0;
      Map160_IRQ_Enable = 0;
    } else {
      Map160_IRQ_Cnt++;
    }
  }
}
//InfoNES_Mapper_160 end
