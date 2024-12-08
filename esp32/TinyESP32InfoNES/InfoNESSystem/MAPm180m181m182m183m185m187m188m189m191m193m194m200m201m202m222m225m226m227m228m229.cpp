#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"


//InfoNES_Mapper_180 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 180  (Nichibutsu)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 180                                            */
/*-------------------------------------------------------------------*/
void Map180_Init()
{
  /* Initialize Mapper */
  MapperInit = Map180_Init;

  /* Write to Mapper */
  MapperWrite = Map180_Write;

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
/*  Mapper 180 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map180_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */ 
  byData &= 0x07;
  byData <<= 1;
  byData %= ( NesHeader.byRomSize << 1 );
  ROMBANK2 = ROMPAGE( byData );
  ROMBANK3 = ROMPAGE( byData + 1 );
}
//InfoNES_Mapper_180 end











//InfoNES_Mapper_181 begin
/*===================================================================*/
/*                                                                   */
/*            Mapper 181 : Hacker International Type2                */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 181                                            */
/*-------------------------------------------------------------------*/
void Map181_Init()
{
  /* Initialize Mapper */
  MapperInit = Map181_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map181_Apu;

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
/*  Mapper 181 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map181_Apu( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr == 0x4120 ) {
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( (((byData&0x08)>>1) + 0 ) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (((byData&0x08)>>1) + 1 ) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData&0x08)>>1) + 2 ) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData&0x08)>>1) + 3 ) % (NesHeader.byRomSize << 1) );

      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( (((byData&0x07)<<3) + 0) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 1 ] = VROMPAGE( (((byData&0x07)<<3) + 1) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 2 ] = VROMPAGE( (((byData&0x07)<<3) + 2) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 3 ] = VROMPAGE( (((byData&0x07)<<3) + 3) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 4 ] = VROMPAGE( (((byData&0x07)<<3) + 4) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 5 ] = VROMPAGE( (((byData&0x07)<<3) + 5) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 6 ] = VROMPAGE( (((byData&0x07)<<3) + 6) % (NesHeader.byVRomSize << 3) );
      PPUBANK[ 7 ] = VROMPAGE( (((byData&0x07)<<3) + 7) % (NesHeader.byVRomSize << 3) );
      InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_181 end












//InfoNES_Mapper_182 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 182  (Pirates)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map182_Regs[1];
unsigned char Map182_IRQ_Enable;
unsigned char Map182_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 182                                            */
/*-------------------------------------------------------------------*/
void Map182_Init()
{
  /* Initialize Mapper */
  MapperInit = Map182_Init;

  /* Write to Mapper */
  MapperWrite = Map182_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map182_HSync;

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
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize IRQ registers */
  Map182_Regs[0] = 0;
  Map182_IRQ_Enable = 0;
  Map182_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map182_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xF003 )
  {
    /* Name Table Mirroring */
    case 0x8001:
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 0 );   /* Horizontal */
      }
      else
      {
        InfoNES_Mirroring( 1 );   /* Vertical */
      }
      break;

    case 0xA000:
      Map182_Regs[0] = byData & 0x07;
      break;

    case 0xC000:
      switch( Map182_Regs[0] )
      { 
        /* Set PPU Banks */ 
        case 0x00:
          PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x01:
          PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x02:
          PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xFE ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xFE ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x03:
          PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        /* Set ROM Banks */ 
        case 0x04:
          ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        case 0x05:
          ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        /* Set PPU Banks */ 
        case 0x06:
          PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x07:
          PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;
      }
      break;

   case 0xE003:
     Map182_IRQ_Cnt = byData;
     Map182_IRQ_Enable = byData;
     break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 182 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map182_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map182_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 240 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map182_IRQ_Cnt ) )
        {
          Map182_IRQ_Cnt = 0;
          Map182_IRQ_Enable = 0;
          IRQ_REQ;
        }
      }
    }
  }
}
//InfoNES_Mapper_182 end









//InfoNES_Mapper_183 begin
/*===================================================================*/
/*                                                                   */
/*                 Mapper 183 : Gimmick (Bootleg)                    */
/*                                                                   */
/*===================================================================*/

