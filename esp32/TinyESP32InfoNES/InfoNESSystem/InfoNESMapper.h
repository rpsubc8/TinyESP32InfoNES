/*===================================================================*/
/*                                                                   */
/*  InfoNES_Mapper.h : InfoNES Mapper Function                       */
/*                                                                   */
/*  2000/05/16  InfoNES Project ( based on NesterJ and pNesX )       */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_MAPPER_H_INCLUDED
#define InfoNES_MAPPER_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include "gbConfig.h"
#include "gbGlobals.h"
//JJ #include "InfoNESTypes.h"

/*-------------------------------------------------------------------*/
/*  Constants                                                        */
/*-------------------------------------------------------------------*/

#define DRAM_SIZE    0xA000

/*-------------------------------------------------------------------*/
/*  Mapper resources                                                 */
/*-------------------------------------------------------------------*/

/* Disk System RAM */
//JJ extern BYTE DRAM[];

/*-------------------------------------------------------------------*/
/*  Macros                                                           */
/*-------------------------------------------------------------------*/

/* The address of 8Kbytes unit of the ROM */
#define ROMPAGE(a)     &ROM[ (a) * 0x2000 ]
/* From behind the ROM, the address of 8kbytes unit */
#define ROMLASTPAGE(a) &ROM[ NesHeader.byRomSize * 0x4000 - ( (a) + 1 ) * 0x2000 ]
/* The address of 1Kbytes unit of the VROM */
#define VROMPAGE(a)    &VROM[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the CRAM */
#define CRAMPAGE(a)   &PPURAM[ 0x0000 + ((a)&0x1F) * 0x400 ]
/* The address of 1Kbytes unit of the VRAM */
#define VRAMPAGE(a)    &PPURAM[ 0x2000 + (a) * 0x400 ]
/* Translate the pointer to ChrBuf into the address of Pattern Table */ 
#define PATTBL(a)      ( ( (a) - ChrBuf ) >> 2 )

/*-------------------------------------------------------------------*/
/*  Macros ( Mapper specific )                                       */
/*-------------------------------------------------------------------*/

/* The address of 8Kbytes unit of the Map5 ROM */
#define Map5_ROMPAGE(a)     &Map5_Wram[ ( (a) & 0x07 ) * 0x2000 ]
/* The address of 1Kbytes unit of the Map6 Chr RAM */
#define Map6_VROMPAGE(a)    &Map6_Chr_Ram[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the Map19 Chr RAM */
#define Map19_VROMPAGE(a)   &Map19_Chr_Ram[ (a) * 0x400 ]
/* The address of 1Kbytes unit of the Map85 Chr RAM */
#define Map85_VROMPAGE(a)   &Map85_Chr_Ram[ (a) * 0x400 ]

/*-------------------------------------------------------------------*/
/*  Table of Mapper initialize function                              */
/*-------------------------------------------------------------------*/

struct MapperTable_tag
{
  int nMapperNo;
  void (*pMapperInit)();
};

//JJ extern struct MapperTable_tag MapperTable[];

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

void Map0_Init();
void Map0_Write( unsigned short int wAddr, unsigned char byData );
void Map0_Sram( unsigned short int wAddr, unsigned char byData );
void Map0_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map0_ReadApu( unsigned short int wAddr );
void Map0_VSync();
void Map0_HSync();
void Map0_PPU( unsigned short int wAddr );
void Map0_RenderScreen( unsigned char byMode );

void Map1_Init();
void Map1_Write( unsigned short int wAddr, unsigned char byData );
void Map1_set_ROM_banks();

void Map2_Init();
void Map2_Write( unsigned short int wAddr, unsigned char byData );

void Map3_Init();
void Map3_Write( unsigned short int wAddr, unsigned char byData );

void Map4_Init();
void Map4_Write( unsigned short int wAddr, unsigned char byData );
void Map4_HSync();
void Map4_Set_CPU_Banks();
void Map4_Set_PPU_Banks();

#ifndef use_lib_not_use_mapper5
 void Map5_Init();
 void Map5_Write( unsigned short int wAddr, unsigned char byData );
 void Map5_Apu( unsigned short int wAddr, unsigned char byData );
 unsigned char Map5_ReadApu( unsigned short int wAddr );
 void Map5_HSync();
 void Map5_RenderScreen( unsigned char byMode );
 void Map5_Sync_Prg_Banks( void );
#endif

#ifndef use_lib_not_use_mapper6
 void Map6_Init();
 void Map6_Write( unsigned short int wAddr, unsigned char byData );
 void Map6_Apu( unsigned short int wAddr, unsigned char byData );
 void Map6_HSync();
