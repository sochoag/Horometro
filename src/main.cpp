// Definiciones
#include <Arduino.h>
#include <horometro.h>
#include <variables.h>
#include <FileSystem.h>
#include <ConexionWifi.h>
#include <ConexionMQTT.h>
#include <BOD.h>
#include <OLED.h>

#define button 2

void setup()
{
  Serial.begin(115200);
  //BODinit();
  setupOled();
  rtcSetup(); 
  Serial.println();
  pinMode(button, INPUT);
  
  setupFS();
  setupAP();
  setupMQTT();
  String msghoro = readHoro();
  escribir_oled(msghoro,14);
  Serial.println("════════════════════════════════════════════════════════════");
  escribir_oled("Sistema Listo",10);
  delay(1000);
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
    escribir_oled(msgHoro,14);
    publishString("lastReg",msgHoro);
    publishJson("vars",horometro,valoresHorometro,nH);
  }


  if (!digitalRead(button))
  {
    limpiarVariables();
    resetAP();
  }
}