//2024/12/08
//port mod x86 InfoNES SDL to ESP32 by ackerman
//VStudio v1.67.2 PlatformIO v3.3.3 Espressif32 v3.3.2
//TTGO VGA32 v1.2x not PSRAM

//	InfoNES
//		SDL ports by mata      03/04/19
//              Modified by Jay        06/02/25
// 
//	Start Date: 2003.04.19 

#include "gbConfig.h"
//#include "hardware.h"
#include "gbGlobals.h"
#include "PS2Kbd.h"
#include "vga6bit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

//#include <SDL/SDL.h>

#include "InfoNES.h"
#include "InfoNESSystem.h"
#include "InfoNESpAPU.h"
#include "InfoNESMapper.h"

#ifdef use_lib_rom_memory
 #include "dataFlash/gbrom.h"
#endif

#include <Arduino.h>

#ifdef use_lib_log_serial
 unsigned char gb_use_debug=1;
#else 
 unsigned char gb_use_debug=0;
#endif 


unsigned char gb_use_keyb_left=0;
unsigned char gb_use_keyb_right=0;
unsigned char gb_use_keyb_up=0;
unsigned char gb_use_keyb_down=0;
unsigned char gb_use_keyb_space=0;
unsigned char gb_use_keyb_return=0;


#ifdef use_lib_keyboard_uart
 unsigned int gb_curTime_keyboard_before_uart;
 unsigned int gb_curTime_keyboard_uart; 
#endif 

unsigned char gb_const_colorNormal[64]; //8 y 64 colores
unsigned char ** gb_buffer_vga;
unsigned int **gb_buffer_vga32;
unsigned char gb_sync_bits;


unsigned char gb_setup_end=0;

 const unsigned char pin_config8[] = {  
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC
 };  
 const unsigned char pin_config64[] = {  
  PIN_RED_LOW,
  PIN_RED_HIGH,
  PIN_GREEN_LOW,
  PIN_GREEN_HIGH,
  PIN_BLUE_LOW,
  PIN_BLUE_HIGH,
  PIN_HSYNC,
  PIN_VSYNC
 };

 const unsigned int *gb_ptrVideo_cur= VgaMode_vga_mode_360x200; //VgaMode_vga_mode_320x200;
 unsigned char gb_vga_videomode_cur= video_mode_vga320x240x60hz_bitluni; //0 //video_mode_vga320x200x70hz_bitluni; //2

#ifdef use_lib_esp32_dac
 const int AMPLITUDE = 28000;
 const int SAMPLE_RATE = 44100;
 //const int SAMPLE_RATE = 8000;
 //SDL_AudioSpec want;
 //SDL_AudioSpec have;
 unsigned int sample_nr = 0;
 volatile unsigned char gbVol_canal_now[4]={0,0,0,0};
 volatile unsigned char gbVolMixer_now[4]={0,0,0,0};
 volatile unsigned int gbFrecMixer_now[4]={0,0,0,0}; 
 unsigned char gb_silence_all_channels=1; 
 //void SDL_audio_callback(void *user_data, Uint8 *raw_buffer, int bytes); 
 //void SDL_InitAudio(void);
 
 unsigned char gb_frec_canal1_low=0;
 unsigned char gb_frec_canal1_high=0;
 unsigned char gb_frec_canal2_low=0;
 unsigned char gb_frec_canal2_high=0;
 unsigned char gb_frec_canal3_low=0;
 unsigned char gb_frec_canal3_high=0; 
 unsigned char gb_frec_canal4_low=0;
 unsigned char gb_frec_canal4_high=0;  
 
 //unsigned int gb_cola_vol_mixer[480][3];
 //unsigned int gb_cola_max_ch[480][3];
 unsigned int gb_vol_now[3]={0,0,0};
 unsigned int gb_max_ch_now[3]={0,0,0};
#endif





unsigned int gb_fps_cur=0;
unsigned char gb_sinfreno=0;

//-------------------------------------------------------------------
//  ROM image file information                                       
//-------------------------------------------------------------------

//char *szRomName; //char szRomName[ 256 ];
//char *szSaveName; //char szSaveName[ 256 ];
int nSRAM_SaveFlag;

//-------------------------------------------------------------------
//  Constants ( Linux specific )                                     
//-------------------------------------------------------------------

#define VBOX_SIZE 7 
static const char * VERSION = "InfoNES v0.97J RC1";

//-------------------------------------------------------------------
//  Global Variables ( SDL specific )                                
//-------------------------------------------------------------------

bool quit = false;

//for video
//SDL_Surface *gb_screen;
//SDL_Color gb_colors[256];

//For Sound Emulation
//SDL_AudioSpec audio_spec;
unsigned char final_wave[2048];
int waveptr;
int wavflag;
int wavdone;

//Pad state
unsigned int dwPad1;
unsigned int dwPad2;
unsigned int dwSystem;

//-------------------------------------------------------------------
//  Function prototypes ( SDL specific )                             
//-------------------------------------------------------------------

int start_application(char *filename);
void poll_event(void);
int LoadSRAM();
int SaveSRAM();

/* Palette data */ 
#if 0
unsigned short int NesPalette[ 64 ] =
{
  0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
  0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
  0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
  0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
  0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
  0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000
};
#else
 #ifdef use_lib_palette_ttgo
  unsigned char NesPalette[ 64 ];
 #else 
  unsigned short int NesPalette[ 64 ];
 #endif 
#endif

