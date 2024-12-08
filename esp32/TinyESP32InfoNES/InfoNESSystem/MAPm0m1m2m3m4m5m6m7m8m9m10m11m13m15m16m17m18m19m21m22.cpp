#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"

//InfoNES_Mapper_000 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 0                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 0                                              */
/*-------------------------------------------------------------------*/
void Map0_Init()
{
  /* Initialize Mapper */
  MapperInit = Map0_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

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
  if ( NesHeader.byRomSize > 1 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 2 );
    ROMBANK3 = ROMPAGE( 3 );
  }
  else if ( NesHeader.byRomSize > 0 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 0 );
    ROMBANK3 = ROMPAGE( 1 );
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 0 );
    ROMBANK2 = ROMPAGE( 0 );
    ROMBANK3 = ROMPAGE( 0 );
  }

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
/*  Mapper 0 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map0_Write( unsigned short int wAddr, unsigned char byData )
{
/*
 *  Dummy Write to Mapper
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to SRAM Function                                  */
/*-------------------------------------------------------------------*/
void Map0_Sram( unsigned short int wAddr, unsigned char byData )
{
/*
 *  Dummy Write to Sram
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void Map0_Apu( unsigned short int wAddr, unsigned char byData )
{
/*
 *  Dummy Write to Apu
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Read from APU Function                                  */
/*-------------------------------------------------------------------*/
unsigned char Map0_ReadApu( unsigned short int wAddr )
{
/*
 *  Dummy Read from Apu
 *
 */
  return ( wAddr >> 8 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 V-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map0_VSync()
{
/*
 *  Dummy Callback at VSync
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map0_HSync()
{
/*
 *  Dummy Callback at HSync
 *
 */
#if 0 
  // Frame IRQ
  FrameStep += STEP_PER_SCANLINE;
  if ( FrameStep > STEP_PER_FRAME && FrameIRQ_Enable )
  {
    FrameStep %= STEP_PER_FRAME;
    IRQ_REQ;
    APU_Reg[ 0x4015 ] |= 0x40;
  }
#endif
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 PPU Function                                            */
/*-------------------------------------------------------------------*/
void Map0_PPU( unsigned short int wAddr )
{
/*
 *  Dummy Callback at PPU
 *
 */
}

/*-------------------------------------------------------------------*/
/*  Mapper 0 Rendering Screen Function                               */
/*-------------------------------------------------------------------*/
void Map0_RenderScreen( unsigned char byMode )
{
/*
 *  Dummy Callback at Rendering Screen
 *
 */
}
//InfoNES_Mapper_000 end









//InfoNES_Mapper_001 begin
/*===================================================================*/
/*                                                                   */
/*                            Mapper 1                               */
/*                                                                   */
/*===================================================================*/

unsigned char  Map1_Regs[ 4 ];
unsigned int Map1_Cnt;
unsigned char  Map1_Latch;
unsigned short int  Map1_Last_Write_Addr;

enum Map1_Size_t
{
  Map1_SMALL,
  Map1_512K,
  Map1_1024K
};

Map1_Size_t Map1_Size;
unsigned int Map1_256K_base;
unsigned int Map1_swap;

// these are the 4 ROM banks currently selected
unsigned int Map1_bank1;
unsigned int Map1_bank2;
unsigned int Map1_bank3;
unsigned int Map1_bank4;

unsigned int Map1_HI1;
unsigned int Map1_HI2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 1                                              */
/*-------------------------------------------------------------------*/
void Map1_Init()
{
  unsigned int size_in_K;

  /* Initialize Mapper */
  MapperInit = Map1_Init;

  /* Write to Mapper */
  MapperWrite = Map1_Write;

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

  /* Initialize State Registers */
  Map1_Cnt = 0;
  Map1_Latch = 0x00;

  Map1_Regs[ 0 ] = 0x0c;
  Map1_Regs[ 1 ] = 0x00;
  Map1_Regs[ 2 ] = 0x00;
  Map1_Regs[ 3 ] = 0x00;

  size_in_K = ( NesHeader.byRomSize << 1 ) * 8;

  if ( size_in_K == 1024 )
  {
    Map1_Size = Map1_1024K;
  } 
  else if(size_in_K == 512)
  {
    Map1_Size = Map1_512K;
  }
  else
  {
    Map1_Size = Map1_SMALL;
  }

  Map1_256K_base = 0; // use first 256K
  Map1_swap = 0;

  if( Map1_Size == Map1_SMALL )
  {
    // set two high pages to last two banks
    Map1_HI1 = ( NesHeader.byRomSize << 1 ) - 2;
    Map1_HI2 = ( NesHeader.byRomSize << 1 ) - 1;
  }
  else
  {
    // set two high pages to last two banks of current 256K region
    Map1_HI1 = ( 256 / 8 ) - 2;
    Map1_HI2 = ( 256 / 8 ) - 1;
  }

  // set CPU bank pointers
  Map1_bank1 = 0;
  Map1_bank2 = 1;
  Map1_bank3 = Map1_HI1;
  Map1_bank4 = Map1_HI2;

  /* Set ROM Banks */
  Map1_set_ROM_banks();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

void Map1_set_ROM_banks()
{
  ROMBANK0 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank1 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );  
  ROMBANK1 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank2 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank3 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( (Map1_256K_base << 5) + (Map1_bank4 & ((256/8)-1)) ) % ( NesHeader.byRomSize << 1 ) ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 1 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map1_Write( unsigned short int wAddr, unsigned char byData )
{
/*
 * MMC1
 */
  unsigned int dwRegNum;

  // if write is to a different reg, reset
  if( ( wAddr & 0x6000 ) != ( Map1_Last_Write_Addr & 0x6000 ) )
  {
    Map1_Cnt = 0;
    Map1_Latch = 0x00;
  }
  Map1_Last_Write_Addr = wAddr;

  // if bit 7 set, reset and return
  if ( byData & 0x80 )
  {
    Map1_Cnt = 0;
    Map1_Latch = 0x00;
    return;
  }

  if ( byData & 0x01 ) Map1_Latch |= ( 1 << Map1_Cnt );
  Map1_Cnt++;
  if( Map1_Cnt < 5 ) return;

  dwRegNum = ( wAddr & 0x7FFF ) >> 13;
  Map1_Regs[ dwRegNum ] = Map1_Latch;

  Map1_Cnt = 0;
  Map1_Latch = 0x00;

  switch( dwRegNum )
  {
    case 0:
      {
        // set mirroring
        if( Map1_Regs[0] & 0x02 )
        {
          if( Map1_Regs[0] & 0x01 )
          {
            InfoNES_Mirroring( 0 );
          }
          else
          {
            InfoNES_Mirroring( 1 );
          }
        }
        else
        {
          // one-screen mirroring
          if( Map1_Regs[0] & 0x01 )
          {
            InfoNES_Mirroring( 2 );
          }
          else
          {
            InfoNES_Mirroring( 3 );
          }
        }
      }
      break;

    case 1:
      {
        unsigned char byBankNum = Map1_Regs[1];

        if ( Map1_Size == Map1_1024K )
        {
          if ( Map1_Regs[0] & 0x10 )
          {
            if ( Map1_swap )
            {
              Map1_256K_base = (Map1_Regs[1] & 0x10) >> 4;
              if(Map1_Regs[0] & 0x08)
              {
                Map1_256K_base |= ((Map1_Regs[2] & 0x10) >> 3);
              }
              Map1_set_ROM_banks();
              Map1_swap = 0;
            }
            else
            {
              Map1_swap = 1;
            }
          }
          else
          {
            // use 1st or 4th 256K banks
            Map1_256K_base = ( Map1_Regs[1] & 0x10 ) ? 3 : 0;
            Map1_set_ROM_banks();
          }
        }
        else if((Map1_Size == Map1_512K) && (!NesHeader.byVRomSize))
        {
          Map1_256K_base = (Map1_Regs[1] & 0x10) >> 4;
          Map1_set_ROM_banks();
        }
        else if ( NesHeader.byVRomSize )
        {
          // set VROM bank at $0000
          if ( Map1_Regs[0] & 0x10 )
          {
            // swap 4K
            byBankNum <<= 2;
            PPUBANK[ 0 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 1 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 2 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 3 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
            InfoNES_SetupChr();
          }
          else
          {
            // swap 8K
            byBankNum <<= 2;
            PPUBANK[ 0 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 1 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 2 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 3 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 4 ] = VROMPAGE( (byBankNum+4) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 5 ] = VROMPAGE( (byBankNum+5) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 6 ] = VROMPAGE( (byBankNum+6) % (NesHeader.byVRomSize << 3) );
            PPUBANK[ 7 ] = VROMPAGE( (byBankNum+7) % (NesHeader.byVRomSize << 3) );
            InfoNES_SetupChr();
          }
        }
      }
      break;

    case 2:
      {
        unsigned char byBankNum = Map1_Regs[2];

        if((Map1_Size == Map1_1024K) && (Map1_Regs[0] & 0x08))
        {
          if(Map1_swap)
          {
            Map1_256K_base =  (Map1_Regs[1] & 0x10) >> 4;
            Map1_256K_base |= ((Map1_Regs[2] & 0x10) >> 3);
            Map1_set_ROM_banks();
            Map1_swap = 0;
          }
          else
          {
            Map1_swap = 1;
          }
        }

        if(!NesHeader.byVRomSize) 
        {
          if ( Map1_Regs[ 0 ] & 0x10 )
          {
            byBankNum <<= 2;
#if 0
            PPUBANK[ 4 ] = VRAMPAGE0( byBankNum+0 );
            PPUBANK[ 5 ] = VRAMPAGE0( byBankNum+1 );
            PPUBANK[ 6 ] = VRAMPAGE0( byBankNum+2 );
            PPUBANK[ 7 ] = VRAMPAGE0( byBankNum+3 );
#else
            PPUBANK[ 4 ] = CRAMPAGE( byBankNum+0 );
            PPUBANK[ 5 ] = CRAMPAGE( byBankNum+1 );
            PPUBANK[ 6 ] = CRAMPAGE( byBankNum+2 );
            PPUBANK[ 7 ] = CRAMPAGE( byBankNum+3 );
#endif
            InfoNES_SetupChr();
            break;
          }
        }

        // set 4K VROM bank at $1000
        if(Map1_Regs[0] & 0x10)
        {
          // swap 4K
          byBankNum <<= 2;
          PPUBANK[ 4 ] = VROMPAGE( (byBankNum+0) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 5 ] = VROMPAGE( (byBankNum+1) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 6 ] = VROMPAGE( (byBankNum+2) % (NesHeader.byVRomSize << 3) );
          PPUBANK[ 7 ] = VROMPAGE( (byBankNum+3) % (NesHeader.byVRomSize << 3) );
          InfoNES_SetupChr(); 
        }
      }
      break;

    case 3:
      {
        unsigned char byBankNum = Map1_Regs[3];

        // set ROM bank
        if ( Map1_Regs[0] & 0x08 )
        {
          // 16K of ROM
          byBankNum <<= 1;

          if ( Map1_Regs[0] & 0x04 )
          {
            // 16K of ROM at $8000
            Map1_bank1 = byBankNum;
            Map1_bank2 = byBankNum+1;
            Map1_bank3 = Map1_HI1;
            Map1_bank4 = Map1_HI2;
          }
          else
          {
            // 16K of ROM at $C000
            if(Map1_Size == Map1_SMALL)
            {
              Map1_bank1 = 0;
              Map1_bank2 = 1;
              Map1_bank3 = byBankNum;
              Map1_bank4 = byBankNum+1;
            }
          }
        }
        else
        {
          // 32K of ROM at $8000
          byBankNum <<= 1;

          Map1_bank1 = byBankNum;
          Map1_bank2 = byBankNum+1;
          if(Map1_Size == Map1_SMALL)
          {
            Map1_bank3 = byBankNum+2;
            Map1_bank4 = byBankNum+3;
          }
        }
        Map1_set_ROM_banks();
      }
      break;
  }
}
//InfoNES_Mapeer_001 end








//InfoNES_Mapper_002 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 2 (UNROM)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 2                                              */
/*-------------------------------------------------------------------*/
void Map2_Init()
{
  /* Initialize Mapper */
  MapperInit = Map2_Init;

  /* Write to Mapper */
  MapperWrite = Map2_Write;

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
/*  Mapper 2 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map2_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  byData %= NesHeader.byRomSize;
  byData <<= 1;

  ROMBANK0 = ROMPAGE( byData );
  ROMBANK1 = ROMPAGE( byData + 1 );
}
//InfoNES_Mapper_002 end







//InfoNES_Mapper_003 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 3 (VROM Switch)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 3                                              */
/*-------------------------------------------------------------------*/
void Map3_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map3_Init;

  /* Write to Mapper */
  MapperWrite = Map3_Write;

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
  if ( ( NesHeader.byRomSize << 1 ) > 2 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 2 );
    ROMBANK3 = ROMPAGE( 3 );    
  } else {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 0 );
    ROMBANK3 = ROMPAGE( 1 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  /* "DragonQuest" doesn't run if IRQ isn't made to occur in CLI */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 3 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map3_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBase;

  /* Set PPU Banks */
  byData %= NesHeader.byVRomSize;
  dwBase = ( (unsigned int)byData ) << 3;

  PPUBANK[ 0 ] = VROMPAGE( dwBase + 0 );
  PPUBANK[ 1 ] = VROMPAGE( dwBase + 1 );
  PPUBANK[ 2 ] = VROMPAGE( dwBase + 2 );
  PPUBANK[ 3 ] = VROMPAGE( dwBase + 3 );
  PPUBANK[ 4 ] = VROMPAGE( dwBase + 4 );
  PPUBANK[ 5 ] = VROMPAGE( dwBase + 5 );
  PPUBANK[ 6 ] = VROMPAGE( dwBase + 6 );
  PPUBANK[ 7 ] = VROMPAGE( dwBase + 7 );

  InfoNES_SetupChr();
}
//InfoNES_Mapper_003 end






//InfoNES_Mapper_004 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 4 (MMC3)                             */
/*                                                                   */
/*===================================================================*/

unsigned char  Map4_Regs[ 8 ];
unsigned int Map4_Rom_Bank;
unsigned int Map4_Prg0, Map4_Prg1;
unsigned int Map4_Chr01, Map4_Chr23;
unsigned int Map4_Chr4, Map4_Chr5, Map4_Chr6, Map4_Chr7;

#define Map4_Chr_Swap()    ( Map4_Regs[ 0 ] & 0x80 )
#define Map4_Prg_Swap()    ( Map4_Regs[ 0 ] & 0x40 )

unsigned char Map4_IRQ_Enable;
unsigned char Map4_IRQ_Cnt;
unsigned char Map4_IRQ_Latch;
unsigned char Map4_IRQ_Request;
unsigned char Map4_IRQ_Present;
unsigned char Map4_IRQ_Present_Vbl;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 4                                              */
/*-------------------------------------------------------------------*/
void Map4_Init()
{
  /* Initialize Mapper */
  MapperInit = Map4_Init;

  /* Write to Mapper */
  MapperWrite = Map4_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map4_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map4_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map4_Prg0 = 0;
  Map4_Prg1 = 1;
  Map4_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map4_Chr01 = 0;
    Map4_Chr23 = 2;
    Map4_Chr4  = 4;
    Map4_Chr5  = 5;
    Map4_Chr6  = 6;
    Map4_Chr7  = 7;
    Map4_Set_PPU_Banks();
  } else {
    Map4_Chr01 = Map4_Chr23 = 0;
    Map4_Chr4 = Map4_Chr5 = Map4_Chr6 = Map4_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map4_IRQ_Enable = 0;
  Map4_IRQ_Cnt = 0;
  Map4_IRQ_Latch = 0;
  Map4_IRQ_Request = 0;
  Map4_IRQ_Present = 0;
  Map4_IRQ_Present_Vbl = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map4_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map4_Regs[ 0 ] = byData;
      Map4_Set_PPU_Banks();
      Map4_Set_CPU_Banks();
      break;

    case 0x8001:
      Map4_Regs[ 1 ] = byData;
      dwBankNum = Map4_Regs[ 1 ];

      switch ( Map4_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map4_Chr01 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map4_Chr23 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr4 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr5 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr6 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map4_Chr7 = dwBankNum;
            Map4_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map4_Prg0 = dwBankNum;
          Map4_Set_CPU_Banks();
          break;

        case 0x07:
          Map4_Prg1 = dwBankNum;
          Map4_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map4_Regs[ 2 ] = byData;

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
      Map4_Regs[ 3 ] = byData;
      break;

    case 0xc000:
      Map4_Regs[ 4 ] = byData;
      Map4_IRQ_Latch = byData;
      break;

    case 0xc001:
      Map4_Regs[ 5 ] = byData;
      if ( PPU_Scanline < 240 )
      {
          Map4_IRQ_Cnt |= 0x80;
          Map4_IRQ_Present = 0xff;
      } else {
          Map4_IRQ_Cnt |= 0x80;
          Map4_IRQ_Present_Vbl = 0xff;
          Map4_IRQ_Present = 0;
      }
      break;

    case 0xe000:
      Map4_Regs[ 6 ] = byData;
      Map4_IRQ_Enable = 0;
			Map4_IRQ_Request = 0;
      break;

    case 0xe001:
      Map4_Regs[ 7 ] = byData;
      Map4_IRQ_Enable = 1;
			Map4_IRQ_Request = 0;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map4_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 ) && 
       ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) )
  {
		if( Map4_IRQ_Present_Vbl ) {
			Map4_IRQ_Cnt = Map4_IRQ_Latch;
			Map4_IRQ_Present_Vbl = 0;
		}
		if( Map4_IRQ_Present ) {
			Map4_IRQ_Cnt = Map4_IRQ_Latch;
			Map4_IRQ_Present = 0;
		} else if( Map4_IRQ_Cnt > 0 ) {
			Map4_IRQ_Cnt--;
		}

		if( Map4_IRQ_Cnt == 0 ) {
			if( Map4_IRQ_Enable ) {
				Map4_IRQ_Request = 0xFF;
			}
			Map4_IRQ_Present = 0xFF;
		}
	}
	if( Map4_IRQ_Request  ) {
		IRQ_REQ;
	}
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Set CPU Banks Function                                  */
/*-------------------------------------------------------------------*/
void Map4_Set_CPU_Banks()
{
  if ( Map4_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map4_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map4_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map4_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map4_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 4 Set PPU Banks Function                                  */
/*-------------------------------------------------------------------*/
void Map4_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map4_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map4_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map4_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map4_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map4_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map4_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map4_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map4_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map4_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map4_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map4_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map4_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map4_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map4_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map4_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map4_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map4_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
  else
  {
    if ( Map4_Chr_Swap() )
    { 
#if 0
      PPUBANK[ 0 ] = VRAMPAGE0( 0 );
      PPUBANK[ 1 ] = VRAMPAGE0( 1 );
      PPUBANK[ 2 ] = VRAMPAGE0( 2 );
      PPUBANK[ 3 ] = VRAMPAGE0( 3 );
      PPUBANK[ 4 ] = VRAMPAGE1( 0 );
      PPUBANK[ 5 ] = VRAMPAGE1( 1 );
      PPUBANK[ 6 ] = VRAMPAGE1( 2 );
      PPUBANK[ 7 ] = VRAMPAGE1( 3 );
#else
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
#endif
      InfoNES_SetupChr();
    } else {
#if 0
      PPUBANK[ 0 ] = VRAMPAGE1( 0 );
      PPUBANK[ 1 ] = VRAMPAGE1( 1 );
      PPUBANK[ 2 ] = VRAMPAGE1( 2 );
      PPUBANK[ 3 ] = VRAMPAGE1( 3 );
      PPUBANK[ 4 ] = VRAMPAGE0( 0 );
      PPUBANK[ 5 ] = VRAMPAGE0( 1 );
      PPUBANK[ 6 ] = VRAMPAGE0( 2 );
      PPUBANK[ 7 ] = VRAMPAGE0( 3 );
#else
      PPUBANK[ 0 ] = CRAMPAGE( 0 );
      PPUBANK[ 1 ] = CRAMPAGE( 1 );
      PPUBANK[ 2 ] = CRAMPAGE( 2 );
      PPUBANK[ 3 ] = CRAMPAGE( 3 );
      PPUBANK[ 4 ] = CRAMPAGE( 4 );
      PPUBANK[ 5 ] = CRAMPAGE( 5 );
      PPUBANK[ 6 ] = CRAMPAGE( 6 );
      PPUBANK[ 7 ] = CRAMPAGE( 7 );
#endif
      InfoNES_SetupChr();
    }
  }    
}
//InfoNES_Mapper_004 end






#ifndef use_lib_not_use_mapper5
//InfoNES_Mapper_005 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 5 (MMC5)                            */
/*                                                                   */
/*===================================================================*/

unsigned char Map5_Wram[ 0x2000 * 8 ];
unsigned char Map5_Ex_Ram[ 0x400 ]; 
unsigned char Map5_Ex_Vram[ 0x400 ];
unsigned char Map5_Ex_Nam[ 0x400 ];

unsigned char Map5_Prg_Reg[ 8 ];
unsigned char Map5_Wram_Reg[ 8 ];
unsigned char Map5_Chr_Reg[ 8 ][ 2 ];

unsigned char Map5_IRQ_Enable;
unsigned char Map5_IRQ_Status;
unsigned char Map5_IRQ_Line;

unsigned int Map5_Value0;
unsigned int Map5_Value1;

unsigned char Map5_Wram_Protect0;
unsigned char Map5_Wram_Protect1;
unsigned char Map5_Prg_Size;
unsigned char Map5_Chr_Size;
unsigned char Map5_Gfx_Mode;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 5                                              */
/*-------------------------------------------------------------------*/
void Map5_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map5_Init;

  /* Write to Mapper */
  MapperWrite = Map5_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map5_Apu;

  /* Read from APU */
  MapperReadApu = Map5_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map5_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map5_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMLASTPAGE( 0 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMLASTPAGE( 0 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  for ( nPage = 0; nPage < 8; ++nPage )
    PPUBANK[ nPage ] = VROMPAGE( nPage );
  InfoNES_SetupChr();

  /* Initialize State Registers */
  for ( nPage = 4; nPage < 8; ++nPage )
  {
    Map5_Prg_Reg[ nPage ] = ( NesHeader.byRomSize << 1 ) - 1;
    Map5_Wram_Reg[ nPage ] = 0xff;
  }
  Map5_Wram_Reg[ 3 ] = 0xff;

  for ( unsigned char byPage = 4; byPage < 8; ++byPage )
  {
    Map5_Chr_Reg[ byPage ][ 0 ] = byPage;
    Map5_Chr_Reg[ byPage ][ 1 ] = ( byPage & 0x03 ) + 4;
  }

  InfoNES_MemorySet( Map5_Wram, 0x00, sizeof( Map5_Wram ) );
  InfoNES_MemorySet( Map5_Ex_Ram, 0x00, sizeof( Map5_Ex_Ram ) );
  InfoNES_MemorySet( Map5_Ex_Vram, 0x00, sizeof( Map5_Ex_Vram ) );
  InfoNES_MemorySet( Map5_Ex_Nam, 0x00, sizeof( Map5_Ex_Nam ) );

  Map5_Prg_Size = 3;
  Map5_Wram_Protect0 = 0;
  Map5_Wram_Protect1 = 0;
  Map5_Chr_Size = 3;
  Map5_Gfx_Mode = 0;

  Map5_IRQ_Enable = 0;
  Map5_IRQ_Status = 0;
  Map5_IRQ_Line = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Read from APU Function                                  */
/*-------------------------------------------------------------------*/
unsigned char Map5_ReadApu( unsigned short int wAddr )
{
  unsigned char byRet = (unsigned char)( wAddr >> 8 );

  switch ( wAddr )
  {
    case 0x5204:
      byRet = Map5_IRQ_Status;
      Map5_IRQ_Status = 0;
      break;

    case 0x5205:
      byRet = (unsigned char)( ( Map5_Value0 * Map5_Value1 ) & 0x00ff );
      break;

    case 0x5206:
      byRet = (unsigned char)( ( ( Map5_Value0 * Map5_Value1 ) & 0xff00 ) >> 8 );
      break;

    default:
      if ( 0x5c00 <= wAddr && wAddr <= 0x5fff )
      {
        byRet = Map5_Ex_Ram[ wAddr - 0x5c00 ];
      }
      break;
  }
  return byRet;
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void Map5_Apu( unsigned short int wAddr, unsigned char byData )
{
  int nPage;

  switch ( wAddr )
  {
    case 0x5100:
      Map5_Prg_Size = byData & 0x03;
      break;

    case 0x5101:
      Map5_Chr_Size = byData & 0x03;
      break;

    case 0x5102:
      Map5_Wram_Protect0 = byData & 0x03;
      break;

    case 0x5103:
      Map5_Wram_Protect1 = byData & 0x03;
      break;

    case 0x5104:
      Map5_Gfx_Mode = byData & 0x03;
      break;

    case 0x5105:
      for ( nPage = 0; nPage < 4; nPage++ )
      {
        unsigned char byNamReg;
        
        byNamReg = byData & 0x03;
        byData = byData >> 2;

        switch ( byNamReg )
        {
          case 0:
#if 1
            PPUBANK[ nPage + 8 ] = VRAMPAGE( 0 );
#else
            PPUBANK[ nPage + 8 ] = CRAMPAGE( 8 );
#endif
            break;
          case 1:
#if 1
            PPUBANK[ nPage + 8 ] = VRAMPAGE( 1 );
#else
            PPUBANK[ nPage + 8 ] = CRAMPAGE( 9 );
#endif
            break;
          case 2:
            PPUBANK[ nPage + 8 ] = Map5_Ex_Vram;
            break;
          case 3:
            PPUBANK[ nPage + 8 ] = Map5_Ex_Nam;
            break;
        }
      }
      break;

    case 0x5106:
      InfoNES_MemorySet( Map5_Ex_Nam, byData, 0x3c0 );
      break;

    case 0x5107:
      byData &= 0x03;
      byData = byData | ( byData << 2 ) | ( byData << 4 ) | ( byData << 6 );
      InfoNES_MemorySet( &( Map5_Ex_Nam[ 0x3c0 ] ), byData, 0x400 - 0x3c0 );
      break;

    case 0x5113:
      Map5_Wram_Reg[ 3 ] = byData & 0x07;
      SRAMBANK = Map5_ROMPAGE( byData & 0x07 );
      break;

    case 0x5114:
    case 0x5115:
    case 0x5116:
    case 0x5117:
      Map5_Prg_Reg[ wAddr & 0x07 ] = byData;
      Map5_Sync_Prg_Banks();
      break;

    case 0x5120:
    case 0x5121:
    case 0x5122:
    case 0x5123:
    case 0x5124:
    case 0x5125:
    case 0x5126:
    case 0x5127:
      Map5_Chr_Reg[ wAddr & 0x07 ][ 0 ] = byData;
      Map5_Sync_Prg_Banks();
      break;

    case 0x5128:
    case 0x5129:
    case 0x512a:
    case 0x512b:
      Map5_Chr_Reg[ ( wAddr & 0x03 ) + 0 ][ 1 ] = byData;
      Map5_Chr_Reg[ ( wAddr & 0x03 ) + 4 ][ 1 ] = byData;
      break;

    case 0x5200:
    case 0x5201:
    case 0x5202:
      /* Nothing to do */
      break;

    case 0x5203:
      if ( Map5_IRQ_Line >= 0x40 )
      {
        Map5_IRQ_Line = byData;
      } else {
        Map5_IRQ_Line += byData;
      }
      break;

    case 0x5204:
      Map5_IRQ_Enable = byData;
      break;

    case 0x5205:
      Map5_Value0 = byData;
      break;

    case 0x5206:
      Map5_Value1 = byData;
      break;

    default:
      if ( 0x5000 <= wAddr && wAddr <= 0x5015 )
      {
        /* Extra Sound */
      } else 
      if ( 0x5c00 <= wAddr && wAddr <= 0x5fff )
      {
        switch ( Map5_Gfx_Mode )
        {
          case 0:
            Map5_Ex_Vram[ wAddr - 0x5c00 ] = byData;
            break;
          case 2:
            Map5_Ex_Ram[ wAddr - 0x5c00 ] = byData;
            break;
        }
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Write to SRAM Function                                  */
/*-------------------------------------------------------------------*/
void Map5_Sram( unsigned short int wAddr, unsigned char byData ) //JJ void Map5_Sram( WORD wAddr, BYTE byData )
{
  if ( Map5_Wram_Protect0 == 0x02 && Map5_Wram_Protect1 == 0x01 )
  {
    if ( Map5_Wram_Reg[ 3 ] != 0xff )
    {
      Map5_Wram[ 0x2000 * Map5_Wram_Reg[ 3 ] + ( wAddr - 0x6000) ] = byData;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map5_Write( unsigned short int wAddr, unsigned char byData )
{
  if ( Map5_Wram_Protect0 == 0x02 && Map5_Wram_Protect1 == 0x01 )
  {
    switch ( wAddr & 0xe000 )
    {
      case 0x8000:      /* $8000-$9fff */
        if ( Map5_Wram_Reg[ 4 ] != 0xff )
        {
          Map5_Wram[ 0x2000 * Map5_Wram_Reg[ 4 ] + ( wAddr - 0x8000) ] = byData;
        }
        break;

      case 0xa000:      /* $a000-$bfff */
        if ( Map5_Wram_Reg[ 5 ] != 0xff )
        {
          Map5_Wram[ 0x2000 * Map5_Wram_Reg[ 5 ] + ( wAddr - 0xa000) ] = byData;
        }
        break;

      case 0xc000:      /* $c000-$dfff */
        if ( Map5_Wram_Reg[ 6 ] != 0xff )
        {
          Map5_Wram[ 0x2000 * Map5_Wram_Reg[ 6 ] + ( wAddr - 0xc000) ] = byData;
        }
        break;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map5_HSync()
{
  if ( PPU_Scanline <= 240 )
  {
    if ( PPU_Scanline == Map5_IRQ_Line )
    {
      Map5_IRQ_Status |= 0x80;

      if ( Map5_IRQ_Enable && Map5_IRQ_Line < 0xf0 )
      {
        IRQ_REQ;
      }
      if ( Map5_IRQ_Line >= 0x40 )
      {
        Map5_IRQ_Enable = 0;
      }
    }
  } else {
    Map5_IRQ_Status |= 0x40;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Rendering Screen Function                               */
/*-------------------------------------------------------------------*/
void Map5_RenderScreen( unsigned char byMode )
{
  unsigned int dwPage[ 8 ];

  switch ( Map5_Chr_Size )
  {
    case 0:
      dwPage[ 7 ] = ( (unsigned int)Map5_Chr_Reg[7][byMode] << 3 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( dwPage[ 7 ] + 0 );
      PPUBANK[ 1 ] = VROMPAGE( dwPage[ 7 ] + 1 );
      PPUBANK[ 2 ] = VROMPAGE( dwPage[ 7 ] + 2 );
      PPUBANK[ 3 ] = VROMPAGE( dwPage[ 7 ] + 3 );
      PPUBANK[ 4 ] = VROMPAGE( dwPage[ 7 ] + 4 );
      PPUBANK[ 5 ] = VROMPAGE( dwPage[ 7 ] + 5 );
      PPUBANK[ 6 ] = VROMPAGE( dwPage[ 7 ] + 6 );
      PPUBANK[ 7 ] = VROMPAGE( dwPage[ 7 ] + 7 );
      InfoNES_SetupChr();
      break;

    case 1:
      dwPage[ 3 ] = ( (unsigned int)Map5_Chr_Reg[3][byMode] << 2 ) % ( NesHeader.byVRomSize << 3 );
      dwPage[ 7 ] = ( (unsigned int)Map5_Chr_Reg[7][byMode] << 2 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( dwPage[ 3 ] + 0 );
      PPUBANK[ 1 ] = VROMPAGE( dwPage[ 3 ] + 1 );
      PPUBANK[ 2 ] = VROMPAGE( dwPage[ 3 ] + 2 );
      PPUBANK[ 3 ] = VROMPAGE( dwPage[ 3 ] + 3 );
      PPUBANK[ 4 ] = VROMPAGE( dwPage[ 7 ] + 0 );
      PPUBANK[ 5 ] = VROMPAGE( dwPage[ 7 ] + 1 );
      PPUBANK[ 6 ] = VROMPAGE( dwPage[ 7 ] + 2 );
      PPUBANK[ 7 ] = VROMPAGE( dwPage[ 7 ] + 3 );
     InfoNES_SetupChr();
      break;

    case 2:
      dwPage[ 1 ] = ( (unsigned int)Map5_Chr_Reg[1][byMode] << 1 ) % ( NesHeader.byVRomSize << 3 );
      dwPage[ 3 ] = ( (unsigned int)Map5_Chr_Reg[3][byMode] << 1 ) % ( NesHeader.byVRomSize << 3 );
      dwPage[ 5 ] = ( (unsigned int)Map5_Chr_Reg[5][byMode] << 1 ) % ( NesHeader.byVRomSize << 3 );
      dwPage[ 7 ] = ( (unsigned int)Map5_Chr_Reg[7][byMode] << 1 ) % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( dwPage[ 1 ] + 0 );
      PPUBANK[ 1 ] = VROMPAGE( dwPage[ 1 ] + 1 );
      PPUBANK[ 2 ] = VROMPAGE( dwPage[ 3 ] + 0 );
      PPUBANK[ 3 ] = VROMPAGE( dwPage[ 3 ] + 1 );
      PPUBANK[ 4 ] = VROMPAGE( dwPage[ 5 ] + 0 );
      PPUBANK[ 5 ] = VROMPAGE( dwPage[ 5 ] + 1 );
      PPUBANK[ 6 ] = VROMPAGE( dwPage[ 7 ] + 0 );
      PPUBANK[ 7 ] = VROMPAGE( dwPage[ 7 ] + 1 );
      InfoNES_SetupChr();
      break;

    default:
      dwPage[ 0 ] = (unsigned int)Map5_Chr_Reg[0][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 1 ] = (unsigned int)Map5_Chr_Reg[1][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 2 ] = (unsigned int)Map5_Chr_Reg[2][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 3 ] = (unsigned int)Map5_Chr_Reg[3][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 4 ] = (unsigned int)Map5_Chr_Reg[4][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 5 ] = (unsigned int)Map5_Chr_Reg[5][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 6 ] = (unsigned int)Map5_Chr_Reg[6][byMode] % ( NesHeader.byVRomSize << 3 );
      dwPage[ 7 ] = (unsigned int)Map5_Chr_Reg[7][byMode] % ( NesHeader.byVRomSize << 3 );

      PPUBANK[ 0 ] = VROMPAGE( dwPage[ 0 ] );
      PPUBANK[ 1 ] = VROMPAGE( dwPage[ 1 ] );
      PPUBANK[ 2 ] = VROMPAGE( dwPage[ 2 ] );
      PPUBANK[ 3 ] = VROMPAGE( dwPage[ 3 ] );
      PPUBANK[ 4 ] = VROMPAGE( dwPage[ 4 ] );
      PPUBANK[ 5 ] = VROMPAGE( dwPage[ 5 ] );
      PPUBANK[ 6 ] = VROMPAGE( dwPage[ 6 ] );
      PPUBANK[ 7 ] = VROMPAGE( dwPage[ 7 ] );
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 5 Sync Program Banks Function                             */
/*-------------------------------------------------------------------*/
void Map5_Sync_Prg_Banks( void )
{
  switch( Map5_Prg_Size )
  {
    case 0:
      Map5_Wram_Reg[ 4 ] = 0xff;
      Map5_Wram_Reg[ 5 ] = 0xff;
      Map5_Wram_Reg[ 6 ] = 0xff;

      ROMBANK0 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7c) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7c) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7c) + 2 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7c) + 3 ) % ( NesHeader.byRomSize << 1 ) );
      break;

    case 1:
      if ( Map5_Prg_Reg[ 5 ] & 0x80 )
      {
        Map5_Wram_Reg[ 4 ] = 0xff;
        Map5_Wram_Reg[ 5 ] = 0xff;
        ROMBANK0 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7e) + 0 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7e) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 4 ] = ( Map5_Prg_Reg[ 5 ] & 0x06 ) + 0;
        Map5_Wram_Reg[ 5 ] = ( Map5_Prg_Reg[ 5 ] & 0x06 ) + 1;
        ROMBANK0 = Map5_ROMPAGE( Map5_Wram_Reg[ 4 ] );
        ROMBANK1 = Map5_ROMPAGE( Map5_Wram_Reg[ 5 ] );
      }

      Map5_Wram_Reg[ 6 ] = 0xff;
      ROMBANK2 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7e) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ( (Map5_Prg_Reg[7] & 0x7e) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      break;

    case 2:
      if ( Map5_Prg_Reg[ 5 ] & 0x80 )
      {
        Map5_Wram_Reg[ 4 ] = 0xff;
        Map5_Wram_Reg[ 5 ] = 0xff;
        ROMBANK0 = ROMPAGE( ( (Map5_Prg_Reg[5] & 0x7e) + 0 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( ( (Map5_Prg_Reg[5] & 0x7e) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 4 ] = ( Map5_Prg_Reg[ 5 ] & 0x06 ) + 0;
        Map5_Wram_Reg[ 5 ] = ( Map5_Prg_Reg[ 5 ] & 0x06 ) + 1;
        ROMBANK0 = Map5_ROMPAGE( Map5_Wram_Reg[ 4 ] );
        ROMBANK1 = Map5_ROMPAGE( Map5_Wram_Reg[ 5 ] );
      }

      if ( Map5_Prg_Reg[ 6 ] & 0x80 )
      {
        Map5_Wram_Reg[ 6 ] = 0xff;
        ROMBANK2 = ROMPAGE( (Map5_Prg_Reg[6] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 6 ] = Map5_Prg_Reg[ 6 ] & 0x07;
        ROMBANK2 = Map5_ROMPAGE( Map5_Wram_Reg[ 6 ] );
      }

      ROMBANK3 = ROMPAGE( (Map5_Prg_Reg[7] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      break;

    default:
      if ( Map5_Prg_Reg[ 4 ] & 0x80 )
      {
        Map5_Wram_Reg[ 4 ] = 0xff;
        ROMBANK0 = ROMPAGE( (Map5_Prg_Reg[4] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 4 ] = Map5_Prg_Reg[ 4 ] & 0x07;
        ROMBANK0 = Map5_ROMPAGE( Map5_Wram_Reg[ 4 ] );
      }

      if ( Map5_Prg_Reg[ 5 ] & 0x80 )
      {
        Map5_Wram_Reg[ 5 ] = 0xff;
        ROMBANK1 = ROMPAGE( (Map5_Prg_Reg[5] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 5 ] = Map5_Prg_Reg[ 5 ] & 0x07;
        ROMBANK1 = Map5_ROMPAGE( Map5_Wram_Reg[ 5 ] );
      }

      if ( Map5_Prg_Reg[ 6 ] & 0x80 )
      {
        Map5_Wram_Reg[ 6 ] = 0xff;
        ROMBANK2 = ROMPAGE( (Map5_Prg_Reg[6] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      } else {
        Map5_Wram_Reg[ 6 ] = Map5_Prg_Reg[ 6 ] & 0x07;
        ROMBANK2 = Map5_ROMPAGE( Map5_Wram_Reg[ 6 ] );
      }

      ROMBANK3 = ROMPAGE( (Map5_Prg_Reg[7] & 0x7f) % ( NesHeader.byRomSize << 1 ) );
      break;
  }
}
//InfoNES_Mapper_005 end
#endif









#ifndef use_lib_not_use_mapper6
//InfoNES_Mapper_006 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 6 (FFE)                              */
/*                                                                   */
/*===================================================================*/

unsigned char Map6_IRQ_Enable;
unsigned int Map6_IRQ_Cnt;
unsigned char Map6_Chr_Ram[ 0x2000 * 4 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 6                                              */
/*-------------------------------------------------------------------*/
void Map6_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map6_Init;

  /* Write to Mapper */
  MapperWrite = Map6_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map6_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map6_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 14 );
  ROMBANK3 = ROMPAGE( 15 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }
  else
  {
    for ( nPage = 0; nPage < 8; ++nPage )
    {
      PPUBANK[ nPage ] = Map6_VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 Write to APU Function                                   */
/*-------------------------------------------------------------------*/
void Map6_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Name Table Mirroring */
    case 0x42fe:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring( 2 );
      } else {
        InfoNES_Mirroring( 3 );
      }
      break;

    case 0x42ff:
      if ( byData & 0x10 )
      {
        InfoNES_Mirroring( 0 );
      } else {
        InfoNES_Mirroring( 1 );
      }
      break;

    case 0x4501:
      Map6_IRQ_Enable = 0;
      break;

    case 0x4502:
      Map6_IRQ_Cnt = ( Map6_IRQ_Cnt & 0xff00 ) | (unsigned int)byData;
      break;

    case 0x4503:
      Map6_IRQ_Cnt = ( Map6_IRQ_Cnt & 0x00ff ) | ( (unsigned int)byData << 8 );
      Map6_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map6_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = ( byData & 0x3c ) >> 2;
  unsigned char byChrBank = byData & 0x03;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank );
  ROMBANK1 = ROMPAGE( byPrgBank + 1 );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 0 * 0x400 ];
  PPUBANK[ 1 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 1 * 0x400 ];
  PPUBANK[ 2 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 2 * 0x400 ];
  PPUBANK[ 3 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 3 * 0x400 ];
  PPUBANK[ 4 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 4 * 0x400 ];
  PPUBANK[ 5 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 5 * 0x400 ];
  PPUBANK[ 6 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 6 * 0x400 ];
  PPUBANK[ 7 ] = &Map6_Chr_Ram[ byChrBank * 0x2000 + 7 * 0x400 ];
  InfoNES_SetupChr();
}

/*-------------------------------------------------------------------*/
/*  Mapper 6 H-Sync Function                                         */
/*-------------------------------------------------------------------*/
void Map6_HSync()
{
  if ( Map6_IRQ_Enable )
  {
    Map6_IRQ_Cnt += 133;
    if ( Map6_IRQ_Cnt >= 0xffff )
    {
      IRQ_REQ;
      Map6_IRQ_Cnt = 0;
    }
  }
}
//InfoNES_Mapper_006 end
#endif







//InfoNES_Mapper_007 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 7 (AOROM)                           */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 7                                              */
/*-------------------------------------------------------------------*/
void Map7_Init()
{
  /* Initialize Mapper */
  MapperInit = Map7_Init;

  /* Write to Mapper */
  MapperWrite = Map7_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 7 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map7_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byBank;

  /* Set ROM Banks */
  byBank = ( byData & 0x07 ) << 2;
  byBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byBank );
  ROMBANK1 = ROMPAGE( byBank + 1 );
  ROMBANK2 = ROMPAGE( byBank + 2 );
  ROMBANK3 = ROMPAGE( byBank + 3 );

  /* Name Table Mirroring */
  InfoNES_Mirroring( byData & 0x10 ? 2 : 3 );
}
//InfoNES_Mapper_007 end









//InfoNES_Mapper_008 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 8 (FFE F3xxx)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 8                                              */
/*-------------------------------------------------------------------*/
void Map8_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map8_Init;

  /* Write to Mapper */
  MapperWrite = Map8_Write;

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
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 8 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map8_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = ( byData & 0xf8 ) >> 3;
  unsigned char byChrBank = byData & 0x07;

  /* Set ROM Banks */
  byPrgBank <<= 1;
  byPrgBank %= ( NesHeader.byRomSize << 1 );

  ROMBANK0 = ROMPAGE( byPrgBank + 0 );
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
}
//InfoNES_Mapper_008 end










//InfoNES_Mapper_009 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 9 (MMC2)                               */
/*                                                                   */
/*===================================================================*/

struct Map9_Latch 
{
  unsigned char lo_bank;
  unsigned char hi_bank;
  unsigned char state;
};

struct Map9_Latch latch1;
struct Map9_Latch latch2;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 9                                              */
/*-------------------------------------------------------------------*/
void Map9_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map9_Init;

  /* Write to Mapper */
  MapperWrite = Map9_Write;

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
  MapperPPU = Map9_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMLASTPAGE( 2 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Init Latch Selector */
  latch1.state = 0xfe;
  latch1.lo_bank = 0;
  latch1.hi_bank = 0;
  latch2.state = 0xfe;
  latch2.lo_bank = 0;
  latch2.hi_bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 9 Write Function                                          */
/*-------------------------------------------------------------------*/
void Map9_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned short int wMapAddr;

  wMapAddr = wAddr & 0xf000;
  switch ( wMapAddr )
  {
    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0xb000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch1.lo_bank = byData;

      if (0xfd == latch1.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xc000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch1.hi_bank = byData;

      if (0xfe == latch1.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xd000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch2.lo_bank = byData;

      if (0xfd == latch2.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 );    
        InfoNES_SetupChr();
      }
      break;

    case 0xe000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch2.hi_bank = byData;

      if (0xfe == latch2.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 ); 
        InfoNES_SetupChr();
      }
      break;

    case 0xf000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x01 ? 0 : 1);
      break;
  }  
}

/*-------------------------------------------------------------------*/
/*  Mapper 9 PPU Function                                            */
/*-------------------------------------------------------------------*/
void Map9_PPU( unsigned short int wAddr )
{
  /* Control Latch Selector */ 
  switch ( wAddr & 0x3ff0 )
  {
    case 0x0fd0:
      /* Latch Control */
      latch1.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch1.lo_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch1.lo_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch1.lo_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch1.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;

    case 0x0fe0:
      /* Latch Control */
      latch1.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch1.hi_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch1.hi_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch1.hi_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch1.hi_bank + 3 );     
      InfoNES_SetupChr();      
      break;

    case 0x1fd0:
      /* Latch Control */
      latch2.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch2.lo_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch2.lo_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch2.lo_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch2.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;      

    case 0x1fe0:
      /* Latch Control */
      latch2.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch2.hi_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch2.hi_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch2.hi_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch2.hi_bank + 3 );     
      InfoNES_SetupChr();            
      break;
  }
}
//InfoNES_Mapper_009 end










//InfoNES_Mapper_010 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 10 (MMC4)                              */
/*                                                                   */
/*===================================================================*/

struct Map10_Latch 
{
  unsigned char lo_bank;
  unsigned char hi_bank;
  unsigned char state;
};

struct Map10_Latch latch3;    // Latch Selector #1
struct Map10_Latch latch4;    // Latch Selector #2

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 10                                             */
/*-------------------------------------------------------------------*/
void Map10_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map10_Init;

  /* Write to Mapper */
  MapperWrite = Map10_Write;

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
  MapperPPU = Map10_PPU;

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
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Init Latch Selector */
  latch3.state = 0xfe;
  latch3.lo_bank = 0;
  latch3.hi_bank = 0;
  latch4.state = 0xfe;
  latch4.lo_bank = 0;
  latch4.hi_bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map10_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned short int wMapAddr;

  wMapAddr = wAddr & 0xf000;
  switch ( wMapAddr )
  {
    case 0xa000:
      /* Set ROM Banks */
      byData %= NesHeader.byRomSize;
      byData <<= 1;
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    case 0xb000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.lo_bank = byData;

      if (0xfd == latch3.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xc000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch3.hi_bank = byData;

      if (0xfe == latch3.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 3 ] = VROMPAGE( byData + 3 );     
        InfoNES_SetupChr();
      }
      break;

    case 0xd000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.lo_bank = byData;

      if (0xfd == latch4.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 );    
        InfoNES_SetupChr();
      }
      break;

    case 0xe000:
      /* Number of 4K Banks to Number of 1K Banks */
      byData %= ( NesHeader.byVRomSize << 1 );
      byData <<= 2;

      /* Latch Control */
      latch4.hi_bank = byData;

      if (0xfe == latch4.state)
      {
        /* Set PPU Banks */
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
        PPUBANK[ 6 ] = VROMPAGE( byData + 2 );
        PPUBANK[ 7 ] = VROMPAGE( byData + 3 ); 
        InfoNES_SetupChr();
      }
      break;

    case 0xf000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x01 ? 0 : 1);
      break;
  }  
}

/*-------------------------------------------------------------------*/
/*  Mapper 10 PPU Function                                           */
/*-------------------------------------------------------------------*/
void Map10_PPU( unsigned short int wAddr )
{
  /* Control Latch Selector */ 
  switch ( wAddr & 0x3ff0 )
  {
    case 0x0fd0:
      /* Latch Control */
      latch3.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch3.lo_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch3.lo_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch3.lo_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch3.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;

    case 0x0fe0:
      /* Latch Control */
      latch3.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 0 ] = VROMPAGE( latch3.hi_bank );
      PPUBANK[ 1 ] = VROMPAGE( latch3.hi_bank + 1 );
      PPUBANK[ 2 ] = VROMPAGE( latch3.hi_bank + 2 );
      PPUBANK[ 3 ] = VROMPAGE( latch3.hi_bank + 3 );     
      InfoNES_SetupChr();      
      break;

    case 0x1fd0:
      /* Latch Control */
      latch4.state = 0xfd;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch4.lo_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch4.lo_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch4.lo_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch4.lo_bank + 3 );     
      InfoNES_SetupChr();
      break;      

    case 0x1fe0:
      /* Latch Control */
      latch4.state = 0xfe;
      /* Set PPU Banks */
      PPUBANK[ 4 ] = VROMPAGE( latch4.hi_bank );
      PPUBANK[ 5 ] = VROMPAGE( latch4.hi_bank + 1 );
      PPUBANK[ 6 ] = VROMPAGE( latch4.hi_bank + 2 );
      PPUBANK[ 7 ] = VROMPAGE( latch4.hi_bank + 3 );     
      InfoNES_SetupChr();            
      break;
  }
}
//InfoNES_Mapper_010 end











//InfoNES_Mapper_011 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 11 (Color Dreams)                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 11                                             */
/*-------------------------------------------------------------------*/
void Map11_Init()
{
  int nPage;

  /* Initialize Mapper */
  MapperInit = Map11_Init;

  /* Write to Mapper */
  MapperWrite = Map11_Write;

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
    for ( nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 11 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map11_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byPrgBank = ( byData & 0x01 ) << 2;
  unsigned char byChrBank = ( ( byData & 0x70 ) >> 4 ) << 3;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( ( byPrgBank + 0 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( ( byPrgBank + 1 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( ( byPrgBank + 2 ) % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( ( byPrgBank + 3 ) % ( NesHeader.byRomSize << 1 ) );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( ( byChrBank + 0 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 1 ] = VROMPAGE( ( byChrBank + 1 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 2 ] = VROMPAGE( ( byChrBank + 2 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 3 ] = VROMPAGE( ( byChrBank + 3 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 4 ] = VROMPAGE( ( byChrBank + 4 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 5 ] = VROMPAGE( ( byChrBank + 5 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 6 ] = VROMPAGE( ( byChrBank + 6 ) % ( NesHeader.byVRomSize << 3 ) );
  PPUBANK[ 7 ] = VROMPAGE( ( byChrBank + 7 ) % ( NesHeader.byVRomSize << 3 ) );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_011 end









//InfoNES_Mapper_013 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 13 : CPROM                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 13                                             */
/*-------------------------------------------------------------------*/
void Map13_Init()
{
  /* Initialize Mapper */
  MapperInit = Map13_Init;

  /* Write to Mapper */
  MapperWrite = Map13_Write;

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
  PPUBANK[ 0 ] = CRAMPAGE( 0 );
  PPUBANK[ 1 ] = CRAMPAGE( 1 );
  PPUBANK[ 2 ] = CRAMPAGE( 2 );
  PPUBANK[ 3 ] = CRAMPAGE( 3 );
  PPUBANK[ 4 ] = CRAMPAGE( 0 );
  PPUBANK[ 5 ] = CRAMPAGE( 1 );
  PPUBANK[ 6 ] = CRAMPAGE( 2 );
  PPUBANK[ 7 ] = CRAMPAGE( 3 );
  InfoNES_SetupChr();

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 13 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map13_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE((((byData&0x30)>>2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((byData&0x30)>>2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((byData&0x30)>>2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((byData&0x30)>>2)+3) % (NesHeader.byRomSize<<1));

  /* Set PPU Banks */
  PPUBANK[ 4 ] = CRAMPAGE(((byData&0x03)<<2)+0);
  PPUBANK[ 5 ] = CRAMPAGE(((byData&0x03)<<2)+1);
  PPUBANK[ 6 ] = CRAMPAGE(((byData&0x03)<<2)+2);
  PPUBANK[ 7 ] = CRAMPAGE(((byData&0x03)<<2)+3);
  InfoNES_SetupChr();
}
//InfoNES_Mapper_013 end












//InfoNES_Mapper_015 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 15 (100-in-1)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 15                                             */
/*-------------------------------------------------------------------*/
void Map15_Init()
{
  /* Initialize Mapper */
  MapperInit = Map15_Init;

  /* Write to Mapper */
  MapperWrite = Map15_Write;

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

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 15 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map15_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byBank;

  switch ( wAddr )
  {
    case 0x8000:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x20 ? 0 : 1);
      
      /* Set ROM Banks */
      byBank = byData & 0x1f;
      byBank %= ( NesHeader.byRomSize << 1 );
      byBank <<= 1;

      ROMBANK0 = ROMPAGE( byBank );
      ROMBANK1 = ROMPAGE( byBank + 1 );
      ROMBANK2 = ROMPAGE( byBank + 2 );
      ROMBANK3 = ROMPAGE( byBank + 3 );
      break;

    case 0x8001:
      /* Set ROM Banks */
      byData &= 0x3f;
      byData %= ( NesHeader.byRomSize << 1 );
      byData <<= 1;

      ROMBANK2 = ROMPAGE( byData );
      ROMBANK3 = ROMPAGE( byData + 1 );
      break;

    case 0x8002:
      /* Set ROM Banks */
      byBank = byData & 0x3f; 
      byBank %= ( NesHeader.byRomSize << 1 );
      byBank <<= 1;
      byBank += ( byData & 0x80 ? 1 : 0 );

      ROMBANK0 = ROMPAGE( byBank );
      ROMBANK1 = ROMPAGE( byBank );
      ROMBANK2 = ROMPAGE( byBank );
      ROMBANK3 = ROMPAGE( byBank );
      break;

    case 0x8003:
      /* Name Table Mirroring */
      InfoNES_Mirroring( byData & 0x20 ? 0 : 1);
      
      /* Set ROM Banks */
      byData &= 0x1f;
      byData %= ( NesHeader.byRomSize << 1 );
      byData <<= 1;

      ROMBANK2 = ROMPAGE( byData );
      ROMBANK3 = ROMPAGE( byData + 1 );
      break;
  }
}
//InfoNES_Mapper_015 end








//InfoNES_Mapper_016 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 16 (Bandai Mapper)                       */
/*                                                                   */
/*===================================================================*/

unsigned char  Map16_Regs[3];

unsigned char  Map16_IRQ_Enable;
unsigned int Map16_IRQ_Cnt;
unsigned int Map16_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 16                                             */
/*-------------------------------------------------------------------*/
void Map16_Init()
{
  /* Initialize Mapper */
  MapperInit = Map16_Init;

  /* Write to Mapper */
  MapperWrite = Map16_Write;

  /* Write to SRAM */
  MapperSram = Map16_Write;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map16_HSync;

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

  /* Initialize State Flag */
  Map16_Regs[ 0 ] = 0;
  Map16_Regs[ 1 ] = 0;
  Map16_Regs[ 2 ] = 0;

  Map16_IRQ_Enable = 0;
  Map16_IRQ_Cnt = 0;
  Map16_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 16 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map16_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0x000f )
  {
    case 0x0000:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0001:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0002:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0003:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0004:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0005:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0006:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0007:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x0008:
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;

    case 0x0009:
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

      case 0x000a:
        Map16_IRQ_Enable = byData & 0x01;
        Map16_IRQ_Cnt = Map16_IRQ_Latch;
        break;

      case 0x000b:
        Map16_IRQ_Latch = ( Map16_IRQ_Latch & 0xff00 ) | byData;
        break;

      case 0x000c:
        Map16_IRQ_Latch = ( (unsigned int)byData << 8 ) | ( Map16_IRQ_Latch & 0x00ff );
        break;

      case 0x000d:
        /* Write Protect */
        break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 16 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map16_HSync()
{
  if ( Map16_IRQ_Enable )
  {
    /* Normal IRQ */
    if ( Map16_IRQ_Cnt <= 114 )
    {
      IRQ_REQ;
      Map16_IRQ_Cnt = 0;
      Map16_IRQ_Enable = 0;
    } else {
      Map16_IRQ_Cnt -= 114;
    }
  }
}
//InfoNES_Mapper_016 end








//InfoNES_Mapper_017 begin
/*===================================================================*/
/*                                                                   */
/*                 Mapper 17 (FFE F8 Series)                         */
/*                                                                   */
/*===================================================================*/

unsigned char  Map17_IRQ_Enable;
unsigned int Map17_IRQ_Cnt;
unsigned int Map17_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 17                                             */
/*-------------------------------------------------------------------*/
void Map17_Init()
{
  /* Initialize Mapper */
  MapperInit = Map17_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map17_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map17_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

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

  /* Initialize State Registers */
  Map17_IRQ_Enable = 0;
  Map17_IRQ_Cnt = 0;
  Map17_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map17_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x42fe:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring( 3 );
      } else {
        InfoNES_Mirroring( 2 );
      }
      break;

    case 0x42ff:
      if ( ( byData & 0x10 ) == 0 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }
      break;

    case 0x4501:
      Map17_IRQ_Enable = 0;
      break;

    case 0x4502:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0xff00 ) | byData;
      break;

    case 0x4503:
      Map17_IRQ_Latch = ( Map17_IRQ_Latch & 0x00ff ) | ( (unsigned int)byData << 8 );
      Map17_IRQ_Cnt = Map17_IRQ_Latch;
      Map17_IRQ_Enable = 1;
      break;

    case 0x4504:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x4505:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x4506:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0x4507:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK3 = ROMPAGE( byData );
      break;

    case 0x4510:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4511:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4512:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4513:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4514:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4515:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4516:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0x4517:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 17 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map17_HSync()
{
  if ( Map17_IRQ_Enable )
  {
    if ( Map17_IRQ_Cnt >= 0xffff - 113 )
    {
      IRQ_REQ;
      Map17_IRQ_Cnt = 0;
      Map17_IRQ_Enable = 0;
    } else {
      Map17_IRQ_Cnt += 113;
    }
  }
}
//InfoNES_Mapper_017 end









//InfoNES_Mapper_018 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 18 (Jaleco SS8806)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map18_Regs[11];
unsigned char Map18_IRQ_Enable;
unsigned short int Map18_IRQ_Latch;
unsigned short int Map18_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 18                                             */
/*-------------------------------------------------------------------*/
void Map18_Init()
{
  /* Initialize Mapper */
  MapperInit = Map18_Init;

  /* Write to Mapper */
  MapperWrite = Map18_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map18_HSync;

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

  /* Initialize Regs */
  for ( int i = 0; i < sizeof( Map18_Regs ); i++ )
  {
    Map18_Regs[ i ] = 0;
  }
  Map18_IRQ_Enable = 0;
  Map18_IRQ_Latch = 0;
  Map18_IRQ_Cnt = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 18 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map18_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
      Map18_Regs[ 0 ] = ( Map18_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK0 = ROMPAGE( Map18_Regs[ 0 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8001:
      Map18_Regs[ 0 ] = ( Map18_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK0 = ROMPAGE( Map18_Regs[ 0 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8002:
      Map18_Regs[ 1 ] = ( Map18_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK1 = ROMPAGE( Map18_Regs[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x8003:
      Map18_Regs[ 1 ] = ( Map18_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK1 = ROMPAGE( Map18_Regs[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9000:
      Map18_Regs[ 2 ] = ( Map18_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      ROMBANK2 = ROMPAGE( Map18_Regs[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    case 0x9001:
      Map18_Regs[ 2 ] = ( Map18_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      ROMBANK2 = ROMPAGE( Map18_Regs[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      break;

    /* Set PPU Banks */
    case 0xA000:
      Map18_Regs[ 3 ]  = ( Map18_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map18_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xA001:
      Map18_Regs[ 3 ] = ( Map18_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map18_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xA002:
      Map18_Regs[ 4 ]  = ( Map18_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map18_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xA003:
      Map18_Regs[ 4 ] = ( Map18_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map18_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xB000:
      Map18_Regs[ 5 ]  = ( Map18_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map18_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xB001:
      Map18_Regs[ 5 ] = ( Map18_Regs[ 5 ] &0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map18_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xB002:
      Map18_Regs[ 6 ]  = ( Map18_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map18_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xB003:
      Map18_Regs[ 6 ] = ( Map18_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map18_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xC000:
      Map18_Regs[ 7 ]  = ( Map18_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map18_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xC001:
      Map18_Regs[ 7 ] = ( Map18_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map18_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xC002:
      Map18_Regs[ 8 ]  = ( Map18_Regs[ 8 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map18_Regs[ 8 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xC003:
      Map18_Regs[ 8 ] = ( Map18_Regs[ 8 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map18_Regs[ 8 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xD000:
      Map18_Regs[ 9 ]  = ( Map18_Regs[ 9 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map18_Regs[ 9 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();      
      break;

    case 0xD001:
      Map18_Regs[ 9 ] = ( Map18_Regs[ 9 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map18_Regs[ 9 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();        
      break;

    case 0xD002:
      Map18_Regs[ 10 ]  = ( Map18_Regs[ 10 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map18_Regs[ 10 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();         
      break;

    case 0xD003:
      Map18_Regs[ 10 ] = ( Map18_Regs[ 10 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map18_Regs[ 10 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(); 
      break;

    case 0xE000:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xfff0 ) | ( byData & 0x0f );
      break;

    case 0xE001:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xff0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xE002:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0xf0ff ) | ( ( byData & 0x0f ) << 8 );
      break;

    case 0xE003:
      Map18_IRQ_Latch = ( Map18_IRQ_Latch & 0x0fff ) | ( ( byData & 0x0f ) << 12 );
      break;

    case 0xF000:
      if ( ( byData & 0x01 ) == 0 )
      {
        Map18_IRQ_Cnt = 0;
      } else {
        Map18_IRQ_Cnt = Map18_IRQ_Latch;
      }
      break;

    case 0xF001:
      Map18_IRQ_Enable = byData & 0x01;
      break;

    /* Name Table Mirroring */
    case 0xF002:
      switch ( byData & 0x03 )
      {
        case 0:
          InfoNES_Mirroring( 0 );   /* Horizontal */
          break;
        case 1:
          InfoNES_Mirroring( 1 );   /* Vertical */            
          break;
        case 2:
          InfoNES_Mirroring( 3 );   /* One Screen 0x2000 */
          break;
      }    
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 18 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map18_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map18_IRQ_Enable )
  {
    if ( Map18_IRQ_Cnt <= 113 )
    {
      IRQ_REQ;
      Map18_IRQ_Cnt = 0;
      Map18_IRQ_Enable = 0;
    } else {
      Map18_IRQ_Cnt -= 113;
    }
  }
}
//InfoNES_Mapper_018 end










#ifndef use_lib_not_use_mapper19
//InfoNES_Mapper_019 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 19 (Namcot 106)                         */
/*                                                                   */
/*===================================================================*/

unsigned char  Map19_Chr_Ram[ 0x2000 ];
unsigned char  Map19_Regs[ 2 ];

unsigned char  Map19_IRQ_Enable;
unsigned int Map19_IRQ_Cnt;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 19                                             */
/*-------------------------------------------------------------------*/
void Map19_Init()
{
  /* Initialize Mapper */
  MapperInit = Map19_Init;

  /* Write to Mapper */
  MapperWrite = Map19_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map19_Apu;

  /* Read from APU */
  MapperReadApu = Map19_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map19_HSync;

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
    unsigned int dwLastPage = (unsigned int)NesHeader.byVRomSize << 3;
    PPUBANK[ 0 ] = VROMPAGE( dwLastPage - 8 );
    PPUBANK[ 1 ] = VROMPAGE( dwLastPage - 7 );
    PPUBANK[ 2 ] = VROMPAGE( dwLastPage - 6 );
    PPUBANK[ 3 ] = VROMPAGE( dwLastPage - 5 );
    PPUBANK[ 4 ] = VROMPAGE( dwLastPage - 4 );
    PPUBANK[ 5 ] = VROMPAGE( dwLastPage - 3 );
    PPUBANK[ 6 ] = VROMPAGE( dwLastPage - 2 );
    PPUBANK[ 7 ] = VROMPAGE( dwLastPage - 1 );
    InfoNES_SetupChr();
  }

  /* Initialize State Register */
  Map19_Regs[ 0 ] = 0x00;
  Map19_Regs[ 1 ] = 0x00;
  Map19_Regs[ 2 ] = 0x00;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 19 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map19_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set PPU Banks */
  switch ( wAddr & 0xf800 )
  {
    case 0x8000:  /* $8000-87ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 0 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 0 ] = Map19_VROMPAGE( 0 );
      }
      InfoNES_SetupChr();
      break;

    case 0x8800:  /* $8800-8fff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 1 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 1 ] = Map19_VROMPAGE( 1 );
      }
      InfoNES_SetupChr();
      break;

    case 0x9000:  /* $9000-97ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 2 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 2 ] = Map19_VROMPAGE( 2 );
      }
      InfoNES_SetupChr();
      break;

    case 0x9800:  /* $9800-9fff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 3 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 3 ] = Map19_VROMPAGE( 3 );
      }
      InfoNES_SetupChr();
      break;

    case 0xa000:  /* $a000-a7ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 4 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 4 ] = Map19_VROMPAGE( 4 );
      }
      InfoNES_SetupChr();
      break;

    case 0xa800:  /* $a800-afff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 5 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 5 ] = Map19_VROMPAGE( 5 );
      }
      InfoNES_SetupChr();
      break;

    case 0xb000:  /* $b000-b7ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 6 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 6 ] = Map19_VROMPAGE( 6 );
      }
      InfoNES_SetupChr();
      break;

    case 0xb800:  /* $b800-bfff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ 7 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 7 ] = Map19_VROMPAGE( 7 );
      }
      InfoNES_SetupChr();
      break;

    case 0xc000:  /* $c000-c7ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ NAME_TABLE0 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ NAME_TABLE0 ] = VRAMPAGE( byData & 0x01 );
      }
      break;

    case 0xc800:  /* $c800-cfff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ NAME_TABLE1 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ NAME_TABLE1 ] = VRAMPAGE( byData & 0x01 );
      }
      break;

    case 0xd000:  /* $d000-d7ff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ NAME_TABLE2 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ NAME_TABLE2 ] = VRAMPAGE( byData & 0x01 );
      }
      break;

    case 0xd800:  /* $d800-dfff */
      if ( byData < 0xe0 || Map19_Regs[ 0 ] == 1 )
      {
        byData %= ( NesHeader.byVRomSize << 3 );
        PPUBANK[ NAME_TABLE3 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ NAME_TABLE3 ] = VRAMPAGE( byData & 0x01 );
      }
      break;

    case 0xe000:  /* $e000-e7ff */
      byData &= 0x3f;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0xe800:  /* $e800-efff */
      Map19_Regs[ 0 ] = ( byData & 0x40 ) >> 6;
      Map19_Regs[ 1 ] = ( byData & 0x80 ) >> 7;

      byData &= 0x3f;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0xf000:  /* $f000-f7ff */
      byData &= 0x3f;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0xf800:  /* $e800-efff */
      if ( wAddr == 0xf800 )
      {
        // Extra Sound
      }
      break;    
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 19 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map19_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf800 )
  {
    case 0x4800:
      if ( wAddr == 0x4800 )
      {
        // Extra Sound
      }
      break;

    case 0x5000:  /* $5000-57ff */
      Map19_IRQ_Cnt = ( Map19_IRQ_Cnt & 0xff00 ) | byData;
      break;

    case 0x5800:  /* $5800-5fff */
      Map19_IRQ_Cnt = ( Map19_IRQ_Cnt & 0x00ff ) | ( (unsigned int)( byData & 0x7f ) << 8 );
      Map19_IRQ_Enable = ( byData & 0x80 ) >> 7;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 19 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
unsigned char Map19_ReadApu( unsigned short int wAddr )
{
  switch ( wAddr & 0xf800 )
  {
    case 0x4800:
      if ( wAddr == 0x4800 )
      {
        // Extra Sound
      }
      return (unsigned char)( wAddr >> 8 );

    case 0x5000:  /* $5000-57ff */
      return (unsigned char)(Map19_IRQ_Cnt & 0x00ff );

    case 0x5800:  /* $5800-5fff */
      return (unsigned char)( (Map19_IRQ_Cnt & 0x7f00) >> 8 );

    default:
      return (unsigned char)( wAddr >> 8 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 19 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map19_HSync()
{
/*
 *  Callback at HSync
 *
 */
  unsigned char Map19_IRQ_Timing = 113;

  if ( Map19_IRQ_Enable )
  {
    if ( Map19_IRQ_Cnt >= (unsigned int)(0x7fff - Map19_IRQ_Timing) )
    {
      Map19_IRQ_Cnt = 0x7fff;
      IRQ_REQ;
    } else {
      Map19_IRQ_Cnt += Map19_IRQ_Timing;
    }
  }
}
//InfoNES_Mapper_019 end
#endif










//InfoNES_Mapper_021 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 21 (Konami VRC4 2A)                      */
/*                                                                   */
/*===================================================================*/

unsigned char Map21_Regs[ 10 ];
unsigned char Map21_IRQ_Enable;
unsigned char Map21_IRQ_Cnt;
unsigned char Map21_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 21                                             */
/*-------------------------------------------------------------------*/
void Map21_Init()
{
  /* Initialize Mapper */
  MapperInit = Map21_Init;

  /* Write to Mapper */
  MapperWrite = Map21_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map21_HSync;

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

  /* Initialize State Registers */
  for ( int nPage = 0; nPage < 8; nPage++ )
  {
    Map21_Regs[ nPage ] = nPage;
  }
  Map21_Regs[ 8 ] = 0;

  Map21_IRQ_Enable = 0;
  Map21_IRQ_Cnt = 0;
  Map21_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 21 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map21_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xf00f )
  {
    /* Set ROM Banks */
    case 0x8000:
      if ( Map21_Regs[ 8 ] & 0x02 )
      {
        byData %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byData );
      } else {
        byData %= ( NesHeader.byRomSize << 1 );
        ROMBANK0 = ROMPAGE( byData );
      }
      break;

    case 0xa000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    /* Name Table Mirroring */
    case 0x9000:
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

    case 0x9002:
      Map21_Regs[ 8 ] = byData;
      break;

    case 0xb000:
      Map21_Regs[ 0 ] = ( Map21_Regs[ 0 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 0 ] = VROMPAGE( Map21_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb002:
      Map21_Regs[ 0 ] = ( Map21_Regs[ 0 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 0 ] = VROMPAGE( Map21_Regs[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb001:
    case 0xb004:
      Map21_Regs[ 1 ] = ( Map21_Regs[ 1 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 1 ] = VROMPAGE( Map21_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xb003:
    case 0xb006:
      Map21_Regs[ 1 ] = ( Map21_Regs[ 1 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 1 ] = VROMPAGE( Map21_Regs[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      Map21_Regs[ 2 ] = ( Map21_Regs[ 2 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 2 ] = VROMPAGE( Map21_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc002:
      Map21_Regs[ 2 ] = ( Map21_Regs[ 2 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 2 ] = VROMPAGE( Map21_Regs[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc001:
    case 0xc004:
      Map21_Regs[ 3 ] = ( Map21_Regs[ 3 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 3 ] = VROMPAGE( Map21_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xc003:
    case 0xc006:
      Map21_Regs[ 3 ] = ( Map21_Regs[ 3 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 3 ] = VROMPAGE( Map21_Regs[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd000:
      Map21_Regs[ 4 ] = ( Map21_Regs[ 4 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 4 ] = VROMPAGE( Map21_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd002:
      Map21_Regs[ 4 ] = ( Map21_Regs[ 4 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 4 ] = VROMPAGE( Map21_Regs[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd001:
    case 0xd004:
      Map21_Regs[ 5 ] = ( Map21_Regs[ 5 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 5 ] = VROMPAGE( Map21_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xd003:
    case 0xd006:
      Map21_Regs[ 5 ] = ( Map21_Regs[ 5 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 5 ] = VROMPAGE( Map21_Regs[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe000:
      Map21_Regs[ 6 ] = ( Map21_Regs[ 6 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 6 ] = VROMPAGE( Map21_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe002:
      Map21_Regs[ 6 ] = ( Map21_Regs[ 6 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 6 ] = VROMPAGE( Map21_Regs[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe001:
    case 0xe004:
      Map21_Regs[ 7 ] = ( Map21_Regs[ 7 ] & 0xf0 ) | ( byData & 0x0f );
      PPUBANK[ 7 ] = VROMPAGE( Map21_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xe003:
    case 0xe006:
      Map21_Regs[ 7 ] = ( Map21_Regs[ 7 ] & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      PPUBANK[ 7 ] = VROMPAGE( Map21_Regs[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0xf000:
      Map21_IRQ_Latch = ( Map21_IRQ_Latch & 0xf0 ) | ( byData & 0x0f );
      break;

    case 0xf002:
      Map21_IRQ_Latch = ( Map21_IRQ_Latch & 0x0f ) | ( ( byData & 0x0f ) << 4 );
      break;

    case 0xf003:
      if ( Map21_IRQ_Enable & 0x01 )
      {
        Map21_IRQ_Enable |= 0x02;
      } else {
        Map21_IRQ_Enable &= 0x01;
      }
      break;

    case 0xf004:
      Map21_IRQ_Enable = byData & 0x03;
      if ( Map21_IRQ_Enable & 0x02 )
      {
        Map21_IRQ_Cnt = Map21_IRQ_Latch;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 21 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map21_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map21_IRQ_Enable & 0x02 )
  {
    if ( Map21_IRQ_Cnt == 0xff )
    {
      Map21_IRQ_Cnt = Map21_IRQ_Latch;
      
      if ( Map21_IRQ_Enable & 0x01 )
      {
        Map21_IRQ_Enable |= 0x02;
      } else {
        Map21_IRQ_Enable &= 0x01;
      }
      IRQ_REQ;
    } else {
      Map21_IRQ_Cnt++;
    }
  }
}
//InfoNES_Mapper_021 end











//InfoNES_Mapper_022 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 22 (Konami VRC2 type A)                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 22                                             */
/*-------------------------------------------------------------------*/
void Map22_Init()
{
  /* Initialize Mapper */
  MapperInit = Map22_Init;

  /* Write to Mapper */
  MapperWrite = Map22_Write;

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
/*  Mapper 22 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map22_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x9000:
      /* Name Table Mirroring */
      switch ( byData & 0x03 )
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

    case 0xa000:
      /* Set ROM Banks */
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;
    
    case 0xb000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 0 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xb001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 1 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
    
    case 0xc000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 2 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 3 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
          
    case 0xd000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;
          
    case 0xe000:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xe001:
      /* Set PPU Banks */
      byData >>= 1;
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 
  }
}
//InfoNES_Mapper_022 end
