#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"
#include "InfoNESMapper.h"
#include "InfoNES.h"
#include "K6502.h"
#include "InfoNESSystem.h"


//InfoNES_Mapper_230 begin
/*===================================================================*/
/*                                                                   */
/*                       Mapper 230 : 22-in-1                        */
/*                                                                   */
/*===================================================================*/

unsigned char Map230_RomSw;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 230                                            */
/*-------------------------------------------------------------------*/
void Map230_Init()
{
  /* Initialize Mapper */
  MapperInit = Map230_Init;

  /* Write to Mapper */
  MapperWrite = Map230_Write;

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

  /* Initialize Registers */
  if( Map230_RomSw ) {
    Map230_RomSw = 0;
  } else {
    Map230_RomSw = 1;
  }

  /* Set ROM Banks */
  if( Map230_RomSw ) {
    ROMBANK0 = ROMPAGE( 0 );
    ROMBANK1 = ROMPAGE( 1 );
    ROMBANK2 = ROMPAGE( 14 );
    ROMBANK3 = ROMPAGE( 15 );
  } else {
    ROMBANK0 = ROMPAGE( 16 );
    ROMBANK1 = ROMPAGE( 17 );
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 230 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map230_Write( unsigned short int wAddr, unsigned char byData )
{
  if( Map230_RomSw ) {
    ROMBANK0 = ROMPAGE((((byData&0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0x07)<<1)+1) % (NesHeader.byRomSize<<1));
  } else {
    if( byData & 0x20 ) {
      ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+17) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+16) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+17) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+18) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+19) % (NesHeader.byRomSize<<1));
    }
    if( byData & 0x40 ) {
      InfoNES_Mirroring( 1 );
    } else {
      InfoNES_Mirroring( 0 );
    }
  }
}
//InfoNES_Mapper_230 end











