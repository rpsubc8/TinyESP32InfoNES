#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"




//InfoNES_Mapper_058 begin
/*===================================================================*/
/*                                                                   */
/*                           Mapper 58                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 58                                             */
/*-------------------------------------------------------------------*/
void Map58_Init()
{
  /* Initialize Mapper */
  MapperInit = Map58_Init;

  /* Write to Mapper */
  MapperWrite = Map58_Write;

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
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

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
/*  Mapper 58 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map58_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr & 0x40 ) {
    ROMBANK0 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE((((wAddr&0x06)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x06)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x06)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x06)<<1)+3) % (NesHeader.byRomSize<<1));
  }

  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE(((wAddr&0x38)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((wAddr&0x38)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((wAddr&0x38)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((wAddr&0x38)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((wAddr&0x38)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((wAddr&0x38)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((wAddr&0x38)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((wAddr&0x38)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }

  if( byData & 0x02 ) InfoNES_Mirroring( 1 );
  else		      InfoNES_Mirroring( 0 );
}
//InfoNES_Mapper_058 end















//InfoNES_Mapper_060 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 60                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 60                                             */
/*-------------------------------------------------------------------*/
void Map60_Init()
{
  /* Initialize Mapper */
  MapperInit = Map60_Init;

  /* Write to Mapper */
  MapperWrite = Map60_Write;

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
/*  Mapper 60 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map60_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr & 0x80 ) {
    ROMBANK0 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE((((wAddr&0x70)>>3)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0x70)>>3)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0x70)>>3)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0x70)>>3)+3) % (NesHeader.byRomSize<<1));
  }
  

  PPUBANK[ 0 ] = VROMPAGE((((wAddr&0x07)<<3)+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE((((wAddr&0x07)<<3)+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE((((wAddr&0x07)<<3)+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE((((wAddr&0x07)<<3)+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE((((wAddr&0x07)<<3)+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE((((wAddr&0x07)<<3)+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE((((wAddr&0x07)<<3)+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE((((wAddr&0x07)<<3)+7) % (NesHeader.byVRomSize<<3));
  InfoNES_SetupChr();
  
  if( byData & 0x08 ) InfoNES_Mirroring( 0 );
  else		      InfoNES_Mirroring( 1 );
}
//InfoNES_Mapper_060 end













//InfoNES_Mapper_061 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 61                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 61                                             */
/*-------------------------------------------------------------------*/
void Map61_Init()
{
  /* Initialize Mapper */
  MapperInit = Map61_Init;

  /* Write to Mapper */
  MapperWrite = Map61_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 61 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map61_Write( unsigned short int wAddr, unsigned char byData )
{
	unsigned char byBank;

	switch( wAddr & 0x30 ) {
		case	0x00:
		case	0x30:
			ROMBANK0 = ROMPAGE((((wAddr&0x0F)<<2)+0) % (NesHeader.byRomSize<<1));
			ROMBANK1 = ROMPAGE((((wAddr&0x0F)<<2)+1) % (NesHeader.byRomSize<<1));
			ROMBANK2 = ROMPAGE((((wAddr&0x0F)<<2)+2) % (NesHeader.byRomSize<<1));
			ROMBANK3 = ROMPAGE((((wAddr&0x0F)<<2)+3) % (NesHeader.byRomSize<<1));
			break;
		case	0x10:
		case	0x20:
			byBank = ((wAddr & 0x0F)<<1)|((wAddr&0x20)>>4);

			ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
			ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
			break;
	}

	if( wAddr & 0x80 ) InfoNES_Mirroring( 0 );
	else		   InfoNES_Mirroring( 1 );
}
//InfoNES_Mapper_061 end













//InfoNES_Mapper_062 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 62                              */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 62                                             */
/*-------------------------------------------------------------------*/
void Map62_Init()
{
  /* Initialize Mapper */
  MapperInit = Map62_Init;

  /* Write to Mapper */
  MapperWrite = Map62_Write;

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
/*  Mapper 62 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map62_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xFF00 ) {
  case	0x8100:
    ROMBANK0 = ROMPAGE((byData+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((byData+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x8500:
    ROMBANK0 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
    break;
  case	0x8700:
    ROMBANK1 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
    break;

  default:
    PPUBANK[ 0 ] = VROMPAGE((byData+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((byData+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((byData+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((byData+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((byData+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((byData+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE((byData+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((byData+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  }
}
//InfoNES_Mapper_062 end













//InfoNES_Mapper_064 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 64 (Tengen RAMBO-1)                     */
/*                                                                   */
/*===================================================================*/

unsigned char Map64_Cmd;
unsigned char Map64_Prg;
unsigned char Map64_Chr;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 64                                             */
/*-------------------------------------------------------------------*/
void Map64_Init()
{
  /* Initialize Mapper */
  MapperInit = Map64_Init;

  /* Write to Mapper */
  MapperWrite = Map64_Write;

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
  ROMBANK0 = ROMLASTPAGE( 0 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMLASTPAGE( 0 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize state flag */
  Map64_Cmd = 0x00;
  Map64_Prg = 0x00;
  Map64_Chr = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 64 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map64_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set state flag */
      Map64_Cmd = byData & 0x0f;
      Map64_Prg = ( byData & 0x40 ) >> 6;
      Map64_Chr = ( byData & 0x80 ) >> 7;
      break;

    case 0x8001:
      switch ( Map64_Cmd )
      {
        case 0x00:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 4 ] = VROMPAGE( byData );
            PPUBANK[ 5 ] = VROMPAGE( byData + 1 );      
          } else {
            PPUBANK[ 0 ] = VROMPAGE( byData );
            PPUBANK[ 1 ] = VROMPAGE( byData + 1 );  
          } 
          InfoNES_SetupChr();
          break;

        case 0x01:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 6 ] = VROMPAGE( byData );
            PPUBANK[ 7 ] = VROMPAGE( byData + 1 );      
          } else {
            PPUBANK[ 2 ] = VROMPAGE( byData );
            PPUBANK[ 3 ] = VROMPAGE( byData + 1 );  
          } 
          InfoNES_SetupChr();
          break;

        case 0x02:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 0 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 4 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x03:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 1 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 5 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x04:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 2 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 6 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x05:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          if ( Map64_Chr )
          {
            PPUBANK[ 3 ] = VROMPAGE( byData );
          } else {
            PPUBANK[ 7 ] = VROMPAGE( byData );
          } 
          InfoNES_SetupChr();
          break;

        case 0x06:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK1 = ROMPAGE( byData );
          } else {
            ROMBANK0 = ROMPAGE( byData );
          } 
          break;

        case 0x07:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK2 = ROMPAGE( byData );
          } else {
            ROMBANK1 = ROMPAGE( byData );
          } 
          break;

        case 0x08:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 1 ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        case 0x09:
          /* Set PPU Banks */
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 3 ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        case 0x0f:
          /* Set ROM Banks */
          byData %= ( NesHeader.byRomSize << 1 );
          if ( Map64_Prg )
          {
            ROMBANK0 = ROMPAGE( byData );
          } else {
            ROMBANK2 = ROMPAGE( byData );
          } 
          break;
      }
      break;

    default:
      switch ( wAddr & 0xf000 )
      {
        case 0xa000:
          /* Name Table Mirroring */
          InfoNES_Mirroring( byData & 0x01 );
          break;

        default:
          break;
      }
      break;
  }
}
//InfoNES_Mapper_064 end















