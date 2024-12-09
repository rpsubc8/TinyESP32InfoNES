#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //SECTION HARDWARE KEYBOARD BEGIN
 //Keyboard config GPIO
 #define KEYBOARD_DATA 32
 #define KEYBOARD_CLK 33
 //SECTION HARDWARE KEYBOARD END


 //SECTION HARDWARE VGA BEGIN
 //VGA config GPIO
 #define PIN_RED_LOW    21
 #define PIN_RED_HIGH   22
 #define PIN_GREEN_LOW  18
 #define PIN_GREEN_HIGH 19
 #define PIN_BLUE_LOW   4
 #define PIN_BLUE_HIGH  5
 #define PIN_HSYNC      23
 #define PIN_VSYNC      15 

 // VGA sync pins
 #define HSYNC_PIN 23
 #define VSYNC_PIN 15
 //SECTION HARDWARE VGA END






 #define use_lib_tinybitluni_fast

 #define use_lib_vga_esp32
 
 //Usar 61440 bytes en lugar de 122880
 #define use_lib_WorkFrame8

 //No usar framebuffer 61440 bytes (ahorro memoria)
 #define use_lib_not_use_framebuffer
 
 //usar roms de memoria
 #define use_lib_rom_memory
 
 //Usa 64 colores paleta ttgo vga32
 #define use_lib_palette_ttgo
 
 //Usa osciladores
 #define use_lib_esp32_dac
 
 //Log sonido
 //#define use_lib_debug_apu_memwrite


 #define use_lib_log_serial

 //Teclado remoto UART putty
 #define use_lib_keyboard_uart

 //timeout read millis
 #define use_lib_keyboard_uart_timeout 0

 //log keyboard serial
 //#define use_lib_log_keyboard_uart


 #define use_lib_frame_skip

 //No usar mapper 5(ahorro memoria)
 #define use_lib_not_use_mapper5

 //No usar mapper 6(ahorro memoria)
 #define use_lib_not_use_mapper6

 //No usar mapper 19(ahorro memoria)
 #define use_lib_not_use_mapper19

 //No usar mapper 19(ahorro memoria)
 #define use_lib_not_use_mapper45
 
 //No usar mapper 85(ahorro memoria)
 #define use_lib_not_use_mapper85

 //No usar mapper 185(ahorro memoria)
 #define use_lib_not_use_mapper185
 
 //No usar mapper 188(ahorro memoria)
 #define use_lib_not_use_mapper188

 //No usar mapper 235 DISK RAM (ahorro memoria) Requiere psram
 #define use_lib_not_use_mapper235

 //Usar psram
 //#define use_lib_psram

#endif

