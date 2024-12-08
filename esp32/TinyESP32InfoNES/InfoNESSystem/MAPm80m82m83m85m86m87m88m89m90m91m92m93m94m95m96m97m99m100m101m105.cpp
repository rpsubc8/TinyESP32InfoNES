#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"


//InfoNES_Mapper_080 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 80 (X1-005)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 80                                             */
/*-------------------------------------------------------------------*/
void Map80_Init()
{
  /* Initialize Mapper */
  MapperInit = Map80_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map80_Sram;

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
/*  Mapper 80 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map80_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Set PPU Banks */
    case 0x7ef0:
      byData &= 0x7f;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 0 ] = VROMPAGE( byData );
      PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;

    case 0x7ef1:
      byData &= 0x7f;
      byData %= ( NesHeader.byVRomSize << 3 );
      
      PPUBANK[ 2 ] = VROMPAGE( byData );
      PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      InfoNES_SetupChr();
      break;
  
    case 0x7ef2:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 4 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;      

    case 0x7ef3:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 5 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break;  

    case 0x7ef4:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 6 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 

    case 0x7ef5:
      byData %= ( NesHeader.byVRomSize << 3 );
      PPUBANK[ 7 ] = VROMPAGE( byData );
      InfoNES_SetupChr();
      break; 

    /* Name Table Mirroring */
    case 0x7ef6:
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }

    /* Set ROM Banks */
    case 0x7efa:
    case 0x7efb:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x7efc:
    case 0x7efd:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x7efe:
    case 0x7eff:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;
  }
}
//InfoNES_Mapper_080 end











//InfoNES_Mapper_082 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 82 (Taito X1-17)                    */
/*                                                                   */
/*===================================================================*/

unsigned char Map82_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 82                                             */
/*-------------------------------------------------------------------*/
void Map82_Init()
{
  /* Initialize Mapper */
  MapperInit = Map82_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map82_Sram;

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

  /* Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 82 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map82_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Set PPU Banks */
    case 0x7ef0:
      byData &= 0xfe;
      byData %= ( NesHeader.byVRomSize << 3 );

      if ( Map82_Regs[ 0 ] )
      {
        PPUBANK[ 4 ] = VROMPAGE( byData );
        PPUBANK[ 5 ] = VROMPAGE( byData + 1 );
      } else {
        PPUBANK[ 0 ] = VROMPAGE( byData );
        PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
      }
      InfoNES_SetupChr();
      break;

    case 0x7ef1:
      byData &= 0xfe;
      byData %= ( NesHeader.byVRomSize << 3 );

      if ( Map82_Regs[ 0 ] )
      {
        PPUBANK[ 6 ] = VROMPAGE( byData );
        PPUBANK[ 7 ] = VROMPAGE( byData + 1 );
      } else {
        PPUBANK[ 2 ] = VROMPAGE( byData );
        PPUBANK[ 3 ] = VROMPAGE( byData + 1 );
      }
      InfoNES_SetupChr();
      break;
  
    case 0x7ef2:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 4 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 0 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;    
      
    case 0x7ef3:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 5 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 1 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    case 0x7ef4:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 6 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 2 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    case 0x7ef5:
      byData %= ( NesHeader.byVRomSize << 3 );
      
      if ( !Map82_Regs[ 0 ] )
      {
        PPUBANK[ 7 ] = VROMPAGE( byData );
      } else {
        PPUBANK[ 3 ] = VROMPAGE( byData );
      }
      InfoNES_SetupChr();
      break;  

    /* Name Table Mirroring */
    case 0x7ef6:
      Map82_Regs[ 0 ] = byData & 0x02;
      
      if ( byData & 0x01 )
      {
        InfoNES_Mirroring( 1 );
      } else {
        InfoNES_Mirroring( 0 );
      }

    /* Set ROM Banks */
    case 0x7efa:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x7efb:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x7efc:
      byData >>= 2;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;
  }
}
//InfoNES_Mapper_082 end