//InfoNES_Mapper_231 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 231 : 20-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 231                                            */
/*-------------------------------------------------------------------*/
void Map231_Init()
{
  /* Initialize Mapper */
  MapperInit = Map231_Init;

  /* Write to Mapper */
  MapperWrite = Map231_Write;

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
/*  Mapper 231 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map231_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byBank = wAddr & 0x1E;

  if( wAddr & 0x0020 ) {
    ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<1)+3) % (NesHeader.byRomSize<<1));
  } else {
    ROMBANK0 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byBank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byBank<<1)+1) % (NesHeader.byRomSize<<1));
  }

  if( wAddr & 0x0080 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
//InfoNES_Mapper_231 end











//InfoNES_Mapper_232 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 232 : Quattro Games                      */
/*                                                                   */
/*===================================================================*/

unsigned char Map232_Regs[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 232                                            */
/*-------------------------------------------------------------------*/
void Map232_Init()
{
  /* Initialize Mapper */
  MapperInit = Map232_Init;

  /* Write to Mapper */
  MapperWrite = Map232_Write;

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

  /* Initialize Registers */
  Map232_Regs[0] = 0x0C;
  Map232_Regs[1] = 0x00;
  
  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 232 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map232_Write( unsigned short int wAddr, unsigned char byData )
{
  if ( wAddr == 0x9000 ) {
    Map232_Regs[0] = (byData & 0x18)>>1;
  } else if ( 0xA000 <= wAddr /* && wAddr <= 0xFFFF */ ) {
    Map232_Regs[1] = byData & 0x03;
  }
  
  ROMBANK0= ROMPAGE((((Map232_Regs[0]|Map232_Regs[1])<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1= ROMPAGE((((Map232_Regs[0]|Map232_Regs[1])<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2= ROMPAGE((((Map232_Regs[0]|0x03)<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK3= ROMPAGE((((Map232_Regs[0]|0x03)<<1)+1) % (NesHeader.byRomSize<<1));
}
//InfoNES_Mapper_232 end











//InfoNES_Mapper_233 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 233 : 42-in-1                          */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 233                                            */
/*-------------------------------------------------------------------*/
void Map233_Init()
{
  /* Initialize Mapper */
  MapperInit = Map233_Init;

  /* Write to Mapper */
  MapperWrite = Map233_Write;

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
/*  Mapper 233 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map233_Write( unsigned short int wAddr, unsigned char byData )
{
  if( byData & 0x20 ) {
    ROMBANK0 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
    ROMBANK2 = ROMPAGE((((byData&0x1F)<<1)+0) % (NesHeader.byRomSize<<1)); 
    ROMBANK3 = ROMPAGE((((byData&0x1F)<<1)+1) % (NesHeader.byRomSize<<1)); 
  } else {
    ROMBANK0 = ROMPAGE((((byData&0x1E)<<1)+0) % (NesHeader.byRomSize<<1)); 
    ROMBANK1 = ROMPAGE((((byData&0x1E)<<1)+1) % (NesHeader.byRomSize<<1)); 
    ROMBANK2 = ROMPAGE((((byData&0x1E)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((byData&0x1E)<<1)+3) % (NesHeader.byRomSize<<1)); 
  }

  if( (byData&0xC0) == 0x00 ) {
    InfoNES_Mirroring( 5 );
  } else if( (byData&0xC0) == 0x40 ) {
    InfoNES_Mirroring( 1 );
  } else if( (byData&0xC0) == 0x80 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 2 );
  }
}
//InfoNES_Mapper_233 end










//InfoNES_Mapper_234 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 234 : Maxi-15                          */
/*                                                                   */
/*===================================================================*/

unsigned char	Map234_Reg[2];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 234                                            */
/*-------------------------------------------------------------------*/
void Map234_Init()
{
  /* Initialize Mapper */
  MapperInit = Map234_Init;

  /* Write to Mapper */
  MapperWrite = Map234_Write;

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

  /* Set Registers */
  Map234_Reg[0] = 0;
  Map234_Reg[1] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 234 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map234_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr >= 0xFF80 && wAddr <= 0xFF9F ) {
    if( !Map234_Reg[0] ) {
      Map234_Reg[0] = byData;
      Map234_Set_Banks();
    }
  }
  
  if( wAddr >= 0xFFE8 && wAddr <= 0xFFF7 ) {
    Map234_Reg[1] = byData;
    Map234_Set_Banks();
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 234 Set Banks Function                                    */
/*-------------------------------------------------------------------*/
void Map234_Set_Banks()
{
  unsigned char byPrg, byChr;

  if( Map234_Reg[0] & 0x80 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
  if( Map234_Reg[0] & 0x40 ) {
    byPrg = (Map234_Reg[0]&0x0E)|(Map234_Reg[1]&0x01);
    byChr = ((Map234_Reg[0]&0x0E)<<2)|((Map234_Reg[1]>>4)&0x07);
  } else {
    byPrg = Map234_Reg[0]&0x0F;
    byChr = ((Map234_Reg[0]&0x0F)<<2)|((Map234_Reg[1]>>4)&0x03);
  }
  
  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
  ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));

  /* Set PPU Banks */
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
//InfoNES_Mapper_234 end





#ifndef use_lib_not_use_mapper235
//InfoNES_Mapper_235 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 235 : 150-in-1                         */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 235                                            */
/*-------------------------------------------------------------------*/
void Map235_Init()
{
  /* Initialize Mapper */
  MapperInit = Map235_Init;

  /* Write to Mapper */
  MapperWrite = Map235_Write;

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

  /* Set Registers */
  for( int i = 0; i < 0x2000; i++ ) {
    DRAM[i] = 0xFF;
  }

  /* Set ROM Banks */
  ROMBANK0 = ROMPAGE( 0 );
  ROMBANK1 = ROMPAGE( 1 );
  ROMBANK2 = ROMPAGE( 2 );
  ROMBANK3 = ROMPAGE( 3 );

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 235 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map235_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byPrg = ((wAddr&0x0300)>>3)|(wAddr&0x001F);
  unsigned char	byBus = 0;

  if( (NesHeader.byRomSize<<1) == 64*2 ) {
    // 100-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byBus = 1; break;
    case	0x0300:	byBus = 1; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 128*2 ) {
    // 150-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byPrg = (byPrg&0x1F)|0x20; break;
    case	0x0300:	byBus = 1; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 192*2 ) {
    // 150-in-1
    switch( wAddr & 0x0300 ) {
    case	0x0000:	break;
    case	0x0100:	byBus = 1; break;
    case	0x0200:	byPrg = (byPrg&0x1F)|0x20; break;
    case	0x0300:	byPrg = (byPrg&0x1F)|0x40; break;
    }
  } else if( (NesHeader.byRomSize<<1) == 256*2 ) {
  }
  
  if( wAddr & 0x0800 ) {
    if( wAddr & 0x1000 ) {
      ROMBANK0 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE(((byPrg<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byPrg<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byPrg<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byPrg<<2)+3) % (NesHeader.byRomSize<<1));
  }
  
  if( byBus ) {
    ROMBANK0 = DRAM;
    ROMBANK1 = DRAM;
    ROMBANK2 = DRAM;
    ROMBANK3 = DRAM;
  }

  if( wAddr & 0x0400 ) {
    InfoNES_Mirroring( 3 );
  } else if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }
}
//InfoNES_Mapper_235 end
#endif










//InfoNES_Mapper_236 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 236 : 800-in-1                           */
/*                                                                   */
/*===================================================================*/

unsigned char    Map236_Bank, Map236_Mode;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 236                                            */
/*-------------------------------------------------------------------*/
void Map236_Init()
{
  /* Initialize Mapper */
  MapperInit = Map236_Init;

  /* Write to Mapper */
  MapperWrite = Map236_Write;

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

  /* Set Registers */
  Map236_Bank = Map236_Mode = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 236 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map236_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr >= 0x8000 && wAddr <= 0xBFFF ) {
    Map236_Bank = ((wAddr&0x03)<<4)|(Map236_Bank&0x07);
  } else {
    Map236_Bank = (wAddr&0x07)|(Map236_Bank&0x30);
    Map236_Mode = wAddr&0x30;
  }

  if( wAddr & 0x20 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }

  switch( Map236_Mode ) {
  case	0x00:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x10:
    Map236_Bank |= 0x37;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank|0x07)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank|0x07)<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  case	0x20:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE((((Map236_Bank&0xFE)<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((Map236_Bank&0xFE)<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((Map236_Bank&0xFE)<<1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((Map236_Bank&0xFE)<<1)+3) % (NesHeader.byRomSize<<1));
    break;
  case	0x30:
    Map236_Bank |= 0x08;
    ROMBANK0 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((Map236_Bank<<1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((Map236_Bank<<1)+1) % (NesHeader.byRomSize<<1));
    break;
  }
}
//InfoNES_Mapper_236 end











//InfoNES_Mapper_240 begin
/*===================================================================*/
/*                                                                   */
/*                  Mapper 240 : Gen Ke Le Zhuan                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 240                                            */
/*-------------------------------------------------------------------*/
void Map240_Init()
{
  /* Initialize Mapper */
  MapperInit = Map240_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map240_Apu;

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
/*  Mapper 240 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map240_Apu( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr>=0x4020 && wAddr<0x6000 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE((((byData&0xF0)>>2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((byData&0xF0)>>2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((byData&0xF0)>>2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((byData&0xF0)>>2)+3) % (NesHeader.byRomSize<<1));

    /* Set PPU Banks */
    PPUBANK[ 0 ] = VROMPAGE((((byData&0x0F)<<3)+0) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((((byData&0x0F)<<3)+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE((((byData&0x0F)<<3)+2) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((((byData&0x0F)<<3)+3) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE((((byData&0x0F)<<3)+4) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((((byData&0x0F)<<3)+5) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE((((byData&0x0F)<<3)+6) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((((byData&0x0F)<<3)+7) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }
}
//InfoNES_Mapper_240 end









//InfoNES_Mapper_241 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 241 : Fon Serm Bon                       */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 241                                            */
/*-------------------------------------------------------------------*/
void Map241_Init()
{
  /* Initialize Mapper */
  MapperInit = Map241_Init;

  /* Write to Mapper */
  MapperWrite = Map241_Write;

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
/*  Mapper 241 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map241_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr == 0x8000 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((byData<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((byData<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((byData<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((byData<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
//InfoNES_Mapper_241 end











//InfoNES_Mapper_242 begin
/*===================================================================*/
/*                                                                   */
/*                Mapper 242 : Wai Xing Zhan Shi                     */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 242                                            */
/*-------------------------------------------------------------------*/
void Map242_Init()
{
  /* Initialize Mapper */
  MapperInit = Map242_Init;

  /* Write to Mapper */
  MapperWrite = Map242_Write;

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
/*  Mapper 242 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map242_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr & 0x01 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE((((wAddr&0xF8)>>1)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((((wAddr&0xF8)>>1)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((((wAddr&0xF8)>>1)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((((wAddr&0xF8)>>1)+3) % (NesHeader.byRomSize<<1));
  }
}
//InfoNES_Mapper_242 end












//InfoNES_Mapper_243 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 243 (Pirates)                         */
/*                                                                   */
/*===================================================================*/

unsigned char Map243_Regs[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 243                                            */
/*-------------------------------------------------------------------*/
void Map243_Init()
{
  /* Initialize Mapper */
  MapperInit = Map243_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map243_Apu;

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
    {
      PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr();
  }

  /* Initialize state registers */
  Map243_Regs[0] = Map243_Regs[1] = Map243_Regs[2] = Map243_Regs[3] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 243 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map243_Apu( unsigned short int wAddr, unsigned char byData )
{
  if ( wAddr == 0x4100 )
  {
    Map243_Regs[0] = byData;
  }
  else if ( wAddr == 0x4101 )
  {
    switch ( Map243_Regs[0] & 0x07 )
    {
      case 0x02:
        Map243_Regs[1] = byData & 0x01;
        break;

      case 0x00:
      case 0x04:
      case 0x07:
        Map243_Regs[2] = ( byData & 0x01 ) << 1;
        break;

      /* Set ROM Banks */
      case 0x05:
        ROMBANK0 = ROMPAGE( ( byData * 4 + 0 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK1 = ROMPAGE( ( byData * 4 + 1 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK2 = ROMPAGE( ( byData * 4 + 2 ) % ( NesHeader.byRomSize << 1 ) );
        ROMBANK3 = ROMPAGE( ( byData * 4 + 3 ) % ( NesHeader.byRomSize << 1 ) );
        break;

      case 0x06:
        Map243_Regs[3] = ( byData & 0x03 ) << 2;
        break;
    }

    /* Set PPU Banks */
    if ( ( NesHeader.byVRomSize << 3 ) <= 64 )
    {
      unsigned char chr_bank = ( Map243_Regs[2] + Map243_Regs[3] ) >> 1;

      PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
    else
    {
      unsigned char chr_bank = Map243_Regs[1] + Map243_Regs[2] + Map243_Regs[3];

      PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_243 end













//InfoNES_Mapper_244 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 244                               */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 244                                            */
/*-------------------------------------------------------------------*/
void Map244_Init()
{
  /* Initialize Mapper */
  MapperInit = Map244_Init;

  /* Write to Mapper */
  MapperWrite = Map244_Write;

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
/*  Mapper 244 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map244_Write( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr>=0x8065 && wAddr<=0x80A4 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((((wAddr-0x8065)&0x3)<<2)+3) % (NesHeader.byRomSize<<1));
  }
  
  if( wAddr>=0x80A5 && wAddr<=0x80E4 ) {
    /* Set ROM Banks */
    ROMBANK0 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(((((wAddr-0x80A5)&0x7)<<2)+3) % (NesHeader.byRomSize<<1));
  }
}
//InfoNES_Mapper_244 end



//InfoNES_Mapper_245 begin
/*===================================================================*/
/*                                                                   */
/*               Mapper 245 : Yong Zhe Dou E Long                    */
/*                                                                   */
/*===================================================================*/

unsigned char	Map245_Reg[8];
unsigned char	Map245_Prg0, Map245_Prg1;
unsigned char	Map245_Chr01, Map245_Chr23, Map245_Chr4, Map245_Chr5, Map245_Chr6, Map245_Chr7;
unsigned char	Map245_WeSram;

unsigned char	Map245_IRQ_Enable;
unsigned char	Map245_IRQ_Counter;
unsigned char	Map245_IRQ_Latch;
unsigned char	Map245_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 245                                            */
/*-------------------------------------------------------------------*/
void Map245_Init()
{
  /* Initialize Mapper */
  MapperInit = Map245_Init;

  /* Write to Mapper */
  MapperWrite = Map245_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map245_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map245_Reg[i] = 0x00;
  }

  Map245_Prg0 = 0;
  Map245_Prg1 = 1;

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

  Map245_WeSram  = 0;		// Disable
  Map245_IRQ_Enable = 0;	// Disable
  Map245_IRQ_Counter = 0;
  Map245_IRQ_Latch = 0;
  Map245_IRQ_Request = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map245_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr&0xF7FF ) {
  case	0x8000:
    Map245_Reg[0] = byData;
    break;
  case	0x8001:
    Map245_Reg[1] = byData;
    switch( Map245_Reg[0] ) {
    case	0x00:
      Map245_Reg[3]=(byData & 2 )<<5;
      ROMBANK2 = ROMPAGE((0x3E|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x3F|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
      break;
    case	0x06:
      Map245_Prg0=byData;
      break;
    case	0x07:
      Map245_Prg1=byData;
      break;
    }
    ROMBANK0 = ROMPAGE((Map245_Prg0|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((Map245_Prg1|Map245_Reg[3]) % (NesHeader.byRomSize<<1));
    break;
  case	0xA000:
    Map245_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	0xA001:
    
    break;
  case	0xC000:
    Map245_Reg[4] = byData;
    Map245_IRQ_Counter = byData;
    Map245_IRQ_Request = 0;
    break;
  case	0xC001:
    Map245_Reg[5] = byData;
    Map245_IRQ_Latch = byData;
    Map245_IRQ_Request = 0;
    break;
  case	0xE000:
    Map245_Reg[6] = byData;
    Map245_IRQ_Enable = 0;
    Map245_IRQ_Request = 0;
    break;
  case	0xE001:
    Map245_Reg[7] = byData;
    Map245_IRQ_Enable = 1;
    Map245_IRQ_Request = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map245_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map245_IRQ_Enable && !Map245_IRQ_Request ) {
	if( PPU_Scanline == 0 ) {
	  if( Map245_IRQ_Counter ) {
	    Map245_IRQ_Counter--;
	  }
	}
	if( !(Map245_IRQ_Counter--) ) {
	  Map245_IRQ_Request = 0xFF;
	  Map245_IRQ_Counter = Map245_IRQ_Latch;
	}
      }
    }
  }
  if( Map245_IRQ_Request ) {
    IRQ_REQ;
  }
}

#if 0
/*-------------------------------------------------------------------*/
/*  Mapper 245 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void SetBank_CPU()
{
  ROMBANK0 = ROMPAGE( Map245_Prg0 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK1 = ROMPAGE( Map245_Prg1 % ( NesHeader.byRomSize << 1 ) );
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 245 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void SetBank_PPU()
{
  if( NesHeader.byVRomSize > 0 ) {
    if( Map245_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE( Map245_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( Map245_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( Map245_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( Map245_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( ( Map245_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( ( Map245_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( ( Map245_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( ( Map245_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE( ( Map245_Chr01 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 1 ] = VROMPAGE( ( Map245_Chr01 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 2 ] = VROMPAGE( ( Map245_Chr23 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 3 ] = VROMPAGE( ( Map245_Chr23 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 4 ] = VROMPAGE( Map245_Chr4 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 5 ] = VROMPAGE( Map245_Chr5 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 6 ] = VROMPAGE( Map245_Chr6 % ( NesHeader.byVRomSize << 3 ) );
      PPUBANK[ 7 ] = VROMPAGE( Map245_Chr7 % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr();
    }
  } else {
    if( Map245_Reg[0] & 0x80 ) {
      PPUBANK[ 4 ] = CRAMPAGE( (Map245_Chr01+0)&0x07 );
      PPUBANK[ 5 ] = CRAMPAGE( (Map245_Chr01+1)&0x07 );
      PPUBANK[ 6 ] = CRAMPAGE( (Map245_Chr23+0)&0x07 );
      PPUBANK[ 7 ] = CRAMPAGE( (Map245_Chr23+1)&0x07 );
      PPUBANK[ 0 ] = CRAMPAGE( Map245_Chr4&0x07 );
      PPUBANK[ 1 ] = CRAMPAGE( Map245_Chr5&0x07 );
      PPUBANK[ 2 ] = CRAMPAGE( Map245_Chr6&0x07 );
      PPUBANK[ 3 ] = CRAMPAGE( Map245_Chr7&0x07 );
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = CRAMPAGE( (Map245_Chr01+0)&0x07 );
      PPUBANK[ 1 ] = CRAMPAGE( (Map245_Chr01+1)&0x07 );
      PPUBANK[ 2 ] = CRAMPAGE( (Map245_Chr23+0)&0x07 );
      PPUBANK[ 3 ] = CRAMPAGE( (Map245_Chr23+1)&0x07 );
      PPUBANK[ 4 ] = CRAMPAGE( Map245_Chr4&0x07 );
      PPUBANK[ 5 ] = CRAMPAGE( Map245_Chr5&0x07 );
      PPUBANK[ 6 ] = CRAMPAGE( Map245_Chr6&0x07 );
      PPUBANK[ 7 ] = CRAMPAGE( Map245_Chr7&0x07 );
      InfoNES_SetupChr();
    }
  }
}
#endif
//InfoNES_Mapper_245 end











//InfoNES_Mapper_246 begin
/*===================================================================*/
/*                                                                   */
/*                 Mapper 246 : Phone Serm Berm                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 246                                            */
/*-------------------------------------------------------------------*/
void Map246_Init()
{
  /* Initialize Mapper */
  MapperInit = Map246_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map246_Sram;

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
/*  Mapper 246 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map246_Sram( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr>=0x6000 && wAddr<0x8000 ) {
    switch( wAddr ) {
    case 0x6000:
      ROMBANK0 = ROMPAGE(((byData<<2)+0) % (NesHeader.byRomSize<<1));
      break;
    case 0x6001:
      ROMBANK1 = ROMPAGE(((byData<<2)+1) % (NesHeader.byRomSize<<1));
      break;
    case 0x6002:
      ROMBANK2 = ROMPAGE(((byData<<2)+2) % (NesHeader.byRomSize<<1));
      break;
    case 0x6003: 
      ROMBANK3 = ROMPAGE(((byData<<2)+3) % (NesHeader.byRomSize<<1));
      break;
    case 0x6004:
      PPUBANK[ 0 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 1 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6005:
      PPUBANK[ 2 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 3 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6006:
      PPUBANK[ 4 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 5 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    case 0x6007:
      PPUBANK[ 6 ] = VROMPAGE(((byData<<1)+0) % (NesHeader.byVRomSize<<3)); 
      PPUBANK[ 7 ] = VROMPAGE(((byData<<1)+1) % (NesHeader.byVRomSize<<3)); 
      InfoNES_SetupChr();
      break;
    default:
      SRAMBANK[wAddr&0x1FFF] = byData;
      break;
    }
  }
}
//InfoNES_Mapper_246 end













//InfoNES_Mapper_248 begin
/*===================================================================*/
/*                                                                   */
/*                   Mapper 248 : Bao Qing Tian                      */
/*                                                                   */
/*===================================================================*/

unsigned char	Map248_Reg[8];
unsigned char	Map248_Prg0, Map248_Prg1;
unsigned char	Map248_Chr01, Map248_Chr23, Map248_Chr4, Map248_Chr5, Map248_Chr6, Map248_Chr7;
unsigned char	Map248_WeSram;

unsigned char	Map248_IRQ_Enable;
unsigned char	Map248_IRQ_Counter;
unsigned char	Map248_IRQ_Latch;
unsigned char	Map248_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 248                                            */
/*-------------------------------------------------------------------*/
void Map248_Init()
{
  /* Initialize Mapper */
  MapperInit = Map248_Init;

  /* Write to Mapper */
  MapperWrite = Map248_Write;

  /* Write to SRAM */
  MapperSram = Map248_Sram;

  /* Write to APU */
  MapperApu = Map248_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map248_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map248_Reg[i] = 0x00;
  }

  /* Set ROM Banks */
  Map248_Prg0 = 0;
  Map248_Prg1 = 1;
  Map248_Set_CPU_Banks();

  /* Set PPU Banks */
  Map248_Chr01 = 0;
  Map248_Chr23 = 2;
  Map248_Chr4  = 4;
  Map248_Chr5  = 5;
  Map248_Chr6  = 6;
  Map248_Chr7  = 7;
  Map248_Set_PPU_Banks();
  
  Map248_WeSram  = 0;		// Disable
  Map248_IRQ_Enable = 0;	// Disable
  Map248_IRQ_Counter = 0;
  Map248_IRQ_Latch = 0;
  Map248_IRQ_Request = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map248_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map248_Reg[0] = byData;
    Map248_Set_CPU_Banks();
    Map248_Set_PPU_Banks();
    break;
  case	0x8001:
    Map248_Reg[1] = byData;
    
    switch( Map248_Reg[0] & 0x07 ) {
    case	0x00:
      Map248_Chr01 = byData & 0xFE;
      Map248_Set_PPU_Banks();
      break;
    case	0x01:
      Map248_Chr23 = byData & 0xFE;
      Map248_Set_PPU_Banks();
      break;
    case	0x02:
      Map248_Chr4 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x03:
      Map248_Chr5 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x04:
      Map248_Chr6 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x05:
      Map248_Chr7 = byData;
      Map248_Set_PPU_Banks();
      break;
    case	0x06:
      Map248_Prg0 = byData;
      Map248_Set_CPU_Banks();
      break;
    case	0x07:
      Map248_Prg1 = byData;
      Map248_Set_CPU_Banks();
      break;
    }
    break;
  case	0xA000:
    Map248_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) {
	InfoNES_Mirroring( 0 );
      } else {
	InfoNES_Mirroring( 1 );
      }
    }
    break;
  case 0xC000:
    Map248_IRQ_Enable=0;
    Map248_IRQ_Latch=0xBE;
    Map248_IRQ_Counter =0xBE;
    break;
  case 0xC001:
    Map248_IRQ_Enable=1;
    Map248_IRQ_Latch=0xBE;
    Map248_IRQ_Counter=0xBE;
    break;
  }	
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map248_Sram( unsigned short int wAddr, unsigned char byData )
{
  ROMBANK0 = ROMPAGE(((byData<<1)+0) % (NesHeader.byRomSize<<1));
  ROMBANK1 = ROMPAGE(((byData<<1)+1) % (NesHeader.byRomSize<<1));
  ROMBANK2 = ROMLASTPAGE( 1 );
  ROMBANK3 = ROMLASTPAGE( 0 );
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map248_Apu( unsigned short int wAddr, unsigned char byData )
{
  Map248_Sram( wAddr, byData );
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map248_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map248_IRQ_Enable ) {
	if( !(Map248_IRQ_Counter--) ) {
	  Map248_IRQ_Counter = Map248_IRQ_Latch;
	  IRQ_REQ;
	}
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map248_Set_CPU_Banks()
{
  if( Map248_Reg[0] & 0x40 ) {
    ROMBANK0 = ROMLASTPAGE( 1 );
    ROMBANK1 = ROMPAGE(Map248_Prg1 % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(Map248_Prg0 % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    ROMBANK0 = ROMPAGE(Map248_Prg0 % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(Map248_Prg1 % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMLASTPAGE( 1 );
    ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 248 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map248_Set_PPU_Banks()
{
  if( NesHeader.byRomSize > 0 ) {
    if( Map248_Reg[0] & 0x80 ) {
      PPUBANK[ 0 ] = VROMPAGE(Map248_Chr4 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE(Map248_Chr5 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE(Map248_Chr6 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE(Map248_Chr7 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE((Map248_Chr01+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE((Map248_Chr01+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE((Map248_Chr23+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE((Map248_Chr23+1) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    } else {
      PPUBANK[ 0 ] = VROMPAGE((Map248_Chr01+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE((Map248_Chr01+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 2 ] = VROMPAGE((Map248_Chr23+0) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE((Map248_Chr23+1) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 4 ] = VROMPAGE(Map248_Chr4 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 5 ] = VROMPAGE(Map248_Chr5 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 6 ] = VROMPAGE(Map248_Chr6 % (NesHeader.byVRomSize<<3));
      PPUBANK[ 7 ] = VROMPAGE(Map248_Chr7 % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
    }
  }
}
//InfoNES_Mapper_248 end



//InfoNES_Mapper_249 begin
/*===================================================================*/
/*                                                                   */
/*                      Mapper 249 : MMC3                            */
/*                                                                   */
/*===================================================================*/

unsigned char	Map249_Spdata;
unsigned char	Map249_Reg[8];

unsigned char	Map249_IRQ_Enable;
unsigned char	Map249_IRQ_Counter;
unsigned char	Map249_IRQ_Latch;
unsigned char	Map249_IRQ_Request;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 249                                            */
/*-------------------------------------------------------------------*/
void Map249_Init()
{
  /* Initialize Mapper */
  MapperInit = Map249_Init;

  /* Write to Mapper */
  MapperWrite = Map249_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map249_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map249_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 8; i++ ) {
    Map249_Reg[i] = 0x00;
  }

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

  /* Set Registers */
  Map249_IRQ_Enable = 0;	// Disable
  Map249_IRQ_Counter = 0;
  Map249_IRQ_Latch = 0;
  Map249_IRQ_Request = 0;

  Map249_Spdata = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map249_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byM0,byM1,byM2,byM3,byM4,byM5,byM6,byM7;

  switch( wAddr&0xFF01 ) {
  case	  0x8000:
  case    0x8800:
    Map249_Reg[0] = byData;
    break;
  case	  0x8001:
  case    0x8801:
    switch( Map249_Reg[0] & 0x07 ) {
    case	0x00:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 0 ] = VROMPAGE((byData&0xFE) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 1 ] = VROMPAGE((byData|0x01) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x01:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 2 ] = VROMPAGE((byData&0xFE) % (NesHeader.byVRomSize<<3));
      PPUBANK[ 3 ] = VROMPAGE((byData|0x01) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x02:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 4 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x03:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 5 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x04:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 6 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x05:
      if( Map249_Spdata == 1 ) {
	byM0=byData&0x1;
	byM1=(byData&0x02)>>1;
	byM2=(byData&0x04)>>2;
	byM3=(byData&0x08)>>3;
	byM4=(byData&0x10)>>4;
	byM5=(byData&0x20)>>5;
	byM6=(byData&0x40)>>6;
	byM7=(byData&0x80)>>7;
	byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
      }
      PPUBANK[ 7 ] = VROMPAGE(byData % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr();
      break;
    case	0x06:
      if( Map249_Spdata == 1 ) {
	if( byData < 0x20 ) {
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=0;
	  byM6=0;
	  byM7=0;
	  byData=(byM7<<7)|(byM6<<6)|(byM5<<5)|(byM2<<4)|(byM1<<3)|(byM3<<2)|(byM4<<1)|byM0;
	} else {
	  byData=byData-0x20;
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=(byData&0x20)>>5;
	  byM6=(byData&0x40)>>6;
	  byM7=(byData&0x80)>>7;
	  byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
	}
      }
      ROMBANK0 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
      break;
    case	0x07:
      if( Map249_Spdata == 1 ) {
	if( byData < 0x20 ) {
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=0;
	  byM6=0;
	  byM7=0;
	  byData=(byM7<<7)|(byM6<<6)|(byM5<<5)|(byM2<<4)|(byM1<<3)|(byM3<<2)|(byM4<<1)|byM0;
	} else {
	  byData=byData-0x20;
	  byM0=byData&0x1;
	  byM1=(byData&0x02)>>1;
	  byM2=(byData&0x04)>>2;
	  byM3=(byData&0x08)>>3;
	  byM4=(byData&0x10)>>4;
	  byM5=(byData&0x20)>>5;
	  byM6=(byData&0x40)>>6;
	  byM7=(byData&0x80)>>7;
	  byData=(byM5<<7)|(byM4<<6)|(byM2<<5)|(byM6<<4)|(byM7<<3)|(byM3<<2)|(byM1<<1)|byM0;
	}
      }
      ROMBANK1 = ROMPAGE(byData % (NesHeader.byRomSize<<1));
      break;
    }
    break;
  case	  0xA000:
  case    0xA800:
    Map249_Reg[2] = byData;
    if( !ROM_FourScr ) {
      if( byData & 0x01 ) InfoNES_Mirroring( 0 );
      else		  InfoNES_Mirroring( 1 );
    }
    break;
  case	  0xA001:
  case    0xA801:
    Map249_Reg[3] = byData;
    break;
  case	  0xC000:
  case    0xC800:
    Map249_Reg[4] = byData;
    Map249_IRQ_Counter = byData;
    Map249_IRQ_Request = 0;
    break;
  case	  0xC001:
  case    0xC801:
    Map249_Reg[5] = byData;
    Map249_IRQ_Latch = byData;
    Map249_IRQ_Request = 0;
    break;
  case	  0xE000:
  case    0xE800:
    Map249_Reg[6] = byData;
    Map249_IRQ_Enable = 0;
    Map249_IRQ_Request = 0;
    break;
  case	  0xE001:
  case    0xE801:
    Map249_Reg[7] = byData;
    Map249_IRQ_Enable = 1;
    Map249_IRQ_Request = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map249_Apu( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr == 0x5000 ) {
    switch( byData ) {
    case	0x00:
      Map249_Spdata = 0;
      break;
    case	0x02:
      Map249_Spdata = 1;
      break;
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 249 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map249_HSync()
{
  if( ( /* PPU_Scanline >= 0 && */ PPU_Scanline <= 239) ) {
    if( PPU_R1 & R1_SHOW_SCR || PPU_R1 & R1_SHOW_SP ) {
      if( Map249_IRQ_Enable && !Map249_IRQ_Request ) {
	if( PPU_Scanline == 0 ) {
	  if( Map249_IRQ_Counter ) {
	    Map249_IRQ_Counter--;
	  }
	}
	if( !(Map249_IRQ_Counter--) ) {
	  Map249_IRQ_Request = 0xFF;
	  Map249_IRQ_Counter = Map249_IRQ_Latch;
	}
      }
    }
  }
  if( Map249_IRQ_Request ) {
    IRQ_REQ;
  }
}
//InfoNES_Mapper_249 end



//InfoNES_Mapper_251 begin
/*===================================================================*/
/*                                                                   */
/*                          Mapper 251                               */
/*                                                                   */
/*===================================================================*/

unsigned char	Map251_Reg[11];
unsigned char	Map251_Breg[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 251                                            */
/*-------------------------------------------------------------------*/
void Map251_Init()
{
  /* Initialize Mapper */
  MapperInit = Map251_Init;

  /* Write to Mapper */
  MapperWrite = Map251_Write;

  /* Write to SRAM */
  MapperSram = Map251_Sram;

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

  /* Set Registers */
  InfoNES_Mirroring( 1 );

  int	i;
  for( i = 0; i < 11; i++ )
    Map251_Reg[i] = 0;
  for( i = 0; i < 4; i++ )
    Map251_Breg[i] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map251_Write( unsigned short int wAddr, unsigned char byData )
{
  switch( wAddr & 0xE001 ) {
  case	0x8000:
    Map251_Reg[8] = byData;
    Map251_Set_Banks();
    break;
  case	0x8001:
    Map251_Reg[Map251_Reg[8]&0x07] = byData;
    Map251_Set_Banks();
    break;
  case	0xA001:
    if( byData & 0x80 ) {
      Map251_Reg[ 9] = 1;
      Map251_Reg[10] = 0;
    } else {
      Map251_Reg[ 9] = 0;
    }
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Write to SRAM Function                                */
/*-------------------------------------------------------------------*/
void Map251_Sram( unsigned short int wAddr, unsigned char byData )
{
  if( (wAddr & 0xE001) == 0x6000 ) {
    if( Map251_Reg[9] ) {
      Map251_Breg[Map251_Reg[10]++] = byData;
      if( Map251_Reg[10] == 4 ) {
	Map251_Reg[10] = 0;
	Map251_Set_Banks();
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 251 Set Banks Function                                    */
/*-------------------------------------------------------------------*/
void Map251_Set_Banks()
{
  int	nChr[6];
  int	nPrg[4];

  for( int i = 0; i < 6; i++ ) {
    nChr[i] = (Map251_Reg[i]|(Map251_Breg[1]<<4)) & ((Map251_Breg[2]<<4)|0x0F);
  }

  if( Map251_Reg[8] & 0x80 ) {
    PPUBANK[ 0 ] = VROMPAGE(nChr[2] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE(nChr[3] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(nChr[4] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE(nChr[5] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(nChr[0] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE((nChr[0]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(nChr[1] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE((nChr[1]+1) % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  } else {
    PPUBANK[ 0 ] = VROMPAGE(nChr[0] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 1 ] = VROMPAGE((nChr[0]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 2 ] = VROMPAGE(nChr[1] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 3 ] = VROMPAGE((nChr[1]+1) % (NesHeader.byVRomSize<<3));
    PPUBANK[ 4 ] = VROMPAGE(nChr[2] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 5 ] = VROMPAGE(nChr[3] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 6 ] = VROMPAGE(nChr[4] % (NesHeader.byVRomSize<<3));
    PPUBANK[ 7 ] = VROMPAGE(nChr[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
  }

  nPrg[0] = (Map251_Reg[6]&((Map251_Breg[3]&0x3F)^0x3F))|(Map251_Breg[1]);
  nPrg[1] = (Map251_Reg[7]&((Map251_Breg[3]&0x3F)^0x3F))|(Map251_Breg[1]);
  nPrg[2] = nPrg[3] =((Map251_Breg[3]&0x3F)^0x3F)|(Map251_Breg[1]);
  nPrg[2] &= (NesHeader.byRomSize<<1)-1;

  if( Map251_Reg[8] & 0x40 ) {
    ROMBANK0 = ROMPAGE(nPrg[2] % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(nPrg[1] % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(nPrg[0] % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(nPrg[3] % (NesHeader.byRomSize<<1));
  } else { 
    ROMBANK0 = ROMPAGE(nPrg[0] % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE(nPrg[1] % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE(nPrg[2] % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE(nPrg[3] % (NesHeader.byRomSize<<1));
  }
}
//InfoNES_Mapper_251 end











//InfoNES_Mapper_252 begin
/*===================================================================*/
/*                                                                   */
/*              Mapper 252 : Sangokushi - Chuugen no hasha           */
/*                                                                   */
/*===================================================================*/

unsigned char	Map252_Reg[9];
unsigned char	Map252_IRQ_Enable;
unsigned char	Map252_IRQ_Counter;
unsigned char	Map252_IRQ_Latch;
unsigned char	Map252_IRQ_Occur;
int	Map252_IRQ_Clock;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 252                                            */
/*-------------------------------------------------------------------*/
void Map252_Init()
{
  /* Initialize Mapper */
  MapperInit = Map252_Init;

  /* Write to Mapper */
  MapperWrite = Map252_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map252_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  SRAMBANK = SRAM;

  /* Set Registers */
  for( int i = 0; i < 9; i++ ) {
    Map252_Reg[i] = i;
  }

  Map252_IRQ_Enable = 0;
  Map252_IRQ_Counter = 0;
  Map252_IRQ_Latch = 0;
  Map252_IRQ_Clock = 0;
  Map252_IRQ_Occur = 0;

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
/*  Mapper 252 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map252_Write( unsigned short int wAddr, unsigned char byData )
{
  if( (wAddr & 0xF000) == 0x8000 ) {
    ROMBANK0 = ROMPAGE( byData % (NesHeader.byRomSize<<1));
    return;
  }
  if( (wAddr & 0xF000) == 0xA000 ) {
    ROMBANK1 = ROMPAGE( byData % (NesHeader.byRomSize<<1));
    return;
  }
  switch( wAddr & 0xF00C ) {
  case 0xB000:
    Map252_Reg[0] = (Map252_Reg[0] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 0 ] = VROMPAGE(Map252_Reg[0] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;	
  case 0xB004:
    Map252_Reg[0] = (Map252_Reg[0] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 0 ] = VROMPAGE(Map252_Reg[0] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xB008:
    Map252_Reg[1] = (Map252_Reg[1] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 1 ] = VROMPAGE(Map252_Reg[1] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xB00C:
    Map252_Reg[1] = (Map252_Reg[1] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 1 ] = VROMPAGE(Map252_Reg[1] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xC000:
    Map252_Reg[2] = (Map252_Reg[2] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 2 ] = VROMPAGE(Map252_Reg[2] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC004:
    Map252_Reg[2] = (Map252_Reg[2] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 2 ] = VROMPAGE(Map252_Reg[2] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC008:
    Map252_Reg[3] = (Map252_Reg[3] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 3 ] = VROMPAGE(Map252_Reg[3] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xC00C:
    Map252_Reg[3] = (Map252_Reg[3] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 3 ] = VROMPAGE(Map252_Reg[3] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xD000:
    Map252_Reg[4] = (Map252_Reg[4] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 4 ] = VROMPAGE(Map252_Reg[4] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD004:
    Map252_Reg[4] = (Map252_Reg[4] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 4 ] = VROMPAGE(Map252_Reg[4] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD008:
    Map252_Reg[5] = (Map252_Reg[5] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 5 ] = VROMPAGE(Map252_Reg[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xD00C:
    Map252_Reg[5] = (Map252_Reg[5] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 5 ] = VROMPAGE(Map252_Reg[5] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xE000:
    Map252_Reg[6] = (Map252_Reg[6] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 6 ] = VROMPAGE(Map252_Reg[6] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE004:
    Map252_Reg[6] = (Map252_Reg[6] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 6 ] = VROMPAGE(Map252_Reg[6] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE008:
    Map252_Reg[7] = (Map252_Reg[7] & 0xF0) | (byData & 0x0F);
    PPUBANK[ 7 ] = VROMPAGE(Map252_Reg[7] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
  case 0xE00C:
    Map252_Reg[7] = (Map252_Reg[7] & 0x0F) | ((byData & 0x0F) << 4);
    PPUBANK[ 7 ] = VROMPAGE(Map252_Reg[7] % (NesHeader.byVRomSize<<3));
    InfoNES_SetupChr();
    break;
    
  case 0xF000:
    Map252_IRQ_Latch = (Map252_IRQ_Latch & 0xF0) | (byData & 0x0F);
    Map252_IRQ_Occur = 0;
    break;
  case 0xF004:
    Map252_IRQ_Latch = (Map252_IRQ_Latch & 0x0F) | ((byData & 0x0F) << 4);
    Map252_IRQ_Occur = 0;
    break;
    
  case 0xF008:
    Map252_IRQ_Enable = byData & 0x03;
    if( Map252_IRQ_Enable & 0x02 ) {
      Map252_IRQ_Counter = Map252_IRQ_Latch;
      Map252_IRQ_Clock = 0;
    }
    Map252_IRQ_Occur = 0;
    break;
    
  case 0xF00C:
    Map252_IRQ_Enable = (Map252_IRQ_Enable & 0x01) * 3;
    Map252_IRQ_Occur = 0;
    break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 252 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map252_HSync()
{
  if( Map252_IRQ_Enable & 0x02 ) {
    if( Map252_IRQ_Counter == 0xFF ) {
      Map252_IRQ_Occur = 0xFF;
      Map252_IRQ_Counter = Map252_IRQ_Latch;
      Map252_IRQ_Enable = (Map252_IRQ_Enable & 0x01) * 3;
    } else {
      Map252_IRQ_Counter++;
    }
  }
  if( Map252_IRQ_Occur ) {
    IRQ_REQ;
  }
}
//InfoNES_Mapper_252 end















//InfoNES_Mapper_255 begin
/*===================================================================*/
/*                                                                   */
/*                     Mapper 255 : 110-in-1                         */
/*                                                                   */
/*===================================================================*/

unsigned char    Map255_Reg[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 255                                            */
/*-------------------------------------------------------------------*/
void Map255_Init()
{
  /* Initialize Mapper */
  MapperInit = Map255_Init;

  /* Write to Mapper */
  MapperWrite = Map255_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map255_Apu;

  /* Read from APU */
  MapperReadApu = Map255_ReadApu;

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

  /* Set Registers */
  InfoNES_Mirroring( 1 );

  Map255_Reg[0] = 0;
  Map255_Reg[1] = 0;
  Map255_Reg[2] = 0;
  Map255_Reg[3] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map255_Write( unsigned short int wAddr, unsigned char byData )
{
  unsigned char	byPrg = (wAddr & 0x0F80)>>7;
  int	nChr = (wAddr & 0x003F);
  int	nBank = (wAddr & 0x4000)>>14;

  if( wAddr & 0x2000 ) {
    InfoNES_Mirroring( 0 );
  } else {
    InfoNES_Mirroring( 1 );
  }

  if( wAddr & 0x1000 ) {
    if( wAddr & 0x0040 ) {
      ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
    } else {
      ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
      ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
      ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
      ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
    }
  } else {
    ROMBANK0 = ROMPAGE((0x80*nBank+byPrg*4+0) % (NesHeader.byRomSize<<1));
    ROMBANK1 = ROMPAGE((0x80*nBank+byPrg*4+1) % (NesHeader.byRomSize<<1));
    ROMBANK2 = ROMPAGE((0x80*nBank+byPrg*4+2) % (NesHeader.byRomSize<<1));
    ROMBANK3 = ROMPAGE((0x80*nBank+byPrg*4+3) % (NesHeader.byRomSize<<1));
  }
  
  PPUBANK[ 0 ] = VROMPAGE((0x200*nBank+nChr*8+0) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 1 ] = VROMPAGE((0x200*nBank+nChr*8+1) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 2 ] = VROMPAGE((0x200*nBank+nChr*8+2) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 3 ] = VROMPAGE((0x200*nBank+nChr*8+3) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 4 ] = VROMPAGE((0x200*nBank+nChr*8+4) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 5 ] = VROMPAGE((0x200*nBank+nChr*8+5) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 6 ] = VROMPAGE((0x200*nBank+nChr*8+6) % (NesHeader.byVRomSize<<3));
  PPUBANK[ 7 ] = VROMPAGE((0x200*nBank+nChr*8+7) % (NesHeader.byVRomSize<<3));
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map255_Apu( unsigned short int wAddr, unsigned char byData )
{
  if( wAddr >= 0x5800 ) {
    Map255_Reg[wAddr&0x0003] = byData & 0x0F;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 255 Read from APU Function                                */
/*-------------------------------------------------------------------*/
unsigned char Map255_ReadApu( unsigned short int wAddr )
{
  if( wAddr >= 0x5800 ) {
    return	Map255_Reg[wAddr&0x0003] & 0x0F;
  } else {
    return	wAddr>>8;
  }
}
//InfoNES_Mapper_255 end
