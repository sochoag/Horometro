// Definiciones
#include <Arduino.h>
#include <horometro.h>
#include <FileSystem.h>
#include <Conexion.h>
#include <variables.h>
#include <BOD.h>
#include <OLED.h>

#define button 0

void setup()
{
  Serial.begin(115200);
  BODinit();
  rtcSetup(); 
  Serial.println();
  pinMode(button, INPUT_PULLUP);
  
  setupFS();
  setHoro(valoresHorometro);
  setupAP();
  setupOled();

  Serial.println("IP Local");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(readHoro());
  escribir_oled(readHoro());
}

void loop()
{
  BODWatch();
  if (changeHoro())
  {
    //WiFi.forceSleepWake();
    //WiFi.mode(WIFI_STA);
    //while(!wm.autoConnect()){}
    //delay(500);
    Serial.println("Equipo: " + String(valoresConfig[0]) + "\tCodigo: " + String(valoresConfig[1]) + "\tHorometro:" + readHoro());
    escribir_oled(readHoro());
    //WiFi.forceSleepBegin();
  }


  if (!digitalRead(0))
  {
    resetAP();
  }
}