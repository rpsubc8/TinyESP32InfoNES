#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H

 #include "gbConfig.h"
 
 //#include "InfoNESTypes.h"
 #include "InfoNES.h"
 #include "InfoNESpAPU.h"
 
 //#include <SDL/SDL.h>
 
 //extern SDL_Surface *gb_screen;
 //extern SDL_Color gb_colors[256];

 #ifdef use_lib_sound_fabgl
  extern unsigned int jj_snd_frec_tail[40][4];
  extern unsigned int jj_snd_vol_tail[40][4];
  extern unsigned int jj_snd_state_tail[40][4];
  extern unsigned int tiempo_snd_inicio;
  
  extern unsigned int gbVolMixer_now_fabgl[4];
  extern unsigned int gbVolMixer_before_fabgl[4];
  extern unsigned int gbFrecMixer_now_fabgl[4];
  extern unsigned int gbFrecMixer_before_fabgl[4];
 #endif

 extern unsigned char gb_setup_end;
 
 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 extern unsigned char gb_const_colorNormal[64]; //8 y 64 colores
 extern unsigned char gb_sync_bits; 
 extern const unsigned char pin_config8[];
 extern const unsigned char pin_config64[];
 extern const unsigned int *gb_ptrVideo_cur;
 extern unsigned char gb_vga_videomode_cur;


 
 #ifdef use_lib_esp32_dac  
  extern unsigned char gb_frec_canal1_low;
  extern unsigned char gb_frec_canal1_high;
  extern unsigned char gb_frec_canal2_low;
  extern unsigned char gb_frec_canal2_high;
  extern unsigned char gb_frec_canal3_low;
  extern unsigned char gb_frec_canal3_high; 
  extern unsigned char gb_frec_canal4_low;
  extern unsigned char gb_frec_canal4_high;   
  
  extern volatile unsigned int gb_cur_cont_ch[5];  
  extern volatile unsigned int gb_max_cont_ch[5];
  extern volatile unsigned char gb_flipflop_ch[5];
  
  extern volatile unsigned char gbVol_canal_now[5];
  extern volatile unsigned char gbVolMixer_now[5];
  extern volatile unsigned int gbFrecMixer_now[4];
  
  
  #ifdef use_lib_esp32_dac_8khz
   #define SAMPLE_RATE 8000
  #else
   #ifdef use_lib_esp32_dac_11khz
    #define SAMPLE_RATE 11025
   #else
    #ifdef use_lib_esp32_dac_16khz
     #define SAMPLE_RATE 16000
    #else
     #ifdef use_lib_esp32_dac_22khz
      #define SAMPLE_RATE 22050
     #else
      #ifdef use_lib_esp32_dac_44khz
       #define SAMPLE_RATE 44100
      #else       
      #endif
     #endif
    #endif            
   #endif
  #endif
   
 #endif
 
 //InfoNES.h begin
 extern unsigned char *RAM;  //RAM   extern unsigned char RAM[];  //RAM
 extern unsigned char *SRAM; //extern unsigned char SRAM[]; //SRAM
 extern unsigned char *ROM;   //ROM
 extern unsigned char *SRAMBANK; //SRAM BANK (8Kb)
 extern unsigned char *ROMBANK0; //ROM BANK ( 8Kb x 4 )
 extern unsigned char *ROMBANK1;
 extern unsigned char *ROMBANK2;
 extern unsigned char *ROMBANK3;
 
 extern unsigned char *PPURAM; //extern unsigned char PPURAM[]; //PPU RAM
 extern unsigned char *VROM;    //VROM
 extern unsigned char *PPUBANK[]; //PPU BANK ( 1Kb x 16 ) 
 
 extern unsigned char *SPRRAM; //extern unsigned char SPRRAM[];  //Sprite RAM
 
 
 extern unsigned char PPU_R0; //PPU Register
 extern unsigned char PPU_R1;
 extern unsigned char PPU_R2;
 extern unsigned char PPU_R3;
 extern unsigned char PPU_R7;

 extern unsigned char PPU_Scr_V;
 extern unsigned char PPU_Scr_V_Next;
 extern unsigned char PPU_Scr_V_Byte;
 extern unsigned char PPU_Scr_V_Byte_Next;
 extern unsigned char PPU_Scr_V_Bit;
 extern unsigned char PPU_Scr_V_Bit_Next;

 extern unsigned char PPU_Scr_H;
 extern unsigned char PPU_Scr_H_Next;
 extern unsigned char PPU_Scr_H_Byte;
 extern unsigned char PPU_Scr_H_Byte_Next;
 extern unsigned char PPU_Scr_H_Bit;
 extern unsigned char PPU_Scr_H_Bit_Next;

 extern unsigned char PPU_Latch_Flag;
 extern unsigned short int PPU_Addr;
 extern unsigned short int PPU_Temp;
 extern unsigned short int PPU_Increment;

 extern unsigned char PPU_Latch_Flag;
 extern unsigned char PPU_UpDown_Clip;
 
 
 extern unsigned short int PPU_Scanline; //urrent Scanline
 extern unsigned char *PPU_ScanTable; //extern unsigned char PPU_ScanTable[]; //Scanline Table
 extern unsigned char PPU_NameTableBank; //Name Table Bank
 extern unsigned char *PPU_BG_Base; //BG Base Address
 extern unsigned char *PPU_SP_Base; //Sprite Base Address
 extern unsigned short int PPU_SP_Height;  //Sprite Height
 
 extern unsigned char byVramWriteEnable; //VRAM Write Enable ( 0: Disable, 1: Enable )
 
 extern unsigned char FrameIRQ_Enable;  //Frame IRQ ( 0: Disabled, 1: Enabled )
 extern unsigned short int FrameStep;
 
 extern unsigned short int FrameSkip; //Frame Skip
 extern unsigned short int FrameCnt;
 extern unsigned short int FrameWait;
 
 extern unsigned char *ChrBuf; //extern unsigned char ChrBuf[];
 extern unsigned char ChrBufUpdate;
 #ifdef use_lib_WorkFrame8
  extern unsigned char *PalTable; //extern unsigned char PalTable[];
 #else
  //extern unsigned short int PalTable[];
  extern unsigned short int *PalTable;
 #endif 
 
 extern unsigned char APU_Reg[];
 extern int APU_Mute;

 extern unsigned int PAD1_Latch;
 extern unsigned int PAD2_Latch;
 extern unsigned int PAD_System;
 extern unsigned int PAD1_Bit;
 extern unsigned int PAD2_Bit;
 
 
 #ifdef use_lib_WorkFrame8
  #ifdef use_lib_not_use_framebuffer
   extern unsigned char WorkFrameOneLine[256]; //Solo una linea
  #else
   extern unsigned char *WorkFrame; //extern unsigned char WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
  #endif 
 #else
  #ifdef use_lib_not_use_framebuffer
   extern unsigned short int WorkFrameOneLine[256]; //Solo una linea
  #else
   extern unsigned short int WorkFrame[ NES_DISP_WIDTH * NES_DISP_HEIGHT ];
  #endif 
 #endif 
 
 extern unsigned char MapperNo; //Mapper No
 extern unsigned char ROM_Mirroring; //Other
 extern unsigned char ROM_SRAM;
 extern unsigned char ROM_Trainer;
 extern unsigned char ROM_FourScr; 
 
 
 /* Initialize Mapper */