//InfoNES_Mapper_065 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 65 (Irem H3001)                       */
/*                                                                   */
/*===================================================================*/

unsigned char  Map65_IRQ_Enable;
unsigned int Map65_IRQ_Cnt;
unsigned int Map65_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 65                                             */
/*-------------------------------------------------------------------*/
void Map65_Init()
{
  /* Initialize Mapper */
  MapperInit = Map65_Init;

  /* Write to Mapper */
  MapperWrite = Map65_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map65_HSync;

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
/*  Mapper 65 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map65_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0x9000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }
      break;

    case 0x9003:
      Map65_IRQ_Enable = byData & 0x80;
      break;

    case 0x9004:
      Map65_IRQ_Cnt = Map65_IRQ_Latch;
      break;

    case 0x9005:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0x00ff ) | ((unsigned int)byData << 8 );
      break;

    case 0x9006:
      Map65_IRQ_Latch = ( Map65_IRQ_Latch & 0xff00 ) | (unsigned int)byData;
      break;

    /* Set PPU Banks */
    case 0xb000:
      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb004:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb005:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb006:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb007:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    /* Set ROM Banks */
    case 0xa000:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;

    case 0xc000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 65 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map65_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map65_IRQ_Enable )
  {
    if ( Map65_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map65_IRQ_Enable = 0;
      Map65_IRQ_Cnt = 0xffff;
    } else {
      Map65_IRQ_Cnt -= 113;
    }
  }
}
//InfoNES_Mapper_065 end