#endif

void Map7_Init();
void Map7_Write( unsigned short int wAddr, unsigned char byData );

void Map8_Init();
void Map8_Write( unsigned short int wAddr, unsigned char byData );

void Map9_Init();
void Map9_Write( unsigned short int wAddr, unsigned char byData );
void Map9_PPU( unsigned short int wAddr );

void Map10_Init();
void Map10_Write( unsigned short int wAddr, unsigned char byData );
void Map10_PPU( unsigned short int wAddr );

void Map11_Init();
void Map11_Write( unsigned short int wAddr, unsigned char byData );

void Map13_Init();
void Map13_Write( unsigned short int wAddr, unsigned char byData );

void Map15_Init();
void Map15_Write( unsigned short int wAddr, unsigned char byData );

void Map16_Init();
void Map16_Write( unsigned short int wAddr, unsigned char byData );
void Map16_HSync();

void Map17_Init();
void Map17_Apu( unsigned short int wAddr, unsigned char byData );
void Map17_HSync();

void Map18_Init();
void Map18_Write( unsigned short int wAddr, unsigned char byData );
void Map18_HSync();

#ifndef use_lib_not_use_mapper19
 void Map19_Init();
 void Map19_Write( unsigned short int wAddr, unsigned char byData );
 void Map19_Apu( unsigned short int wAddr, unsigned char byData );
 unsigned char Map19_ReadApu( unsigned short int wAddr );
 void Map19_HSync();
#endif 

void Map21_Init();
void Map21_Write( unsigned short int wAddr, unsigned char byData );
void Map21_HSync();

void Map22_Init();
void Map22_Write( unsigned short int wAddr, unsigned char byData );

void Map23_Init();
void Map23_Write( unsigned short int wAddr, unsigned char byData );
void Map23_HSync();

void Map24_Init();
void Map24_Write( unsigned short int wAddr, unsigned char byData );
void Map24_HSync();

void Map25_Init();
void Map25_Write( unsigned short int wAddr, unsigned char byData );
void Map25_Sync_Vrom( int nBank );
void Map25_HSync();

void Map26_Init();
void Map26_Write( unsigned short int wAddr, unsigned char byData );
void Map26_HSync();

void Map32_Init();
void Map32_Write( unsigned short int wAddr, unsigned char byData );

void Map33_Init();
void Map33_Write( unsigned short int wAddr, unsigned char byData );
void Map33_HSync();

void Map34_Init();
void Map34_Write( unsigned short int wAddr, unsigned char byData );
void Map34_Sram( unsigned short int wAddr, unsigned char byData );

void Map40_Init();
void Map40_Write( unsigned short int wAddr, unsigned char byData );
void Map40_HSync();

void Map41_Init();
void Map41_Write( unsigned short int wAddr, unsigned char byData );
void Map41_Sram( unsigned short int wAddr, unsigned char byData );

void Map42_Init();
void Map42_Write( unsigned short int wAddr, unsigned char byData );
void Map42_HSync();

void Map43_Init();
void Map43_Write( unsigned short int wAddr, unsigned char byData );
void Map43_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map43_ReadApu( unsigned short int wAddr );
void Map43_HSync();

void Map44_Init();
void Map44_Write( unsigned short int wAddr, unsigned char byData );
void Map44_HSync();
void Map44_Set_CPU_Banks();
void Map44_Set_PPU_Banks();

#ifndef use_lib_not_use_mapper45
 void Map45_Init();
 void Map45_Sram( unsigned short int wAddr, unsigned char byData );
 void Map45_Write( unsigned short int wAddr, unsigned char byData );
 void Map45_HSync();
 void Map45_Set_CPU_Bank4( unsigned char byData );
 void Map45_Set_CPU_Bank5( unsigned char byData );
 void Map45_Set_CPU_Bank6( unsigned char byData );
 void Map45_Set_CPU_Bank7( unsigned char byData );
 void Map45_Set_PPU_Banks();
#endif

void Map46_Init();
void Map46_Sram( unsigned short int wAddr, unsigned char byData );
void Map46_Write( unsigned short int wAddr, unsigned char byData );
void Map46_Set_ROM_Banks();

void Map47_Init();
void Map47_Sram( unsigned short int wAddr, unsigned char byData );
void Map47_Write( unsigned short int wAddr, unsigned char byData );
void Map47_HSync();
void Map47_Set_CPU_Banks();
void Map47_Set_PPU_Banks();

