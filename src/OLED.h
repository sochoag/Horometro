#ifndef oled
#define oled

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

void setupOled()
{
    u8g2.begin();
    u8g2.clearBuffer();
}

void escribir_oled(String variable,int font_size)
{
    u8g2.clearBuffer();
    u8g2.setFontMode(1);
    u8g2.setFontRefHeightAll();
    if(font_size == 10)
    {
        u8g2.setFont(u8g2_font_6x10_tr);
    }
    else if(font_size == 14)
    {
        u8g2.setFont(u8g2_font_fub14_tf);
    }
    int x = (u8g2.getDisplayWidth() - u8g2.getUTF8Width(variable.c_str()))/2;
    int y = (u8g2.getDisplayHeight() + u8g2.getAscent())/2;
    //Serial.println("Tamaño texto:"+String(y));
    u8g2.drawStr(x, y, variable.c_str());
    u8g2.sendBuffer();
}

#endif