//InfoNES_Mapper_066 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 66 (GNROM)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 66                                             */
/*-------------------------------------------------------------------*/
void Map66_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map66_Init;

  /* Write to Mapper */
  MapperWrite = Map66_Write;

  /* Write to SRAM */
  MapperSram = Map66_Write;

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

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 0 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 66 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map66_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byRom;
  unsigned char byVRom;

  byRom  = ( byData >> 4 ) & 0x0F;
  byVRom = byData & 0x0F;

  /* Set ROM Banks */
  byRom <<= 1;
  byRom %= NesHeader.byRomSize;
  byRom <<= 1;

  ROMBANK0 = ROMPAGE( byRom );
  ROMBANK1 = ROMPAGE( byRom + 1 );
  ROMBANK2 = ROMPAGE( byRom + 2 );
  ROMBANK3 = ROMPAGE( byRom + 3 );

  /* Set PPU Banks */
  byVRom <<= 3;
  byVRom %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byVRom );
  PPUBANK[ 1 ] = VROMPAGE( byVRom + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byVRom + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byVRom + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byVRom + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byVRom + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byVRom + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byVRom + 7 );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_066 end















//InfoNES_Mapper_067 begin
/*===================================================================*/
/*                                                                   */
/*               Mapper 67 (Sunsoft Mapper #3)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map67_IRQ_Enable;
unsigned char Map67_IRQ_Cnt;
unsigned char Map67_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 67                                             */
/*-------------------------------------------------------------------*/
void Map67_Init()
{
  /* Initialize Mapper */
  MapperInit = Map67_Init;

  /* Write to Mapper */
  MapperWrite = Map67_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map67_HSync;

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
  PPUBANK[ 0 ] = VROMPAGE( 0 );
  PPUBANK[ 1 ] = VROMPAGE( 1 );
  PPUBANK[ 2 ] = VROMPAGE( 2 );
  PPUBANK[ 3 ] = VROMPAGE( 3 );
  PPUBANK[ 4 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 4 );
  PPUBANK[ 5 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 3 );
  PPUBANK[ 6 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 2 );
  PPUBANK[ 7 ] = VROMPAGE( ( NesHeader.byVRomSize << 3 ) - 1 );
  InfoNES_SetupChr();

  /* Initialize IRQ Registers */
  Map67_IRQ_Enable = 0;
  Map67_IRQ_Cnt = 0;
  Map67_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map67_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf800 )
  {
    /* Set PPU Banks */
    case 0x8800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0x9800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 2 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xa800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 4 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xb800:
      byData <<= 1;
      byData %= ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 6 ] = VROMPAGE( byData + 0 );
      PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0xc800:
      Map67_IRQ_Cnt = Map67_IRQ_Latch;
      Map67_IRQ_Latch = byData;
      break;

    case 0xd800:
      Map67_IRQ_Enable = byData & 0x10;
      break;

    case 0xe800:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 1 );
          break;
        case 1:
          InfoNES_Mirroring( 0 );
          break;
        case 2:
          InfoNES_Mirroring( 3 );
          break;
        case 3:
          InfoNES_Mirroring( 2 );
          break;
      }
      break;

    /* Set ROM Banks */
    case 0xf800:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byData + 0 );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 67 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map67_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map67_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( --Map67_IRQ_Cnt == 0xf6 )
        {
          Map67_IRQ_Cnt = Map67_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}
//InfoNES_Mapper_067 end















//InfoNES_Mapper_068 begin
/*===================================================================*/
/*                                                                   */
/*                Mapper 68 (Sunsoft Mapper #4)                      */
/*                                                                   */
/*===================================================================*/