void Map48_Init();
void Map48_Write( unsigned short int wAddr, unsigned char byData );
void Map48_HSync();

void Map49_Init();
void Map49_Sram( unsigned short int wAddr, unsigned char byData );
void Map49_Write( unsigned short int wAddr, unsigned char byData );
void Map49_HSync();
void Map49_Set_CPU_Banks();
void Map49_Set_PPU_Banks();

void Map50_Init();
void Map50_Apu( unsigned short int wAddr, unsigned char byData );
void Map50_HSync();

void Map51_Init();
void Map51_Sram( unsigned short int wAddr, unsigned char byData );
void Map51_Write( unsigned short int wAddr, unsigned char byData );
void Map51_Set_CPU_Banks();

void Map57_Init();
void Map57_Write( unsigned short int wAddr, unsigned char byData );

void Map58_Init();
void Map58_Write( unsigned short int wAddr, unsigned char byData );

void Map60_Init();
void Map60_Write( unsigned short int wAddr, unsigned char byData );

void Map61_Init();
void Map61_Write( unsigned short int wAddr, unsigned char byData );

void Map62_Init();
void Map62_Write( unsigned short int wAddr, unsigned char byData );

void Map64_Init();
void Map64_Write( unsigned short int wAddr, unsigned char byData );

void Map65_Init();
void Map65_Write( unsigned short int wAddr, unsigned char byData );
void Map65_HSync();

void Map66_Init();
void Map66_Write( unsigned short int wAddr, unsigned char byData );

void Map67_Init();
void Map67_Write( unsigned short int wAddr, unsigned char byData );
void Map67_HSync();

void Map68_Init();
void Map68_Write( unsigned short int wAddr, unsigned char byData );
void Map68_SyncMirror();

void Map69_Init();
void Map69_Write( unsigned short int wAddr, unsigned char byData );
void Map69_HSync();

void Map70_Init();
void Map70_Write( unsigned short int wAddr, unsigned char byData );

void Map71_Init();
void Map71_Write( unsigned short int wAddr, unsigned char byData );

void Map72_Init();
void Map72_Write( unsigned short int wAddr, unsigned char byData );

void Map73_Init();
void Map73_Write( unsigned short int wAddr, unsigned char byData );
void Map73_HSync();

void Map74_Init();
void Map74_Write( unsigned short int wAddr, unsigned char byData );
void Map74_HSync();
void Map74_Set_CPU_Banks();
void Map74_Set_PPU_Banks();

void Map75_Init();
void Map75_Write( unsigned short int wAddr, unsigned char byData );

void Map76_Init();
void Map76_Write( unsigned short int wAddr, unsigned char byData );

void Map77_Init();
void Map77_Write( unsigned short int wAddr, unsigned char byData );

void Map78_Init();
void Map78_Write( unsigned short int wAddr, unsigned char byData );

void Map79_Init();
void Map79_Apu( unsigned short int wAddr, unsigned char byData );

void Map80_Init();
void Map80_Sram( unsigned short int wAddr, unsigned char byData );

void Map82_Init();
void Map82_Sram( unsigned short int wAddr, unsigned char byData );

void Map83_Init();
void Map83_Write( unsigned short int wAddr, unsigned char byData );
void Map83_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map83_ReadApu( unsigned short int wAddr );
void Map83_HSync();

#ifndef use_lib_not_use_mapper85
 void Map85_Init();
 void Map85_Write( unsigned short int wAddr, unsigned char byData );
 void Map85_HSync();
#endif 

void Map86_Init();
void Map86_Sram( unsigned short int wAddr, unsigned char byData );

void Map87_Init();
void Map87_Sram( unsigned short int wAddr, unsigned char byData );

void Map88_Init();
void Map88_Write( unsigned short int wAddr, unsigned char byData );

void Map89_Init();
void Map89_Write( unsigned short int wAddr, unsigned char byData );

void Map90_Init();
void Map90_Write( unsigned short int wAddr, unsigned char byData );
void Map90_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map90_ReadApu( unsigned short int wAddr );
void Map90_HSync();
void Map90_Sync_Mirror( void );
void Map90_Sync_Prg_Banks( void );
void Map90_Sync_Chr_Banks( void );

void Map91_Init();
void Map91_Sram( unsigned short int wAddr, unsigned char byData );

void Map92_Init();
void Map92_Write( unsigned short int wAddr, unsigned char byData );

void Map93_Init();
void Map93_Sram( unsigned short int wAddr, unsigned char byData );

void Map94_Init();
void Map94_Write( unsigned short int wAddr, unsigned char byData );