unsigned char	Map183_Reg[8];
unsigned char	Map183_IRQ_Enable;
int	Map183_IRQ_Counter;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 183                                            */
/*-------------------------------------------------------------------*/
void Map183_Init()
{
  /* Initialize Mapper */
  MapperInit = Map183_Init;

  /* Write to Mapper */
  MapperWrite = Map183_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map183_HSync;

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

  /* Initialize Registers */
  for( int i = 0; i < 8; i++ ) {
    Map183_Reg[i] = i;
  }
  Map183_IRQ_Enable = 0;
  Map183_IRQ_Counter = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 183 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map183_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr ) {
  case	0x8800:
    ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
  case	0xA800:
    ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
  case	0xA000:
    ROMBANK2 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );
    break;
    
  case	0xB000:
    Map183_Reg[0] = (Map183_Reg[0]&0xF0)|(byData&0x0F);
    PPUBANK[ 0 ] = VROMPAGE( Map183_Reg[0] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB004:
    Map183_Reg[0] = (Map183_Reg[0]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 0 ] = VROMPAGE( Map183_Reg[0] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB008:
    Map183_Reg[1] = (Map183_Reg[1]&0xF0)|(byData&0x0F);
    PPUBANK[ 1 ] = VROMPAGE( Map183_Reg[1] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xB00C:
    Map183_Reg[1] = (Map183_Reg[1]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 1 ] = VROMPAGE( Map183_Reg[1] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xC000:
    Map183_Reg[2] = (Map183_Reg[2]&0xF0)|(byData&0x0F);
    PPUBANK[ 2 ] = VROMPAGE( Map183_Reg[2] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC004:
    Map183_Reg[2] = (Map183_Reg[2]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 2 ] = VROMPAGE( Map183_Reg[2] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC008:
    Map183_Reg[3] = (Map183_Reg[3]&0xF0)|(byData&0x0F);
    PPUBANK[ 3 ] = VROMPAGE( Map183_Reg[3] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xC00C:
    Map183_Reg[3] = (Map183_Reg[3]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 3 ] = VROMPAGE( Map183_Reg[3] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xD000:
    Map183_Reg[4] = (Map183_Reg[4]&0xF0)|(byData&0x0F);
    PPUBANK[ 4 ] = VROMPAGE( Map183_Reg[4] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD004:
    Map183_Reg[4] = (Map183_Reg[4]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 4 ] = VROMPAGE( Map183_Reg[4] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD008:
    Map183_Reg[5] = (Map183_Reg[5]&0xF0)|(byData&0x0F);
    PPUBANK[ 5 ] = VROMPAGE( Map183_Reg[5] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xD00C:
    Map183_Reg[5] = (Map183_Reg[5]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 5 ] = VROMPAGE( Map183_Reg[5] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0xE000:
    Map183_Reg[6] = (Map183_Reg[6]&0xF0)|(byData&0x0F);
    PPUBANK[ 6 ] = VROMPAGE( Map183_Reg[6] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE004:
    Map183_Reg[6] = (Map183_Reg[6]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 6 ] = VROMPAGE( Map183_Reg[6] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE008:
    Map183_Reg[7] = (Map183_Reg[3]&0xF0)|(byData&0x0F);
    PPUBANK[ 7 ] = VROMPAGE( Map183_Reg[7] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
  case	0xE00C:
    Map183_Reg[7] = (Map183_Reg[3]&0x0F)|((byData&0x0F)<<4);
    PPUBANK[ 7 ] = VROMPAGE( Map183_Reg[7] % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();
    break;
    
  case	0x9008:
    if( byData == 1 ) {
      for( int i = 0; i < 8; i++ ) {
	Map183_Reg[i] = i;
      }
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( 0 );
      ROMBANK1 = ROMPAGE( 1 );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );

      /* Set PPU Banks */
      if ( NesHeader.byVRomSize > 0 ) {
	PPUBANK[ 0 ] = VROMPAGE( 0 );
	PPUBANK[ 1 ] = VROMPAGE( 1 );
	PPUBANK[ 2 ] = VROMPAGE( 2 );
	PPUBANK[ 3 ] = VROMPAGE( 3 );
	PPUBANK[ 4 ] = VROMPAGE( 4 );
	PPUBANK[ 5 ] = VROMPAGE( 5 );
	PPUBANK[ 6 ] = VROMPAGE( 6 );
	PPUBANK[ 7 ] = VROMPAGE( 7 );
	InfoNES_SetupChr();
      }
    }
    break;
    
  case	0x9800:
    if( byData == 0 )      InfoNES_Mirroring( 1 );
    else if( byData == 1 ) InfoNES_Mirroring( 0 );
    else if( byData == 2 ) InfoNES_Mirroring( 2 );
    else if( byData == 3 ) InfoNES_Mirroring( 3 );
    break;
    
  case	0xF000:
    Map183_IRQ_Counter = (Map183_IRQ_Counter&0xFF00)|byData;
    break;
  case	0xF004:
    Map183_IRQ_Counter = (Map183_IRQ_Counter&0x00FF)|(byData<<8);
    break;
  case	0xF008:
    Map183_IRQ_Enable = byData;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 183 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map183_HSync()
{
  if( Map183_IRQ_Enable & 0x02 ) {
    if( Map183_IRQ_Counter <= 113 ) {
      Map183_IRQ_Counter = 0;
      IRQ_REQ;
    } else {
      Map183_IRQ_Counter -= 113;
    }
  }
}
//InfoNES_Mapper_183 end










#ifndef use_lib_not_use_mapper185
//InfoNES_Mapper_185 begin
/*===================================================================*/
/*                                                                   */
/*                         Mapper 185  (Tecmo)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map185_Dummy_Chr_Rom[ 0x400 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 185                                            */
/*-------------------------------------------------------------------*/
void Map185_Init()
{
  /* Initialize Mapper */
  MapperInit = Map185_Init;

  /* Write to Mapper */
  MapperWrite = Map185_Write;

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

  /* Initialize Dummy VROM */
  for ( int nPage = 0; nPage < 0x400; nPage++ )
  {
    Map185_Dummy_Chr_Rom[ nPage ] = 0xff;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 185 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map185_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set PPU Banks */ 
  if ( byData & 0x03 )
  {
    PPUBANK[ 0 ] = VROMPAGE( 0 );
    PPUBANK[ 1 ] = VROMPAGE( 1 );
    PPUBANK[ 2 ] = VROMPAGE( 2 );
    PPUBANK[ 3 ] = VROMPAGE( 3 );
    PPUBANK[ 4 ] = VROMPAGE( 4 );
    PPUBANK[ 5 ] = VROMPAGE( 5 );
    PPUBANK[ 6 ] = VROMPAGE( 6 );
    PPUBANK[ 7 ] = VROMPAGE( 7 );
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 1 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 2 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 3 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 4 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 5 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 6 ] = Map185_Dummy_Chr_Rom;
    PPUBANK[ 7 ] = Map185_Dummy_Chr_Rom;
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_185 end
#endif









//InfoNES_Mapper_187 begin
/*===================================================================*/
/*                                                                   */
/*            Mapper 187 : Street Fighter Zero 2 97                  */
/*                                                                   */
/*===================================================================*/

unsigned char	Map187_Prg[4];
int	Map187_Chr[8];
unsigned char	Map187_Bank[8];

unsigned char	Map187_ExtMode;
unsigned char	Map187_ChrMode;
unsigned char	Map187_ExtEnable;

unsigned char	Map187_IRQ_Enable;
unsigned char	Map187_IRQ_Counter;
unsigned char	Map187_IRQ_Latch;
unsigned char	Map187_IRQ_Occur;
unsigned char	Map187_LastWrite;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 187                                            */
/*-------------------------------------------------------------------*/
void Map187_Init()
{
  /* Initialize Mapper */
  MapperInit = Map187_Init;

  /* Write to Mapper */
  MapperWrite = Map187_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map187_Apu;

  /* Read from APU */
  MapperReadApu = Map187_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map187_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map187_Chr[i] = 0x00;
    Map187_Bank[i] = 0x00;
  }

  /* Set ROM Banks */
  Map187_Prg[0] = (NesHeader.byRomSize<<1)-4;
  Map187_Prg[1] = (NesHeader.byRomSize<<1)-3;
  Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
  Map187_Prg[3] = (NesHeader.byRomSize<<1)-1;
  Map187_Set_CPU_Banks();

  Map187_ExtMode = 0;
  Map187_ChrMode = 0;
  Map187_ExtEnable = 0;

  Map187_IRQ_Enable = 0;
  Map187_IRQ_Counter = 0;
  Map187_IRQ_Latch = 0;

  Map187_LastWrite = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map187_Write( unsigned short int wAddr, unsigned char byData )
{
  Map187_LastWrite = byData;
  switch( wAddr ) {
  case	0x8003:
    Map187_ExtEnable = 0xFF;
    Map187_ChrMode = byData;
    if( (byData&0xF0) == 0 ) {
      Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
      Map187_Set_CPU_Banks();
    }
    break;
    
  case	0x8000:
    Map187_ExtEnable = 0;
    Map187_ChrMode = byData;
    break;

  case	0x8001:
    if( !Map187_ExtEnable ) {
      switch( Map187_ChrMode & 7 ) {
      case	0:
	byData &= 0xFE;
	Map187_Chr[4] = (int)byData+0x100;
	Map187_Chr[5] = (int)byData+0x100+1;
	Map187_Set_PPU_Banks();
	break;
      case	1:
	byData &= 0xFE;
	Map187_Chr[6] = (int)byData+0x100;
	Map187_Chr[7] = (int)byData+0x100+1;
	Map187_Set_PPU_Banks();
	break;
      case	2:
	Map187_Chr[0] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	3:
	Map187_Chr[1] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	4:
	Map187_Chr[2] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	5:
	Map187_Chr[3] = byData;
	Map187_Set_PPU_Banks();
	break;
      case	6:
	if( (Map187_ExtMode&0xA0)!=0xA0 ) {
	  Map187_Prg[0] = byData;
	  Map187_Set_CPU_Banks();
	}
	break;
      case	7:
	if( (Map187_ExtMode&0xA0)!=0xA0 ) {
	  Map187_Prg[1] = byData;
	  Map187_Set_CPU_Banks();
	}
	break;
      default:
	break;
      }
    } else {
      switch( Map187_ChrMode ) {
      case	0x2A:
	Map187_Prg[1] = 0x0F;
	break;
      case	0x28:
	Map187_Prg[2] = 0x17;
	break;
      case	0x26:
	break;
      default:
	break;
      }
      Map187_Set_CPU_Banks();
    }
    Map187_Bank[Map187_ChrMode&7] = byData;
    break;
    
  case	0xA000:
    if( byData & 0x01 ) {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    break;
    
  case	0xC000:
    Map187_IRQ_Counter = byData;
    Map187_IRQ_Occur = 0;
    break;
  case	0xC001:
    Map187_IRQ_Latch = byData;
    Map187_IRQ_Occur = 0;
    break;
  case	0xE000:
  case	0xE002:
    Map187_IRQ_Enable = 0;
    Map187_IRQ_Occur = 0;
    break;
  case	0xE001:
  case	0xE003:
    Map187_IRQ_Enable = 1;
    Map187_IRQ_Occur = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map187_Apu( unsigned short int wAddr, unsigned char byData )
{
  Map187_LastWrite = byData;
  if( wAddr == 0x5000 ) {
    Map187_ExtMode = byData;
    if( byData & 0x80 ) {
      if( byData & 0x20 ) {
	Map187_Prg[0] = ((byData&0x1E)<<1)+0;
	Map187_Prg[1] = ((byData&0x1E)<<1)+1;
	Map187_Prg[2] = ((byData&0x1E)<<1)+2;
	Map187_Prg[3] = ((byData&0x1E)<<1)+3;
      } else {
	Map187_Prg[2] = ((byData&0x1F)<<1)+0;
	Map187_Prg[3] = ((byData&0x1F)<<1)+1;
      }
    } else {
      Map187_Prg[0] = Map187_Bank[6];
      Map187_Prg[1] = Map187_Bank[7];
      Map187_Prg[2] = (NesHeader.byRomSize<<1)-2;
      Map187_Prg[3] = (NesHeader.byRomSize<<1)-1;
    }
    Map187_Set_CPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Read from APU Function                                */
/*-------------------------------------------------------------------*/
unsigned char Map187_ReadApu( unsigned short int wAddr )
{
  switch( Map187_LastWrite&0x03 ) {
  case 0:
    return 0x83;
  case 1:
    return 0x83;
  case 2:
    return 0x42;
  case 3:
    return 0x00;
  }
  return 0;
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map187_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map187_IRQ_Enable ) {
	if( !Map187_IRQ_Counter ) {
	  Map187_IRQ_Counter--;
	  Map187_IRQ_Enable = 0;
	  Map187_IRQ_Occur = 0xFF;
	} else {
	  Map187_IRQ_Counter--;
	}
      }
    }
  }
  if ( Map187_IRQ_Occur ) {
	  IRQ_REQ;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map187_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE(((Map187_Prg[0]<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((Map187_Prg[1]<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((Map187_Prg[2]<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((Map187_Prg[3]<<2)+3) % (NesHeader.byRomSize<<1));
}

/*-------------------------------------------------------------------*/
/*  Mapper 187 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map187_Set_PPU_Banks()
{
  PPUBANK[ 0 ] = VROMPAGE(((Map187_Chr[0]<<3)+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE(((Map187_Chr[1]<<3)+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE(((Map187_Chr[2]<<3)+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE(((Map187_Chr[3]<<3)+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE(((Map187_Chr[4]<<3)+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE(((Map187_Chr[5]<<3)+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE(((Map187_Chr[6]<<3)+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE(((Map187_Chr[7]<<3)+7) % (NesHeader.byVRomSize<<3));
  InfoNES_SetupChr();
}
//InfoNES_Mapper_187 end












#ifndef use_lib_not_use_mapper188
//InfoNES_Mapper_188 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 188 (Bandai)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map188_Dummy[ 0x2000 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 188                                            */
/*-------------------------------------------------------------------*/
void Map188_Init()
{
  /* Initialize Mapper */
  MapperInit = Map188_Init;

  /* Write to Mapper */
  MapperWrite = Map188_Write;

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
  SRAMBANK = Map188_Dummy;

  /* Set ROM Banks */
  if ( ( NesHeader.byRomSize << 1 ) > 16 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 14 );
    ROMBANK3 = ROMPAGE( 15 );
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Magic Code */
  Map188_Dummy[ 0 ] = 0x03;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 188 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map188_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  if ( byData )
  {
    if ( byData & 0x10 )
    {
      byData = ( byData & 0x07 ) << 1;
      ROMBANK0 = ROMPAGE( ( byData + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( byData + 1 ) % ( NesHeader.byRomSize << 1 ) );
    } else {
      byData <<= 1;
      ROMBANK0 = ROMPAGE( ( byData + 16 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( byData + 17 ) % ( NesHeader.byRomSize << 1 ) );
    }
  } 
  else 
  {
    if ( ( NesHeader.byRomSize << 1 ) == 0x10 )
    {
      ROMBANK0 = ROMPAGE( 14 );
      ROMBANK1 = ROMPAGE( 15 );
    } else {
      ROMBANK0 = ROMPAGE( 16 );
      ROMBANK1 = ROMPAGE( 17 );
    }
  }
}
//InfoNES_Mapper_188 end
#endif








//InfoNES_Mapper_189 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 189 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map189_Regs[ 1 ];
unsigned char Map189_IRQ_Cnt;
unsigned char Map189_IRQ_Latch;
unsigned char Map189_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 189                                            */
/*-------------------------------------------------------------------*/
void Map189_Init()
{
  /* Initialize Mapper */
  MapperInit = Map189_Init;

  /* Write to Mapper */
  MapperWrite = Map189_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map189_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map189_HSync;

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
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize IRQ registers */
  Map189_IRQ_Cnt = 0;
  Map189_IRQ_Latch = 0;
  Map189_IRQ_Enable = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map189_Apu( unsigned short int wAddr, unsigned char byData )
{
  if ( wAddr >= 0x4100 && wAddr <= 0x41FF )
  {
    byData = ( byData & 0x30 ) >> 4;
    ROMBANK0 = ROMPAGE( ( byData * 4 + 0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( byData * 4 + 1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( byData * 4 + 2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( byData * 4 + 3 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map189_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  switch( wAddr )
  {
    case 0x8000:
      Map189_Regs[0] = byData;
      break;

    case 0x8001:
      switch( Map189_Regs[0] )
      {
        case 0x40:
	  PPUBANK[ 0 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
	  PPUBANK[ 1 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x41:
	  PPUBANK[ 2 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
	  PPUBANK[ 3 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x42:
	  PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x43:
	  PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x44:
	  PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x45:
	  PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
	  InfoNES_SetupChr();
	  break;

        case 0x46:
	  ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
	  break;  

        case 0x47:
	  ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
	  break;  
      }
      break;

    case 0xC000:
      Map189_IRQ_Cnt = byData;
      break;

    case 0xC001:
      Map189_IRQ_Latch = byData;
      break;

    case 0xE000:
      Map189_IRQ_Enable = 0;
      break;

    case 0xE001:
      Map189_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 189 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map189_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map189_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map189_IRQ_Cnt ) )
        {
          Map189_IRQ_Cnt = Map189_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}
//InfoNES_Mapper_189 end










//InfoNES_Mapper_191 begin
/*===================================================================*/
/*                                                                   */
/* Mapper 191 : SACHEN Super Cartridge Xin1 (Ver.1-9), Q-BOY Support */
/*                                                                   */
/*===================================================================*/

unsigned char	Map191_Reg[8];
unsigned char	Map191_Prg0, Map191_Prg1;
unsigned char	Map191_Chr0, Map191_Chr1, Map191_Chr2, Map191_Chr3;
unsigned char	Map191_Highbank;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 191                                            */
/*-------------------------------------------------------------------*/
void Map191_Init()
{
  /* Initialize Mapper */
  MapperInit = Map191_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map191_Apu;

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
  for( int i = 0; i < 8; i++ ) {
    Map191_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map191_Prg0 = 0;
  // Map191_Prg1 = 1;
  Map191_Set_CPU_Banks();

  /* Set PPU Banks */
  Map191_Chr0 = 0;
  Map191_Chr1 = 0;
  Map191_Chr2 = 0;
  Map191_Chr3 = 0;
  Map191_Highbank = 0;
  Map191_Set_PPU_Banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 191 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map191_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map191_Reg[0]=byData;
    break;
  case	0x4101:
    Map191_Reg[1]=byData;
    switch( Map191_Reg[0] ) {
    case	0:
      Map191_Chr0=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	1:
      Map191_Chr1=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	2:
      Map191_Chr2=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	3:
      Map191_Chr3=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	4:
      Map191_Highbank=byData&7;
      Map191_Set_PPU_Banks();
      break;
    case	5:
      Map191_Prg0=byData&7;
      Map191_Set_CPU_Banks();
      break;
    case	7:
      if( byData & 0x02 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
      break;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 191 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map191_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE( ((Map191_Prg0<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ((Map191_Prg0<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ((Map191_Prg0<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ((Map191_Prg0<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );
};

/*-------------------------------------------------------------------*/
/*  Mapper 191 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void	Map191_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 ) {
    PPUBANK[ 0 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr0)<<2)+0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr0)<<2)+1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr1)<<2)+2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr1)<<2)+3 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr2)<<2)+0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr2)<<2)+1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr3)<<2)+2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ((((Map191_Highbank<<3)+Map191_Chr3)<<2)+3 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_191 end











//InfoNES_Mapper_193 begin
/*===================================================================*/
/*                                                                   */
/*         Mapper 193 : MEGA SOFT (NTDEC) : Fighting Hero            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 193                                            */
/*-------------------------------------------------------------------*/
void Map193_Init()
{
  /* Initialize Mapper */
  MapperInit = Map193_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map193_Sram;

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
  ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

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
/*  Mapper 193 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map193_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr ) {
  case	0x6000:
    PPUBANK[ 0 ] = VROMPAGE( ((byData&0xfc) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ((byData&0xfc) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ((byData&0xfc) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ((byData&0xfc) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6001:
    PPUBANK[ 4 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6002:
    PPUBANK[ 6 ] = VROMPAGE( ( byData + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( byData + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0x6003:
    ROMBANK0 = ROMPAGE( ((byData<<2) + 0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ((byData<<2) + 1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ((byData<<2) + 2 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ((byData<<2) + 3 ) % ( NesHeader.byRomSize << 1 ) );
    break;
  }
}
//InfoNES_Mapper_193 end











//InfoNES_Mapper_194 begin
/*===================================================================*/
/*                                                                   */
/*            Mapper 194 : Meikyuu Jiin Dababa                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 194                                            */
/*-------------------------------------------------------------------*/
void Map194_Init()
{
  /* Initialize Mapper */
  MapperInit = Map194_Init;

  /* Write to Mapper */
  MapperWrite = Map194_Write;

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
  ROMBANK0 = ROMPAGE( (NesHeader.byRomSize<<1) - 4 );
  ROMBANK1 = ROMPAGE( (NesHeader.byRomSize<<1) - 3 );
  ROMBANK2 = ROMPAGE( (NesHeader.byRomSize<<1) - 2 );
  ROMBANK3 = ROMPAGE( (NesHeader.byRomSize<<1) - 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 194 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map194_Write( unsigned short int wAddr, unsigned char byData )
{
  SRAMBANK = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
}
//InfoNES_Mapper_194 end











//InfoNES_Mapper_200 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 200 (1200-in-1)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 200                                            */
/*-------------------------------------------------------------------*/
void Map200_Init()
{
  /* Initialize Mapper */
  MapperInit = Map200_Init;

  /* Write to Mapper */
  MapperWrite = Map200_Write;

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
/*  Mapper 200 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map200_Write( unsigned short int wAddr, unsigned char byData )
{
	/* Set ROM Banks */
  ROMBANK0 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((wAddr&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((wAddr&0x07)<<1)+1) % (NesHeader.byRomSize<<1));

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE((((wAddr&0x07)<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE((((wAddr&0x07)<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE((((wAddr&0x07)<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE((((wAddr&0x07)<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE((((wAddr&0x07)<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE((((wAddr&0x07)<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE((((wAddr&0x07)<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE((((wAddr&0x07)<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();

	if ( wAddr & 0x01 ) {
		InfoNES_Mirroring( 1 );
	} else {
		InfoNES_Mirroring( 0 );
	}
}
//InfoNES_Mapper_200 end






//InfoNES_Mapper_201 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 201 (21-in-1)                            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 201                                            */
/*-------------------------------------------------------------------*/
void Map201_Init()
{
  /* Initialize Mapper */
  MapperInit = Map201_Init;

  /* Write to Mapper */
  MapperWrite = Map201_Write;

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
/*  Mapper 201 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map201_Write( unsigned short int wAddr, unsigned char byData )
{
	unsigned char byBank = (unsigned char)wAddr & 0x03;
	if (!(wAddr&0x08) )
		byBank = 0;

	/* Set ROM Banks */
  ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE(((byBank<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE(((byBank<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE(((byBank<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE(((byBank<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE(((byBank<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE(((byBank<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE(((byBank<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE(((byBank<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();
}
//InfoNES_Mapper_201 end








//InfoNES_Mapper_202 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 202 (150-in-1)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 202                                            */
/*-------------------------------------------------------------------*/
void Map202_Init()
{
  /* Initialize Mapper */
  MapperInit = Map202_Init;

  /* Write to Mapper */
  MapperWrite = Map202_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map202_Apu;

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
  ROMBANK0 = ROMPAGE( 12 );
  ROMBANK1 = ROMPAGE( 13 );
  ROMBANK2 = ROMPAGE( 14 );
  ROMBANK3 = ROMPAGE( 15 );

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
/*  Mapper 202 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map202_Apu( unsigned short int wAddr, unsigned char byData )
{
	Map202_WriteSub( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map202_Write( unsigned short int wAddr, unsigned char byData )
{
	Map202_WriteSub( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 202 Write Sub Function                                    */
/*-------------------------------------------------------------------*/
void Map202_WriteSub( unsigned short int wAddr, unsigned char byData )
{
	int bank = (wAddr>>1) & 0x07;

	/* Set ROM Banks */
	ROMBANK0 = ROMPAGE(((bank<<1)+0) % (NesHeader.byRomSize<<1));
	ROMBANK1 = ROMPAGE(((bank<<1)+1) % (NesHeader.byRomSize<<1));

	if ((wAddr&0x0c) == 0x0c) {
		ROMBANK2 = ROMPAGE((((bank+1)<<1)+0) % (NesHeader.byRomSize<<1));
		ROMBANK3 = ROMPAGE((((bank+1)<<1)+1) % (NesHeader.byRomSize<<1));
	} else {		
		ROMBANK2 = ROMPAGE((((bank+0)<<1)+0) % (NesHeader.byRomSize<<1));
		ROMBANK3 = ROMPAGE((((bank+0)<<1)+1) % (NesHeader.byRomSize<<1));
	}	

	/* Set PPU Banks */
	PPUBANK[0] = VROMPAGE(((bank<<3)+0) % (NesHeader.byVRomSize<<3));
	PPUBANK[1] = VROMPAGE(((bank<<3)+1) % (NesHeader.byVRomSize<<3));
	PPUBANK[2] = VROMPAGE(((bank<<3)+2) % (NesHeader.byVRomSize<<3));
	PPUBANK[3] = VROMPAGE(((bank<<3)+3) % (NesHeader.byVRomSize<<3));
	PPUBANK[4] = VROMPAGE(((bank<<3)+4) % (NesHeader.byVRomSize<<3));
	PPUBANK[5] = VROMPAGE(((bank<<3)+5) % (NesHeader.byVRomSize<<3));
	PPUBANK[6] = VROMPAGE(((bank<<3)+6) % (NesHeader.byVRomSize<<3));
	PPUBANK[7] = VROMPAGE(((bank<<3)+7) % (NesHeader.byVRomSize<<3));
	InfoNES_SetupChr();

	if ( wAddr & 0x01 ) {
		InfoNES_Mirroring( 0 );
	} else {
		InfoNES_Mirroring( 1 );
	}
}
//InfoNES_Mapper_202 end










//InfoNES_Mapper_222 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 222                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 222                                            */
/*-------------------------------------------------------------------*/
void Map222_Init()
{
  /* Initialize Mapper */
  MapperInit = Map222_Init;

  /* Write to Mapper */
  MapperWrite = Map222_Write;

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

  /* Set Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 222 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map222_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xF003 ) {
  case	0x8000:
    ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xA000:
    ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
    break;
  case	0xB000:
    PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xB002:
    PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xC000:
    PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xC002:
    PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xD000:
    PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xD002:
    PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xE000:
    PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  case	0xE002:
    PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
    break;
  }
}
//InfoNES_Mapper_222 end












//InfoNES_Mapper_225 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 225 : 72-in-1                            */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 225                                            */
/*-------------------------------------------------------------------*/
void Map225_Init()
{
  /* Initialize Mapper */
  MapperInit = Map225_Init;

  /* Write to Mapper */
  MapperWrite = Map225_Write;

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
/*  Mapper 225 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map225_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = (wAddr & 0x0F80) >> 7;
  unsigned char byChrBank = wAddr & 0x003F;
  
  PPUBANK[ 0 ] = VROMPAGE(((byChrBank<<3)+0) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 1 ] = VROMPAGE(((byChrBank<<3)+1) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 2 ] = VROMPAGE(((byChrBank<<3)+2) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 3 ] = VROMPAGE(((byChrBank<<3)+3) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 4 ] = VROMPAGE(((byChrBank<<3)+4) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 5 ] = VROMPAGE(((byChrBank<<3)+5) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 6 ] = VROMPAGE(((byChrBank<<3)+6) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 7 ] = VROMPAGE(((byChrBank<<3)+7) % ( NesHeader.byVRomSize << 3 ) );
  InfoNES_SetupChr();

  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
  
  if( wAddr & 0x1000 ) {
    // 16KBbank
    if( wAddr & 0x0040 ) {
      ROMBANK0 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
    } else {
      ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    }
  } else {
    ROMBANK0 = ROMPAGE(((byPrgBank<<2)+0) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE(((byPrgBank<<2)+1) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE(((byPrgBank<<2)+2) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE(((byPrgBank<<2)+3) % ( NesHeader.byRomSize << 1 ) );
  }
}
//InfoNES_Mapper_225 end














//InfoNES_Mapper_226 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 226 : 76-in-1                            */
/*                                                                   */
/*===================================================================*/

unsigned char	Map226_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 226                                            */
/*-------------------------------------------------------------------*/
void Map226_Init()
{
  /* Initialize Mapper */
  MapperInit = Map226_Init;

  /* Write to Mapper */
  MapperWrite = Map226_Write;

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

  /* Initialize Registers */
  Map226_Reg[0] = 0;
  Map226_Reg[1] = 0;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 226 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map226_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr & 0x0001 ) {
    Map226_Reg[1] = byData;
  } else {
    Map226_Reg[0] = byData;
  }
  
  if( Map226_Reg[0] & 0x40 ) {
    InfoNES_Mirroring( 1 );
  } else {
    InfoNES_Mirroring( 0 );
  }
  
  unsigned char	byBank = ((Map226_Reg[0]&0x1E)>>1)|((Map226_Reg[0]&0x80)>>3)|((Map226_Reg[1]&0x01)<<5);
  
  if( Map226_Reg[0] & 0x20 ) {
    if( Map226_Reg[0] & 0x01 ) {
      ROMBANK0 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
//InfoNES_Mapper_226 end






//InfoNES_Mapper_227 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 227 : 1200-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 227                                            */
/*-------------------------------------------------------------------*/
void Map227_Init()
{
  /* Initialize Mapper */
  MapperInit = Map227_Init;

  /* Write to Mapper */
  MapperWrite = Map227_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 227 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map227_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byBank = ((wAddr&0x0100)>>4)|((wAddr&0x0078)>>3);

  if( wAddr & 0x0001 ) {
    ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
  } else {
    if( wAddr & 0x0004 ) {
      ROMBANK0 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byBank<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byBank<<2)+1) % (NesHeader.byRomSize<<1));
    }
  }
  
  if( !(wAddr & 0x0080) ) {
    if( wAddr & 0x0200 ) {
      ROMBANK2 = ROMPAGE((((byBank&0x1C)<<2)+14) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byBank&0x1C)<<2)+15) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK2 = ROMPAGE((((byBank&0x1C)<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byBank&0x1C)<<2)+1) % (NesHeader.byRomSize<<1));
    }
  }
  if( wAddr & 0x0002 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
//InfoNES_Mapper_227 end





//InfoNES_Mapper_228 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 228 : Action 52                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 228                                            */
/*-------------------------------------------------------------------*/
void Map228_Init()
{
  /* Initialize Mapper */
  MapperInit = Map228_Init;

  /* Write to Mapper */
  MapperWrite = Map228_Write;

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
/*  Mapper 228 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map228_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byPrg = (wAddr&0x0780)>>7;

  switch( (wAddr&0x1800)>>11 ) {
  case	1:
    byPrg |= 0x10;
    break;
  case	3:
    byPrg |= 0x20;
    break;
  }

  if( wAddr & 0x0020 ) {
    byPrg <<= 1;
    if( wAddr & 0x0040 ) {
      byPrg++;
    }
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
  }

  unsigned char byChr = ((wAddr&0x000F)<<2)|(byData&0x03);

  PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3)); 
  PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3)); 
  InfoNES_SetupChr();
  
  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
//InfoNES_Mapper_228 end





//InfoNES_Mapper_229 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 229 : 31-in-1                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 229                                            */
/*-------------------------------------------------------------------*/
void Map229_Init()
{
  /* Initialize Mapper */
  MapperInit = Map229_Init;

  /* Write to Mapper */
  MapperWrite = Map229_Write;

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
/*  Mapper 229 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map229_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr & 0x001E ) {
    unsigned char byPrg = wAddr&0x001F;

    ROMBANK0 = ROMPAGE(((byPrg<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<1)+1) % (NesHeader.byRomSize<<1));

    unsigned char byChr = wAddr & 0x0FFF;

    PPUBANK[ 0 ] = VROMPAGE(((byChr<<3) + 0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((byChr<<3) + 1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((byChr<<3) + 2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((byChr<<3) + 3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((byChr<<3) + 4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((byChr<<3) + 5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((byChr<<3) + 6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((byChr<<3) + 7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 2 );
    ROMBANK3 = ROMPAGE( 3 );

    PPUBANK[ 0 ] = VROMPAGE( 0 );
    PPUBANK[ 1 ] = VROMPAGE( 1 );
    PPUBANK[ 2 ] = VROMPAGE( 2 );
    PPUBANK[ 3 ] = VROMPAGE( 3 );
    PPUBANK[ 4 ] = VROMPAGE( 4 );
    PPUBANK[ 5 ] = VROMPAGE( 5 );
    PPUBANK[ 6 ] = VROMPAGE( 6 );
    PPUBANK[ 7 ] = VROMPAGE( 7 );
    InfoNES_SetupChr();
  }

  if( wAddr & 0x0020 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
//InfoNES_Mapper_229 end