unsigned char Map68_Regs[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 68                                              */
/*-------------------------------------------------------------------*/
void Map68_Init()
{
  /* Initialize Mapper */
  MapperInit = Map68_Init;

  /* Write to Mapper */
  MapperWrite = Map68_Write;

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

  /* Initialize state flag */
  for ( int i = 0; i < 4; i++ )
    Map68_Regs[ i ] = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 68 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map68_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr )
  {
    case 0x8000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0x9000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 2 ] = VROMPAGE( byData );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xA000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 4 ] = VROMPAGE( byData );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xB000:  
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 2 );
      byData <<= 1;
      PPUBANK[ 6 ] = VROMPAGE( byData );
      PPUBANK[ 7 ] = VROMPAGE( byData + 1);
      InfoNES_SetupChr();
      break;

    case 0xC000:  
      Map68_Regs[ 2 ] = byData;
      Map68_SyncMirror();
      break;

    case 0xD000:  
      Map68_Regs[ 3 ] = byData;
      Map68_SyncMirror();
      break;

    case 0xE000:  
      Map68_Regs[ 0 ] = ( byData & 0x10 ) >> 4;
      Map68_Regs[ 1 ] = byData & 0x03;
      Map68_SyncMirror();
      break;

    case 0xF000:  
      /* Set ROM Banks */
      byData %= NesHeader.byRomSize;
      byData <<= 1;
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 68 Sub Function                                           */
/*-------------------------------------------------------------------*/
void Map68_SyncMirror( void )
{
  if ( Map68_Regs[ 0 ] )
  {
    switch( Map68_Regs[ 1 ] )
    {
      case 0x00:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;

      case 0x01:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;

      case 0x02:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 2 ] + 0x80 );
        break;

      case 0x03:
        PPUBANK[  8 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[  9 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 10 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        PPUBANK[ 11 ] = VROMPAGE( Map68_Regs[ 3 ] + 0x80 );
        break;
    }
    InfoNES_SetupChr();
  } 
  else 
  {
    InfoNES_Mirroring( Map68_Regs[ 1 ] );
  }
}
//InfoNES_Mapper_068 end














//InfoNES_Mapper_069 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 69 (Sunsoft FME-7)                        */
/*                                                                   */
/*===================================================================*/

unsigned char  Map69_IRQ_Enable;
unsigned int Map69_IRQ_Cnt;
unsigned char  Map69_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 69                                             */
/*-------------------------------------------------------------------*/
void Map69_Init()
{
  /* Initialize Mapper */
  MapperInit = Map69_Init;

  /* Write to Mapper */
  MapperWrite = Map69_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map69_HSync;

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

  /* Initialize IRQ Reg */
  Map69_IRQ_Enable = 0;
  Map69_IRQ_Cnt    = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 69 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map69_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map69_Regs[ 0 ] = byData & 0x0f;
      break;

    case 0xA000:
      switch ( Map69_Regs[ 0 ] )
      {
        /* Set PPU Banks */
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ Map69_Regs[ 0 ] ] = VROMPAGE( byData );
          InfoNES_SetupChr();
          break;

        /* Set ROM Banks */
#if 0
        case 0x08:
          if ( !( byData & 0x40 ) )
          {
            byData %= ( NesHeader.byRomSize << 1 );
            SRAMBANK = ROMPAGE( byData );
          }
          break;
#endif

        case 0x09:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK0 = ROMPAGE( byData );
          break;

        case 0x0a:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK1 = ROMPAGE( byData );
          break;

        case 0x0b:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK2 = ROMPAGE( byData );
          break;

        /* Name Table Mirroring */
        case 0x0c:  
          switch ( byData & 0x03 )
          {
            case 0:
              InfoNES_Mirroring( 1 );   /* Vertical */
              break;
            case 1:
              InfoNES_Mirroring( 0 );   /* Horizontal */
              break;
            case 2:
              InfoNES_Mirroring( 3 );   /* One Screen 0x2400 */
              break;
            case 3:
              InfoNES_Mirroring( 2 );   /* One Screen 0x2000 */
              break;
          }
          break;

        case 0x0d:
          Map69_IRQ_Enable = byData;
          break;

        case 0x0e:
          Map69_IRQ_Cnt = ( Map69_IRQ_Cnt & 0xff00) | (unsigned int)byData;
          break;

        case 0x0f:
          Map69_IRQ_Cnt = ( Map69_IRQ_Cnt & 0x00ff) | ( (unsigned int)byData << 8 );
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 69 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map69_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map69_IRQ_Enable )
  {
    if ( Map69_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map69_IRQ_Cnt = 0;
    } else {
      Map69_IRQ_Cnt -= 113;
    }
  }
}
//InfoNES_Mapper_069 end













