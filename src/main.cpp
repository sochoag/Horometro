// Definiciones
#include <Arduino.h>
#include <horometro.h>
#include <variables.h>
#include <FileSystem.h>
#include <ConexionWifi.h>
#include <ConexionMQTT.h>
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
  setupMQTT();
  setupOled();

  Serial.println("IP Local");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.subnetMask());
  String msghoro = readHoro();
  Serial.println(msghoro);
  escribir_oled(msghoro);
}

void loop()
{
  //BODWatch();
  MQTTWatch();
  if (changeHoro())
  {
    String msgHoro = readHoro();
    Serial.println("Equipo: " + String(valoresConfig[4]) + "\tCodigo: " + String(valoresConfig[5]) + "\tHorometro:" + msgHoro);
    escribir_oled(msgHoro);
    publishString("lastReg",msgHoro);
  }


  if (!digitalRead(0))
  {
    resetAP();
  }
}