unsigned char NesPaletteRGB[64][3] = {
   112, 112, 112,     32,  24, 136,      0,   0, 168,     64,   0, 152,
   136,   0, 112,    168,   0,  16,    160,   0,   0,    120,   8,   0,
    64,  40,   0,      0,  64,   0,      0,  80,   0,      0,  56,  16,
    24,  56,  88,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   184, 184, 184,      0, 112, 232,     32,  56, 232,    128,   0, 240,
   184,   0, 184,    224,   0,  88,    216,  40,   0,    200,  72,   8,
   136, 112,   0,      0, 144,   0,      0, 168,   0,      0, 144,  56,
     0, 128, 136,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   248, 248, 248,     56, 184, 248,     88, 144, 248,     64, 136, 248,
   240, 120, 248,    248, 112, 176,    248, 112,  96,    248, 152,  56,
   240, 184,  56,    128, 208,  16,     72, 216,  72,     88, 248, 152,
     0, 232, 216,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   248, 248, 248,    168, 224, 248,    192, 208, 248,    208, 200, 248,
   248, 192, 248,    248, 192, 216,    248, 184, 176,    248, 216, 168,
   248, 224, 160,    224, 248, 160,    168, 240, 184,    176, 248, 200,
   152, 248, 240,      0,   0,   0,      0,   0,   0,      0,   0,   0,
};






#ifdef use_lib_vga_esp32
/*
 unsigned char ** gb_buffer_vga;
 unsigned int **gb_buffer_vga32;
 //SDL_Surface * gb_screen;
 
 void PreparaBitluniVGA(void);
 void LiberaBitluniVGA(void);
 void bitluni_dump_vga(void);
 
 //***************************************
 void PreparaBitluniVGA()
 {
  printf("PreparaBitluniVGA\n"); fflush(stdout);
  gb_buffer_vga = (unsigned char **)malloc(480*sizeof(unsigned char **)*640);
  gb_buffer_vga32= (unsigned int **)gb_buffer_vga;
  for (int y=0;y<480;y++)
  {      
   gb_buffer_vga[y]= (unsigned char *)malloc(640);
   gb_buffer_vga32[y]= (unsigned int *)gb_buffer_vga32[y];
   //memset(gb_buffer_vga[y],0,320);
   memset(gb_buffer_vga[y],0x3F,640); //NES 0 Es gris 0x3F es negro
  }
 }

 //***************************************
 void LiberaBitluniVGA()
 {
  printf("LiberaBitluniVGA\n"); fflush(stdout);      
  for (int y=0;y<480;y++)
  {      
   free(gb_buffer_vga[y]);
  }
  free(gb_buffer_vga);
 }
 
 //***************************************
 void bitluni_dump_vga()
 {
  //gb_screen = screen;
  Uint8* p = (Uint8*)gb_screen->pixels + gb_screen->pitch;  
  //for (int y=0;y<224;y++)
  for (int y=0;y<480;y++)
  {
   p = (Uint8*)gb_screen->pixels + (y*gb_screen->pitch);     
   //for (int x=0;x<256;x++)
   for (int x=0;x<640;x++)
   {
    *p++= gb_buffer_vga[y][x];
   }
  }   
 }
 */
#endif





//*******************************
void PrepareColorsBitluniVGA()
{  
 //if (gb_use_debug==1){ Serial.printf("PrepareColorsBitluniVGA 64 colores\r\n"); }
 unsigned char r,g,b;
 for (unsigned char i=0;i<64;i++) 
 {
  //gb_const_colorNormal[i]= (i & 0x3F) | gb_sync_bits;

  r= ((NesPaletteRGB[i][0]>>6)&0x03);
  g= ((NesPaletteRGB[i][1]>>6)&0x03);
  b= ((NesPaletteRGB[i][2]>>6)&0x03);
  gb_const_colorNormal[i]= ((b<<4)|(g<<2)|r) | gb_sync_bits;
 }
}








void setup() 
{
 gb_setup_end= 0;

 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);
  Serial.printf("BEGIN Setup\r\n");
  //Serial.printf("HEAP BEGIN %d\r\n", gb_ramfree_ini);
 #endif   

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif  

 #ifdef use_lib_psram
  psramInit();
 #endif 

 //szRomName= (char *)ps_malloc(256);
 //szSaveName= (char *)ps_malloc(256);

 #ifndef use_lib_not_use_mapper235
  DRAM= (unsigned char *)ps_malloc(DRAM_SIZE); //40960
  Serial.printf("DRAM %d\r\n", ESP.getFreeHeap());
 #endif

 #ifdef use_lib_not_use_framebuffer
 #else
  WorkFrame= (unsigned char *)malloc((NES_DISP_WIDTH * NES_DISP_HEIGHT)); //61440
 #endif

 Serial.printf("WORKFRAME %d\r\n", ESP.getFreeHeap());
 ChrBuf= (unsigned char *)malloc((256 * 2 * 8 * 8)); //32768
 Serial.printf("ChrBuf %d\r\n", ESP.getFreeHeap()); 
 PPURAM= (unsigned char *)malloc(PPURAM_SIZE); //16384
 Serial.printf("PPURAM %d\r\n", ESP.getFreeHeap());

 RAM= (unsigned char *)malloc(RAM_SIZE); //8192
 Serial.printf("RAM %d\r\n", ESP.getFreeHeap());
 SRAM= (unsigned char *)malloc(SRAM_SIZE); //8192
 Serial.printf("SRAM %d\r\n", ESP.getFreeHeap());

 SPRRAM= (unsigned char *)malloc(SPRRAM_SIZE); //256
 Serial.printf("SPRRAM %d\r\n", ESP.getFreeHeap());
 PPU_ScanTable= (unsigned char *)malloc(263); //263
 Serial.printf("PPUScanTable %d\r\n", ESP.getFreeHeap());
 PalTable= (unsigned char *)malloc(32); //32
 Serial.printf("PalTable %d\r\n", ESP.getFreeHeap());



 kb_begin(); //Cargo teclado antes seleccion modo video
 delay(100);
 Serial.printf("keyboard %d\r\n", ESP.getFreeHeap());

 
 unsigned char is8colors=0;
 unsigned char usepllcteforce=0;
 unsigned char usecustompll=0;
 unsigned int p0=0;
 unsigned int p1=0;
 unsigned int p2=0;
 unsigned int p3=0; 

 is8colors=0; gb_ptrVideo_cur= VgaMode_vga_mode_320x240; usepllcteforce=0; usecustompll=0; //gb_vga_ancho= 320; gb_vga_alto=240;

 vga_init((is8colors==1)?pin_config8:pin_config64,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3,usecustompll,is8colors, gb_vga_videomode_cur);
 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
 PrepareColorsBitluniVGA();

 Serial.printf("VGA %d\r\n", ESP.getFreeHeap());

 gb_setup_end= 1;
 //if(psramInit())
 //{
 // gb_setup_end= 1;
 //}
 //else
 //{
 // Serial.println("PSRAM not available");
 // gb_setup_end= 0;     
 //}
}