//InfoNES_Mapper_070 begin
/*===================================================================*/
/*                                                                   */
/*                 Mapper 70 (74161/32 Bandai)                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 70                                             */
/*-------------------------------------------------------------------*/
void Map70_Init()
{
  /* Initialize Mapper */
  MapperInit = Map70_Init;

  /* Write to Mapper */
  MapperWrite = Map70_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 70 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map70_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byChrBank = byData & 0x0f;
  unsigned char byPrgBank = ( byData & 0x70 ) >> 4;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr();

  /* Name Table Mirroring */
  if ( byData & 0x80 )
  {
    InfoNES_Mirroring( 2 );
  } else {
    InfoNES_Mirroring( 3 );
  }
}
//InfoNES_Mapper_070 end









//InfoNES_Mapper_071 begin
/*===================================================================*/
/*                                                                   */
/*            Mapper 71 (Camerica Custom Mapper)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 71                                             */
/*-------------------------------------------------------------------*/
void Map71_Init()
{
  /* Initialize Mapper */
  MapperInit = Map71_Init;

  /* Write to Mapper */
  MapperWrite = Map71_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 71 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map71_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf000 )
  {
    case 0x9000:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring( 2 );
      } else {
        InfoNES_Mirroring( 3 );
      }
      break;

    /* Set ROM Banks */
    case 0xc000:
    case 0xd000:
    case 0xe000:
    case 0xf000:
      ROMBANK0 = ROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      break;
  }
}
//InfoNES_Mapper_071 end












//InfoNES_Mapper_072 begin
/*===================================================================*/
/*                                                                   */
/*           Mapper 72 (Jaleco Early Mapper #0)                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 72                                             */
/*-------------------------------------------------------------------*/
void Map72_Init()
{
  /* Initialize Mapper */
  MapperInit = Map72_Init;

  /* Write to Mapper */
  MapperWrite = Map72_Write;

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
/*  Mapper 72 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map72_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byBank = byData & 0x0f;

  if ( byData & 0x80 )
  {
    /* Set ROM Banks */
    byBank <<= 1;
    byBank %= ( NesHeader.byRomSize << 1 );
    ROMBANK0 = ROMPAGE( byBank );
    ROMBANK1 = ROMPAGE( byBank + 1 );
  } else 
  if ( byData & 0x40 )
  {
    /* Set PPU Banks */
    byBank <<= 3;
    byBank %= ( NesHeader.byVRomSize << 3 );
    PPUBANK[ 0 ] = VROMPAGE( byBank );
    PPUBANK[ 1 ] = VROMPAGE( byBank + 1 );
    PPUBANK[ 2 ] = VROMPAGE( byBank + 2 );
    PPUBANK[ 3 ] = VROMPAGE( byBank + 3 );
    PPUBANK[ 4 ] = VROMPAGE( byBank + 4 );
    PPUBANK[ 5 ] = VROMPAGE( byBank + 5 );
    PPUBANK[ 6 ] = VROMPAGE( byBank + 6 );
    PPUBANK[ 7 ] = VROMPAGE( byBank + 7 );
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_072 end















//InfoNES_Mapper_073 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 73 (Konami VRC 3)                      */
/*                                                                   */
/*===================================================================*/

unsigned char  Map73_IRQ_Enable;
unsigned int Map73_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 73                                             */
/*-------------------------------------------------------------------*/
void Map73_Init()
{
  /* Initialize Mapper */
  MapperInit = Map73_Init;

  /* Write to Mapper */
  MapperWrite = Map73_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map73_HSync;

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
  Map73_IRQ_Enable = 0;
  Map73_IRQ_Cnt = 0;  

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 73 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map73_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xfff0 ) | ( byData & 0x0f );
      break;

    case 0x9000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xff0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xa000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0xf0ff ) | ( ( byData & 0x0f ) << 8 );
      break;

    case 0xb000:
      Map73_IRQ_Cnt = ( Map73_IRQ_Cnt & 0x0fff ) | ( ( byData & 0x0f ) << 12 );
      break;

    case 0xc000:
      Map73_IRQ_Enable = byData;
      break;

    /* Set ROM Banks */
    case 0xf000:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 73 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map73_HSync()
{
/*
 *  Callback at HSync
 *
 */
#if 1
  if ( Map73_IRQ_Enable & 0x02 )
  {
    if ( ( Map73_IRQ_Cnt += STEP_PER_SCANLINE ) > 0xffff )
    {
      Map73_IRQ_Cnt &= 0xffff;
      IRQ_REQ;
      Map73_IRQ_Enable = 0;
    }
  }
#else
  if ( Map73_IRQ_Enable )
  {
    if ( Map73_IRQ_Cnt > 0xffff - 114 )
    {
      IRQ_REQ;
      Map73_IRQ_Enable = 0;
    } else {
      Map73_IRQ_Cnt += 114;
    }
  }
#endif
}
//InfoNES_Mapper_073 end



