void Map95_Init();
void Map95_Write( unsigned short int wAddr, unsigned char byData );
void Map95_Set_CPU_Banks();
void Map95_Set_PPU_Banks();

void Map96_Init();
void Map96_Write( unsigned short int wAddr, unsigned char byData );
void Map96_PPU( unsigned short int wAddr );
void Map96_Set_Banks();

void Map97_Init();
void Map97_Write( unsigned short int wAddr, unsigned char byData );

void Map99_Init();
void Map99_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map99_ReadApu( unsigned short int wAddr );

void Map100_Init();
void Map100_Write( unsigned short int wAddr, unsigned char byData );
void Map100_HSync();
void Map100_Set_CPU_Banks();
void Map100_Set_PPU_Banks();

void Map101_Init();
void Map101_Write( unsigned short int wAddr, unsigned char byData );

void Map105_Init();
void Map105_Write( unsigned short int wAddr, unsigned char byData );
void Map105_HSync();

void Map107_Init();
void Map107_Write( unsigned short int wAddr, unsigned char byData );

void Map108_Init();
void Map108_Write( unsigned short int wAddr, unsigned char byData );

void Map109_Init();
void Map109_Apu( unsigned short int wAddr, unsigned char byData );
void Map109_Set_PPU_Banks();

void Map110_Init();
void Map110_Apu( unsigned short int wAddr, unsigned char byData );

void Map112_Init();
void Map112_Write( unsigned short int wAddr, unsigned char byData );
void Map112_HSync();
void Map112_Set_CPU_Banks();
void Map112_Set_PPU_Banks();

void Map113_Init();
void Map113_Apu( unsigned short int wAddr, unsigned char byData );
void Map113_Write( unsigned short int wAddr, unsigned char byData );

void Map114_Init();
void Map114_Sram( unsigned short int wAddr, unsigned char byData );
void Map114_Write( unsigned short int wAddr, unsigned char byData );
void Map114_HSync();
void Map114_Set_CPU_Banks();
void Map114_Set_PPU_Banks();

void Map115_Init();
void Map115_Sram( unsigned short int wAddr, unsigned char byData );
void Map115_Write( unsigned short int wAddr, unsigned char byData );
void Map115_HSync();
void Map115_Set_CPU_Banks();
void Map115_Set_PPU_Banks();

void Map116_Init();
void Map116_Write( unsigned short int wAddr, unsigned char byData );
void Map116_HSync();
void Map116_Set_CPU_Banks();
void Map116_Set_PPU_Banks();

void Map117_Init();
void Map117_Write( unsigned short int wAddr, unsigned char byData );
void Map117_HSync();

void Map118_Init();
void Map118_Write( unsigned short int wAddr, unsigned char byData );
void Map118_HSync();
void Map118_Set_CPU_Banks();
void Map118_Set_PPU_Banks();

void Map119_Init();
void Map119_Write( unsigned short int wAddr, unsigned char byData );
void Map119_HSync();
void Map119_Set_CPU_Banks();
void Map119_Set_PPU_Banks();

void Map122_Init();
void Map122_Sram( unsigned short int wAddr, unsigned char byData );

void Map133_Init();
void Map133_Apu( unsigned short int wAddr, unsigned char byData );

void Map134_Init();
void Map134_Apu( unsigned short int wAddr, unsigned char byData );

void Map135_Init();
void Map135_Apu( unsigned short int wAddr, unsigned char byData );
void Map135_Set_PPU_Banks();

void Map140_Init();
void Map140_Sram( unsigned short int wAddr, unsigned char byData );
void Map140_Apu( unsigned short int wAddr, unsigned char byData );

void Map151_Init();
void Map151_Write( unsigned short int wAddr, unsigned char byData );

void Map160_Init();
void Map160_Write( unsigned short int wAddr, unsigned char byData );
void Map160_HSync();

void Map180_Init();
void Map180_Write( unsigned short int wAddr, unsigned char byData );

void Map181_Init();
void Map181_Apu( unsigned short int wAddr, unsigned char byData );

void Map182_Init();
void Map182_Write( unsigned short int wAddr, unsigned char byData );
void Map182_HSync();

void Map183_Init();
void Map183_Write( unsigned short int wAddr, unsigned char byData );
void Map183_HSync();

#ifndef use_lib_not_use_mapper185
 void Map185_Init();
 void Map185_Write( unsigned short int wAddr, unsigned char byData );
#endif 

