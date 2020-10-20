#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
char c;

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

U8G2LOG u8g2log;
#define U8LOG_WIDTH 19
#define U8LOG_HEIGHT 3
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];


 void setup(void) {
   u8g2.begin();
   u8g2.clearBuffer(); 
   Serial.begin(9600);
   
   u8g2log.begin(u8g2, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
   u8g2log.setLineHeightOffset(0); // set extra space between lines in pixel, this can be negative
   u8g2log.setRedrawMode(1);   // 0: Update screen with newline, 1: Update screen for every char  
}
  void loop(void) 
  {
   String variable = "15/05/15";
   u8g2.setFontMode(1);
   //u8g2.setFont(u8g2_font_6x10_tr);
   u8g2.setFont(u8g2_font_cu12_tr);
   u8g2.drawStr(35,25,variable.c_str());
   u8g2.sendBuffer();
   }
    //u8g2.setFontMode(1);
    //u8g2.setDrawColor(1);
    //u8g2.setFont(u8g2_font_fub11_tr); 
    //u8g2.drawStr(46,25,"Prueba");  // Escribe en la memoria interna.
    //u8g2.sendBuffer();         // Pasa de la memoria interna a la pantalla. 
    //delay (1000);

    //u8g2.setDrawColor(0);
    //u8g2.drawBox(36,0,120,50); 
    //u8g2.sendBuffer();
    //delay(1);

    //u8g2.setDrawColor(1);
    //u8g2.drawStr(45,25,"Display");  // Escribe en la memoria interna.
    //u8g2.sendBuffer();         // Pasa de la memoria interna a la pantalla. 
    //delay (1000); 
    
    //u8g2.setDrawColor(0);
    //u8g2.drawBox(36,0,120,50); 
    //u8g2.sendBuffer();
    //delay(1);
//}