//InfoNES_Mapper_074 begin
/*===================================================================*/
/*                                                                   */
/*         Mapper 74 : Metal Max (Zhong Zhuang Ji Bing               */
/*                                                                   */
/*===================================================================*/

unsigned char  Map74_Regs[ 8 ];
unsigned int Map74_Rom_Bank;
unsigned int Map74_Prg0, Map74_Prg1;
unsigned int Map74_Chr01, Map74_Chr23;
unsigned int Map74_Chr4, Map74_Chr5, Map74_Chr6, Map74_Chr7;

#define Map74_Chr_Swap()    ( Map74_Regs[ 0 ] & 0x80 )
#define Map74_Prg_Swap()    ( Map74_Regs[ 0 ] & 0x40 )

unsigned char Map74_IRQ_Enable;
unsigned char Map74_IRQ_Cnt;
unsigned char Map74_IRQ_Latch;
unsigned char Map74_IRQ_Request;
unsigned char Map74_IRQ_Present;
unsigned char Map74_IRQ_Present_Vbl;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 74                                             */
/*-------------------------------------------------------------------*/
void Map74_Init()
{
  /* Initialize Mapper */
  MapperInit = Map74_Init;

  /* Write to Mapper */
  MapperWrite = Map74_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map74_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map74_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map74_Prg0 = 0;
  Map74_Prg1 = 1;
  Map74_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map74_Chr01 = 0;
    Map74_Chr23 = 2;
    Map74_Chr4  = 4;
    Map74_Chr5  = 5;
    Map74_Chr6  = 6;
    Map74_Chr7  = 7;
    Map74_Set_PPU_Banks();
  } else {
    Map74_Chr01 = Map74_Chr23 = 0;
    Map74_Chr4 = Map74_Chr5 = Map74_Chr6 = Map74_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map74_IRQ_Enable = 0;
  Map74_IRQ_Cnt = 0;
  Map74_IRQ_Latch = 0;
  Map74_IRQ_Request = 0;
  Map74_IRQ_Present = 0;
  Map74_IRQ_Present_Vbl = 0;

  /* VRAM Write Enabled */
  byVramWriteEnable = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map74_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map74_Regs[ 0 ] = byData;
      Map74_Set_PPU_Banks();
      Map74_Set_CPU_Banks();
      break;

    case 0x8001:
      Map74_Regs[ 1 ] = byData;
      dwBankNum = Map74_Regs[ 1 ];

      switch ( Map74_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map74_Chr01 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map74_Chr23 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr4 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr5 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr6 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map74_Chr7 = dwBankNum;
            Map74_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map74_Prg0 = dwBankNum;
          Map74_Set_CPU_Banks();
          break;

        case 0x07:
          Map74_Prg1 = dwBankNum;
          Map74_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map74_Regs[ 2 ] = byData;

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
      Map74_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map74_Regs[ 4 ] = byData;
      Map74_IRQ_Latch = byData;
      break;

    case 0xc001:
      Map74_Regs[ 5 ] = byData;
      if ( PPU_Scanline < 240 )
      {
          Map74_IRQ_Cnt |= 0x80;
          Map74_IRQ_Present = 0xff;
      } else {
          Map74_IRQ_Cnt |= 0x80;
          Map74_IRQ_Present_Vbl = 0xff;
          Map74_IRQ_Present = 0;
      }
      break;

    case 0xe000:
      Map74_Regs[ 6 ] = byData;
      Map74_IRQ_Enable = 0;
			Map74_IRQ_Request = 0;
      break;

    case 0xe001:
      Map74_Regs[ 7 ] = byData;
      Map74_IRQ_Enable = 1;
			Map74_IRQ_Request = 0;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map74_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 ) && 
       ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) )
  {
		if( Map74_IRQ_Present_Vbl ) {
			Map74_IRQ_Cnt = Map74_IRQ_Latch;
			Map74_IRQ_Present_Vbl = 0;
		}
		if( Map74_IRQ_Present ) {
			Map74_IRQ_Cnt = Map74_IRQ_Latch;
			Map74_IRQ_Present = 0;
		} else if( Map74_IRQ_Cnt > 0 ) {
			Map74_IRQ_Cnt--;
		}

		if( Map74_IRQ_Cnt == 0 ) {
			if( Map74_IRQ_Enable ) {
				Map74_IRQ_Request = 0xFF;
			}
			Map74_IRQ_Present = 0xFF;
		}
	}
	if( Map74_IRQ_Request  ) {
		IRQ_REQ;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map74_Set_CPU_Banks()
{
  if ( Map74_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map74_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map74_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map74_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map74_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 74 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map74_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map74_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map74_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map74_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map74_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map74_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map74_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map74_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map74_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map74_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map74_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map74_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map74_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map74_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map74_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map74_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map74_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map74_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
  else
  {
    if ( Map74_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
      InfoNES_SetupChr();
    }
  }    
}
//InfoNES_Mapper_074 end














//InfoNES_Mapper_075 begin
/*===================================================================*/
/*                                                                   */
/*           Mapper 75 (Konami VRC 1 and Jaleco SS8805)              */
/*                                                                   */
/*===================================================================*/

unsigned char Map75_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 75                                             */
/*-------------------------------------------------------------------*/
void Map75_Init()
{
  /* Initialize Mapper */
  MapperInit = Map75_Init;

  /* Write to Mapper */
  MapperWrite = Map75_Write;

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

  /* Initialize State Flag */
  Map75_Regs[ 0 ] = 0;
  Map75_Regs[ 1 ] = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 75 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map75_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf000 )
  {
    /* Set ROM Banks */
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x9000:
      /* Set Mirroring */
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }

      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x02 ) << 3 );
      PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );

      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x04 ) << 2 );
      PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;

    /* Set ROM Banks */
    case 0xA000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    /* Set ROM Banks */
    case 0xC000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0xE000:
      /* Set PPU Banks */
      Map75_Regs[ 0 ] = ( Map75_Regs[ 0 ] & 0x10 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 0 );
      PPUBANK[ 1 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 1 );
      PPUBANK[ 2 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 2 );
      PPUBANK[ 3 ] = VROMPAGE( ( Map75_Regs[ 0 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;

    case 0xF000:
      /* Set PPU Banks */
      Map75_Regs[ 1 ] = ( Map75_Regs[ 1 ] & 0x10 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 0 );
      PPUBANK[ 5 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 1 );
      PPUBANK[ 6 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 2 );
      PPUBANK[ 7 ] = VROMPAGE( ( Map75_Regs[ 1 ] << 2 ) + 3 );
      InfoNES_SetupChr();
      break;
  }
}
//InfoNES_Mapper_075 end












//InfoNES_Mapper_076 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 76 (Namcot 109)                         */
/*                                                                   */
/*===================================================================*/

unsigned char Map76_Reg;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 76                                             */
/*-------------------------------------------------------------------*/
void Map76_Init()
{
  /* Initialize Mapper */
  MapperInit = Map76_Init;

  /* Write to Mapper */
  MapperWrite = Map76_Write;

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
/*  Mapper 76 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map76_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map76_Reg = byData;
      break;

    case 0x8001:
      switch ( Map76_Reg & 0x07 )
      {
        case 0x02:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 0 ] = VROMPAGE( byData );
          PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x03:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 2 ] = VROMPAGE( byData );
          PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x04:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 4 ] = VROMPAGE( byData );
          PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x05:
          byData <<= 1;
          byData %= ( NesHeader.byVRomSize << 3 );
          PPUBANK[ 6 ] = VROMPAGE( byData );
          PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
          InfoNES_SetupChr();
          break;

        case 0x06:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK0 = ROMPAGE( byData );
          break;

        case 0x07:
          byData %= ( NesHeader.byRomSize << 1 );
          ROMBANK1 = ROMPAGE( byData );
          break;
      }
      break;
  }  
}
//InfoNES_Mapper_076 end










//InfoNES_Mapper_077 begin
/*===================================================================*/
/*                                                                   */
/*                Mapper 77  (Irem Early Mapper #0)                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 77                                             */
/*-------------------------------------------------------------------*/
void Map77_Init()
{
  /* Initialize Mapper */
  MapperInit = Map77_Init;

  /* Write to Mapper */
  MapperWrite = Map77_Write;

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
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* VRAM Write Enabled */
  byVramWriteEnable = 1;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 77 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map77_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byRomBank = byData & 0x07;
  unsigned char byChrBank = ( byData & 0xf0 ) >> 4;

  /* Set ROM Banks */
  byRomBank <<= 2;
  byRomBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byRomBank );
  ROMBANK1 = ROMPAGE( byRomBank + 1 );
  ROMBANK2 = ROMPAGE( byRomBank + 2 );
  ROMBANK3 = ROMPAGE( byRomBank + 3 );

  /* Set PPU Banks */
  byChrBank <<= 1;
  byChrBank %= ( NesHeader.byVRomSize << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_077 end













//InfoNES_Mapper_078 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 78 (74161/32 Irem)                    */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 78                                             */
/*-------------------------------------------------------------------*/
void Map78_Init()
{
  /* Initialize Mapper */
  MapperInit = Map78_Init;

  /* Write to Mapper */
  MapperWrite = Map78_Write;

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
/*  Mapper 78 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map78_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = byData & 0x0f;
  unsigned char byChrBank = ( byData & 0xf0 ) >> 4;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );
  ROMBANK0 = ROMPAGE( byPrgBank );
  ROMBANK1 = ROMPAGE( byPrgBank + 1);

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize << 3 );
  PPUBANK[ 0 ] = VROMPAGE( byChrBank );  
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 ); 
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 ); 
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 ); 
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 ); 
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 ); 
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 ); 
  InfoNES_SetupChr();  

  /* Set Name Table Mirroring */
  if ( ( wAddr & 0xfe00 ) != 0xfe00 )
  {
    if ( byData & 0x08 )
    {
      InfoNES_Mirroring( 2 );
    } else {
      InfoNES_Mirroring( 3 );
    }
  }
}
//InfoNES_Mapper_078 end













//InfoNES_Mapper_079 begin
/*===================================================================*/
/*                                                                   */
/*   Mapper 79 (American Video Entertainment/Sachen Custom Mapper)   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 79                                             */
/*-------------------------------------------------------------------*/
void Map79_Init()
{
  /* Initialize Mapper */
  MapperInit = Map79_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map79_Apu;

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
/*  Mapper 79 Write to Apu Function                                  */
/*-------------------------------------------------------------------*/
void Map79_Apu( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = ( byData & 0x08 ) >> 3;
  unsigned char byChrBank = byData & 0x07;
 
  /* Set ROM Banks */
  byPrgBank <<= 2;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank + 0 );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );
  ROMBANK2 = ROMPAGE( byPrgBank + 2 );
  ROMBANK3 = ROMPAGE( byPrgBank + 3 );

  /* Set PPU Banks */
  byChrBank <<= 3;
  byChrBank %= ( NesHeader.byVRomSize  << 3 );

  PPUBANK[ 0 ] = VROMPAGE( byChrBank + 0 );
  PPUBANK[ 1 ] = VROMPAGE( byChrBank + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byChrBank + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byChrBank + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byChrBank + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byChrBank + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byChrBank + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byChrBank + 7 );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_079 end



