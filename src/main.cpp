// Definiciones
#include <Arduino.h>
#include <horometro.h>
#include <variables.h>
#include <FileSystem.h>
#include <ConexionWifi.h>
#include <ConexionMQTT.h>
#include <BOD.h>
#include <OLED.h>

#define button D0

void setup()
{
  Serial.begin(115200);
  BODinit();
  rtcSetup(); 
  Serial.println();
  pinMode(button, INPUT_PULLDOWN_16);
  
  setupFS();
  setupAP();
  setupMQTT();
  setupOled();
  String msghoro = readHoro();
  escribir_oled(msghoro);
  Serial.println("════════════════════════════════════════════════════════════");
  Serial.println("Sistema Listo");
  Serial.println("════════════════════════════════════════════════════════════");
}

void loop()
{
  //BODWatch();
  MQTTWatch();
  if (changeHoro() && !banderaMQTT)
  {
    String msgHoro = readHoro();
    Serial.println("Equipo: " + String(valoresConfig[4]) + "\tCodigo: " + String(valoresConfig[5]) + "\tHorometro:" + msgHoro);
    escribir_oled(msgHoro);
    publishString("lastReg",msgHoro);
    publishJson("vars",horometro,valoresHorometro,nH);
  }


  if (digitalRead(button))
  {
    limpiarVariables();
    resetAP();
  }
}