extern void (*MapperInit)();
/* Write to Mapper */
extern void (*MapperWrite)( unsigned short int wAddr, unsigned char byData );
/* Write to SRAM */
extern void (*MapperSram)( unsigned short int wAddr, unsigned char byData );
/* Write to APU */
extern void (*MapperApu)( unsigned short int wAddr, unsigned char byData );
/* Read from Apu */
extern unsigned char (*MapperReadApu)( unsigned short int wAddr );
/* Callback at VSync */
extern void (*MapperVSync)();
/* Callback at HSync */
extern void (*MapperHSync)();
/* Callback at PPU read/write */
extern void (*MapperPPU)( unsigned short int wAddr );
/* Callback at Rendering Screen 1:BG, 0:Sprite */
extern void (*MapperRenderScreen)( unsigned char byMode ); 


 extern struct NesHeader_tag NesHeader;
 //InfoNES.h end
 
 
 
 //InfoNESMapper.h BEGIN 
 #ifndef use_lib_not_use_mapper235
  extern unsigned char * DRAM; //extern unsigned char DRAM[];  //Disk System RAM
 #endif 
 extern struct MapperTable_tag MapperTable[];
 //InfoNESMapper.h END
 

 //InfoNESSystem.h BEGIN
 #ifdef use_lib_palette_ttgo
  extern unsigned char NesPalette[]; //Paleta 8 bits
 #else
  extern unsigned short int NesPalette[]; //Palette data  
 #endif 
 //InfoNESSystem.h END


 //InfoNESpAPU.h BEGIN
 extern ApuWritefunc pAPUSoundRegs[20];
 extern int ApuQuality;
 extern unsigned char  ApuC1Atl;
 extern unsigned char  ApuC2Atl;   
 extern unsigned char  ApuC3a;
 extern unsigned char  ApuC3Atl;
 extern unsigned int ApuC3Llc;                             //Linear Length Counter
 extern unsigned char  ApuC4Atl;
 //InfoNESpAPU.h END


 //K6502.h BEGIN
 extern unsigned char IRQ_State; //The state of the IRQ pin
 extern unsigned char NMI_State; // The state of the NMI pin
 extern unsigned short int g_wPassedClocks; // The number of the clocks that it passed
 //K6502.h END


 extern unsigned int gb_fps_cur;
 
 extern unsigned char gb_sinfreno;


 extern unsigned char gb_use_keyb_left;
 extern unsigned char gb_use_keyb_right;
 extern unsigned char gb_use_keyb_up;
 extern unsigned char gb_use_keyb_down;
 extern unsigned char gb_use_keyb_space;
 extern unsigned char gb_use_keyb_return;

 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
 #endif

 extern unsigned char gb_use_debug;

 extern unsigned char gb_use_video_mode_pal;


 
 extern volatile unsigned int gbDutty_canal_now[4];
 extern volatile unsigned int gb_max_cont_pos_ch[4];
 extern volatile unsigned int gb_max_cont_neg_ch[4]; 
 extern volatile unsigned int gb_triangle_inc_ch;
 extern unsigned char gbRuidoEnv;
 extern unsigned char gbPulso0Env;
 extern unsigned char gbPulso1Env; 
 extern volatile unsigned int gb_triangle_value;
 extern volatile unsigned int gb_cont_triangulo;
 
 extern unsigned char gb_use_Pulso0Env;
 extern unsigned char gb_use_Pulso1Env;
 extern unsigned char gb_use_RuidoEnv;
 extern unsigned char gb_use_dmc_deltaSigma;

 extern unsigned int gb_latch_inc_triangle;//1 canal triangulo
 extern unsigned int gb_latch_vol_pulse[4];//2 canales pulso volumen 
 extern unsigned char gb_latch_vol_mix[5]; //4 canales de mezcla   
 extern unsigned int gb_latch_vol_triangle;//1 canal triangulo volumen
 extern unsigned int gb_latch_pos_max_pulse[4];//2 canales pulso positivo
 extern unsigned int gb_latch_neg_max_pulse[4];//2 canales pulso negativo  
 extern unsigned int gb_latch_max_triangle;//1 canal triangulo 
 extern unsigned int gb_dmc_rate;
 #define max_gb_dmc_sample 9870
 extern signed char gb_dmc_sample[max_gb_dmc_sample];
 extern volatile unsigned int gb_dmc_sample_len;
 extern unsigned int gb_dmc_addr;
 extern volatile unsigned int gb_dmc_sample_cur; 
 extern unsigned char gb_dmc_instrument_id[3]; //3 bytes id para no cargar de nuevo    


 extern unsigned int gb_latch_vol_noise; //1 canal ruido 
 extern unsigned int gb_latch_vol_dmc;



 #ifdef use_lib_esp32_dac
  //Especial
  extern unsigned char gb_latch_loop_pulse[2];
  extern unsigned char gb_latch_c_pulse[2];     
  extern unsigned char gb_noise_force_begin;
  extern unsigned char gb_square0_force_begin;
  extern unsigned char gb_square1_force_begin;  
 #endif

#endif

 
 