//InfoNES_Mapper_083 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 83 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map83_Regs[3];
unsigned int Map83_Chr_Bank;
unsigned int Map83_IRQ_Cnt;
unsigned char Map83_IRQ_Enabled;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 83                                             */
/*-------------------------------------------------------------------*/
void Map83_Init()
{
  /* Initialize Mapper */
  MapperInit = Map83_Init;

  /* Write to Mapper */
  MapperWrite = Map83_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map83_Apu;

  /* Read from APU */
  MapperReadApu = Map83_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map83_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Initialize Registers */
  Map83_Regs[0] = Map83_Regs[1] = Map83_Regs[2] = 0;

  /* Set ROM Banks */
  if ( ( NesHeader.byRomSize << 1 ) >= 32 )
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 30 );
    ROMBANK3 = ROMPAGE( 31 );
    Map83_Regs[ 1 ] = 0x30;
  }
  else
  {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Initialize IRQ Registers */
  Map83_IRQ_Enabled = 0;
  Map83_IRQ_Cnt = 0;
  Map83_Chr_Bank = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
unsigned char Map83_ReadApu( unsigned short int wAddr )
{
  if ( ( wAddr & 0x5100 ) == 0x5100 )
  {
    return Map83_Regs[2];
  }
  else
  {
    return wAddr >> 8;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map83_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch(wAddr)
  {
    case 0x5101:
    case 0x5102:
    case 0x5103:
      Map83_Regs[2] = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map83_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  switch( wAddr )
  {
    case 0x8000:
    case 0xB000:
    case 0xB0FF:
    case 0xB1FF:
      Map83_Regs[0] = byData;
      Map83_Chr_Bank = (byData & 0x30) << 4;

      ROMBANK0 = ROMPAGE( (byData*2+0) % (NesHeader.byRomSize << 1) );
      ROMBANK1 = ROMPAGE( (byData*2+1) % (NesHeader.byRomSize << 1) );
      ROMBANK2 = ROMPAGE( (((byData&0x30)|0x0F)*2+0) % (NesHeader.byRomSize << 1) );
      ROMBANK3 = ROMPAGE( (((byData&0x30)|0x0F)*2+1) % (NesHeader.byRomSize << 1) );
      break;

    case 0x8100:
      if ( NesHeader.byVRomSize <= 32 )
      {
	Map83_Regs[1] = byData;
      }
      if ((byData & 0x03) == 0x00)
      {
	InfoNES_Mirroring( 1 );
      }
      else if((byData & 0x03) == 0x01)
      {
	InfoNES_Mirroring( 0 );
      }
      else if((byData & 0x03) == 0x02)
      {
	InfoNES_Mirroring( 3 );
      }
      else
      {
	InfoNES_Mirroring( 2 );
      }
      break;

    case 0x8200:
      Map83_IRQ_Cnt = ( Map83_IRQ_Cnt & 0xFF00 ) | (unsigned int)byData;
      break;

    case 0x8201:
      Map83_IRQ_Cnt = ( Map83_IRQ_Cnt & 0x00FF ) | ((unsigned int)byData << 8);
      Map83_IRQ_Enabled = byData;
      break;

    case 0x8300:
      ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8301:
      ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8302:
      ROMBANK2 = ROMPAGE( byData % (NesHeader.byRomSize << 1) );	
      break;

    case 0x8310:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 0 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      { 
	PPUBANK[ 0 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 1 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8311:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 1 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 2 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 3 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8312:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 2 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8313:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 3 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8314:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      { 
	PPUBANK[ 4 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8315:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 5 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8316:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 6 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ((Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 4 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 5 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8317:
      if ((Map83_Regs[1] & 0x30) == 0x30)
      {
	PPUBANK[ 7 ] = VROMPAGE( (Map83_Chr_Bank^byData) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      else if ( (Map83_Regs[1] & 0x30) == 0x10 || (Map83_Regs[1] & 0x30) == 0x20)
      {
	PPUBANK[ 6 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+0) % (NesHeader.byVRomSize << 3) );
	PPUBANK[ 7 ] = VROMPAGE( (((Map83_Chr_Bank^byData)*2)+1) % (NesHeader.byVRomSize << 3) );
	InfoNES_SetupChr();
      }
      break;

    case 0x8318:
      ROMBANK0 = ROMPAGE( (((Map83_Regs[0]&0x30)|byData)*2+0) % (NesHeader.byRomSize << 1) );	
      ROMBANK1 = ROMPAGE( (((Map83_Regs[0]&0x30)|byData)*2+1) % (NesHeader.byRomSize << 1) );	
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 83 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map83_HSync()
{
  if (Map83_IRQ_Enabled)
  {
    if (Map83_IRQ_Cnt <= 114)
    {
      IRQ_REQ;
      Map83_IRQ_Enabled = 0;
    }
    else
    {
      Map83_IRQ_Cnt -= 114;
    }
  }
}
//InfoNES_Mapper_083 end











#ifndef use_lib_not_use_mapper85
//InfoNES_Mapper_085 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 85 (Konami VRC7)                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map85_Chr_Ram[ 0x100 * 0x400 ];
unsigned char Map85_Regs[ 1 ];
unsigned char Map85_IRQ_Enable;
unsigned char Map85_IRQ_Cnt;
unsigned char Map85_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 85                                             */
/*-------------------------------------------------------------------*/
void Map85_Init()
{
  /* Initialize Mapper */
  MapperInit = Map85_Init;

  /* Write to Mapper */
  MapperWrite = Map85_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map85_HSync;

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
  PPUBANK[ 0 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 1 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 2 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 3 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 4 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 5 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 6 ] = Map85_VROMPAGE( 0 );
  PPUBANK[ 7 ] = Map85_VROMPAGE( 0 );
  InfoNES_SetupChr();

  /* Initialize State Registers */
  Map85_Regs[ 0 ] = 0;
  Map85_IRQ_Enable = 0;
  Map85_IRQ_Cnt = 0;
  Map85_IRQ_Latch = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 85 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map85_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xf030 )
  {
    case 0x8000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      break;

    case 0x8010:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK1 = ROMPAGE( byData );
      break;

    case 0x9000:
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK2 = ROMPAGE( byData );
      break;

    case 0x9010:
    case 0x9030:
      /* Extra Sound */

    case 0xa000:
      PPUBANK[ 0 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xa010:
      PPUBANK[ 1 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xb000:
      PPUBANK[ 2 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xb010:
      PPUBANK[ 3 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc000:
      PPUBANK[ 4 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xc010:
      PPUBANK[ 5 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd000:
      PPUBANK[ 6 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    case 0xd010:
      PPUBANK[ 7 ] = Map85_VROMPAGE( byData );
      InfoNES_SetupChr();
      break;

    /* Name Table Mirroring */
    case 0xe000:
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

    case 0xe010:
      Map85_IRQ_Latch = 0xff - byData;
      break;

    case 0xf000:
      Map85_Regs[ 0 ] = byData & 0x01;
      Map85_IRQ_Enable = ( byData & 0x02 ) >> 1;
      Map85_IRQ_Cnt = Map85_IRQ_Latch;
      break;

    case 0xf010:
      Map85_IRQ_Enable = Map85_Regs[ 0 ];
      Map85_IRQ_Cnt = Map85_IRQ_Latch;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 85 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map85_HSync()
{
  if ( Map85_IRQ_Enable )
  {
    if ( Map85_IRQ_Cnt == 0 )
    {
      IRQ_REQ;
      Map85_IRQ_Enable = 0;
    } else {
      Map85_IRQ_Cnt--;
    }
  }
}
//InfoNES_Mapper_085 end
#endif










//InfoNES_Mapper_086 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 86 (Jaleco)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 86                                             */
/*-------------------------------------------------------------------*/
void Map86_Init()
{
  /* Initialize Mapper */
  MapperInit = Map86_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map86_Sram;

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
/*  Mapper 86 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map86_Sram( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byChrBank;
  unsigned char byPrgBank;

  switch ( wAddr )
  {
    case 0x6000:
      byChrBank = byData & 0x03 | ( byData & 0x40 ) >> 4;
      byPrgBank = ( byData & 0x30 ) >> 4;

      byPrgBank = ( byPrgBank << 2 ) % ( NesHeader.byRomSize << 1 );
      byChrBank = ( byChrBank << 3 ) % ( NesHeader.byVRomSize << 3 );

      /* Set ROM Banks */
      ROMBANK0 = ROMPAGE( byPrgBank + 0 );
      ROMBANK1 = ROMPAGE( byPrgBank + 1 );
      ROMBANK2 = ROMPAGE( byPrgBank + 2 );
      ROMBANK3 = ROMPAGE( byPrgBank + 3 );

      /* Set PPU Banks */
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
//InfoNES_Mapper_086 end













//InfoNES_Mapper_087 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 87 (74161/32)                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 87                                             */
/*-------------------------------------------------------------------*/
void Map87_Init()
{
  /* Initialize Mapper */
  MapperInit = Map87_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map87_Sram;

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
/*  Mapper 87 Write to SRAM Function                                 */
/*-------------------------------------------------------------------*/
void Map87_Sram( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byChrBank;

  switch ( wAddr )
  {
    case 0x6000:
      byChrBank = ( byData & 0x02 ) >> 1;
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
      break;
  }
}
//InfoNES_Mapper_087 end









//InfoNES_Mapper_088 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 88 (Namco 118)                       */
/*                                                                   */
/*===================================================================*/

unsigned char  Map88_Regs[ 1 ];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 88                                             */
/*-------------------------------------------------------------------*/
void Map88_Init()
{
  /* Initialize Mapper */
  MapperInit = Map88_Init;

  /* Write to Mapper */
  MapperWrite = Map88_Write;

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
/*  Mapper 88 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map88_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x8000:
      Map88_Regs[ 0 ] = byData;
      break;

    case 0x8001:
      switch ( Map88_Regs[ 0 ] & 0x07 )
      { 
        case 0x00:
          PPUBANK[ 0 ] = VROMPAGE( ( ( byData & 0xfe ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 1 ] = VROMPAGE( ( ( byData & 0xfe ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x01:
          PPUBANK[ 2 ] = VROMPAGE( ( ( byData & 0xfe ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
          PPUBANK[ 3 ] = VROMPAGE( ( ( byData & 0xfe ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x02:
          PPUBANK[ 4 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x03:
          PPUBANK[ 5 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x04:
          PPUBANK[ 6 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x05:
          PPUBANK[ 7 ] = VROMPAGE( ( byData + 0x40 ) % ( NesHeader.byVRomSize << 3 ) );
          InfoNES_SetupChr();
          break;

        case 0x06:
          ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;

        case 0x07:
          ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) );
          break;
      }
      break;

    case 0xc000:
      if ( byData ) 
      {
        InfoNES_Mirroring( 2 );
      } else {
        InfoNES_Mirroring( 3 );
      }
      break;
  }
}
//InfoNES_Mapper_088 end












//InfoNES_Mapper_089 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 89 (Sunsoft)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 89                                             */
/*-------------------------------------------------------------------*/
void Map89_Init()
{
  /* Initialize Mapper */
  MapperInit = Map89_Init;

  /* Write to Mapper */
  MapperWrite = Map89_Write;

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
/*  Mapper 89 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map89_Write( unsigned short int wAddr, unsigned char byData )
{
  if ( ( wAddr & 0xFF00 ) == 0xC000 )
  {
    unsigned char byPrgBank = (byData & 0x70) >> 4;
    unsigned char byChrBank = ((byData & 0x80) >> 4) | (byData & 0x07);

    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE( (byPrgBank*2+0) % (NesHeader.byRomSize << 1) );
    ROMBANK1 = ROMPAGE( (byPrgBank*2+1) % (NesHeader.byRomSize << 1) );

    PPUBANK[ 0 ] = VROMPAGE( (byChrBank*8+0) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 1 ] = VROMPAGE( (byChrBank*8+1) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 2 ] = VROMPAGE( (byChrBank*8+2) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 3 ] = VROMPAGE( (byChrBank*8+3) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 4 ] = VROMPAGE( (byChrBank*8+4) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 5 ] = VROMPAGE( (byChrBank*8+5) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 6 ] = VROMPAGE( (byChrBank*8+6) % (NesHeader.byVRomSize << 3) );
    PPUBANK[ 7 ] = VROMPAGE( (byChrBank*8+7) % (NesHeader.byVRomSize << 3) );
    InfoNES_SetupChr();

    if ( byData & 0x08 )
    {
      InfoNES_Mirroring( 2 );
    }
    else
    {
      InfoNES_Mirroring( 3 );
    }
  }
}
//InfoNES_Mapper_089 end











//InfoNES_Mapper_090 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 90 (PC-JY-??)                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map90_Prg_Reg[ 4 ];
unsigned char Map90_Chr_Low_Reg[ 8 ];
unsigned char Map90_Chr_High_Reg[ 8 ];
unsigned char Map90_Nam_Low_Reg[ 4 ];
unsigned char Map90_Nam_High_Reg[ 4 ];

unsigned char Map90_Prg_Bank_Size;
unsigned char Map90_Prg_Bank_6000;
unsigned char Map90_Prg_Bank_E000;
unsigned char Map90_Chr_Bank_Size;
unsigned char Map90_Mirror_Mode;
unsigned char Map90_Mirror_Type;

unsigned int Map90_Value1;
unsigned int Map90_Value2;

unsigned char Map90_IRQ_Enable;
unsigned char Map90_IRQ_Cnt;
unsigned char Map90_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 90                                             */
/*-------------------------------------------------------------------*/
void Map90_Init()
{
  /* Initialize Mapper */
  MapperInit = Map90_Init;

  /* Write to Mapper */
  MapperWrite = Map90_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map90_Apu;

  /* Read from APU */
  MapperReadApu = Map90_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map90_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

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

  /* Initialize IRQ Registers */
  Map90_IRQ_Cnt = 0;
  Map90_IRQ_Latch = 0;
  Map90_IRQ_Enable = 0;

  for ( unsigned char byPage = 0; byPage < 4; byPage++ )
  {
    Map90_Prg_Reg[ byPage ] = ( NesHeader.byRomSize << 1 ) - 4 + byPage;
    Map90_Nam_Low_Reg[ byPage ] = 0;
    Map90_Nam_High_Reg[ byPage ] = 0;
    Map90_Chr_Low_Reg[ byPage ] = byPage;
    Map90_Chr_High_Reg[ byPage ] = 0;
    Map90_Chr_Low_Reg[ byPage + 4 ] = byPage + 4;
    Map90_Chr_High_Reg[ byPage + 4 ] = 0;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map90_Apu( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    case 0x5000:
      Map90_Value1 = byData;
      break;

    case 0x5001:
      Map90_Value2 = byData;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
unsigned char Map90_ReadApu( unsigned short int wAddr )
{
  if ( wAddr == 0x5000 )
  {
    return ( unsigned char )( ( Map90_Value1 * Map90_Value2 ) & 0x00ff );
  } else {
    return ( unsigned char )( wAddr >> 8 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map90_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr )
  {
    /* Set ROM Banks */
    case 0x8000:
    case 0x8001:
    case 0x8002:
    case 0x8003:
      Map90_Prg_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Prg_Banks();
      break;
    
    case 0x9000:
    case 0x9001:
    case 0x9002:
    case 0x9003:
    case 0x9004:
    case 0x9005:
    case 0x9006:
    case 0x9007:
      Map90_Chr_Low_Reg[ wAddr & 0x07 ] = byData;
      Map90_Sync_Chr_Banks();
      break;

    case 0xa000:
    case 0xa001:
    case 0xa002:
    case 0xa003:
    case 0xa004:
    case 0xa005:
    case 0xa006:
    case 0xa007:
      Map90_Chr_High_Reg[ wAddr & 0x07 ] = byData;
      Map90_Sync_Chr_Banks();
      break;

    case 0xb000:
    case 0xb001:
    case 0xb002:
    case 0xb003:
      Map90_Nam_Low_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Mirror();
      break;

    case 0xb004:
    case 0xb005:
    case 0xb006:
    case 0xb007:
      Map90_Nam_High_Reg[ wAddr & 0x03 ] = byData;
      Map90_Sync_Mirror();
      break;

    case 0xc002:
      Map90_IRQ_Enable = 0;
      break;

    case 0xc003:
    case 0xc004:
      if ( Map90_IRQ_Enable == 0 )
      {
        Map90_IRQ_Enable = 1;
        Map90_IRQ_Cnt = Map90_IRQ_Latch;
      }
      break;

    case 0xc005:
      Map90_IRQ_Cnt = byData;
      Map90_IRQ_Latch = byData;
      break;

    case 0xd000:
      Map90_Prg_Bank_6000 = byData & 0x80;
      Map90_Prg_Bank_E000 = byData & 0x04;
      Map90_Prg_Bank_Size = byData & 0x03;
      Map90_Chr_Bank_Size = ( byData & 0x18 ) >> 3;
      Map90_Mirror_Mode = byData & 0x20;
      
      Map90_Sync_Prg_Banks();
      Map90_Sync_Chr_Banks();
      Map90_Sync_Mirror();
      break;

    case 0xd001:
      Map90_Mirror_Type = byData & 0x03;
      Map90_Sync_Mirror();
      break;

    case 0xd003:
      /* Bank Page*/
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 H-Sync Function                                        */
/*-------------------------------------------------------------------*/
void Map90_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( /* 0 <= PPU_Scanline && */ PPU_Scanline <= 239 )
  {
    if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP )
    {
      if ( Map90_IRQ_Cnt == 0 )
      {
        if ( Map90_IRQ_Enable )
        {
          IRQ_REQ;
        }
        Map90_IRQ_Latch = 0;
        Map90_IRQ_Enable = 0;
      } else {
        Map90_IRQ_Cnt--;
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Mirror Function                                   */
/*-------------------------------------------------------------------*/
void Map90_Sync_Mirror( void )
{
  unsigned char byPage;
  unsigned int dwNamBank[ 4 ];

  for ( byPage = 0; byPage < 4; byPage++ )
  {
    dwNamBank[ byPage ] = 
      ( (unsigned int)Map90_Nam_High_Reg[ byPage ] << 8 ) | (unsigned int)Map90_Nam_Low_Reg[ byPage ];
  }

  if ( Map90_Mirror_Mode )
  {
    for ( byPage = 0; byPage < 4; byPage++ )
    {
      if ( !Map90_Nam_High_Reg[ byPage ] && ( Map90_Nam_Low_Reg[ byPage ] == byPage ) )
      {
        Map90_Mirror_Mode = 0;
      }
    }

    if ( Map90_Mirror_Mode )
    {
        PPUBANK[ NAME_TABLE0 ] = VROMPAGE( dwNamBank[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE1 ] = VROMPAGE( dwNamBank[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE2 ] = VROMPAGE( dwNamBank[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
        PPUBANK[ NAME_TABLE3 ] = VROMPAGE( dwNamBank[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
    }
  } else {
    switch ( Map90_Mirror_Type )
    {
      case 0x00:
        InfoNES_Mirroring( 1 );
        break;

      case 0x01:
        InfoNES_Mirroring( 0 );
        break;

      default:
        InfoNES_Mirroring( 3 );
        break;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Char Banks Function                               */
/*-------------------------------------------------------------------*/
void Map90_Sync_Chr_Banks( void )
{
  unsigned char byPage;
  unsigned int dwChrBank[ 8 ];

  for ( byPage = 0; byPage < 8; byPage++ )
  {
    dwChrBank[ byPage ] =
      ( (unsigned int)Map90_Chr_High_Reg[ byPage ] << 8 ) | (unsigned int)Map90_Chr_Low_Reg[ byPage ];
  }

  switch ( Map90_Chr_Bank_Size )
  {
    case 0:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 3 ) + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 1:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 2 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 2 ) + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 2:
      PPUBANK[ 0 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( ( dwChrBank[ 0 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( ( dwChrBank[ 2 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( ( dwChrBank[ 2 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( ( dwChrBank[ 4 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( ( dwChrBank[ 6 ] << 1 ) + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( ( dwChrBank[ 6 ] << 1 ) + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    default:
      PPUBANK[ 0 ] = VROMPAGE( dwChrBank[ 0 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( dwChrBank[ 1 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( dwChrBank[ 2 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( dwChrBank[ 3 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( dwChrBank[ 4 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( dwChrBank[ 5 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( dwChrBank[ 6 ] % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( dwChrBank[ 7 ] % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 90 Sync Program Banks Function                            */
/*-------------------------------------------------------------------*/
void Map90_Sync_Prg_Banks( void )
{
  switch ( Map90_Prg_Bank_Size )
  {
    case 0:
      ROMBANK0 = ROMLASTPAGE( 3 );
      ROMBANK1 = ROMLASTPAGE( 2 );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );      
      break;

    case 1:
      ROMBANK0 = ROMPAGE( ( ( Map90_Prg_Reg[ 1 ] << 1 ) + 0 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ( ( Map90_Prg_Reg[ 1 ] << 1 ) + 1 ) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMLASTPAGE( 1 );
      ROMBANK3 = ROMLASTPAGE( 0 );      
      break;

    case 2:
      if ( Map90_Prg_Bank_E000 )
      {
        ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );   
      } else {
        if ( Map90_Prg_Bank_6000 )
        {
          SRAMBANK = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );
        }
        ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMLASTPAGE( 0 );  
      }
      break;

    default:
      /* 8k in reverse mode? */
      ROMBANK0 = ROMPAGE( Map90_Prg_Reg[ 3 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( Map90_Prg_Reg[ 2 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( Map90_Prg_Reg[ 1 ] % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( Map90_Prg_Reg[ 0 ] % ( NesHeader.byRomSize << 1 ) ); 
      break;
  }
}
//InfoNES_Mapper_090 end












//InfoNES_Mapper_091 begin
/*===================================================================*/
/*                                                                   */
/*                        Mapper 91 (Pirates)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 91                                             */
/*-------------------------------------------------------------------*/
void Map91_Init()
{
  /* Initialize Mapper */
  MapperInit = Map91_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map91_Sram;

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
  ROMBANK0 = ROMLASTPAGE( 1 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    for ( int nPage = 0; nPage < 8; ++nPage )
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr();
  }

  /* Set Name Table Mirroring */
  InfoNES_Mirroring( 1 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 91 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map91_Sram( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xF00F)
  {
    /* Set PPU Banks */
    case 0x6000:
      PPUBANK[ 0 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6001:
      PPUBANK[ 2 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6002:
      PPUBANK[ 4 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    case 0x6003:
      PPUBANK[ 6 ] = VROMPAGE( (byData*2+0) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( (byData*2+1) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
      break;

    /* Set CPU Banks */
    case 0x7000:
      ROMBANK0 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;

    case 0x7001:
      ROMBANK1 = ROMPAGE( byData % ( NesHeader.byRomSize << 1 ) ); 
      break;
  }
}
//InfoNES_Mapper_091 end










//InfoNES_Mapper_092 begin
/*===================================================================*/
/*                                                                   */
/*              Mapper 92 (Jaleco Early Mapper #1)                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 92                                             */
/*-------------------------------------------------------------------*/
void Map92_Init()
{
  /* Initialize Mapper */
  MapperInit = Map92_Init;

  /* Write to Mapper */
  MapperWrite = Map92_Write;

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
/*  Mapper 92 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map92_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char byRomBank;
  unsigned char byChrBank;
  
  byData = (unsigned char)wAddr & 0xff;
  byRomBank = ( byData & 0x0f ) << 1;
  byChrBank = byData & 0x0f;

  if ( wAddr >= 0x9000 )
  {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xd0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byRomBank );
        ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;
        
      /* Set PPU Banks */
      case 0xe0:
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
        break;
    }
  } else {
    switch ( byData & 0xf0 )
    {
      /* Set ROM Banks */
      case 0xb0:
        byRomBank %= ( NesHeader.byRomSize << 1 );
        ROMBANK2 = ROMPAGE( byRomBank );
        ROMBANK3 = ROMPAGE( byRomBank + 1 );     
        break;

      /* Set PPU Banks */
      case 0x70:
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
        break;
    }
  }
}
//InfoNES_Mapper_092 end










//InfoNES_Mapper_093 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 93 (74161/32)                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 93                                             */
/*-------------------------------------------------------------------*/
void Map93_Init()
{
  /* Initialize Mapper */
  MapperInit = Map93_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map93_Sram;

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
/*  Mapper 93 Write to Sram Function                                 */
/*-------------------------------------------------------------------*/
void Map93_Sram( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  if ( wAddr == 0x6000 )
  {
    byData <<= 1;
    byData %= ( NesHeader.byRomSize << 1 );
  
    ROMBANK0 = ROMPAGE( byData );
    ROMBANK1 = ROMPAGE( byData + 1 );
  }
}
//InfoNES_Mapper_093 end










//InfoNES_Mapper_094 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 94 (74161/32 Capcom)                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 94                                             */
/*-------------------------------------------------------------------*/
void Map94_Init()
{
  /* Initialize Mapper */
  MapperInit = Map94_Init;

  /* Write to Mapper */
  MapperWrite = Map94_Write;

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
/*  Mapper 94 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map94_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xfff0 )
  {
    /* Set ROM Banks */
    case 0xff00:
      byData = ( byData & 0x1c ) >> 2;
      byData <<= 1;
      byData %= ( NesHeader.byRomSize << 1 );
      ROMBANK0 = ROMPAGE( byData );
      ROMBANK1 = ROMPAGE( byData + 1 );
      break;
  }
}
//InfoNES_Mapper_094 end












//InfoNES_Mapper_095 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 95 (Namco 1??)                       */
/*                                                                   */
/*===================================================================*/

unsigned char  Map95_Regs[ 1 ];
unsigned int Map95_Prg0, Map95_Prg1;
unsigned int Map95_Chr01, Map95_Chr23;
unsigned int Map95_Chr4, Map95_Chr5, Map95_Chr6, Map95_Chr7;

#define Map95_Chr_Swap()    ( Map95_Regs[ 0 ] & 0x80 )
#define Map95_Prg_Swap()    ( Map95_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 95                                             */
/*-------------------------------------------------------------------*/
void Map95_Init()
{
  /* Initialize Mapper */
  MapperInit = Map95_Init;

  /* Write to Mapper */
  MapperWrite = Map95_Write;

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
  Map95_Regs[ 0 ] = 0;

  /* Set ROM Banks */
  Map95_Prg0 = 0;
  Map95_Prg1 = 1;
  Map95_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    Map95_Chr01 = 0;
    Map95_Chr23 = 2;
    Map95_Chr4  = 4;
    Map95_Chr5  = 5;
    Map95_Chr6  = 6;
    Map95_Chr7  = 7;
    Map95_Set_PPU_Banks();
  } else {
    Map95_Chr01 = Map95_Chr23 = 0;
    Map95_Chr4 = Map95_Chr5 = Map95_Chr6 = Map95_Chr7 = 0;
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map95_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned int dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map95_Regs[ 0 ] = byData;
      Map95_Set_PPU_Banks();
      Map95_Set_CPU_Banks();
      break;

    case 0x8001:
      if ( Map95_Regs[ 0 ] <= 0x05 )
      {
        if ( byData & 0x20 ) 
        {
          InfoNES_Mirroring( 2 );
        } else {
          InfoNES_Mirroring( 3 );
        }
        byData &= 0x1f;
      }

      dwBankNum = byData;

      switch ( Map95_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr01 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( NesHeader.byVRomSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map95_Chr23 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr4 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr5 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr6 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( NesHeader.byVRomSize > 0 )
          {
            Map95_Chr7 = dwBankNum;
            Map95_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map95_Prg0 = dwBankNum;
          Map95_Set_CPU_Banks();
          break;

        case 0x07:
          Map95_Prg1 = dwBankNum;
          Map95_Set_CPU_Banks();
          break;
      }
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set CPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_CPU_Banks()
{
  if ( Map95_Prg_Swap() )
  {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE( Map95_Prg0 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK1 = ROMPAGE( Map95_Prg1 % ( NesHeader.byRomSize << 1 ) );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 95 Set PPU Banks Function                                 */
/*-------------------------------------------------------------------*/
void Map95_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
    if ( Map95_Chr_Swap() )
    { 
      PPUBANK[ 0 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map95_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map95_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map95_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map95_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map95_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map95_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map95_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map95_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_095 end










//InfoNES_Mapper_096 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 96 : Bandai 74161                        */
/*                                                                   */
/*===================================================================*/

unsigned char	Map96_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 96                                             */
/*-------------------------------------------------------------------*/
void Map96_Init()
{
  /* Initialize Mapper */
  MapperInit = Map96_Init;

  /* Write to Mapper */
  MapperWrite = Map96_Write;

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
  MapperPPU = Map96_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  Map96_Reg[0] = Map96_Reg[1] = 0;

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  Map96_Set_Banks();
  InfoNES_Mirroring( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map96_Write( unsigned short int wAddr, unsigned char byData )
{
  ROMBANK0 = ROMPAGE((((byData & 0x03)<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE((((byData & 0x03)<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE((((byData & 0x03)<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE((((byData & 0x03)<<2)+3) % (NesHeader.byRomSize<<1));
  
  Map96_Reg[0] = (byData & 0x04) >> 2;
  Map96_Set_Banks();
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 PPU Function                                           */
/*-------------------------------------------------------------------*/
void Map96_PPU( unsigned short int wAddr )
{
  if( (wAddr & 0xF000) == 0x2000 ) {
    Map96_Reg[1] = (wAddr>>8)&0x03;
    Map96_Set_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 96 Set Banks Function                                     */
/*-------------------------------------------------------------------*/
void Map96_Set_Banks()
{
  PPUBANK[ 0 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+0);
  PPUBANK[ 1 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+1);
  PPUBANK[ 2 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+2);
  PPUBANK[ 3 ] = CRAMPAGE(((Map96_Reg[0]*4+Map96_Reg[1])<<2)+3);
  PPUBANK[ 4 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+0);
  PPUBANK[ 5 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+1);
  PPUBANK[ 6 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+2);
  PPUBANK[ 7 ] = CRAMPAGE(((Map96_Reg[0]*4+0x03)<<2)+3);
  InfoNES_SetupChr();
}
//InfoNES_Mapper_096 end










//InfoNES_Mapper_097 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 97 (74161/32 Irem)                        */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 97                                             */
/*-------------------------------------------------------------------*/
void Map97_Init()
{
  /* Initialize Mapper */
  MapperInit = Map97_Init;

  /* Write to Mapper */
  MapperWrite = Map97_Write;

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
  ROMBANK0 = ROMLASTPAGE( 1 );
  ROMBANK1 = ROMLASTPAGE( 0 );
  ROMBANK2 = ROMPAGE( 0 );
  ROMBANK3 = ROMPAGE( 1 );

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
/*  Mapper 97 Write Function                                         */
/*-------------------------------------------------------------------*/
void Map97_Write( unsigned short int wAddr, unsigned char byData )
{
  /* Set ROM Banks */
  if ( wAddr < 0xc000 )
  {
    unsigned char byPrgBank = byData & 0x0f;

    byPrgBank <<= 1;
    byPrgBank %= ( NesHeader.byRomSize << 1 );

    ROMBANK2 = ROMPAGE( byPrgBank );
    ROMBANK3 = ROMPAGE( byPrgBank + 1 );

    if ( ( byData & 0x80 ) == 0 )
    {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
  }
}
//InfoNES_Mapper_097 end











//InfoNES_Mapper_099 begin
/*===================================================================*/
/*                                                                   */
/*                    Mapper 099  VS-Unisystem                       */
/*                                                                   */
/*===================================================================*/

unsigned char Map99_Coin;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 099                                            */
/*-------------------------------------------------------------------*/
void Map99_Init()
{
  /* Initialize Mapper */
  MapperInit = Map99_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map99_Apu;

  /* Read from APU */
  MapperReadApu = Map99_ReadApu;

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

  Map99_Coin = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 99 Write to APU Function                                  */
/*-------------------------------------------------------------------*/
void Map99_Apu( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr == 0x4016 ) {
    if( byData & 0x04 ) {
      PPUBANK[ 0 ] = VROMPAGE(  8 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE(  9 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 10 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 11 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 12 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 13 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 14 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 15 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( 0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( 1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( 2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( 3 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( 4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( 5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( 6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( 7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }

  if( wAddr == 0x4020 ) {
    Map99_Coin = byData;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 99 Read from APU Function                                 */
/*-------------------------------------------------------------------*/
unsigned char Map99_ReadApu( unsigned short int wAddr )
{
  if( wAddr == 0x4020 ) {
    return Map99_Coin;
  }
  return ( wAddr >> 8 );
}
//InfoNES_Mapper_099 end










//InfoNES_Mapper_100 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 100 : Nestile MMC 3                      */
/*                                                                   */
/*===================================================================*/

unsigned char	Map100_Reg[8];
unsigned char	Map100_Prg0, Map100_Prg1, Map100_Prg2, Map100_Prg3;
unsigned char	Map100_Chr0, Map100_Chr1, Map100_Chr2, Map100_Chr3; 
unsigned char	Map100_Chr4, Map100_Chr5, Map100_Chr6, Map100_Chr7;

unsigned char	Map100_IRQ_Enable;
unsigned char	Map100_IRQ_Cnt;
unsigned char	Map100_IRQ_Latch;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 100                                            */
/*-------------------------------------------------------------------*/
void Map100_Init()
{
  /* Initialize Mapper */
  MapperInit = Map100_Init;

  /* Write to Mapper */
  MapperWrite = Map100_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map100_HSync;

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
    Map100_Chr0 = 0;
    Map100_Chr1 = 1;
    Map100_Chr2 = 2;
    Map100_Chr3 = 3;
    Map100_Chr4 = 4;
    Map100_Chr5 = 5;
    Map100_Chr6 = 6;
    Map100_Chr7 = 7;
    Map100_Set_PPU_Banks();
  } else {
    Map100_Chr0 = Map100_Chr2 = Map100_Chr4 = Map100_Chr5 = Map100_Chr6 = Map100_Chr7 = 0;
    Map100_Chr1 = Map100_Chr3 = 1;
  }

  /* Set IRQ Registers */
  Map100_IRQ_Enable = 0;
  Map100_IRQ_Cnt = 0;
  Map100_IRQ_Latch = 0;
  for( int i = 0; i < 8; i++ ) { Map100_Reg[ i ] = 0x00; }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map100_Write( unsigned short int wAddr, unsigned char byData )
{
  switch ( wAddr & 0xE001 )
  {
    case 0x8000:
      Map100_Reg[0] = byData;
      break;

    case 0x8001:
      Map100_Reg[1] = byData;

      switch ( Map100_Reg[0] & 0xC7 ) {
        case 0x00:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr0 = byData&0xFE;
	    Map100_Chr1 = Map100_Chr0+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;
	  
        case 0x01:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr2 = byData&0xFE;
	    Map100_Chr3 = Map100_Chr2+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x02:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr4 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x03:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr5 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x04:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr6 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x05:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr7 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x06:
	  Map100_Prg0 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x07:
	  Map100_Prg1 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x46:
	  Map100_Prg2 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x47:
	  Map100_Prg3 = byData;
	  Map100_Set_CPU_Banks();
	  break;

        case 0x80:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr4 = byData&0xFE;
	    Map100_Chr5 = Map100_Chr4+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x81:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr6 = byData&0xFE;
	    Map100_Chr7 = Map100_Chr6+1;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x82:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr0 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x83:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr1 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x84:
	  if ( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr2 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

        case 0x85:
	  if( NesHeader.byVRomSize > 0 ) {
	    Map100_Chr3 = byData;
	    Map100_Set_PPU_Banks();
	  }
	  break;

      }
      break;

    case 0xA000:
      Map100_Reg[2] = byData;
      if ( !ROM_FourScr )
      {
	if( byData & 0x01 ) InfoNES_Mirroring( 0 );
	else		    InfoNES_Mirroring( 1 );
      }
      break;

    case 0xA001:
      Map100_Reg[3] = byData;
      break;

    case 0xC000:
      Map100_Reg[4] = byData;
      Map100_IRQ_Cnt = byData;
      break;

    case 0xC001:
      Map100_Reg[5] = byData;
      Map100_IRQ_Latch = byData;
      break;

    case 0xE000:
      Map100_Reg[6] = byData;
      Map100_IRQ_Enable = 0;
      break;

    case 0xE001:
      Map100_Reg[7] = byData;
      Map100_IRQ_Enable = 0xFF;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map100_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if ( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map100_IRQ_Enable ) {
	if( !(Map100_IRQ_Cnt--) ) {
	  Map100_IRQ_Cnt = Map100_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map100_Set_CPU_Banks()
{
  ROMBANK0 = ROMPAGE( Map100_Prg0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( Map100_Prg1 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMPAGE( Map100_Prg2 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK3 = ROMPAGE( Map100_Prg3 % ( NesHeader.byRomSize << 1 ) );
}

/*-------------------------------------------------------------------*/
/*  Mapper 100 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map100_Set_PPU_Banks()
{
  if ( NesHeader.byVRomSize > 0 )
  {
      PPUBANK[ 0 ] = VROMPAGE( Map100_Chr0 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map100_Chr1 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map100_Chr2 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map100_Chr3 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map100_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map100_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map100_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map100_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_100 end











//InfoNES_Mapper_101 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 101 ()                                   */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 101                                            */
/*-------------------------------------------------------------------*/
void Map101_Init()
{
  /* Initialize Mapper */
  MapperInit = Map101_Init;

  /* Write to Mapper */
  MapperWrite = Map101_Write;

  /* Write to SRAM */
  MapperSram = Map101_Write;

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
/*  Mapper 101 Write & Write to SRAM Function                        */
/*-------------------------------------------------------------------*/
void Map101_Write( unsigned short int wAddr, unsigned char byData )
{
  byData &= 0x03;
  byData <<= 3;
  byData %= ( NesHeader.byVRomSize << 3 );

  /* Set PPU Banks */
  PPUBANK[ 0 ] = VROMPAGE( byData );
  PPUBANK[ 1 ] = VROMPAGE( byData + 1 );
  PPUBANK[ 2 ] = VROMPAGE( byData + 2 );
  PPUBANK[ 3 ] = VROMPAGE( byData + 3 );
  PPUBANK[ 4 ] = VROMPAGE( byData + 4 );
  PPUBANK[ 5 ] = VROMPAGE( byData + 5 );
  PPUBANK[ 6 ] = VROMPAGE( byData + 6 );
  PPUBANK[ 7 ] = VROMPAGE( byData + 7 );
  InfoNES_SetupChr();
}
//InfoNES_Mapper_101 end









//InfoNES_Mapper_105 begin
/*===================================================================*/
/*                                                                   */
/*        Mapper 105 : Nintendo World Championship                   */
/*                                                                   */
/*===================================================================*/

unsigned char	Map105_Init_State;
unsigned char	Map105_Write_Count;
unsigned char	Map105_Bits;
unsigned char	Map105_Reg[4];

unsigned char	Map105_IRQ_Enable;
int	Map105_IRQ_Counter;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 105                                            */
/*-------------------------------------------------------------------*/
void Map105_Init()
{
  /* Initialize Mapper */
  MapperInit = Map105_Init;

  /* Write to Mapper */
  MapperWrite = Map105_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map105_HSync;

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
  Map105_Reg[0] = 0x0C;
  Map105_Reg[1] = 0x00;
  Map105_Reg[2] = 0x00;
  Map105_Reg[3] = 0x10;
  
  Map105_Bits = 0;
  Map105_Write_Count = 0;
  
  Map105_IRQ_Counter = 0;
  Map105_IRQ_Enable = 0;
  Map105_Init_State = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 105 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map105_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned short int reg_num = (wAddr & 0x7FFF) >> 13;

  if( byData & 0x80 ) {
    Map105_Bits = Map105_Write_Count = 0;
    if( reg_num == 0 ) {
      Map105_Reg[reg_num] |= 0x0C;
    }
  } else {
    Map105_Bits |= (byData & 1) << Map105_Write_Count++;
    if( Map105_Write_Count == 5) {
      Map105_Reg[reg_num] = Map105_Bits & 0x1F;
      Map105_Bits = Map105_Write_Count = 0;
    }
  }

  if( Map105_Reg[0] & 0x02 ) {
    if( Map105_Reg[0] & 0x01 ) {
      InfoNES_Mirroring( 0 );
    } else {
      InfoNES_Mirroring( 1 );
    }
  } else {
    if( Map105_Reg[0] & 0x01 ) {
      InfoNES_Mirroring( 3 );
    } else {
      InfoNES_Mirroring( 2 );
    }
  }
  
  switch( Map105_Init_State ) {
  case 0:
  case 1:
    Map105_Init_State++;
    break;

  case 2:
    if(Map105_Reg[1] & 0x08) {
      if (Map105_Reg[0] & 0x08) {
	if (Map105_Reg[0] & 0x04) {
	  ROMBANK0 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK1 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK2 = ROMPAGE( 30 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK3 = ROMPAGE( 31 % ( NesHeader.byRomSize << 1 ) );
	} else {
	  ROMBANK0 = ROMPAGE( 16 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK1 = ROMPAGE( 17 % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK2 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	  ROMBANK3 = ROMPAGE( ((Map105_Reg[3] & 0x07) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	}
      } else {
	ROMBANK0 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 16) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK1 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 17) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK2 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 18) % ( NesHeader.byRomSize << 1 ) );
	ROMBANK3 = ROMPAGE( ((Map105_Reg[3] & 0x06) * 2 + 19) % ( NesHeader.byRomSize << 1 ) );
      }
    } else {
      ROMBANK0 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 0) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK1 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 1) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK2 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 2) % ( NesHeader.byRomSize << 1 ) );
      ROMBANK3 = ROMPAGE( ((Map105_Reg[1] & 0x06) * 2 + 3) % ( NesHeader.byRomSize << 1 ) );
    }
    
    if( Map105_Reg[1] & 0x10 ) {
      Map105_IRQ_Counter = 0;
      Map105_IRQ_Enable = 0;
    } else {
      Map105_IRQ_Enable = 1;
    }
    break;

  default:
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 105 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map105_HSync()
{
  if( !PPU_Scanline ) {
    if( Map105_IRQ_Enable ) {
      Map105_IRQ_Counter += 29781;
    }
    if( ((Map105_IRQ_Counter | 0x21FFFFFF) & 0x3E000000) == 0x3E000000 ) {
      IRQ_REQ;
    }
  }
}
//InfoNES_Mapper_105 end



