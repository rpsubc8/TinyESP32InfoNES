#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"


//InfoNES_Mapper_023 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 23 (Konami VRC2 type B)                   */
/*                                                                   */
/*===================================================================*/

unsigned char Map23_Regs[ 9 ];

unsigned char Map23_IRQ_Enable;
unsigned char Map23_IRQ_Cnt;
unsigned char Map23_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 23                                             */
/*-------------------------------------------------------------------*/
void Map23_Init()
{
  /* Initialize Mapper */
  MapperInit = Map23_Init;

  /* Write to Mapper */
  MapperWrite = Map23_Write;

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
  Map23_Regs[ 0 ] = 0;
  Map23_Regs[ 1 ] = 1;
  Map23_Regs[ 2 ] = 2;
  Map23_Regs[ 3 ] = 3;
  Map23_Regs[ 4 ] = 4;
  Map23_Regs[ 5 ] = 5;
  Map23_Regs[ 6 ] = 6;
  Map23_Regs[ 7 ] = 7;
  Map23_Regs[ 8 ] = 0;

  Map23_IRQ_Enable = 0;
  Map23_IRQ_Cnt = 0;
  Map23_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 23 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map23_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
    case 0x8004:
    case 0x8008:
    case 0x800c:
      byData %= ( NesHeader.byRomSize << 1 );

      if ( Map23_Regs[ 8 ] )
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0x9000:
      switch ( byData & 0x03 )
      {
       case 0x00:
          InfoNES_Mirroring( 1 );
          break;
        case 0x01:
          InfoNES_Mirroring( 0 ); 
          break;
        case 0x02:
          InfoNES_Mirroring( 3 );  
          break;
        case 0x03:
          InfoNES_Mirroring( 2 );  
          break;
      }
      break;

    case 0x9008:
      Map23_Regs[ 8 ] = byData & 0x02;
      break;

    case 0xa000:
    case 0xa004:
    case 0xa008:
    case 0xa00c:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xb000:
      Map23_Regs[ 0 ] = ( Map23_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map23_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
    case 0xb004:
      Map23_Regs[ 0 ] = ( Map23_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map23_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
    case 0xb008:
      Map23_Regs[ 1 ] = ( Map23_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map23_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
    case 0xb00c:
      Map23_Regs[ 1 ] = ( Map23_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map23_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map23_Regs[ 2 ] = ( Map23_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map23_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc004:
      Map23_Regs[ 2 ] = ( Map23_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map23_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc002:
    case 0xc008:
      Map23_Regs[ 3 ] = ( Map23_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map23_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc003:
    case 0xc00c:
      Map23_Regs[ 3 ] = ( Map23_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map23_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd000:
      Map23_Regs[ 4 ] = ( Map23_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map23_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd001:
    case 0xd004:
      Map23_Regs[ 4 ] = ( Map23_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map23_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd002:
    case 0xd008:
      Map23_Regs[ 5 ] = ( Map23_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map23_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd003:
    case 0xd00c:
      Map23_Regs[ 5 ] = ( Map23_Regs[ 5 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map23_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;   
      
    case 0xe000:
      Map23_Regs[ 6 ] = ( Map23_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map23_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe001:
    case 0xe004:
      Map23_Regs[ 6 ] = ( Map23_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map23_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe002:
    case 0xe008:
      Map23_Regs[ 7 ] = ( Map23_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map23_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe003:
    case 0xe00c:
      Map23_Regs[ 7 ] = ( Map23_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map23_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xf000:
      Map23_IRQ_Latch = ( Map23_IRQ_Latch & 0xf0 ) | ( byData & 0x0f );
      break;

    case 0xf004:
      Map23_IRQ_Latch = ( Map23_IRQ_Latch & 0xf0 ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xf008:
      Map23_IRQ_Enable = byData & 0x03;
      if ( Map23_IRQ_Enable & 0x02 )
      {
        Map23_IRQ_Cnt = Map23_IRQ_Latch;
      }
      break;

    case 0xf00c:
      if ( Map23_IRQ_Enable & 0x01 )
      {
        Map23_IRQ_Enable |= 0x02;
      } else {
        Map23_IRQ_Enable &= 0x01;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 23 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map23_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map23_IRQ_Enable & 0x02 )
  {
    if ( Map23_IRQ_Cnt == 0xff )
    {
      IRQ_REQ;

      Map23_IRQ_Cnt = Map23_IRQ_Latch;
      if ( Map23_IRQ_Enable & 0x01 )
      {
        Map23_IRQ_Enable |= 0x02;
      } else {
        Map23_IRQ_Enable &= 0x01;
      }
    } else {
      Map23_IRQ_Cnt++;
    }
  }
}
//InfoNES_Mapper_023 end











//InfoNES_Mapper_024 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 24 (Konami VRC6)                          */
/*                                                                   */
/*===================================================================*/

unsigned char Map24_IRQ_Count;
unsigned char Map24_IRQ_State;
unsigned char Map24_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 24                                             */
/*-------------------------------------------------------------------*/
void Map24_Init()
{
  /* Initialize Mapper */
  MapperInit = Map24_Init;

  /* Write to Mapper */
  MapperWrite = Map24_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map24_HSync;

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
/*  Mapper 24 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map24_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( ( byData + 0 ) % ( NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( ( byData + 1 ) % ( NesHeader.byRomSize << 1) );
      break;

    case 0xb003:
      /* Name Table Mirroring */
      switch ( byData & 0x0c )
      {
        case 0x00:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 0x04:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 0x08:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
        case 0x0c:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2400 */
          break;
      }
      break;

	  case 0xC000:
      ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
		  break;

	  case 0xD000:
      PPUBANK[ 0 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD001:
      PPUBANK[ 1 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD002:
      PPUBANK[ 2 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xD003:
      PPUBANK[ 3 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE000:
      PPUBANK[ 4 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE001:
      PPUBANK[ 5 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE002:
      PPUBANK[ 6 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xE003:
      PPUBANK[ 7 ] = VROMPAGE( byData % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

	  case 0xF000:
			Map24_IRQ_Latch = byData;
	  	break;

	  case 0xF001:
			Map24_IRQ_State = byData & 0x03;
			if(Map24_IRQ_State & 0x02)
			{
				Map24_IRQ_Count = Map24_IRQ_Latch;
			}
		  break;

	  case 0xF002:
			if(Map24_IRQ_State & 0x01)
			{
				Map24_IRQ_State |= 0x02;
			}
			else
			{
				Map24_IRQ_State &= 0x01;
			}
		break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 24 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map24_HSync()
{
/*
 *  Callback at HSync
 *
 */
	if(Map24_IRQ_State & 0x02)
	{
	  if(Map24_IRQ_Count == 0xFF)
		{
			IRQ_REQ;
			Map24_IRQ_Count = Map24_IRQ_Latch;
		}
		else
		{
			Map24_IRQ_Count++;
		}
	}
}
//InfoNES_Mapper_024 end













//InfoNES_Mapper_025 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 25 (Konami VRC4 type B)                   */
/*                                                                   */
/*===================================================================*/

unsigned char Map25_Bank_Selector;
unsigned char Map25_VBank[16];

unsigned char Map25_IRQ_Count;
unsigned char Map25_IRQ_State;
unsigned char Map25_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 25                                             */
/*-------------------------------------------------------------------*/
void Map25_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map25_Init;

  /* Write to Mapper */
  MapperWrite = Map25_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map25_HSync;

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

  /* Reset VBank Registers */
  for (nPage = 0; nPage < 16; nPage++)
    Map25_VBank[ nPage ] = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map25_Write( unsigned short int wAddr, unsigned char byData )
{
  int nBank;

  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      if ( Map25_Bank_Selector ) 
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0x9000:
      /* Name Table Mirroring */
      switch (byData & 0x03)
      {
        case 0:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 1:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 2:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2000 */
          break;
        case 3:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2400 */
          break;
      }
      break;

    case 0x9002:
      /* TODO: SaveRAM Toggle */
      /* $8000 Switching Mode */
      Map25_Bank_Selector = byData & 0x02;
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;
    
    default:
      /* Set PPU Banks */
      switch ( wAddr & 0xfffc)
      {
        case 0xb000:
          Map25_VBank[ 0 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 0 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;
  
        case 0xc000:
          Map25_VBank[ 4 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 2 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xd000:
          Map25_VBank[ 8 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 4 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xe000:
          Map25_VBank[ 12 + ( wAddr & 0x0003 ) ] = byData & 0x0f;
          nBank = 6 + ( wAddr & 0x0001 );
          Map25_Sync_Vrom( nBank );
          break;

        case 0xf000:
          switch ( wAddr & 0x0003 )
          {
            case 0:
            case 1:
              Map25_IRQ_Latch = byData;
              break;
            case 2:
              Map25_IRQ_State = ( byData & 0x01 ) ? Map25_IRQ_State : 0x00;
              Map25_IRQ_State = ( byData & 0x02 ) ? 0x01 : Map25_IRQ_State;
              Map25_IRQ_Count = Map25_IRQ_Latch;
              break;
            case 3:
              Map25_IRQ_State = ( Map25_IRQ_State << 1 ) | ( Map25_IRQ_State & 1 );
              break;
          }
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 Sync VROM Function                                     */
/*-------------------------------------------------------------------*/
void Map25_Sync_Vrom( int nBank )
{
  unsigned char byValue;
  
  byValue  = Map25_VBank[ ( nBank << 1 ) - ( nBank & 0x01 ) ];
  byValue |= Map25_VBank[ ( nBank << 1 ) - ( nBank & 0x01 ) + 2] << 4;
  byValue %= ( NesHeader.byVRomSize << 3 );
  PPUBANK[ nBank ] = VROMPAGE( byValue );
  InfoNES_SetupChr();
}

/*-------------------------------------------------------------------*/
/*  Mapper 25 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map25_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( Map25_IRQ_State & 0x02 ) && ( ++Map25_IRQ_Count == 0 ) )
  {
    IRQ_REQ;
    Map25_IRQ_Count = Map25_IRQ_Latch;
  }
}
//InfoNES_Mapper_025 end











//InfoNES_Mapper_026 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 26 (Konami VRC 6V)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map26_IRQ_Enable;
unsigned char Map26_IRQ_Cnt;
unsigned char Map26_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 26                                             */
/*-------------------------------------------------------------------*/
void Map26_Init()
{
  /* Initialize Mapper */
  MapperInit = Map26_Init;

  /* Write to Mapper */
  MapperWrite = Map26_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map26_HSync;

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
  Map26_IRQ_Enable = 0;
  Map26_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 26 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map26_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      byData <<= 1;      
      byData %= ( NesHeader.byRomSize << 1 );
      
      ROMBANK0 = ROMPAGE( byData + 0 );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    /* Name Table Mirroring */
    case 0xb003:  
      switch ( byData & 0x7f )
      {
        case 0x08:
        case 0x2c:
          InfoNES_Mirroring( 2 );   /* One Screen 0x2400 */
          break;
        case 0x20:
          InfoNES_Mirroring( 1 );   /* Vertical */
          break;
        case 0x24:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 0x28:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
      }
      break;

    /* Set ROM Banks */
    case 0xc000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );      
      InfoNES_SetupChr();
      break;

    /* Set PPU Bank */
    case 0xd000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    /* Set IRQ Registers */
    case 0xf000:
      Map26_IRQ_Latch = byData;
      break;

    case 0xf001:
      Map26_IRQ_Enable = byData & 0x01;
      break;

    case 0xf002:
      Map26_IRQ_Enable = byData & 0x03;

      if ( Map26_IRQ_Enable & 0x02 )
      {
        Map26_IRQ_Cnt = Map26_IRQ_Latch;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 26 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map26_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map26_IRQ_Enable & 0x03 )
  {
    if ( Map26_IRQ_Cnt >= 0xfe )
    {
      IRQ_REQ;
      Map26_IRQ_Cnt = Map26_IRQ_Latch;
      Map26_IRQ_Enable = 0;
    } else {
      Map26_IRQ_Cnt++;
    }
  }
}
//InfoNES_Mapper_026 end















//InfoNES_Mapper_032 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 32 (Irem G-101)                      */
/*                                                                   */
/*===================================================================*/

unsigned char Map32_Saved;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 32                                             */
/*-------------------------------------------------------------------*/
void Map32_Init()
{
  /* Initialize Mapper */
  MapperInit = Map32_Init;

  /* Write to Mapper */
  MapperWrite = Map32_Write;

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

  /* Initialize state flag */
  Map32_Saved = 0x00;

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
/*  Mapper 32 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map32_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf000 )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );

      if ( Map32_Saved & 0x02 ) 
      {
        ROMBANK2 = ROMPAGE( byData );
      } else {
        ROMBANK0 = ROMPAGE( byData );
      }
      break;
      
    case 0x9000:
      Map32_Saved = byData;
      
      // Name Table Mirroring
      InfoNES_Mirroring( byData & 0x01 );
      break;

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xb000:
      /* Set PPU Banks */
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ wAddr & 0x0007 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    default:
      break;
  }
}
//InfoNES_Mapper_032 end














//InfoNES_Mapper_033 begin
/*===================================================================*/
/*                                                                   */
/*                 Mapper 33 (Taito TC0190/TC0350)                   */
/*                                                                   */
/*===================================================================*/

unsigned char Map33_Regs[ 8 ];
unsigned char Map33_Switch;

unsigned char Map33_IRQ_Enable;
unsigned char Map33_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 33                                             */
/*-------------------------------------------------------------------*/
void Map33_Init()
{
  /* Initialize Mapper */
  MapperInit = Map33_Init;

  /* Write to Mapper */
  MapperWrite = Map33_Write;

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
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
      Map33_Regs[ nPage ] = nPage;
    }
    InfoNES_SetupChr();
  } 
  else 
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      Map33_Regs[ nPage ] = 0;
    }
  }

  /* Initialize State Registers */
  Map33_Switch = 0;
  Map33_IRQ_Enable = 0;
  Map33_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map33_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  switch ( wAddr )
  {
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8001:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x8002:
      Map33_Regs[ 0 ] = byData * 2;
      Map33_Regs[ 1 ] = byData * 2 + 1;

      PPUBANK[ 0 ] = VROMPAGE( Map33_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map33_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x8003:
      Map33_Regs[ 2 ] = byData * 2;
      Map33_Regs[ 3 ] = byData * 2 + 1;

      PPUBANK[ 2 ] = VROMPAGE( Map33_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map33_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa000:
      Map33_Regs[ 4 ] = byData;
      PPUBANK[ 4 ] = VROMPAGE( Map33_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa001:
      Map33_Regs[ 5 ] = byData;
      PPUBANK[ 5 ] = VROMPAGE( Map33_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa002:
      Map33_Regs[ 6 ] = byData;
      PPUBANK[ 6 ] = VROMPAGE( Map33_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa003:
      Map33_Regs[ 7 ] = byData;
      PPUBANK[ 7 ] = VROMPAGE( Map33_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map33_IRQ_Cnt = byData;
      break;

    case 0xc001:
    case 0xc002:
    case 0xe001:
    case 0xe002:
      if ( Map33_IRQ_Cnt == byData )
      {
        Map33_IRQ_Enable = 0xff;
      } else {
        Map33_IRQ_Enable = byData;
      }
      break;

    case 0xe000:
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 33 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map33_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map33_IRQ_Enable )
  {
    if ( Map33_IRQ_Enable == 0xff )
    {
      if ( PPU_Scanline == (unsigned short int)( 0xff - Map33_IRQ_Cnt ) )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      }
    } else {
      if ( Map33_IRQ_Cnt == 0xff )
      {
        IRQ_REQ;
        Map33_IRQ_Cnt = 0;
        Map33_IRQ_Enable = 0;
      } else {
        Map33_IRQ_Cnt++;
      }
    }
  }
}
//InfoNES_Mapper_033 end













//InfoNES_Mapper_034 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 34 (Nina-1)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 34                                             */
/*-------------------------------------------------------------------*/
void Map34_Init()
{
  /* Initialize Mapper */
  MapperInit = Map34_Init;

  /* Write to Mapper */
  MapperWrite = Map34_Write;

  /* Write to SRAM */
  MapperSram = Map34_Sram;

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
/*  Mapper 34 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map34_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  byData <<= 2;
  byData %= ( NesHeader.byRomSize << 1);

  ROMBANK0 = ROMPAGE( byData );
  ROMBANK1 = ROMPAGE( byData + 1 );
  ROMBANK2 = ROMPAGE( byData + 2 );
  ROMBANK3 = ROMPAGE( byData + 3 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 34 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map34_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch(wAddr)
  {
    /* Set ROM Banks */
    case 0x7ffd:
      byData <<= 2;
      byData %= ( NesHeader.byRomSize << 1 );

      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      ROMBANK2 = ROMPAGE( byData + 2 );
      ROMBANK3 = ROMPAGE( byData + 3 );
      break;

    /* Set PPU Banks */
    case 0x7ffe:
      byData <<= 2;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
      PPUBANK[ 3 ] = VROMPAGE( byData + 3 );
      InfoNES_SetupChr();
      break;

    /* Set PPU Banks */
    case 0x7fff:
      byData <<= 2;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 4 ] = VROMPAGE( byData );
      PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
      PPUBANK[ 7 ] = VROMPAGE( byData + 3 );
      InfoNES_SetupChr();
      break;
  }
}
//InfoNES_Mapper_034 end












//InfoNES_Mapper_040 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 40 (SMB2J)                           */
/*                                                                   */
/*===================================================================*/

unsigned char  Map40_IRQ_Enable;
unsigned int Map40_Line_To_IRQ;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 40                                             */
/*-------------------------------------------------------------------*/
void Map40_Init()
{
  /* Initialize Mapper */
  MapperInit = Map40_Init;

  /* Write to Mapper */
  MapperWrite = Map40_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map40_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 6 );

  /* Initialize IRQ Registers */
  Map40_IRQ_Enable = 0;
  Map40_Line_To_IRQ = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 4 );
  ROMBANK1 = ROMPAGE( 5 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 7 );

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
/*  Mapper 40 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map40_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xe000 )
  {
    case 0x8000:
      Map40_IRQ_Enable = 0;
      break;

    case 0xa000:
      Map40_IRQ_Enable = 1;
      Map40_Line_To_IRQ = 37;
      break;

    case 0xc000:
      break;

    case 0xe000:
      /* Set ROM Banks */
      ROMBANK2 = ROMPAGE ( ( byData & 0x07 ) % ( NesHeader.byRomSize << 1 ) );
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 40 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map40_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map40_IRQ_Enable )
  {
    if ( ( --Map40_Line_To_IRQ ) <= 0 )
    {
      IRQ_REQ;
    }
  }
}

/* End of InfoNES_Mapper_40.cpp */
//InfoNES_Mapper_040 end














//InfoNES_Mapper_041 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 41 ()                               */
/*                                                                   */
/*===================================================================*/

unsigned char Map41_Regs[ 2 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 41                                             */
/*-------------------------------------------------------------------*/
void Map41_Init()
{
  /* Initialize Mapper */
  MapperInit = Map41_Init;

  /* Write to Mapper */
  MapperWrite = Map41_Write;

  /* Write to SRAM */
  MapperSram = Map41_Sram;

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
/*  Mapper 41 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map41_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set PPU Banks */
  if ( Map41_Regs[ 0 ] )
  {
    unsigned char byChrBank;
    
    byChrBank = Map41_Regs[ 1 ] | ( byData & 0x0003 );
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
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 41 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map41_Sram( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byBank;

  if ( wAddr < 0x6800 )
  {
    byData = ( unsigned char )( wAddr & 0xff );

    /* Set CPU Banks */
    byBank = ( byData & 0x07 ) << 2;
    byBank %= ( NesHeader.byRomSize << 1 );

    ROMBANK0 = ROMPAGE( byBank );
    ROMBANK1 = ROMPAGE( byBank + 1 );
    ROMBANK2 = ROMPAGE( byBank + 2 );
    ROMBANK3 = ROMPAGE( byBank + 3 );

    Map41_Regs[ 0 ] = ( byData & 0x04 );
    Map41_Regs[ 1 ] = ( byData & 0x18 ) >> 1;

    /* Name Table Mirroring */
    if ( byData & 0x20 )
    {
      InfoNES_Mirroring( 0 );         /* Horizontal */
    } else {
      InfoNES_Mirroring( 1 );         /* Vertical */
    }
  }
}
//InfoNES_Mapper_041 end

















//InfoNES_Mapper_042 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 42 (Pirates)                           */
/*                                                                   */
/*===================================================================*/

unsigned char Map42_IRQ_Cnt;
unsigned char Map42_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 42                                             */
/*-------------------------------------------------------------------*/
void Map42_Init()
{
  /* Initialize Mapper */
  MapperInit = Map42_Init;

  /* Write to Mapper */
  MapperWrite = Map42_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map42_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 0 );

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 3 );
  ROMBANK1 = ROMLASTPAGE( 2 );
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
/*  Mapper 42 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map42_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xe003 )
  {
    /* Set ROM Banks */
    case 0xe000:
      SRAMBANK = ROMPAGE( ( byData & 0x0f ) % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0xe001:
      if ( byData & 0x08 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      break;

    case 0xe002:
      if ( byData & 0x02 )
      {
        Map42_IRQ_Enable = 1;
      } else {
        Map42_IRQ_Enable = 0;
        Map42_IRQ_Cnt = 0;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 42 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map42_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map42_IRQ_Enable )
  {
    if ( Map42_IRQ_Cnt < 215 )
    {
      Map42_IRQ_Cnt++;
    }
    if ( Map42_IRQ_Cnt == 215 )
    {
      IRQ_REQ;
      Map42_IRQ_Enable = 0;
    }
  }
}
//InfoNES_Mapper_042 end













//InfoNES_Mapper_043 begin
/*===================================================================*/
/*                                                                   */
/*                         Mapper 43 (SMB2J)                         */
/*                                                                   */
/*===================================================================*/

unsigned int Map43_IRQ_Cnt;
unsigned char Map43_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 43                                             */
/*-------------------------------------------------------------------*/
void Map43_Init()
{
  /* Initialize Mapper */
  MapperInit = Map43_Init;

  /* Write to Mapper */
  MapperWrite = Map43_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map43_Apu;

  /* Read from APU */
  MapperReadApu = Map43_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map43_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 2 );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 1 );
  ROMBANK1 = ROMPAGE( 0 );
  ROMBANK2 = ROMPAGE( 4 );
  ROMBANK3 = ROMPAGE( 9 );

  /* Initialize State Registers */
	Map43_IRQ_Enable = 1;
	Map43_IRQ_Cnt = 0;

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
/*  Mapper 43 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
unsigned char Map43_ReadApu( unsigned short int wAddr )
{
  if ( 0x5000 <= wAddr && wAddr < 0x6000 ) 
  {
    return ROM[ 0x2000*8 + 0x1000 + (wAddr - 0x5000) ];
  }
  return (unsigned char)(wAddr >> 8);
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map43_Apu( unsigned short int wAddr, unsigned char byData )
{
	if( (wAddr&0xF0FF) == 0x4022 ) 
  {
		switch( byData&0x07 ) 
    {
			case	0x00:
			case	0x02:
			case	0x03:
			case	0x04:
        ROMBANK2 = ROMPAGE( 4 );
				break;
			case	0x01:
        ROMBANK2 = ROMPAGE( 3 );
				break;
			case	0x05:
        ROMBANK2 = ROMPAGE( 7 );
				break;
			case	0x06:
        ROMBANK2 = ROMPAGE( 5 );
				break;
			case	0x07:
        ROMBANK2 = ROMPAGE( 6 );
				break;
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map43_Write( unsigned short int wAddr, unsigned char byData )
{
	if( wAddr == 0x8122 ) {
		if( byData&0x03 ) {
			Map43_IRQ_Enable = 1;
		} else {
			Map43_IRQ_Cnt = 0;
			Map43_IRQ_Enable = 0;
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 43 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map43_HSync()
{
	if( Map43_IRQ_Enable ) 
  {
		Map43_IRQ_Cnt += 114;
		if( Map43_IRQ_Cnt >= 4096 ) {
			Map43_IRQ_Cnt -= 4096;
			IRQ_REQ;
		}
	}
}
//InfoNES_Mapper_043 end














//InfoNES_Mapper_044 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 44 (Nin1)                            */
/*                                                                   */
/*===================================================================*/

unsigned char  Map44_Regs[ 8 ];
unsigned int Map44_Rom_Bank;
unsigned int Map44_Prg0, Map44_Prg1;
unsigned int Map44_Chr01, Map44_Chr23;
unsigned int Map44_Chr4, Map44_Chr5, Map44_Chr6, Map44_Chr7;

#define Map44_Chr_Swap()    ( Map44_Regs[ 0 ] & 0x80 )
#define Map44_Prg_Swap()    ( Map44_Regs[ 0 ] & 0x40 )

unsigned char Map44_IRQ_Enable;
unsigned char Map44_IRQ_Cnt;
unsigned char Map44_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 44                                             */
/*-------------------------------------------------------------------*/
void Map44_Init()
{
  /* Initialize Mapper */
  MapperInit = Map44_Init;

  /* Write to Mapper */
  MapperWrite = Map44_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map44_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map44_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map44_Rom_Bank = 0;
  Map44_Prg0 = 0;
  Map44_Prg1 = 1;
  Map44_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map44_Chr01 = 0;
    Map44_Chr23 = 2;
    Map44_Chr4  = 4;
    Map44_Chr5  = 5;
    Map44_Chr6  = 6;
    Map44_Chr7  = 7;
    Map44_Set_PPU_Banks();
  } else {
    Map44_Chr01 = Map44_Chr23 = 0;
    Map44_Chr4 = Map44_Chr5 = Map44_Chr6 = Map44_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map44_IRQ_Enable = 0;
  Map44_IRQ_Cnt = 0;
  Map44_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map44_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map44_Regs[ 0 ] = byData;
      Map44_Set_PPU_Banks();
      Map44_Set_CPU_Banks();
      break;

    case 0x8001:
      Map44_Regs[ 1 ] = byData;
      dwBankNum = Map44_Regs[ 1 ];

      switch ( Map44_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map44_Chr01 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map44_Chr23 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr4 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr5 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr6 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map44_Chr7 = dwBankNum;
            Map44_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map44_Prg0 = dwBankNum;
          Map44_Set_CPU_Banks();
          break;

        case 0x07:
          Map44_Prg1 = dwBankNum;
          Map44_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map44_Regs[ 2 ] = byData;

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
      Map44_Rom_Bank = byData & 0x07;
      if ( Map44_Rom_Bank == 7 )
      {
        Map44_Rom_Bank = 6;
      }
      Map44_Set_CPU_Banks();
      Map44_Set_PPU_Banks();
      break;

    case 0xc000:
      Map44_Regs[ 4 ] = byData;
      Map44_IRQ_Cnt = Map44_Regs[ 4 ];
      break;

    case 0xc001:
      Map44_Regs[ 5 ] = byData;
      Map44_IRQ_Latch = Map44_Regs[ 5 ];
      break;

    case 0xe000:
      Map44_Regs[ 6 ] = byData;
      Map44_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map44_Regs[ 7 ] = byData;
      Map44_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map44_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map44_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map44_IRQ_Cnt-- ) )
        {
          Map44_IRQ_Cnt = Map44_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map44_Set_CPU_Banks()
{
  if ( Map44_Prg_Swap() )
  {
    ROMBANK0 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    ROMBANK0 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + Map44_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map44_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 44 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map44_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map44_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map44_Rom_Bank << 7 ) + Map44_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_044 end
















#ifndef use_lib_not_use_mapper45
//InfoNES_Mapper_045 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 45 (Pirates)                         */
/*                                                                   */
/*===================================================================*/

unsigned char  Map45_Regs[7];
unsigned int Map45_P[4],Map45_Prg0,Map45_Prg1,Map45_Prg2,Map45_Prg3;
unsigned int Map45_C[4],Map45_Chr0, Map45_Chr1,Map45_Chr2, Map45_Chr3;
unsigned int Map45_Chr4, Map45_Chr5, Map45_Chr6, Map45_Chr7;

unsigned char Map45_IRQ_Enable;
unsigned char Map45_IRQ_Cnt;
unsigned char Map45_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 45                                             */
/*-------------------------------------------------------------------*/
void Map45_Init()
{
  /* Initialize Mapper */
  MapperInit = Map45_Init;

  /* Write to Mapper */
  MapperWrite = Map45_Write;

  /* Write to SRAM */
  MapperSram = Map45_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map45_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  Map45_Prg0 = 0;
  Map45_Prg1 = 1;
  Map45_Prg2 = NesHeader.byRomSize * 2 - 2;
  Map45_Prg3 = NesHeader.byRomSize * 2 - 1;

  ROMBANK0 = ROMPAGE( Map45_Prg0 );  Map45_P[0] = Map45_Prg0;
  ROMBANK1 = ROMPAGE( Map45_Prg1 );  Map45_P[1] = Map45_Prg1;
  ROMBANK2 = ROMPAGE( Map45_Prg2 );  Map45_P[2] = Map45_Prg2;
  ROMBANK3 = ROMPAGE( Map45_Prg3 );  Map45_P[3] = Map45_Prg3;
  
  /* Set PPU Banks */
  Map45_Chr0 = 0;  Map45_C[0] = Map45_Chr0;
  Map45_Chr1 = 1;  Map45_C[1] = Map45_Chr1;
  Map45_Chr2 = 2;  Map45_C[2] = Map45_Chr2;
  Map45_Chr3 = 3;  Map45_C[3] = Map45_Chr3;
  Map45_Chr4 = 4;  Map45_C[4] = Map45_Chr4;
  Map45_Chr5 = 5;  Map45_C[5] = Map45_Chr5;
  Map45_Chr6 = 6;  Map45_C[6] = Map45_Chr6;
  Map45_Chr7 = 7;  Map45_C[7] = Map45_Chr7;

  for ( int nPage = 0; nPage < 8; ++nPage ) {
    PPUBANK[ nPage ] = VROMPAGE( nPage );
  }
  InfoNES_SetupChr();
  
  /* Initialize IRQ Registers */
  Map45_IRQ_Enable = 0;
  Map45_IRQ_Cnt = 0;
  Map45_IRQ_Latch = 0;

  Map45_Regs[0] = Map45_Regs[1] = Map45_Regs[2] = Map45_Regs[3] = 0;
  Map45_Regs[4] = Map45_Regs[5] = Map45_Regs[6] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map45_Sram( unsigned short int wAddr, unsigned char byData )
{
	if(wAddr == 0x6000)
	{
		Map45_Regs[Map45_Regs[5]] = byData;
		Map45_Regs[5]= (Map45_Regs[5]+1) & 0x03;
		Map45_Set_CPU_Bank4((unsigned char)Map45_Prg0);
		Map45_Set_CPU_Bank5((unsigned char)Map45_Prg1);
		Map45_Set_CPU_Bank6((unsigned char)Map45_Prg2);
		Map45_Set_CPU_Bank7((unsigned char)Map45_Prg3);
		Map45_Set_PPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map45_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int swap;

	switch(wAddr & 0xE001) 
  {
    case 0x8000:
			if((byData & 0x40) != (Map45_Regs[6] & 0x40))
			{
				swap = Map45_Prg0; Map45_Prg0 = Map45_Prg2; Map45_Prg2 = swap;
				swap = Map45_P[0]; Map45_P[0] = Map45_P[2]; Map45_P[2] = swap;
        ROMBANK0 = ROMPAGE( Map45_P[0] % ( NesHeader.byRomSize << 1) );
        ROMBANK2 = ROMPAGE( Map45_P[2] % ( NesHeader.byRomSize << 1) );
			}
			if (NesHeader.byRomSize > 0)
			{
				if((byData & 0x80) != (Map45_Regs[6] & 0x80))
				{
					swap = Map45_Chr4; Map45_Chr4 = Map45_Chr0; Map45_Chr0 = swap;
					swap = Map45_Chr5; Map45_Chr5 = Map45_Chr1; Map45_Chr1 = swap;
					swap = Map45_Chr6; Map45_Chr6 = Map45_Chr2; Map45_Chr2 = swap;
					swap = Map45_Chr7; Map45_Chr7 = Map45_Chr3; Map45_Chr3 = swap;
					swap = Map45_C[4]; Map45_C[4] = Map45_C[0]; Map45_C[0] = swap;
					swap = Map45_C[5]; Map45_C[5] = Map45_C[1]; Map45_C[1] = swap;
					swap = Map45_C[6]; Map45_C[6] = Map45_C[2]; Map45_C[2] = swap;
					swap = Map45_C[7]; Map45_C[7] = Map45_C[3]; Map45_C[3] = swap;
					
          PPUBANK[ 0 ] = VROMPAGE( Map45_C[0] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 1 ] = VROMPAGE( Map45_C[1] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 2 ] = VROMPAGE( Map45_C[2] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 3 ] = VROMPAGE( Map45_C[3] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 4 ] = VROMPAGE( Map45_C[4] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 5 ] = VROMPAGE( Map45_C[5] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 6 ] = VROMPAGE( Map45_C[6] % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 7 ] = VROMPAGE( Map45_C[7] % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
				}
			}
			Map45_Regs[6] = byData;
		  break;

	case 0x8001:
	  switch(Map45_Regs[6] & 0x07)
		{
			case 0x00:
				Map45_Chr0 = (byData & 0xFE)+0;
				Map45_Chr1 = (byData & 0xFE)+1;
				Map45_Set_PPU_Banks();
				break;

			case 0x01:
			  Map45_Chr2 = (byData & 0xFE)+0;
				Map45_Chr3 = (byData & 0xFE)+1;
				Map45_Set_PPU_Banks();
				break;

			case 0x02:
				Map45_Chr4 = byData;
				Map45_Set_PPU_Banks();
				break;

			case 0x03:
				Map45_Chr5 = byData;
				Map45_Set_PPU_Banks();
				break;

			case 0x04:
				Map45_Chr6 = byData;
				Map45_Set_PPU_Banks();
				break;

			case 0x05:
				Map45_Chr7 = byData;
				Map45_Set_PPU_Banks();
				break;

      case 0x06:
				if(Map45_Regs[6] & 0x40)
				{
				  Map45_Prg2 = byData & 0x3F;
					Map45_Set_CPU_Bank6(byData);
				}
				else
				{
					Map45_Prg0 = byData & 0x3F;
					Map45_Set_CPU_Bank4(byData);
				}
				break;

			case 0x07:
				Map45_Prg1 = byData & 0x3F;
				Map45_Set_CPU_Bank5(byData);
				break;
		}
		break;

	case 0xA000:
     if ( byData & 0x01 )
     {
       InfoNES_Mirroring( 0 );
     } else {
       InfoNES_Mirroring( 1 );
     }
		break;

	case 0xC000:
	  Map45_IRQ_Cnt = byData;
		break;

	case 0xC001:
		Map45_IRQ_Latch = byData;
		break;

	case 0xE000:
		Map45_IRQ_Enable = 0;
		break;

	case 0xE001:
		Map45_IRQ_Enable = 1;
		break;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map45_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if(Map45_IRQ_Enable)
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
			{
				if(!(Map45_IRQ_Cnt--))
				{
					Map45_IRQ_Cnt = Map45_IRQ_Latch;
          IRQ_REQ;
				}
			}
		}
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/

void Map45_Set_CPU_Bank4(unsigned char byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
  ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
	Map45_P[0] = byData;
}

void Map45_Set_CPU_Bank5(unsigned char byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
  ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
	Map45_P[1] = byData;
}

void Map45_Set_CPU_Bank6(unsigned char byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
  ROMBANK2 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
	Map45_P[2] = byData;
}

void Map45_Set_CPU_Bank7(unsigned char byData)
{
	byData &= (Map45_Regs[3] & 0x3F) ^ 0xFF;
	byData &= 0x3F;
	byData |= Map45_Regs[1];
  ROMBANK3 = ROMPAGE( byData % ( NesHeader.byRomSize << 1) );
	Map45_P[3] = byData;
}

/*-------------------------------------------------------------------*/
/*  Mapper 45 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map45_Set_PPU_Banks()
{
	unsigned char table[16] =
	    {
	        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	        0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF
	    };
	Map45_C[0] = Map45_Chr0;
	Map45_C[1] = Map45_Chr1;
	Map45_C[2] = Map45_Chr2;
	Map45_C[3] = Map45_Chr3;
	Map45_C[4] = Map45_Chr4;
	Map45_C[5] = Map45_Chr5;
	Map45_C[6] = Map45_Chr6;
	Map45_C[7] = Map45_Chr7;
	for(unsigned char i = 0; i < 8; i++)
	{
		Map45_C[i] &= table[Map45_Regs[2] & 0x0F];
		Map45_C[i] |= Map45_Regs[0] & 0xff;
		Map45_C[i] += (unsigned char)(Map45_Regs[2] & 0x10) << 4;
	}
	if(Map45_Regs[6] & 0x80)
	{
    PPUBANK[ 0 ] = VROMPAGE( Map45_C[4] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( Map45_C[5] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( Map45_C[6] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( Map45_C[7] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( Map45_C[0] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( Map45_C[1] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( Map45_C[2] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( Map45_C[3] % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
	}
	else
	{
    PPUBANK[ 0 ] = VROMPAGE( Map45_C[0] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( Map45_C[1] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( Map45_C[2] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( Map45_C[3] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( Map45_C[4] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( Map45_C[5] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( Map45_C[6] % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( Map45_C[7] % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
	}
}
//InfoNES_Mapper_045 end
#endif


















//InfoNES_Mapper_046 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 46 (Color Dreams)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map46_Regs[ 4 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 46                                             */
/*-------------------------------------------------------------------*/
void Map46_Init()
{
  /* Initialize Mapper */
  MapperInit = Map46_Init;

  /* Write to Mapper */
  MapperWrite = Map46_Write;

  /* Write to SRAM */
  MapperSram = Map46_Sram;

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
  Map46_Regs[ 0 ] = Map46_Regs[ 1 ] = Map46_Regs[ 2 ] = Map46_Regs[ 3 ] = 0;
  Map46_Set_ROM_Banks();

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map46_Sram( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  Map46_Regs[ 0 ] = byData & 0x0f;
  Map46_Regs[ 1 ] = ( byData & 0xf0 ) >> 4;
  Map46_Set_ROM_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map46_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  Map46_Regs[ 2 ] = byData & 0x01;
  Map46_Regs[ 3 ] = ( byData & 0x70 ) >> 4;
  Map46_Set_ROM_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 46 Setup ROM Banks Function                               */
/*-------------------------------------------------------------------*/
void Map46_Set_ROM_Banks()
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );  
  ROMBANK1 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( ( Map46_Regs[ 0 ] << 3 ) + ( Map46_Regs[ 2 ] << 2 ) + 3 ) % ( NesHeader.byRomSize << 1 ) ); 

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 1 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 2 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 3 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 4 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 4 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 5 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 5 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 6 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 6 ) % ( NesHeader.byVRomSize << 3 ) ); 
  PPUBANK[ 7 ] = VROMPAGE( ( ( Map46_Regs[ 1 ] << 6 ) + ( Map46_Regs[ 3 ] << 3 ) + 7 ) % ( NesHeader.byVRomSize << 3 ) ); 
  InfoNES_SetupChr();
}
//InfoNES_Mapper_046 end













//InfoNES_Mapper_047 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 47 (MMC)                             */
/*                                                                   */
/*===================================================================*/

unsigned char  Map47_Regs[ 8 ];
unsigned int Map47_Rom_Bank;
unsigned int Map47_Prg0, Map47_Prg1;
unsigned int Map47_Chr01, Map47_Chr23;
unsigned int Map47_Chr4, Map47_Chr5, Map47_Chr6, Map47_Chr7;

#define Map47_Chr_Swap()    ( Map47_Regs[ 0 ] & 0x80 )
#define Map47_Prg_Swap()    ( Map47_Regs[ 0 ] & 0x40 )

unsigned char Map47_IRQ_Enable;
unsigned char Map47_IRQ_Cnt;
unsigned char Map47_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 47                                             */
/*-------------------------------------------------------------------*/
void Map47_Init()
{
  /* Initialize Mapper */
  MapperInit = Map47_Init;

  /* Write to Mapper */
  MapperWrite = Map47_Write;

  /* Write to SRAM */
  MapperSram = Map47_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map47_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map47_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map47_Rom_Bank = 0;
  Map47_Prg0 = 0;
  Map47_Prg1 = 1;
  Map47_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map47_Chr01 = 0;
    Map47_Chr23 = 2;
    Map47_Chr4  = 4;
    Map47_Chr5  = 5;
    Map47_Chr6  = 6;
    Map47_Chr7  = 7;
    Map47_Set_PPU_Banks();
  } else {
    Map47_Chr01 = Map47_Chr23 = 0;
    Map47_Chr4 = Map47_Chr5 = Map47_Chr6 = Map47_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map47_IRQ_Enable = 0;
  Map47_IRQ_Cnt = 0;
  Map47_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x6000:
      Map47_Rom_Bank = byData & 0x01;
      Map47_Set_CPU_Banks();
      Map47_Set_PPU_Banks();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map47_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map47_Regs[ 0 ] = byData;
      Map47_Set_PPU_Banks();
      Map47_Set_CPU_Banks();
      break;

    case 0x8001:
      Map47_Regs[ 1 ] = byData;
      dwBankNum = Map47_Regs[ 1 ];

      switch ( Map47_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map47_Chr01 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map47_Chr23 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr4 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr5 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr6 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map47_Chr7 = dwBankNum;
            Map47_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map47_Prg0 = dwBankNum;
          Map47_Set_CPU_Banks();
          break;

        case 0x07:
          Map47_Prg1 = dwBankNum;
          Map47_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map47_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map47_Regs[ 4 ] = byData;
      Map47_IRQ_Cnt = Map47_Regs[ 4 ];
      break;

    case 0xc001:
      Map47_Regs[ 5 ] = byData;
      Map47_IRQ_Latch = Map47_Regs[ 5 ];
      break;

    case 0xe000:
      Map47_Regs[ 6 ] = byData;
      Map47_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map47_Regs[ 7 ] = byData;
      Map47_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map47_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map47_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( --Map47_IRQ_Cnt ) )
        {
          Map47_IRQ_Cnt = Map47_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Set_CPU_Banks()
{
  if ( Map47_Prg_Swap() )
  {
    ROMBANK0 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  } else {
    ROMBANK0 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg0 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + Map47_Prg1 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 14 ) % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMPAGE( ( ( Map47_Rom_Bank << 4 ) + 15 ) % ( NesHeader.byRomSize << 1 ) );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 47 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map47_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map47_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr01 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 0 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + ( Map47_Chr23 + 1 ) ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( Map47_Rom_Bank << 7 ) + Map47_Chr7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_047 end














//InfoNES_Mapper_048 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 48 (Taito TC0190V)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map48_Regs[ 1 ];
unsigned char Map48_IRQ_Enable;
unsigned char Map48_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 48                                             */
/*-------------------------------------------------------------------*/
void Map48_Init()
{
  /* Initialize Mapper */
  MapperInit = Map48_Init;

  /* Write to Mapper */
  MapperWrite = Map48_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map48_HSync;

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
  Map48_Regs[ 0 ] = 0;
  Map48_IRQ_Enable = 0;
  Map48_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map48_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Name Table Mirroring */ 
      if ( !Map48_Regs[ 0 ] )
      {
        if ( byData & 0x40 )
        {
          InfoNES_Mirroring( 0 );
        } else {
          InfoNES_Mirroring( 1 );
        }
      }
      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      /* Set ROM Banks */
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
      break;  
 
    /* Set PPU Banks */
    case 0x8002:
      PPUBANK[ 0 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x8003:
      PPUBANK[ 2 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( byData << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa000:
      PPUBANK[ 4 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa001:
      PPUBANK[ 5 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa002:
      PPUBANK[ 6 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xa003:
      PPUBANK[ 7 ] = VROMPAGE( ( ( byData << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map48_IRQ_Cnt = byData;
      break;

    case 0xc001:
      Map48_IRQ_Enable = byData & 0x01;
      break;

    case 0xe000:
      /* Name Table Mirroring */ 
      if ( byData & 0x40 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      Map48_Regs[ 0 ] = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 48 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map48_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map48_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( Map48_IRQ_Cnt == 0xff )
        {
          IRQ_REQ;
          Map48_IRQ_Enable = 0;
        } else {
          Map48_IRQ_Cnt++;
        }
      }
    }
  }
}
//InfoNES_Mapper_048 end












//InfoNES_Mapper_049 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 49 (Nin1)                            */
/*                                                                   */
/*===================================================================*/

unsigned char  Map49_Regs[ 3 ];
unsigned int Map49_Prg0, Map49_Prg1;
unsigned int Map49_Chr01, Map49_Chr23;
unsigned int Map49_Chr4, Map49_Chr5, Map49_Chr6, Map49_Chr7;

unsigned char Map49_IRQ_Enable;
unsigned char Map49_IRQ_Cnt;
unsigned char Map49_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 49                                             */
/*-------------------------------------------------------------------*/
void Map49_Init()
{
  /* Initialize Mapper */
  MapperInit = Map49_Init;

  /* Write to Mapper */
  MapperWrite = Map49_Write;

  /* Write to SRAM */
  MapperSram = Map49_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map49_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  Map49_Prg0 = 0;
  Map49_Prg1 = 1;
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );
  
  /* Set PPU Banks */
  Map49_Chr01 = 0;
  Map49_Chr23 = 2;
  Map49_Chr4  = 4;
  Map49_Chr5  = 5;
  Map49_Chr6  = 6;
  Map49_Chr7  = 7;

  for ( int nPage = 0; nPage < 8; ++nPage )
  {
    PPUBANK[ nPage ] = VROMPAGE( nPage );
  }
  InfoNES_SetupChr();
  
  /* Initialize IRQ Registers */
  Map49_Regs[ 0 ] = Map49_Regs[ 1 ] = Map49_Regs[ 2 ] = 0;
  Map49_IRQ_Enable = 0;
  Map49_IRQ_Cnt = 0;
  Map49_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Sram( unsigned short int wAddr, unsigned char byData )
{
  if ( Map49_Regs[ 2 ] & 0x80 )
  {
    Map49_Regs[ 1 ] = byData;
    Map49_Set_CPU_Banks();
    Map49_Set_PPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map49_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      if ( ( byData & 0x40 ) != ( Map49_Regs[ 0 ] & 0x40 ) )
      {
        Map49_Set_CPU_Banks();
      }
      if ( ( byData & 0x80 ) != ( Map49_Regs[ 0 ] & 0x80 ) )
      {
        Map49_Regs[ 0 ] = byData;
        Map49_Set_PPU_Banks();
      }
      Map49_Regs[ 0 ] = byData;
      break;

    case 0x8001:
      switch ( Map49_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          Map49_Chr01 = byData & 0xfe;
          Map49_Set_PPU_Banks();
          break;

        case 0x01:
          Map49_Chr23 = byData & 0xfe;
          Map49_Set_PPU_Banks();
          break;

        case 0x02:
          Map49_Chr4 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x03:
          Map49_Chr5 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x04:
          Map49_Chr6 = byData;
          Map49_Set_PPU_Banks();
          break;

        case 0x05:
          Map49_Chr7 = byData;
          Map49_Set_PPU_Banks();
          break;

        /* Set ROM Banks */
        case 0x06:
          Map49_Prg0 = byData;
          Map49_Set_CPU_Banks();
          break;

        case 0x07:
          Map49_Prg1 = byData;
          Map49_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
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
      Map49_Regs[ 2 ] = byData;
      break;

    case 0xc000:
      Map49_IRQ_Cnt = byData;
      break;

    case 0xc001:
      Map49_IRQ_Latch = byData;
      break;

    case 0xe000:
      Map49_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map49_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map49_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map49_IRQ_Enable )
  {
    if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
    {
      if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map49_IRQ_Cnt-- ) )
        {
          Map49_IRQ_Cnt = Map49_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Set_CPU_Banks()
{
  unsigned int dwBank0, dwBank1, dwBank2, dwBank3;

  if ( Map49_Regs[ 1 ] & 0x01 )
  {
    if ( Map49_Regs[ 0 ] & 0x40 )
    {
      dwBank0 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0e ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank1 = ( Map49_Prg1 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank2 = ( Map49_Prg0 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank3 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
    } else {
      dwBank0 = ( Map49_Prg0 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank1 = ( Map49_Prg1 & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank2 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0e ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
      dwBank3 = ( ( ( NesHeader.byRomSize << 1 ) - 1 ) & 0x0f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) >> 2 );
    }
  } else {
    dwBank0 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 0;
    dwBank1 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 1;
    dwBank2 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 2;
    dwBank3 = ( ( Map49_Regs[ 1 ] & 0x70 ) >> 2 ) | 3;
  }

  /* Set ROM Banks */ 
  ROMBANK0 = ROMPAGE( dwBank0 % ( NesHeader.byRomSize << 1) );
  ROMBANK1 = ROMPAGE( dwBank1 % ( NesHeader.byRomSize << 1) );
  ROMBANK2 = ROMPAGE( dwBank2 % ( NesHeader.byRomSize << 1) );
  ROMBANK3 = ROMPAGE( dwBank3 % ( NesHeader.byRomSize << 1) );
}

/*-------------------------------------------------------------------*/
/*  Mapper 49 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map49_Set_PPU_Banks()
{
  Map49_Chr01 = ( Map49_Chr01 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr23 = ( Map49_Chr23 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr4 = ( Map49_Chr4 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr5 = ( Map49_Chr5 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr6 = ( Map49_Chr6 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );
  Map49_Chr7 = ( Map49_Chr7 & 0x7f ) | ( ( Map49_Regs[ 1 ] & 0xc0 ) << 1 );

  /* Set PPU Banks */ 
  if ( Map49_Regs[ 0 ] & 0x80 )
  { 
    PPUBANK[ 0 ] = VROMPAGE( Map49_Chr4 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( Map49_Chr5 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( Map49_Chr6 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( Map49_Chr7 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( ( Map49_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( ( Map49_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( ( Map49_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( ( Map49_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = VROMPAGE( ( Map49_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 1 ] = VROMPAGE( ( Map49_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 2 ] = VROMPAGE( ( Map49_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 3 ] = VROMPAGE( ( Map49_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 4 ] = VROMPAGE( Map49_Chr4 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 5 ] = VROMPAGE( Map49_Chr5 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 6 ] = VROMPAGE( Map49_Chr6 % ( NesHeader.byVRomSize << 3 ) );
    PPUBANK[ 7 ] = VROMPAGE( Map49_Chr7 % ( NesHeader.byVRomSize << 3 ) );
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_049 end













//InfoNES_Mapper_050 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 50 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map50_IRQ_Enable;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 50                                             */
/*-------------------------------------------------------------------*/
void Map50_Init()
{
  /* Initialize Mapper */
  MapperInit = Map50_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map50_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map50_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = ROMPAGE( 15 % ( NesHeader.byRomSize << 1 ) );

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 8 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( 9 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( 0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( 11 % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }    
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map50_IRQ_Enable = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 50 Write to Apu Function                                  */
/*-------------------------------------------------------------------*/
void Map50_Apu( unsigned short int wAddr, unsigned char byData )
{
  if ( ( wAddr & 0xE060 ) == 0x4020 )
  {
    if( wAddr & 0x0100 )
    {
      Map50_IRQ_Enable = byData & 0x01;
    }
    else
    {
      unsigned char byDummy;

      byDummy = ( byData & 0x08 ) | ( ( byData & 0x01 ) << 2 ) | ( ( byData & 0x06 ) >> 1 );
      ROMBANK2 = ROMPAGE( byDummy % ( NesHeader.byRomSize << 1 ) );
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 50 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map50_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map50_IRQ_Enable )
  {
    if ( PPU_Scanline == 21 )
    {
      IRQ_REQ;
    }
  }
}
//InfoNES_Mapper_050 end











//InfoNES_Mapper_051 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 51 : 11-in-1                           */
/*                                                                   */
/*===================================================================*/

int     Map51_Mode, Map51_Bank;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 51                                             */
/*-------------------------------------------------------------------*/
void Map51_Init()
{
  /* Initialize Mapper */
  MapperInit = Map51_Init;

  /* Write to Mapper */
  MapperWrite = Map51_Write;

  /* Write to SRAM */
  MapperSram = Map51_Sram;

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

  /* Set Registers */
  Map51_Bank = 0;
  Map51_Mode = 1;

  /* Set ROM Banks */
  Map51_Set_CPU_Banks();

  /* Set PPU Banks */
  for ( int nPage = 0; nPage < 8; ++nPage )
    PPUBANK[ nPage ] = CRAMPAGE( nPage );
  InfoNES_SetupChr();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map51_Write( unsigned short int wAddr, unsigned char byData )
{
  Map51_Bank = (byData & 0x0f) << 2;
  if( 0xC000 <= wAddr && wAddr <= 0xDFFF ) {
    Map51_Mode = (Map51_Mode & 0x01) | ((byData & 0x10) >> 3);
  }
  Map51_Set_CPU_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Sram( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr>=0x6000 ) {
    Map51_Mode = ((byData & 0x10) >> 3) | ((byData & 0x02) >> 1);
    Map51_Set_CPU_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 51 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map51_Set_CPU_Banks()
{
  switch(Map51_Mode) {
  case 0:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x2c|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x0c|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x0c|3) % (NesHeader.byRomSize<<1));
    break;
  case 1:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  case 2:
    InfoNES_Mirroring( 1 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x2e|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x02|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x02|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x0e|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x0e|3) % (NesHeader.byRomSize<<1));
    break;
  case 3:
    InfoNES_Mirroring( 0 );
    SRAMBANK = ROMPAGE((Map51_Bank|0x20|3) % (NesHeader.byRomSize<<1));
    ROMBANK0 = ROMPAGE((Map51_Bank|0x00|0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map51_Bank|0x00|1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((Map51_Bank|0x00|2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((Map51_Bank|0x00|3) % (NesHeader.byRomSize<<1));
    break;
  }
}
//InfoNES_Mapper_051 end














//InfoNES_Mapper_057 begin
/*===================================================================*/
/*                                                                   */
/*                           Mapper 57                               */
/*                                                                   */
/*===================================================================*/

unsigned char	Map57_Reg;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 57                                             */
/*-------------------------------------------------------------------*/
void Map57_Init()
{
  /* Initialize Mapper */
  MapperInit = Map57_Init;

  /* Write to Mapper */
  MapperWrite = Map57_Write;

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

  /* Set Registers */
  Map57_Reg = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 57 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map57_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byChr;

  switch( wAddr ) {
  case	0x8000:
  case	0x8001:
  case	0x8002:
  case	0x8003:
    if( byData & 0x40 ) {
      byChr = (byData&0x03)+((Map57_Reg&0x10)>>1)+(Map57_Reg&0x07);

      PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
    break;
  case	0x8800:
    Map57_Reg = byData;
    
    if( byData & 0x80 ) {
      ROMBANK0 = ROMPAGE((((byData & 0x40)>>6)*4+8+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData & 0x40)>>6)*4+8+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData & 0x40)>>6)*4+8+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData & 0x40)>>6)*4+8+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((((byData & 0x60)>>5)*2+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData & 0x60)>>5)*2+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData & 0x60)>>5)*2+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData & 0x60)>>5)*2+1) % (NesHeader.byRomSize<<1));
    }
    
    byChr = (byData&0x07)+((byData&0x10)>>1);
    
    PPUBANK[ 0 ] = VROMPAGE(((byChr<<3)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(((byChr<<3)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(((byChr<<3)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(((byChr<<3)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(((byChr<<3)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(((byChr<<3)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(((byChr<<3)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(((byChr<<3)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();

    if( byData & 0x08 ) InfoNES_Mirroring( 0 );
    else		InfoNES_Mirroring( 1 );
    
    break;
  }
}
//InfoNES_Mapper_057 end