void Map187_Init();
void Map187_Write( unsigned short int wAddr, unsigned char byData );
void Map187_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map187_ReadApu( unsigned short int wAddr );
void Map187_HSync();
void Map187_Set_CPU_Banks();
void Map187_Set_PPU_Banks();

#ifndef use_lib_not_use_mapper188
 void Map188_Init();
 void Map188_Write( unsigned short int wAddr, unsigned char byData );
#endif


void Map189_Init();
void Map189_Apu( unsigned short int wAddr, unsigned char byData );
void Map189_Write( unsigned short int wAddr, unsigned char byData );
void Map189_HSync();

void Map191_Init();
void Map191_Apu( unsigned short int wAddr, unsigned char byData );
void Map191_Set_CPU_Banks();
void Map191_Set_PPU_Banks();

void Map193_Init();
void Map193_Sram( unsigned short int wAddr, unsigned char byData );

void Map194_Init();
void Map194_Write( unsigned short int wAddr, unsigned char byData );

void Map200_Init();
void Map200_Write( unsigned short int wAddr, unsigned char byData );

void Map201_Init();
void Map201_Write( unsigned short int wAddr, unsigned char byData );

void Map202_Init();
void Map202_Apu( unsigned short int wAddr, unsigned char byData );
void Map202_Write( unsigned short int wAddr, unsigned char byData );
void Map202_WriteSub( unsigned short int wAddr, unsigned char byData );

void Map222_Init();
void Map222_Write( unsigned short int wAddr, unsigned char byData );

void Map225_Init();
void Map225_Write( unsigned short int wAddr, unsigned char byData );

void Map226_Init();
void Map226_Write( unsigned short int wAddr, unsigned char byData );

void Map227_Init();
void Map227_Write( unsigned short int wAddr, unsigned char byData );

void Map228_Init();
void Map228_Write( unsigned short int wAddr, unsigned char byData );

void Map229_Init();
void Map229_Write( unsigned short int wAddr, unsigned char byData );

void Map230_Init();
void Map230_Write( unsigned short int wAddr, unsigned char byData );

void Map231_Init();
void Map231_Write( unsigned short int wAddr, unsigned char byData );

void Map232_Init();
void Map232_Write( unsigned short int wAddr, unsigned char byData );

void Map233_Init();
void Map233_Write( unsigned short int wAddr, unsigned char byData );

void Map234_Init();
void Map234_Write( unsigned short int wAddr, unsigned char byData );
void Map234_Set_Banks();

#ifndef use_lib_not_use_mapper235
 void Map235_Init();
 void Map235_Write( unsigned short int wAddr, unsigned char byData );
#endif 

void Map236_Init();
void Map236_Write( unsigned short int wAddr, unsigned char byData );

void Map240_Init();
void Map240_Apu( unsigned short int wAddr, unsigned char byData );

void Map241_Init();
void Map241_Write( unsigned short int wAddr, unsigned char byData );

void Map242_Init();
void Map242_Write( unsigned short int wAddr, unsigned char byData );

void Map243_Init();
void Map243_Apu( unsigned short int wAddr, unsigned char byData );

void Map244_Init();
void Map244_Write( unsigned short int wAddr, unsigned char byData );

void Map245_Init();
void Map245_Write( unsigned short int wAddr, unsigned char byData );
void Map245_HSync();
#if 0
void Map245_Set_CPU_Banks();
void Map245_Set_PPU_Banks();
#endif 

void Map246_Init();
void Map246_Sram( unsigned short int wAddr, unsigned char byData );

void Map248_Init();
void Map248_Write( unsigned short int wAddr, unsigned char byData );
void Map248_Apu( unsigned short int wAddr, unsigned char byData );
void Map248_Sram( unsigned short int wAddr, unsigned char byData );
void Map248_HSync();
void Map248_Set_CPU_Banks();
void Map248_Set_PPU_Banks();

void Map249_Init();
void Map249_Write( unsigned short int wAddr, unsigned char byData );
void Map249_Apu( unsigned short int wAddr, unsigned char byData );
void Map249_HSync();

void Map251_Init();
void Map251_Write( unsigned short int wAddr, unsigned char byData );
void Map251_Sram( unsigned short int wAddr, unsigned char byData );
void Map251_Set_Banks();

void Map252_Init();
void Map252_Write( unsigned short int wAddr, unsigned char byData );
void Map252_HSync();

void Map255_Init();
void Map255_Write( unsigned short int wAddr, unsigned char byData );
void Map255_Apu( unsigned short int wAddr, unsigned char byData );
unsigned char Map255_ReadApu( unsigned short int wAddr );

#endif //!InfoNES_MAPPER_H_INCLUDED


