// Definiciones
#include <Arduino.h>
#include <FileSystem.h>
#include <Conexion.h>
#include <horometro.h>
#include <variables.h>
#include <BOD.h>

#define button 0

void setup()
{
  BODinit();

  Serial.begin(115200);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  Serial.println();
  pinMode(button, INPUT_PULLUP);
  
  setupFS();
  setHoro(valoresHorometro);
  setupAP();
  Serial.println("IP Local");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(readHoro());
}

void loop()
{
  BODWatch();
  if (changeHoro())
  {
    WiFi.forceSleepWake();
    WiFi.mode(WIFI_STA);
    while(!wm.autoConnect())
    delay(500);
    Serial.println("Equipo: " + String(valoresConfig[0]) + "\tCodigo: " + String(valoresConfig[1]) + "\tHorometro:" + readHoro());
    WiFi.forceSleepBegin();
  }


  if (!digitalRead(0))
  {
    resetAP();
  }
}