#ifdef use_lib_keyboard_uart
 char gb_buf_uart[BUFFER_SIZE_UART];
 unsigned char gb_rlen_uart=0;

  //array de 54 teclas
  //0,
  //SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f,
  //SDLK_g, SDLK_h, SDLK_i, SDLK_j, SDLK_k, SDLK_l,
  //SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r,
  //SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x,
  //SDLK_y, SDLK_z,
  //SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT,
  //SDLK_SPACE,
  //SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
  //SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
  //SDLK_SEMICOLON, SDLK_QUOTEDBL, SDLK_COMMA, SDLK_EQUALS,
  //SDLK_PERIOD, SDLK_SLASH, SDLK_RETURN, SDLK_TAB, SDLK_LCTRL,
  //SDLK_DELETE, SDLK_SCROLLOCK, SDLK_LSHIFT

 void do_keyboard_uart()
 {
  unsigned int contBuf=0;
  unsigned short int auxAddr;
  
  //unsigned char aux_Kmap[54];
  //memset(aux_Kmap,0xFF,sizeof(aux_Kmap));

  gb_use_keyb_up=0;
  gb_use_keyb_down=0;
  gb_use_keyb_left=0;
  gb_use_keyb_right=0;
  gb_use_keyb_space=0;
  gb_use_keyb_return=0;

  dwPad1= 0;
  dwPad2= 0;


  if (gb_rlen_uart>0)
  {
   while (contBuf < gb_rlen_uart)
   {
    switch (gb_buf_uart[contBuf])
    {
     case 0x01: //F2   01 62 4F 51      
      if ((contBuf+3) < gb_rlen_uart)
      {       
//       gb_do_action_key_f1= 1;
       contBuf+= 3;
      }
      break;

     case 0x09: //TAB saco menu tambien
//      gb_do_action_key_f1= 1;
      break;      

     case 0x08: case 0x7F: //Borrar
      //aux_Kmap[29]= 0;
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER 
      //keyboard[6]&= 0xFE; //ENTER
      //aux_Kmap[48]= 0; //SDLK_RETURN id en kmap 48
      gb_use_keyb_return=1;
      break;

     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: gb_use_keyb_up=1; dwPad1 |= (1<<4); break; //arriba 1B 5B 41
         case 0x42: gb_use_keyb_down=1; dwPad1 |= (1<<5); break; //abajo 1B 5B 42
         case 0x44: gb_use_keyb_left=1; dwPad1 |= (1<<6); break; //izquierda 1B 5B 44
         case 0x43: gb_use_keyb_right=1; dwPad1 |= (1<<7); break; //derecha 1B 5B 43
         default: break;
        }
       }
      }
      break;      

    case 0x38: gb_use_keyb_up=1; dwPad1 |= (1<<4); break; //8 Arriba
    case 0x32: gb_use_keyb_down=1; dwPad1 |= (1<<5); break; //2 Abajo
    case 0x34: gb_use_keyb_left=1; dwPad1 |= (1<<6); break; //4 Izquierda
    case 0x36: gb_use_keyb_right=1; dwPad1 |= (1<<7); break; //6 Derecha

    case 0x61: case 0x41: dwPad1 |= (1<<2); break; //A  SELECT
    case 0x73: case 0x53: dwPad1 |= (1<<3); break; //S  START
    case 0x78: case 0x58: dwPad1 |= (1<<0); break; //X  A
    case 0x7A: case 0x5A: dwPad1 |= (1<<1); break; //Z  B    


     //case 0x2D: aux_Kmap[53]= 0; break; //SHIFT LEFT -
     //case 0x2E: aux_Kmap[46]= 0; break; //.


     //La velocidad se hace desde OSD
     //case 0x61: case 0x41: gb_use_speed_fast=1; break; //A equivale F10 Rapido
     //case 0x62: case 0x42: gb_use_speed_fast=0; break; //B equivale F11 Lento

/*
     case 0x63: case 0x43: aux_Kmap[3]= 0; gb_Kmap[3]= 0; break; //C
     case 0x64: case 0x44: aux_Kmap[4]= 0; gb_Kmap[4]= 0; break; //D
     case 0x65: case 0x45: aux_Kmap[5]= 0; gb_Kmap[5]= 0; break; //E
     case 0x66: case 0x46: aux_Kmap[6]= 0; gb_Kmap[6]= 0; break; //F
     case 0x67: case 0x47: aux_Kmap[7]= 0; break; //G
     case 0x68: case 0x48: aux_Kmap[8]= 0; break; //H
     case 0x69: case 0x49: aux_Kmap[9]= 0; break; //I
     case 0x6A: case 0x4A: aux_Kmap[10]= 0; break; //J
     case 0x6B: case 0x4B: aux_Kmap[11]= 0; break; //K
     case 0x6C: case 0x4C: aux_Kmap[12]= 0; break; //L
     case 0x6D: case 0x4D: aux_Kmap[13]= 0; break; //M
     case 0x6E: case 0x4E: aux_Kmap[14]= 0; break; //N
     case 0x6F: case 0x4F: aux_Kmap[15]= 0; break; //O
     case 0x70: case 0x50: aux_Kmap[16]= 0; break; //P
     case 0x71: case 0x51: aux_Kmap[17]= 0; break; //Q
     case 0x72: case 0x52: aux_Kmap[18]= 0; break; //R
     case 0x73: case 0x53: aux_Kmap[19]= 0; break; //S
     case 0x74: case 0x54: aux_Kmap[20]= 0; break; //T
     case 0x75: case 0x55: aux_Kmap[21]= 0; break; //U
     case 0x76: case 0x56: aux_Kmap[22]= 0; break; //V
     case 0x77: case 0x57: aux_Kmap[23]= 0; break; //W
     case 0x78: case 0x58: aux_Kmap[24]= 0; break; //X
     case 0x79: case 0x59: aux_Kmap[25]= 0; break; //Y
     case 0x7A: case 0x5A: aux_Kmap[26]= 0; break; //Z
     */
     
     case 0x20: gb_use_keyb_space= 1; break; //SPACE    
      

     //Modos de video inicio provional
//     case 0x30: gb_key_video[0]=0; break; //0
//     case 0x31: gb_key_video[1]=0; break; //1
//     case 0x32: gb_key_video[2]=0; break; //2
//     case 0x33: gb_key_video[3]=0; break; //3
//     case 0x34: gb_key_video[4]=0; break; //4
//     case 0x35: gb_key_video[5]=0; break; //5               
//     case 0x36: gb_key_video[6]=0; break; //6
//     case 0x37: gb_key_video[7]=0; break; //7     
//     case 0x38: gb_key_video[8]=0; break; //8     


     //La carga de nivel se hace por OSD
     //case 0x30: gb_use_game_part= 10; gb_use_send_game_part= 1; break; //0
     //case 0x31: gb_use_game_part= 1; gb_use_send_game_part= 1; break; //1
     //case 0x32: gb_use_game_part= 2; gb_use_send_game_part= 1; break; //2
     //case 0x33: gb_use_game_part= 3; gb_use_send_game_part= 1; break; //3
     //case 0x34: gb_use_game_part= 4; gb_use_send_game_part= 1; break; //4
     //case 0x35: gb_use_game_part= 5; gb_use_send_game_part= 1; break; //5               
     //case 0x36: gb_use_game_part= 6; gb_use_send_game_part= 1; break; //6
     //case 0x37: gb_use_game_part= 7; gb_use_send_game_part= 1; break; //7     
     //case 0x38: gb_use_game_part= 8; gb_use_send_game_part= 1; break; //8
     //case 0x39: gb_use_game_part= 9; gb_use_send_game_part= 1; break; //9


     default: break;
    }//fin switch gb_buf_uart
    contBuf++;
   }//fin while contBuf   
  }//fin if gb_rlen_uart

 }
