/*===================================================================*/
/*                                                                   */
/*  InfoNES_System.h : The function which depends on a system        */
/*                                                                   */
/*  2000/05/29  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef InfoNES_SYSTEM_H_INCLUDED
#define InfoNES_SYSTEM_H_INCLUDED

#include "gbConfig.h"
#include "gbGlobals.h"
/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

//JJ #include "InfoNESTypes.h"

/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/
//JJ extern WORD NesPalette[];

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Menu screen */
int InfoNES_Menu();

/* Read ROM image file */
int InfoNES_ReadRom( const char *pszFileName );

/* Release a memory for ROM */
void InfoNES_ReleaseRom();

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame();

/* Get a joypad state */
void InfoNES_PadState( unsigned int *pdwPad1, unsigned int *pdwPad2, unsigned int *pdwSystem ); //void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem );

/* memcpy */
void *InfoNES_MemoryCopy( void *dest, const void *src, int count );

/* memset */
void *InfoNES_MemorySet( void *dest, int c, int count );

/* Print debug message */
void InfoNES_DebugPrint( char *pszMsg );

/* Wait */
//void InfoNES_Wait();

/* Sound Initialize */
//void InfoNES_SoundInit( void );

//Sound Open
//int InfoNES_SoundOpen( int samples_per_sync, int sample_rate );

//Sound Close
//void InfoNES_SoundClose( void );

//Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM
//void InfoNES_SoundOutput(int samples, unsigned char *wave1, unsigned char *wave2, unsigned char *wave3, unsigned char *wave4, unsigned char *wave5);

/* Print system message */
void InfoNES_MessageBox( char *pszMsg, ... );

#ifdef use_lib_sound_fabgl
 void sound_cycleFabgl(void);
 void SilenceAllChannels(void);
#endif

#endif /* !InfoNES_SYSTEM_H_INCLUDED */