#endif

#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    if (gb_use_debug==1){ Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart); }
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     if (gb_use_debug==1){ Serial.printf("%02X ",gb_buf_uart[i]); }
    }
    if (gb_use_debug==1){ Serial.printf("\r\n"); }
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }

 //***************************************************************************
 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44        
         default: break;
        }
       }
      }
      else
      {       
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC              
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      break;

     default: break;
    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
#endif











//===================================================================
//                                                                   
//                main() : Application main                          
//                                                                   
//===================================================================

//Application main
//int main(int argc, char **argv)
void loop()
{
  int argc=1; //pendiente quitar
  //char *argv[2]={"contra.nes","nada.nes"};

  unsigned int 	i,vmode;

 if (gb_setup_end==1)
 {

  Serial.printf("BEGIN\n");
  #ifdef use_lib_not_use_framebuffer
   Serial.printf("Size WorkFrameOneLine:%d\n",sizeof(WorkFrameOneLine));
  #else
   Serial.printf("Size WorkFrame:%d\n",sizeof(WorkFrame));
  #endif 
//  #ifdef use_lib_vga_esp32
//   PreparaBitluniVGA();
//  #endif   

  //Screen initialization
//  if (SDL_Init(SDL_INIT_VIDEO|
//	       SDL_INIT_AUDIO|
//	       SDL_INIT_JOYSTICK))
//  {
//    /* Failure initialization of SDL */
//    fprintf(stderr, "SDL_Init() failed.\n");
//    return 1;
//  }
  
//  #ifdef use_lib_esp32_dac
//   SDL_InitAudio();
//  #endif 
  
  //JJ vmode = SDL_HWPALETTE | SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
  //vmode = SDL_HWPALETTE | SDL_SWSURFACE;
  //vmode = SDL_SWSURFACE;
  #ifdef use_lib_vga_esp32
   //gb_screen = SDL_SetVideoMode(640,480,8,vmode);  
  #else
   gb_screen = SDL_SetVideoMode(NES_DISP_WIDTH+VBOX_SIZE,
 			    NES_DISP_HEIGHT+VBOX_SIZE,8,vmode);
  #endif
  //if(!gb_screen)
  //{
  //  fprintf(stderr, "SDL_SetVideoMode() failed.\n");
  //  return 1;
  //}
  //SDL_ShowCursor(0);
  //SDL_WM_SetCaption(VERSION,VERSION);
  
  #ifdef use_lib_esp32_dac
   //SDL_PauseAudio(0);
  #endif 

  //if(SDL_NumJoysticks() > 0)
  //{
  //  SDL_JoystickOpen(0);
  //  SDL_JoystickEventState(SDL_ENABLE);
  //}

  #ifdef use_lib_palette_ttgo
//   for (int i=0;i<256;i++)
//   {
//    gb_colors[i].r= gb_colors[i].g= gb_colors[i].b= 0;
//   }
   
//   for(i=0; i<64; ++i)
//   {
//    gb_colors[i].r= ((NesPaletteRGB[i][0]>>6)&0x03)*84;
//    gb_colors[i].g= ((NesPaletteRGB[i][1]>>6)&0x03)*84;
//    gb_colors[i].b= ((NesPaletteRGB[i][2]>>6)&0x03)*84;    
//   }
//   //SDL_SetPalette(gb_screen, SDL_LOGPAL|SDL_PHYSPAL, gb_colors, 0, 256);
//   SDL_SetPalette(gb_screen, SDL_LOGPAL|SDL_PHYSPAL, gb_colors, 0, 256);
//   printf("Forzar Paleta SDL_SetPalette\n"); fflush(stdout);     
  #endif

  for(i=0; i<64; ++i)
  {           
   #ifdef use_lib_palette_ttgo
    NesPalette[i] = i;
    //unsigned char r= (NesPaletteRGB[i][0]>>6)&0x03;
    //unsigned char g= (NesPaletteRGB[i][1]>>6)&0x03;
    //unsigned char b= (NesPaletteRGB[i][2]>>6)&0x03;
    //NesPalette[i] = (r<<4)|(g<<2)|b;
   /*
    NesPalette[i] = SDL_MapRGB(gb_screen->format,
			       ((NesPaletteRGB[i][0]>>6)&0x03)*84,
			       ((NesPaletteRGB[i][1]>>6)&0x03)*84,
			       ((NesPaletteRGB[i][2]>>6)&0x03)*84);			          
			       */
   #else
    NesPalette[i] = SDL_MapRGB(gb_screen->format,
			       NesPaletteRGB[i][0],
			       NesPaletteRGB[i][1],
			       NesPaletteRGB[i][2]);
   #endif
  }
	
  /* If a rom name specified, start it */
  //JJ if (argc>=2) 
  if (argc>=1) 
  {        
    if ( start_application((argc>=2) ? (char *)"nada.nes":(char *)"contrapal.nes") )
    {
      //MainLoop
      InfoNES_Main();
      
      //End
      SaveSRAM();
    }
    else
    {
      //Not a NES format file
      //Serial.printf("%s isn't a NES format file.\n", argv[1] );
      Serial.printf("isn't a NES format file.\n");
    }    
  }
  else
  {
    //Help Message
    Serial.printf("Copyright (C) 1998-2006 Jay's Factory,\r\n");
    Serial.printf("SDL Ports by mata.\r\n");
    Serial.printf("%s. Usage: %s [NES format file]\r\n", VERSION);
  }
	    

//  #ifdef use_lib_esp32_dac	    
//   SDL_PauseAudio(1);
//   SDL_CloseAudio(); 	    
//  #endif 
	    
  //Finalization of SDL
//  SDL_FreeSurface(gb_screen);    
//  SDL_Quit();
  
//  #ifdef use_lib_vga_esp32
//   LiberaBitluniVGA();
//  #endif

 } 
 delay(1);
}

//Start application
int start_application(char *filename)
{
  //Set a ROM image name
  //strcpy( szRomName, filename );

  //Load cassette
  //if(InfoNES_Load(szRomName)==0)
  if(InfoNES_Load("")==0) 
  { 
    //Load SRAM
    LoadSRAM();

    //Success
    return 1;
  }
  //Failure
  return 0;
}

//===================================================================
//                                                                   
//           LoadSRAM() : Load a SRAM                                
//                                                                   
//===================================================================
int LoadSRAM()
{

 //  Load a SRAM
 //
 //  Return values
 //     0 : Normally
 //    -1 : SRAM data couldn't be read
 
 /*

  FILE *fp;
  unsigned char pSrcBuf[ SRAM_SIZE ];
  unsigned char chData;
  unsigned char chTag;
  int nRunLen;
  int nDecoded;
  int nDecLen;
  int nIdx;

  // It doesn't need to save it
  nSRAM_SaveFlag = 0;

  // It is finished if the ROM doesn't have SRAM
  if ( !ROM_SRAM )
    return 0;

  // There is necessity to save it
  nSRAM_SaveFlag = 1;

  // The preparation of the SRAM file name
  strcpy( szSaveName, szRomName );
  strcpy( strrchr( szSaveName, '.' ) + 1, "srm" );

  //-------------------------------------------------------------------
  //  Read a SRAM data                                                 
  //-------------------------------------------------------------------

  // Open SRAM file
  fp = fopen( szSaveName, "rb" );
  if ( fp == NULL )
    return -1;

  // Read SRAM data
  fread( pSrcBuf, SRAM_SIZE, 1, fp );

  // Close SRAM file
  fclose( fp );

  //-------------------------------------------------------------------
  //  Extract a SRAM data                                              
  //-------------------------------------------------------------------

  nDecoded = 0;
  nDecLen = 0;

  chTag = pSrcBuf[ nDecoded++ ];

  while ( nDecLen < 8192 )
  {
    chData = pSrcBuf[ nDecoded++ ];

    if ( chData == chTag )
    {
      chData = pSrcBuf[ nDecoded++ ];
      nRunLen = pSrcBuf[ nDecoded++ ];
      for ( nIdx = 0; nIdx < nRunLen + 1; ++nIdx )
      {
        SRAM[ nDecLen++ ] = chData;
      }
    }
    else
    {
      SRAM[ nDecLen++ ] = chData;
    }
  }
*/

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           SaveSRAM() : Save a SRAM                                */
/*                                                                   */
/*===================================================================*/
int SaveSRAM()
{
 //  Save a SRAM
 //
 //  Return values
 //     0 : Normally
 //    -1 : SRAM data couldn't be written
 
/*
  FILE *fp;
  int nUsedTable[ 256 ];
  unsigned char chData;
  unsigned char chPrevData;
  unsigned char chTag;
  int nIdx;
  int nEncoded;
  int nEncLen;
  int nRunLen;
  unsigned char pDstBuf[ SRAM_SIZE ];

  if ( !nSRAM_SaveFlag )
    return 0;  // It doesn't need to save it

  //-------------------------------------------------------------------
  //  Compress a SRAM data                                             
  //-------------------------------------------------------------------

  memset( nUsedTable, 0, sizeof nUsedTable );

  for ( nIdx = 0; nIdx < SRAM_SIZE; ++nIdx )
  {
    ++nUsedTable[ SRAM[ nIdx++ ] ];
  }
  for ( nIdx = 1, chTag = 0; nIdx < 256; ++nIdx )
  {
    if ( nUsedTable[ nIdx ] < nUsedTable[ chTag ] )
      chTag = nIdx;
  }

  nEncoded = 0;
  nEncLen = 0;
  nRunLen = 1;

  pDstBuf[ nEncLen++ ] = chTag;

  chPrevData = SRAM[ nEncoded++ ];

  while ( nEncoded < SRAM_SIZE && nEncLen < SRAM_SIZE - 133 )
  {
    chData = SRAM[ nEncoded++ ];

    if ( chPrevData == chData && nRunLen < 256 )
      ++nRunLen;
    else
    {
      if ( nRunLen >= 4 || chPrevData == chTag )
      {
        pDstBuf[ nEncLen++ ] = chTag;
        pDstBuf[ nEncLen++ ] = chPrevData;
        pDstBuf[ nEncLen++ ] = nRunLen - 1;
      }
      else
      {
        for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
          pDstBuf[ nEncLen++ ] = chPrevData;
      }

      chPrevData = chData;
      nRunLen = 1;
    }

  }
  if ( nRunLen >= 4 || chPrevData == chTag )
  {
    pDstBuf[ nEncLen++ ] = chTag;
    pDstBuf[ nEncLen++ ] = chPrevData;
    pDstBuf[ nEncLen++ ] = nRunLen - 1;
  }
  else
  {
    for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
      pDstBuf[ nEncLen++ ] = chPrevData;
  }

  //-------------------------------------------------------------------
  //  Write a SRAM data                                                
  //-------------------------------------------------------------------

  // Open SRAM file
  fp = fopen( szSaveName, "wb" );
  if ( fp == NULL )
    return -1;

  // Write SRAM data
  fwrite( pDstBuf, nEncLen, 1, fp );

  // Close SRAM file
  fclose( fp );

  // Successful
 */

  return 0;
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
int InfoNES_Menu(){
  if(quit) return -1;
  return 0;
}

//Read ROM image file
int InfoNES_ReadRom( const char *pszFileName )
{
 #ifdef use_lib_rom_memory
  unsigned int contOffs=0;
  memcpy(&NesHeader, &gb_list_rom_data[0][contOffs], sizeof(NesHeader) );
  contOffs+= sizeof(NesHeader);
  
  printf("NesHeader size:%d\n",sizeof(NesHeader)); fflush(stdout);
  printf("NesHeader byRomSize:%d\n",NesHeader.byRomSize); fflush(stdout);
  
  if( memcmp( NesHeader.byID, "NES\x1a", 4 )!=0){
    //not .nes file
    printf("not .nes file\n"); fflush(stdout);
    return -1;
  }
  
  memset( SRAM, 0, SRAM_SIZE );
  
  if(NesHeader.byInfo1 & 4)
  {
    memcpy( &SRAM[0x1000], &gb_list_rom_data[0][contOffs],  512 );    
    contOffs+= 512;
  }
  
  ROM = (unsigned char*)&gb_list_rom_data[0][contOffs];
  printf("ROM size FLASH %s cont:%d size:%d\n",gb_list_rom_title[0],contOffs,(NesHeader.byRomSize * 0x4000)); fflush(stdout);
  contOffs+= (0x4000*NesHeader.byRomSize);
  
  if(NesHeader.byVRomSize>0)
  {
    printf("VROM flash size:%d offs:%d\n",(NesHeader.byVRomSize * 0x2000 ), contOffs); fflush(stdout);
    //VROM = (unsigned char *)&gb_list_rom_title[0][contOffs];
    #ifdef use_lib_psram
     VROM = (unsigned char *)ps_malloc( NesHeader.byVRomSize * 0x2000 );
    #else
     VROM = (unsigned char *)malloc( NesHeader.byVRomSize * 0x2000 );
    #endif 
    memcpy(VROM,&gb_list_rom_data[0][contOffs],(NesHeader.byVRomSize * 0x2000) );
    //fread( VROM, 0x2000, NesHeader.byVRomSize, fp );
  }  
  
  return 0;
 #else
  FILE *fp;

  /* Open ROM file */
  fp=fopen(pszFileName,"rb");
  if(fp==NULL) return -1;

  /* Read ROM Header */
  fread( &NesHeader, sizeof NesHeader, 1, fp );
  if( memcmp( NesHeader.byID, "NES\x1a", 4 )!=0){
    /* not .nes file */
    fclose( fp );
    return -1;
  }

  /* Clear SRAM */
  memset( SRAM, 0, SRAM_SIZE );

  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if(NesHeader.byInfo1 & 4){
    fread( &SRAM[ 0x1000 ], 512, 1, fp );
  }

  //Allocate Memory for ROM Image
  ROM = (unsigned char *)malloc( NesHeader.byRomSize * 0x4000 );
  printf("ROM size:%d\n",(NesHeader.byRomSize * 0x4000 )); fflush(stdout);
 
  //Read ROM Image
  fread( ROM, 0x4000, NesHeader.byRomSize, fp );


  if(NesHeader.byVRomSize>0)
  {
    //Allocate Memory for VROM Image
    VROM = (unsigned char *)malloc( NesHeader.byVRomSize * 0x2000 );
    printf("VROM size:%d\n",(NesHeader.byVRomSize * 0x2000 )); fflush(stdout);

    //Read VROM Image
    fread( VROM, 0x2000, NesHeader.byVRomSize, fp );
  }

  /* File close */
  fclose( fp );

  /* Successful */
  return 0;
 #endif
}

/* Release a memory for ROM */
void InfoNES_ReleaseRom(){
  if(ROM){
    free(ROM);
    ROM=NULL;
  }

  if(VROM){
    free(VROM);
    VROM=NULL;
  }
}

/* Transfer the contents of work frame on the screen */
unsigned int gb_vga_before=0;
unsigned int gb_vga_cur=0;
void InfoNES_LoadFrame()
{    
 unsigned char a0,a1,a2,a3;
 unsigned int a32;
 unsigned char *pw;

  gb_fps_cur++;

  gb_vga_cur= millis(); //SDL_GetTicks();
  if ((gb_vga_cur-gb_vga_before)<19)  
  {//50 fps
   return;
  }
  gb_vga_before= gb_vga_cur;

 #ifdef use_lib_not_use_framebuffer
 #else
  //pl=(BYTE *)screen->pixels;
  pw = WorkFrame;
  for(unsigned char y=0; y<NES_DISP_HEIGHT; y++)
  {
   //p=pl;
   for(unsigned char x=0; x<64; x++) //for(x=0; x<NES_DISP_WIDTH; x++)
   {               
    //*p++ = (BYTE)*pw++;
    //gb_buffer_vga[y][x]= (unsigned char)*pw++;
    a0= gb_const_colorNormal[*pw++];
    a1= gb_const_colorNormal[*pw++];
    a2= gb_const_colorNormal[*pw++];
    a3= gb_const_colorNormal[*pw++];
    a32= a2 | (a3<<8) | (a0<<16) | (a1<<24); //ESP32
    gb_buffer_vga32[y][x]= a32; 
   }
   //pl+=screen->pitch; 
   //pw+=(0x100-NES_DISP_WIDTH);
  }

 #endif
 
//  bitluni_dump_vga();
//  SDL_Flip(gb_screen);  
}

// Get a joypad state
void InfoNES_PadState( unsigned int *pdwPad1, unsigned int *pdwPad2, unsigned int *pdwSystem )
{
  poll_event();
  
  #ifdef use_lib_keyboard_uart
   keyboard_uart_poll();
   ///read_keyboard_uart();
   do_keyboard_uart();
  #endif

  *pdwPad1 = dwPad1;
  *pdwPad2 = dwPad2;
  *pdwSystem = dwSystem;
}

static const int joy_commit_range = 3276;

void poll_event(void)
{
/*JJ pendiente  

  SDL_Event	e;
  static signed char	x_joy=0, y_joy=0;
		
  while(SDL_PollEvent(&e))
  {
    switch(e.type)
    {
    case SDL_QUIT:
      dwSystem|=PAD_SYS_QUIT;
      quit=1;
      break;
      
    case SDL_KEYDOWN:
      switch(e.key.keysym.sym)
      {
     case SDLK_F1: 
     gb_sinfreno= ((gb_sinfreno+1)&0x01);
     break;
                                   
      case SDLK_RETURN:
	if( !(e.key.keysym.mod & KMOD_ALT)) break;
	SDL_WM_ToggleFullScreen(gb_screen);
	break;

      case SDLK_ESCAPE:
	dwSystem|=PAD_SYS_QUIT;
	quit=1;
	break;
	
      case SDLK_RIGHT:   dwPad1 |= (1<<7);break;
      case SDLK_LEFT:    dwPad1 |= (1<<6);break;
      case SDLK_DOWN:    dwPad1 |= (1<<5);break;
      case SDLK_UP:      dwPad1 |= (1<<4);break;
      case SDLK_s:	 dwPad1 |= (1<<3);break;    //Start
      case SDLK_a:	 dwPad1 |= (1<<2);break;    //Select
      case SDLK_z:	 dwPad1 |= (1<<1);break;    // 'B' 
      case SDLK_x:	 dwPad1 |= (1<<0);break;    // 'A' 
      case SDLK_m:       
	//Toggle of sound mute
	APU_Mute = ( APU_Mute ? 0 : 1 );break;      
      case SDLK_c:
	//Toggle up and down clipping
	PPU_UpDown_Clip = ( PPU_UpDown_Clip ? 0 : 1 ); break;
      } //keydown
      break;		

    case SDL_KEYUP:
      switch(e.key.keysym.sym)
      {
      case SDLK_RIGHT:   dwPad1 &=~(1<<7);break;
      case SDLK_LEFT:    dwPad1 &=~(1<<6);break;
      case SDLK_DOWN:	 dwPad1 &=~(1<<5);break;
      case SDLK_UP:	 dwPad1 &=~(1<<4);break;
      case SDLK_s:	 dwPad1 &=~(1<<3);break;   // Start 
      case SDLK_a:	 dwPad1 &=~(1<<2);break;   // Select 
      case SDLK_z:	 dwPad1 &=~(1<<1);break;   // 'B' 
      case SDLK_x:	 dwPad1 &=~(1<<0);break;   // 'A' 
      } // keyup
      break;
      
    case SDL_JOYAXISMOTION:
      switch(e.jaxis.axis){
      case 0:	// X axis
	if(e.jaxis.value >  joy_commit_range){
	  if(x_joy > 0) break;	
	  if(x_joy < 0) dwPad1 &=~(1<<6);
	  dwPad1 |= (1<<7); x_joy=+1; break; }
	if(e.jaxis.value < -joy_commit_range){
	  if(x_joy < 0) break;
	  if(x_joy > 0) dwPad1 &=~(1<<7);
	  dwPad1 |= (1<<6); x_joy=-1; break; }
	if     (x_joy < 0) dwPad1 &=~(1<<6);
	else if(x_joy > 0) dwPad1 &=~(1<<7);
	x_joy= 0; break;
      case 1: //Y asis
	if(e.jaxis.value >  joy_commit_range){
	  if(y_joy > 0) break;
	  if(y_joy < 0) dwPad1 &=~(1<<4);
	  dwPad1 |= (1<<5); y_joy=+1; break; }
	if(e.jaxis.value < -joy_commit_range){
	  if(y_joy < 0) break;
	  if(y_joy > 0) dwPad1 &=~(1<<5);
	  dwPad1 |= (1<<4); y_joy=-1; break; }
	if      (y_joy < 0) dwPad1 &=~(1<<4);
	else if (y_joy > 0) dwPad1 &=~(1<<5);
	y_joy= 0; break;
      } //joysxismotion

    case SDL_JOYBUTTONUP:
      switch(e.jbutton.button){
      case 2: dwPad1 &=~(1<<0);break; // A 
      case 1: dwPad1 &=~(1<<1);break; // B 
      case 8: dwPad1 &=~(1<<2);break; // select
      case 9: dwPad1 &=~(1<<3);break; // start 
      } break;	
    case SDL_JOYBUTTONDOWN:
      switch(e.jbutton.button){
      case 2: dwPad1 |= (1<<0);break; // A 
      case 1: dwPad1 |= (1<<1);break; // B 
      case 8: dwPad1 |= (1<<2);break; // select 
      case 9: dwPad1 |= (1<<3);break; // start 
      } break;
    }
  }
 
 */
}

// memcpy
void *InfoNES_MemoryCopy( void *dest, const void *src, int count ){
  memcpy( dest, src, count );
  return dest;
}

//memset
void *InfoNES_MemorySet( void *dest, int c, int count ){
  memset( dest, c, count);  
  return dest;
}

// Print debug message 
void InfoNES_DebugPrint( char *pszMsg ) {
  fprintf(stderr,"%s\n", pszMsg);
}

// Wait
void InfoNES_Wait(){}

// Sound Initialize
void InfoNES_SoundInit( void ){}

void waveout(void *udat,unsigned char *stream,int len)
{
  if ( !wavdone )
  {
    //we always expect that len is 1024
    memcpy( stream, &final_wave[(wavflag - 1) << 10], len );
    wavflag = 0; wavdone = 1;
  }
}

//Sound Open
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate )
{
  /*JJ pendiente
  SDL_AudioSpec asp;

  asp.freq=sample_rate;
  asp.format=AUDIO_U8;
  asp.channels=1;
  asp.samples=1024; 
  asp.callback=waveout;
		
  if(SDL_OpenAudio(&asp,&audio_spec)<0){
    fprintf(stderr,"Can't Open SDL Audio\n");
    return -1;
  } 
  waveptr = wavflag = 0; wavdone = 1;
  SDL_PauseAudio(0);
	
  //Successful
  */

  return 1;
}

//Sound Close
void InfoNES_SoundClose( void )
{
  /*JJ pendiente
  SDL_CloseAudio();
  */
}
  
//Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise. 1 DPCM
void InfoNES_SoundOutput(int samples, unsigned char *wave1, unsigned char *wave2, unsigned char *wave3, unsigned char *wave4, unsigned char *wave5){
  /*JJ pendiente
  int i;

  for (i = 0; i < samples; i++)
  {
    final_wave[waveptr] = 
      ( wave1[i] + wave2[i] + wave3[i] + wave4[i] + wave5[i] ) / 5; 

    waveptr++;
    if ( waveptr == 2048 ) 
    {
      waveptr = 0;
      wavflag = 2; wavdone=0;
    }
    else if ( waveptr == 1024 )
    {
      wavflag = 1; wavdone=0;
    }
    
    if (gb_sinfreno==0)
    {
     while (!wavdone) SDL_Delay(0);
    }
  }
  */
}




//SONIDO ESP32 DAC SDL BEGIN
#ifdef use_lib_esp32_dac
void SDL_InitAudio()
{
  /*JJ pendiente
    want.freq = SAMPLE_RATE; // number of samples per second
    want.format = AUDIO_S16SYS; // sample type (here: signed short i.e. 16 bit)
    want.channels = 1; // only one channel
    //want.channels = 2; //2 canales
    //want.samples = 16; // buffer-size
    //want.samples = 64; // buffer-size
    //want.samples = 256; // buffer-size
    //want.samples = 512; // buffer-size
    //want.samples = 1024; // buffer-size
    want.samples = 2048; // buffer-size
    //want.samples = 512; // buffer-size
    //want.samples = 4096; // buffer-size
    //want.samples = 8192; // buffer-size
    //want.samples = 16384; // buffer-size
    want.callback = SDL_audio_callback; // function SDL calls periodically to refill the buffer
    want.userdata = &sample_nr; // counter, keeping track of current sample number

    if(SDL_OpenAudio(&want, &have) != 0) 
     printf("Failed to open audio: %s\n", SDL_GetError());
    if(want.format != have.format) 
     printf ("Failed to get the desired AudioSpec\n");
  */
}

 volatile unsigned int gb_cur_cont_ch[3]={0,0,0};
 volatile unsigned char gb_flipflop_ch[3]={0,0,0};
 volatile unsigned int gb_max_cont_ch[3]={1,1,1};

//******************************************************************************
/*JJ pendiente
void SDL_audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
}
*/
#endif
//SONIDO ESP32 DAC SDL END









//Print system message
void InfoNES_MessageBox(char *pszMsg, ...){}

//End of InfoNES_System_SDL.cpp


